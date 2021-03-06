%top {
    # include <string>
    # include <memory>

    # include "argcheck.parser.hpp"
    using token = ceammc::ArgCheckParser::token;
%}

%option freespace unicode
%option namespace=ceammc
%option lexer=ArgCheckLexer
%option header-file=argcheck.lexer.h
%option outfile=argcheck.lexer.cpp
/*%option graphs-file*/
/*%option debug*/
%option bison-cc-namespace="ceammc"
%option bison-cc-parser="ArgCheckParser"
%option bison-cc
%option bison-complete
%option noyywrap
%option reentrant

udec     0 | [1-9] \d*
int      [-+]? {udec}
frac     \. \d+
float    {int} {frac}?

space    [ \t\n]+
symbol   [a-zA-Z_.] [a-zA-Z_.0-9]*
range    \.\./({int}|])
int_range {int}\.\.{int}
squote    '
dquote   \"
sqstring {squote} ([^{squote}]|`{squote})* {squote}
dqstring {dquote} ([^{dquote}]|`{dquote})* {dquote}


%%

a               { return token::TATOM; }
b               { return token::TBOOL; }
d               { return token::TDATA; }
f               { return token::TFLOAT; }
s               { return token::TSYMBOL; }
i               { return token::TINT; } // for simple i types
i/{int_range}   { return token::TINT; } // to prevent {symbol} longest match in i2..4 case
\+              { return token::PLUS; }
\*              { return token::ASTERISK; }
\?              { return token::QUESTION; }
\[              { return token::REPEAT_START; }
\]              { return token::REPEAT_END; }
\(              { return token::GROUP_START; }
\)              { return token::GROUP_END; }
\|              { return token::OR; }
%               { return token::MODULUS; }
<=              { return token::LE; }
<               { return token::LT; }
>=              { return token::GE; }
>               { return token::GT; }
!=              { return token::NE; }
=               { return token::EQ; }
~               { return token::TILDE; }
\^              { return token::CAPS; }
\^2             { return token::POWER_OF_TWO; }
\$              { return token::DOLLAR; }
{space}         { ; }
{range}         { return token::REPEAT_RANGE; }
{int}           { return ArgCheckParser::make_INTEGER(std::strtol(text(), 0, 10)); }
{float}         { return ArgCheckParser::make_FLOAT(std::strtod(text(), 0)); }
{symbol}        { return ArgCheckParser::make_SYMBOL(text()); }

{sqstring}      {
                    const auto N = strlen(text()) - 2;
                    return ArgCheckParser::make_STRING(std::string(text() + 1, N));
                }

{dqstring}      {
                    const auto N = strlen(text()) - 2;
                    return ArgCheckParser::make_STRING(std::string(text() + 1, N));
                }

<*> .           { return token::LEXER_ERR; /* error */ }

%%


