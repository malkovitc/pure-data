#!/usr/bin/env python3
import sys, os, re
from argparse import ArgumentParser
import json

import CppHeaderParser3 as CppHeaderParser
import common_functions as gen
from common_functions import error_msg, warning_msg, debug_msg
import converter_object_templates as objt

def write_header(mod_name, fp, version="0.0"):
    fp.write("// autogenerated file: do not edit!\n");
    fp.write("#include \"wrapper.h\"\n\n")
    fp.write(f"#include \"iface_{mod_name}.h\"\n")
    fp.write("extern \"C\" {\n");
    fp.write(f"void {mod_name}_setup() {{\n")
    fp.write(f"    post(\"  library: \\\"{mod_name}\\\"\");\n")
    fp.write(f"    post(\"  version: \\\"{version}\\\"\");\n\n")

def write_footer(fp):
    fp.write("}\n")
    fp.write("}")

def parse_example(lines):
    res = "\n".join(
        map(lambda x: x.strip(),
            map(lambda x: x[1:] if x.startswith("*") else x, lines)
        )
    )

    return res


def parse_doxygen(txt):
    res = dict()
    res["pd_aliases"] = []
    res["params"] = []
    res["param_names"] = []
    res["return"] = []

    example = []

    for l in txt.split("\n"):
        # @example tag
        is_example = re.match(r'\*\s+@example', l)
        if is_example is not None:
            example.append("")
            continue

        if re.match(r'\*\s+@ignore', l) is not None:
            res["ignore"] = True
            continue

        result = re.match(r'\*\s+(@\w+)\s+(.+)', l)
        if result is not None:
            # new tag after @example
            if len(example) > 0 and result.group(1) != "@example":
                res["example"] = parse_example(example)
                example = []

            if result.group(1) == "@pd_alias":
                res["pd_aliases"] += map(lambda x: x.strip(), result.group(2).split(","))
            elif result.group(1) == "@pd_name":
                res["pd_name"] = result.group(2)
            elif result.group(1) == "@pd_class":
                res["pd_class"] = result.group(2)
            elif result.group(1) == "@example_class_args":
                res["example_class_args"] = list(map(lambda x: x.strip(), result.group(2).split(",")))
            elif result.group(1) == "@pd_ns":
                res["pd_ns"] = result.group(2)
            elif result.group(1) == "@pd_fullname":
                res["pd_fullname"] = result.group(2)
            elif result.group(1) == "@brief":
                res["brief"] = result.group(2)
            elif result.group(1) in ("@author", "@authors"):
                res["authors"] = list(map(lambda x: x.strip(), result.group(2).split(",")))
            elif result.group(1) == "@library":
                res["library"] = result.group(2)
            elif result.group(1) == "@license":
                res["license"] = result.group(2)
            elif result.group(1) == "@version":
                res["version"] = result.group(2)
            elif result.group(1) == "@since":
                res["since"] = result.group(2)
            elif result.group(1) == "@see":
                res["see"] = list(map(lambda x: x.strip(), result.group(2).split(" ")))
            elif result.group(1) == "@keywords":
                res["keywords"] = result.group(2).split(' ')
            elif result.group(1) == "@param":
                # common format is @param arg - first argument
                # so we need to get "first argument"
                txt = result.group(2)
                m1 = re.match(r'(\w+)\s+-\s+(.+)', txt)
                if m1 is not None:
                    res["param_names"].append(m1.group(1))
                    res["params"].append(m1.group(2))
                else:
                    # without -
                    # @param arg is ...
                    m2 = re.match(r'(\w+)\s+(.+)', txt)
                    if m2 is None:
                        res["params"].append(txt)
                        res["param_names"] = ""
                    else:
                        res["param_names"].append(m2.group(1))
                        res["params"].append(m2.group(2))

            elif result.group(1) == "@return":
                res["return"].append(result.group(2))
            elif result.group(1) == "@constructor":
                res["constructor"] = result.group(2)
            else:
                warning_msg("unknown doxygen: {}".format(result.group(1)))
        else:
            # has @example tag and skip last doxygen */
            if len(example) and l.strip() != '*/':
                # append multiline example
                example.append(l)

    # @example tag was last in doxygen
    if len(example):
        res["example"] = parse_example(example)

    return res

def generate_plain_functions(cpp, fp):
    for f in cpp.functions:
        fn_name = f["name"]
        fn_ns = f["namespace"]

        # skipping template functions
        if f["template"]:
            continue

        # skip library init function
        if fn_name in ("wrapper_init"):
            continue

        # exclude operators now
        if fn_name.startswith("operator"):
            continue

        #todo: add & for references
        fn_type = [t["type"] for t in f["parameters"]]
        fn_result = f["rtnType"]

        if f["static"]:
            fn_result = fn_result.replace('static ', '')

        if f["inline"]:
            fn_result = fn_result.replace('inline ', '')

        pd_info = dict()
        # Doxygen process
        if "doxygen" in f:
            pd_info = parse_doxygen(f["doxygen"])

        txt = objt.genFunc(fn_ns, fn_name, fn_result, ",".join(fn_type),
            pdname=pd_info.get("pd_name"),
            aliases=pd_info.get("pd_aliases"))

        fp.write(txt)

