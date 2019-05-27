/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#include "ceammc_pd.h"
#include "ceammc_atomlist.h"
#include "ceammc_externals.h"
#include "ceammc_factory.h"
#include "ceammc_platform.h"

#include "m_pd.h"

extern "C" {
#include "g_canvas.h"
#include "m_imp.h"
void pd_init();
void obj_sendinlet(t_object* x, int n, t_symbol* s, int argc, t_atom* argv);
}

#include <exception>
#include <iostream>

typedef t_object* (*t_newempty)();
typedef t_object* (*t_newfloat)(t_float);
typedef t_object* (*t_newsymbol)(t_symbol*);
typedef t_object* (*t_newgimme)(t_symbol* s, int argc, t_atom* argv);

using namespace ceammc;

pd::External::External(const char* name, const AtomList& lst)
    : obj_(0)
{
    try {
        t_symbol* OBJ_NAME = gensym(name);
        pd_typedmess(&pd_objectmaker, OBJ_NAME, lst.size(), lst.toPdData());

        t_pd* ptr = pd_newest();
        if (!ptr) {
            printf("object creation failed\n");
            return;
        }

        t_object* res = pd_checkobject(ptr);
        if (!res) {
            printf("invalid object\n");
            return;
        }

        obj_ = res;
    } catch (std::exception& e) {
        std::cerr << "error: " << e.what() << std::endl;
        obj_ = 0;
    }
}

pd::External::~External()
{
    if (obj_) {
        pd_free(&obj_->te_g.g_pd);
    }
}

bool pd::External::isNull() const
{
    return obj_ == 0;
}

bool pd::External::connectTo(int outn, t_object* dest, int inln)
{
    if (!obj_) {
        printf("[connectTo] NULL object\n");
        return false;
    }

#define OBJ_NAME(obj) obj->te_g.g_pd->c_name->s_name

    if (obj_ == dest) {
        printf("[%s: connectTo] self-connect\n", OBJ_NAME(obj_));
        return false;
    }

    if (!dest) {
        printf("[%s: connectTo] NULL destination\n", OBJ_NAME(obj_));
        return false;
    }

    if (inln >= obj_ninlets(dest)) {
        printf("[%s: connectTo %s] invalid destination inlet: %d\n",
            OBJ_NAME(obj_), OBJ_NAME(dest), inln);
        return false;
    }

    if (outn >= numOutlets()) {
        printf("[%s: connectTo %s] invalid source outlet: %d\n",
            OBJ_NAME(obj_), OBJ_NAME(dest), outn);
        return false;
    }

#undef OBJ_NAME

    return obj_connect(obj_, outn, dest, inln) != 0;
}

bool pd::External::connectTo(int outn, pd::External& ext, int inln)
{
    return connectTo(outn, ext.object(), inln);
}

bool pd::External::connectFrom(int outn, t_object* src, int inln)
{
    if (!obj_) {
        printf("[connectFrom] NULL object\n");
        return false;
    }

#define OBJ_NAME(obj) obj->te_g.g_pd->c_name->s_name

    if (obj_ == src) {
        printf("[%s: connectFrom] self-connect\n", OBJ_NAME(obj_));
        return false;
    }

    if (!src) {
        printf("[%s: connectFrom] NULL source\n", OBJ_NAME(obj_));
        return false;
    }

    if (inln >= numInlets()) {
        printf("[%s: connectFrom %s] invalid destination inlet: %dn",
            OBJ_NAME(obj_), OBJ_NAME(src), inln);
        return false;
    }

    if (outn >= obj_noutlets(src)) {
        printf("[%s: connectFrom %s] invalid source outlet: %d\n",
            OBJ_NAME(obj_), OBJ_NAME(src), outn);
        return false;
    }

#undef OBJ_NAME

    return obj_connect(src, outn, obj_, inln) != 0;
}

bool pd::External::connectFrom(int outn, pd::External& ext, int inln)
{
    return connectFrom(outn, ext.object(), inln);
}

t_object* pd::External::object()
{
    return obj_;
}

void pd::External::sendBang()
{
    if (!obj_)
        return;

    pd_bang(pd());
}

void pd::External::sendFloat(t_float v)
{
    if (!obj_)
        return;

    pd_float(pd(), v);
}

void pd::External::sendSymbol(t_symbol* s)
{
    if (!obj_)
        return;

    pd_symbol(pd(), s);
}

void pd::External::sendList(const AtomList& l)
{
    if (!obj_)
        return;

    pd_list(pd(), &s_list, l.size(), l.toPdData());
}

void pd::External::sendBangTo(size_t inlet)
{
    if (inlet == 0)
        sendBang();
    else {
        External pd_b("bang");
        if (pd_b.connectTo(0, *this, inlet))
            pd_bang(pd_b.pd());
    }
}

