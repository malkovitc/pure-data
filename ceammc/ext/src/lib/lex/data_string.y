%skeleton "lalr1.cc"
%require "3.5"
%output "data_string.parser.cpp"
%debug

%defines
%define api.token.prefix {TOK_}
%define api.namespace {ceammc}
%define api.parser.class {DataStringParser}
%define api.value.type variant
%define api.token.constructor
%define parse.error verbose
%locations
%define api.location.file "data_string.location.hpp"

%code requires{
    # include <memory>
    # include <vector>

    # include "ceammc_atomlist.h"
    # include "ceammc_datastorage.h"

    namespace ceammc {
        class DataStringLexer;
    }

# ifndef YY_NULLPTR
#   define YY_NULLPTR nullptr
# endif
}

%parse-param { ceammc::DataStringLexer& lexer }
%parse-param { ceammc::AtomList& result }

%code {
    # include <memory>
    # include <string>
    # include <utility>

    // header file generated with reflex
    # include "data_string.lexer.h"

    # include "ceammc_atomlist.h"
    # include "ceammc_datastorage.h"
    # include "ceammc_datatypes.h"
    # include "ceammc_log.h"
    # include "ceammc_format.h"
    # include "ceammc_string.h"
    # include "datatype_dict.h"
    # include "datatype_mlist.h"
    # include "datatype_string.h"
    # include "fmt/format.h"

    static ceammc::Atom createDataDict(const ceammc::Dict& v) {
        using namespace ceammc;

        auto dict = new DataTypeDict;

        if(v.empty())
            return dict;

        for(auto& kv: v) {
            auto key = gensym(kv.first.c_str());
            auto& val = kv.second;

            if(val.isA<void>())
                dict->insert(key, AtomList());
            else if(val.isA<Atom>())
                dict->insert(key, val.asT<Atom>());
            else
                dict->insert(key, val);
        }

        return dict;
    }

    // create t_symbol* Atom with unescaped string
    static ceammc::Atom createSimpleString(const std::string& str) {
        return gensym(ceammc::string::pd_string_unescape(str).c_str());
    }

    // create Atom with DataTypeString
    static inline ceammc::Atom createDataString(const std::string& str) {
        return new ceammc::DataTypeString(ceammc::string::pd_string_unescape(str).c_str());
    }

    static inline ceammc::Atom createDataList(const ceammc::AtomList& l) {
        return new ceammc::DataTypeMList(l);
    }

    static inline ceammc::Atom createFromList(const std::string& name, const ceammc::AtomList& args) {
        using namespace ceammc;

        auto fn = DataStorage::instance().fromListFunction(name);
        if(!fn) {
            LIB_ERR << fmt::format("function {}() not found", name);
            return Atom();
        }

        return fn(args);
    }

    static inline ceammc::Atom createFromDict(const std::string& name, const ceammc::Dict& v) {
        using namespace ceammc;

        auto fn = DataStorage::instance().fromDictFunction(name);
        if(fn == nullptr) {
            LIB_ERR << fmt::format("can't create type {} from dict", name);
            return Atom();
        }

        return fn(v);
    }

    # undef yylex
    # define yylex lexer.lex  // Within bison's parse() we should invoke lexer.lex(), not the global yylex()
}

%token                        NULL
%token                        OPEN_DICT_BRACKET
%token                        CLOSE_DICT_BRACKET
%token                        OPEN_LIST_BRACKET
%token                        CLOSE_LIST_BRACKET
%token <double>               FLOAT
%token                        LEXER_ERROR
%token <const char*>          SYMBOL
%token <std::string>          KEY
%token <std::string>          DATA_TYPE
%token                        DATA_TYPE_STRING
%token <std::string>          FUNC_CALL
%token <std::string>          STRING
%token                        END 0 "end of string"

%nterm <ceammc::Atom>         atom data function_call
%nterm <ceammc::AtomList>     atom_list expr
%nterm <ceammc::DictEntry>    pair
%nterm <ceammc::Dict>         pair_list


%start expr

%%

atom
    : NULL                    { $$ = ceammc::Atom(); }
    | FLOAT                   { $$ = ceammc::Atom($1); }
    | SYMBOL                  { $$ = ceammc::Atom(gensym($1)); }
    | STRING                  { $$ = createSimpleString($1); }
    | DATA_TYPE_STRING STRING { $$ = createDataString($2); }
    | data
    | function_call
    ;

atom_list
    : %empty           { $$ = ceammc::AtomList(); }
    | atom_list atom   { $$.append($1); $$.append($2); }
    ;

pair
    : KEY atom_list   { $$ = { $1, $2 }; }
    ;

pair_list
    : %empty         { $$ = Dict(); }
    | pair_list pair { $$.insert($$.end(), $1.begin(), $1.end()); $$.push_back($2); }
    ;

function_call
    : FUNC_CALL OPEN_LIST_BRACKET atom_list CLOSE_LIST_BRACKET   { $$ = createFromList($1, $3); }
    ;

data
    : OPEN_DICT_BRACKET pair_list CLOSE_DICT_BRACKET             { $$ = createDataDict($2); }
    | DATA_TYPE OPEN_DICT_BRACKET pair_list CLOSE_DICT_BRACKET   { $$ = createFromDict($1, $3); }
    | OPEN_LIST_BRACKET atom_list CLOSE_LIST_BRACKET             { $$ = createDataList($2); }
    | DATA_TYPE OPEN_LIST_BRACKET atom_list CLOSE_LIST_BRACKET   { $$ = createFromList($1, $3); }
    ;

expr
    : atom_list { result.append($1); }
    ;

%%

void ceammc::DataStringParser::error(const location& loc, const std::string& err_message)
{
    lexer.out() << err_message << std::endl;
    lexer.out() << lexer.indent()
                << lexer.matcher().line() << std::endl;
    lexer.out() << lexer.indent(loc.begin.column)
                << std::string(loc.end.column - loc.begin.column + 1, '^') << std::endl;

}