def cxx_clean_type(t):
    # remove const, &, *, etc.
    res = filter(lambda x: x not in ("const", "&", "*"), t.split(" "))
    res = "".join(res).replace("&", "")\
                .replace("*", "")\
                .replace("double", "float")\
                .replace("char", "int")\
                .replace("short", "int")\
                .replace("size_t", "int")\
                .replace("long", "int")\
                .replace("bool", "int")\
                .replace("unsigned", "")

    if res.startswith("std::vector") \
            or res.startswith("vector") \
            or res.startswith("std::list") \
            or res.startswith("list"):
        return "list"

    if res.startswith("std::string") \
            or res.startswith("string") \
            or res.startswith("t_symbol"):
        return "symbol"

    return res

def pddoc_arg_attrs(t):
    if "bool" in t:
        return { "enum": "0 1" }
    if "unsigned " in t:
        return { "minvalue": 0 }
    if "size_t" in t:
        return { "minvalue": 0 }

    return {}


def pd_arg_type(arg):
    if arg in ("int", "float", "symbol", "list", "any", "pointer"):
        return arg
    else:
        return "data"

def pd_return_type(r):
    if r.startswith("std::tuple") \
            or r.startswith("tuple"):
        b = r.find('<')
        e = r.rfind('>')
        types = list(map(lambda x: pd_arg_type(cxx_clean_type(x.strip())), r[slice(b + 1, e)].split(",")))
        return types
    else:
        return [r]