void pd::External::sendFloatTo(t_float v, size_t inlet)
{
    if (inlet == 0)
        sendFloat(v);
    else {
        External pd_f("float");
        if (pd_f.connectTo(0, *this, inlet))
            pd_float(pd_f.pd(), v);
    }
}

void pd::External::sendSymbolTo(t_symbol* s, size_t inlet)
{
    if (inlet == 0)
        sendSymbol(s);
    else {
        External pd_s("symbol");
        if (pd_s.connectTo(0, *this, inlet))
            pd_symbol(pd_s.pd(), s);
    }
}

void pd::External::sendListTo(const AtomList& l, size_t inlet)
{
    if (inlet == 0)
        sendList(l);
    else {
        External pd_l("list");
        if (pd_l.connectTo(0, *this, inlet))
            pd_list(pd_l.pd(), &s_list, l.size(), l.toPdData());
    }
}

void pd::External::sendMessage(t_symbol* msg, const AtomList& args)
{
    if (!obj_)
        return;

    pd_typedmess(&obj_->te_g.g_pd, msg, args.size(), args.toPdData());
}

int pd::External::numOutlets() const
{
    if (!obj_)
        return 0;

    return obj_noutlets(obj_);
}

int pd::External::numInlets() const
{
    if (!obj_)
        return 0;

    return obj_ninlets(obj_);
}

int pd::External::xPos() const
{
    if (!obj_)
        return 0;

    return obj_->te_xpix;
}

int pd::External::yPos() const
{
    if (!obj_)
        return 0;

    return obj_->te_ypix;
}

void pd::External::setXPos(int x)
{
    if (!obj_)
        return;

    obj_->te_xpix = x;
}

void pd::External::setYPos(int y)
{
    if (!obj_)
        return;

    obj_->te_ypix = y;
}

std::vector<t_symbol*> pd::External::methods() const
{
    std::vector<t_symbol*> res;
    if (!obj_)
        return res;

    t_class* c = obj_->te_g.g_pd;
    for (int i = 0; i < c->c_nmethod; i++) {
        auto m = &c->c_methods[i];
        res.push_back(m->me_name);
    }

    return res;
}

bool pd::External::isCeammc() const
{
    return is_ceammc(obj_);
}

bool pd::External::isCeammcBase() const
{
    return is_ceammc_base(obj_);
}

bool pd::External::isCeammcUI() const
{
    return is_ceammc_ui(obj_);
}

bool pd::External::isCeammcFaust() const
{
    return is_ceammc_faust(obj_);
}

bool pd::External::isCeammcFlext() const
{
    return is_ceammc_flext(obj_);
}

const BaseObject* pd::External::asCeammcBaseObject() const
{
    return ceammc_to_base_object(obj_);
}

const UIObject* pd::External::asCeammcUIObject() const
{
    return ceammc_to_ui_object(obj_);
}

std::vector<PropertyInfo> pd::External::properties() const
{
    if (!isCeammc())
        return {};

    if (isCeammcBase())
        return ceammc_base_properties(obj_);
    else if (isCeammcUI())
        return ceammc_ui_properties(obj_);
    else if (isCeammcFaust())
        return ceammc_faust_properties(obj_);
    else
        return {};
}

PureData::PureData()
{
    pd_init();
}

CanvasPtr PureData::createTopCanvas(const char* name, const AtomList& args)
{
    CanvasPtr ptr;

    CanvasMap::iterator it = canvas_map_.find(name);
    if (it != canvas_map_.end())
        return it->second;

    AtomList l(0.f, 0.f); // x, y
    l.append(600); // width
    l.append(400); // height
    l.append(10); // font size

    if (canvas_getcurrent())
        canvas_unsetcurrent(canvas_getcurrent());

    if (platform::is_path_relative(name)) {
        glob_setfilename(0, gensym(name), gensym("~"));
    } else {
        std::string dir = platform::dirname(name);
        std::string fname = platform::basename(name);
        glob_setfilename(0, gensym(fname.c_str()), gensym(dir.c_str()));
    }

    if (!args.empty())
        canvas_setargs(args.size(), args.toPdData());

    assert(l.size() == 5);
    t_canvas* cnv = canvas_new(0, gensym(name), l.size(), l.toPdData());
    cnv->gl_loading = 0;

    if (!cnv)
        return ptr;

    ptr.reset(new Canvas(cnv));
    canvas_map_[name] = ptr;
    return ptr;
}

CanvasPtr PureData::createSubpatch(_glist* parent, const char* name)
{
    t_canvas* cnv = canvas_new(0, gensym(name), 0, NULL);
    if (!cnv)
        return CanvasPtr();

    cnv->gl_name = gensym(name);
    cnv->gl_owner = parent;

    return CanvasPtr(new Canvas(cnv));
}

PureData& PureData::instance()
{
    static PureData pd;
    return pd;
}