def generate_method(ns, c_name, c_args, m_list, out_fp, cls_info):
    m = m_list[0]
    has_overload = len(m_list) > 1
    # skip constructors
    if m["name"] == c_name:
        return

    m_return = m["rtnType"]
    # cleanup return value
    if m["inline"]:
        m_return = m_return.replace('inline ', '')
    if m["static"]:
        m_return = m_return.replace('static ', '')

    m_const = m["const"] == True
    m_args = [t["type"] for t in m["parameters"]]
    m_name = m["name"]

    pd_info = dict()
    m_info = dict()
    doc_name = None
    pd_name = c_name.lower()

    # fill m_info ...
    # Doxygen process
    if "doxygen" in m:
        dox = parse_doxygen(m["doxygen"])
        m_info = dox
        doc_name = gen.method_pd_name(ns, c_name, m_name, **dox)
        keys = ('pd_name', 'pd_class', 'pd_ns', 'pd_fullname', 'pd_aliases')
        pd_info = dict(filter(lambda kv: kv[0] in keys, dox.items()))

    if "ignore" in m_info:
        warning_msg(f"ignoring {m_name} function")
        return

    if not doc_name:
        doc_name = gen.method_pd_name(ns, c_name, m_name)

    m_info["doc_name"] = doc_name

    # class info
    m_info["doc_class"] = {
        "pd_name": pd_name,
        "pd_args": cls_info["constructor"]
    }

    if m["static"]:
        arg_list = m_args
    else:
        arg_list = [c_name] + m_args

    # method args info
    m_info["doc_args"] = {
        "pd_types": list(map(lambda x: pd_arg_type(cxx_clean_type(x)), arg_list)),
    }

    m_info["pddoc_args"] = list()
    # pddoc <arguments>
    for idx in range(len(m_args)):
        a = m_args[idx]
        clean_arg = cxx_clean_type(a)
        # dict keys: name, type, info
        pddoc_args = { "name": m_info["param_names"][idx] }

        # append Constructor pddoc arguments
        if clean_arg == c_name:
            pddoc_args["type"] = "list"
        else:
            pddoc_args["type"] = pd_arg_type(clean_arg)

        pddoc_args["info"] = ""
        if "params" in m_info and idx < len(m_info["params"]):
            pddoc_args["info"] = m_info["params"][idx]

        extra_attrs = pddoc_arg_attrs(a)
        for k, v in extra_attrs.items():
            pddoc_args[k] = v

        m_info["pddoc_args"].append(pddoc_args)

    # inlets
    # c_name is first argument
    # first inlet
    m_info["doc_inlets"] = list()
    if not m["static"]:
        ctor_args = [{
            "type": "list",
            "info": "{} data constructor: {}".format(
                c_name, cls_info["constructor"])
        }]

        ctor_args.append({
            "type": "data",
            "info": f"{c_name} data"
        })

        m_info["doc_inlets"].append(ctor_args)


    # other inlets
    for idx in range(len(m_args)):
        a = m_args[idx]
        clean_arg = cxx_clean_type(a)
        inlet_arg = list()
        # append Constructor inlet arguments
        if clean_arg == c_name:
            inlet_arg.append({
                "type": "list",
                "info": "{}: {}".format(
                    m_info["params"][idx] if "params" in m_info and idx < len(m_info["params"]) else "",
                    cls_info["constructor"])
            })

        inlet_arg.append({
            "type": pd_arg_type(clean_arg),
            "info": "{}".format(
                    m_info["params"][idx] if "params" in m_info and idx < len(m_info["params"]) else "")
        })

        m_info["doc_inlets"].append(inlet_arg)

    m_info["doc_outlets"] = list()
    pd_return = pd_return_type(m_return)
    for idx in range(len(pd_return)):
        out = pd_return[idx]
        m_info["doc_outlets"].append({
            "type": out,
            "info": m_info["return"][idx] if "return" in m_info and idx < len(m_info["return"]) else ""
        })

    cls_info["methods"][m_name] = m_info


    if m["static"]:
        out_fp.write(objt.genClassStaticMethod(ns, c_name,
                                               m_name, m_return, ", ".join(m_args),
                                               **pd_info, constructor_args=c_args))
    else:
        overload_info = []

        # has overloaded methods
        if len(m_list) > 1:
            # fill overload info
            for mover in m_list[1:]:
                mover_args = [t["type"] for t in mover["parameters"]]
                overload_info.append(mover_args)

            has_dox = [True for mover in m_list if "doxygen" in mover]
            # all dox
            if len(has_dox) == len(m_list):
                pd_args = list()
                pd_inlets = list()
                for m in m_list:
                    dox = parse_doxygen(m["doxygen"])
                    if "params" not in dox:
                        error_msg(f"overload is not documented: {m_name}")
                        continue

                    doc_str = dox["params"][0]
                    clean_arg = cxx_clean_type(m["parameters"][0]["type"])
                    if clean_arg == c_name:
                        doc_str = "{} ({} {})".format(dox["params"][0],
                            c_name,
                            cls_info["constructor"])

                    pd_args.append(doc_str)
                    pd_inlets.append(doc_str)

                cls_info["methods"][m_name]["pddoc_args"][0]["info"] = " or ".join(pd_args)
                cls_info["methods"][m_name]["pddoc_args"][0]["type"] = "list"

                for m in m_list[1:]:
                    dox = parse_doxygen(m["doxygen"])
                    doc_str = dox["params"][0]
                    clean_arg = cxx_clean_type(m["parameters"][0]["type"])
                    if clean_arg == c_name:
                        doc_str = "{} ({})".format(dox["params"][0], cls_info["constructor"])

                    cls_info["methods"][m_name]["doc_inlets"][1].append(
                    {
                        "type": pd_arg_type(clean_arg),
                        "info": doc_str
                    }
                    )
            else:
                error_msg(f"no doxygen doc for {m_name}")

        out_fp.write(objt.genClassMethod(ns, c_name, m_name, m_return, ", ".join(m_args),
            m_const,
            overloadInfo=overload_info,
            **pd_info,
            constructor_args=c_args))


def generate_classes(cpp_header, out_fp):
    class_info_list = []

    for c_name in cpp_header.classes:
        cls = cpp_header.classes[c_name]
        ns = cls["namespace"]

        all_methods = cls["methods"]["public"]

        cls_info = dict()
        if "doxygen" in cls:
            cls_info = parse_doxygen(cls["doxygen"])

        cls_info["class"] = c_name
        cls_info["pd_class"] = c_name.lower()

        # generate constructor and get constructor arguments
        # constructor args are required to construct data types from puredata atoms
        c_args = generate_constructor(ns, c_name, all_methods, out_fp, cls_info)

        # all public class methods
        generate_methods(ns, c_name, c_args, all_methods, out_fp, cls_info)

        out_fp.write("\n")

        class_info_list.append(cls_info)

    return class_info_list


def generate_constructor(ns, c_name, methods, out_fp, cls_info):
    c_args = []

    # get constructor args
    for m in methods:
        # not a constructor
        if m["name"] != c_name:
            continue

        # constructor args types
        m_type = [t["type"] for t in m["parameters"]]
        # skip defined default constructor
        if m_type == "":
            continue

        cls_info["pddoc"] = dict()
        cls_info["pddoc"]["arguments"] = []
        cls_info["pddoc"]["methods"] = [{
            "name": "set",
            "info": "set new data without output"
        }]

        inlet0 = [{
            "type": "bang",
            "info": "output current data"
        },
        {
            "type": "data",
            "info": "(on {}) set new data value and output it".format(c_name)
        }]

        ctor_info = dict()
        # parse constructor doxygen
        if "doxygen" in m:
            ctor_info = parse_doxygen(m["doxygen"])

        # add argument info
        idx = 0
        arg_list = []
        arg_info = []
        for a in m_type:
            clean_arg = cxx_clean_type(a)
            if "params" in ctor_info and idx < len(ctor_info["params"]):
                argi = ctor_info["params"][idx]
            else:
                argi = ""

            cls_info["pddoc"]["arguments"].append({
                "name": ctor_info["param_names"][idx],
                "type": pd_arg_type(clean_arg),
                "info": argi
            })

            arg_list.append(pd_arg_type(clean_arg))
            arg_info.append(argi)
            idx += 1

        inlet0.append({
            "type": "list",
            "info": "list: {} ({})".format(" ".join(arg_list), "/".join(arg_info))
        })
        cls_info["pddoc"]["inlets"] = [inlet0]

        if "example" in ctor_info:
            cls_info["pddoc"]["example"] = ctor_info["example"]

        # constructor with arguments
        c_args = m_type
        out_fp.write(objt.genClass(ns, c_name))

        break

    return c_args


def generate_methods(ns, c_name, c_args, methods, out_fp, cls_info):
    cls_info["methods"] = dict()

    overload_hist = dict()
    # find overloads
    for m in methods:
        name = m["name"]
        if name in overload_hist:
            overload_hist[name] += 1
        else:
            overload_hist[name] = 0

    method_overload = {k: 0 for k, v in overload_hist.items() if v > 0}

    for m in methods:
        # skip templates
        if m["template"]:
            continue

        name = m["name"]
        # skip helper functions
        if name in ("typeName", "toString", "setFromFloat", "setFromList", "setFromSymbol", "setFromAny"):
            continue

        # skip operators:
        if name.startswith("operator"):
            continue

        # skip direct generation of overloaded methods
        if name in method_overload:
            # check done flag
            if method_overload[name] == 0:
                # set done flag
                method_overload[name] = 1
            else:
                # warning_msg(f"skipping overloaded method {name}")

                if "doxygen" in m:
                    m_dox = parse_doxygen(m["doxygen"])
                    if "overloads" not in cls_info["methods"][name]:
                        cls_info["methods"][name]["overloads"] = {"params": [], "param_names": []}

                    cls_info["methods"][name]["overloads"]["params"].append(m_dox["params"])
                    cls_info["methods"][name]["overloads"]["param_names"] += m_dox["param_names"]

                continue


        generate_method(ns, c_name, c_args, [m for m in methods if m["name"] == name], out_fp, cls_info)

def generate_library_entry(mod_name, version, out_fp):
    out_fp.write(objt.genLibraryEntry(mod_name, version))

def main():
    parser = ArgumentParser()
    parser.add_argument("-m", "--module-name", dest="mod_name", help="module name", required=True)
    parser.add_argument("-d", "--output-dir", dest="out_dir", help="output directory", required=True)
    args = parser.parse_args()
    mod_name = args.mod_name
    out_dir = args.out_dir

    # C++ source path
    iface_path = f"{out_dir}/iface_{mod_name}.h"
    if not os.path.exists(iface_path):
        error_msg(f"interface file not found: \"{iface_path}\"")
        sys.exit(1)

    # generated wrapper path
    out_path = f"{out_dir}/gen_{mod_name}.cpp"
    out_fp = open(out_path, "w")
    if not out_fp:
        error_msg(f"can't open {out_path} for writing")

    mod_version = "0.0"
    mod_info = dict()

    # parsing interface file
    try:
        debug_msg(f"parsing: {iface_path}")
        cpp_header = CppHeaderParser.CppHeader(iface_path)

        # get version
        # version info expected in doxygen comment in wrapper_init() function
        for fn in cpp_header.functions:
            if fn["name"] != "wrapper_init":
                continue

            if "doxygen" not in fn:
                error_msg("no doxygen doc in wrapper_init() function")
                sys.exit(1)

            mod_info = parse_doxygen(fn["doxygen"])
            mod_version = mod_info.get("version", "")
            break

        if mod_version == "":
            mod_version = "0.0"
    except CppHeaderParser.CppParseError as e:
        error_msg(e)
        sys.exit(1)

    write_header(mod_name, out_fp, version=mod_version)

    out_fp.write("// {0}\n".format(os.path.basename(iface_path)))

    # library object
    generate_library_entry(mod_name, mod_version, out_fp)
    # plain functions
    generate_plain_functions(cpp_header, out_fp)
    # classes
    mod_info["classes"] = generate_classes(cpp_header, out_fp)

    write_footer(out_fp)

    json_path = f"{out_dir}/library.json"
    with open(json_path, 'w') as json_fp:
        json.dump(mod_info, json_fp, sort_keys=True, indent=4, ensure_ascii=True)

if __name__ == "__main__":
    main()
