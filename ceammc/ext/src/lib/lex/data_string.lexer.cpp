// data_string.lexer.cpp generated by reflex 1.6.3 from data_string.l

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  OPTIONS USED                                                              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#define REFLEX_OPTION_YYLTYPE             ceammc::location
#define REFLEX_OPTION_YYSTYPE             ceammc::DataStringParser::semantic_type
#define REFLEX_OPTION_bison_cc            true
#define REFLEX_OPTION_bison_cc_namespace  ceammc
#define REFLEX_OPTION_bison_cc_parser     DataStringParser
#define REFLEX_OPTION_bison_complete      true
#define REFLEX_OPTION_freespace           true
#define REFLEX_OPTION_header_file         "data_string.lexer.h"
#define REFLEX_OPTION_lex                 lex
#define REFLEX_OPTION_lexer               DataStringLexer
#define REFLEX_OPTION_namespace           ceammc
#define REFLEX_OPTION_noyywrap            true
#define REFLEX_OPTION_outfile             "data_string.lexer.cpp"
#define REFLEX_OPTION_reentrant           true
#define REFLEX_OPTION_token_eof           ceammc::DataStringParser::symbol_type(0)
#define REFLEX_OPTION_token_type          ceammc::DataStringParser::symbol_type
#define REFLEX_OPTION_unicode             true

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  SECTION 1: %top{ user code %}                                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#line 1 "data_string.l"

    # include <string>
    # include <memory>

    # include "lex/data_string.parser.hpp"
    # include "ceammc_log.h"

    using token = ceammc::DataStringParser::token;


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  REGEX MATCHER                                                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <reflex/matcher.h>

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  ABSTRACT LEXER CLASS                                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <reflex/abslexer.h>

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  LEXER CLASS                                                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

namespace ceammc {

class DataStringLexer : public reflex::AbstractLexer<reflex::Matcher> {
 public:
  typedef reflex::AbstractLexer<reflex::Matcher> AbstractBaseLexer;
  DataStringLexer(
      const reflex::Input& input = reflex::Input(),
      std::ostream&        os    = std::cout)
    :
      AbstractBaseLexer(input, os)
  {
  }
  static const int INITIAL = 0;
  virtual ceammc::DataStringParser::symbol_type lex(void);
};

} // namespace ceammc

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  SECTION 1: %{ user code %}                                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#line 11 "data_string.l"

    # include "fmt/format.h"

#line 20 "data_string.l"
/*%option graphs-file*/
/*%option debug*/
/*%option perf-report*/

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  BISON C++                                                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  SECTION 2: rules                                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

ceammc::DataStringParser::symbol_type ceammc::DataStringLexer::lex()
{
  static const char *REGEX_INITIAL = "(?mx)((?:[\\x09\\x0a\\x20]+))|(null)|(true)|(false)|(S(?=(?:\"(?:(?:[\\x00-!]|[\\x23-_]|[a-\\x7f]|[\\xc2-\\xdf][\\x80-\\xbf]|\\xe0[\\xa0-\\xbf][\\x80-\\xbf]|[\\xe1-\\xec][\\x80-\\xbf][\\x80-\\xbf]|\\xed[\\x80-\\x9f][\\x80-\\xbf]|[\\xee\\xef][\\x80-\\xbf][\\x80-\\xbf]|\\xf0[\\x90-\\xbf][\\x80-\\xbf][\\x80-\\xbf]|[\\xf1-\\xf3][\\x80-\\xbf][\\x80-\\xbf][\\x80-\\xbf]|\\xf4[\\x80-\\x8f][\\x80-\\xbf][\\x80-\\xbf])|`\"|``|`\\.|`:|`\\(|`\\))*\")))|((?:[A-Z][A-Za-z]*)(?=(?:\\()))|((?:[A-Z][A-Za-z]*)(?=(?:\\[)))|((?:\\[))|((?:\\]))|((?:\\())|((?:\\)))|((?:(?:[\\x2b\\x2d]?(?:0|[1-9][0-9]*))(?:\\.[0-9]+)?))|((?:[\\x2b\\x2d]?0[Xx][0-9A-Fa-f]+))|((?:0b[01]+))|((?:(?:[\\x23\\x2e0-9@-Z_a-z])+:))|((?:(?:[\\x23\\x2e0-9@-Z_a-z])(?:[\\x23'*+\\x2d-:<-Z_a-z\\x7c])*))|((?:\"(?:(?:[\\x00-!]|[\\x23-_]|[a-\\x7f]|[\\xc2-\\xdf][\\x80-\\xbf]|\\xe0[\\xa0-\\xbf][\\x80-\\xbf]|[\\xe1-\\xec][\\x80-\\xbf][\\x80-\\xbf]|\\xed[\\x80-\\x9f][\\x80-\\xbf]|[\\xee\\xef][\\x80-\\xbf][\\x80-\\xbf]|\\xf0[\\x90-\\xbf][\\x80-\\xbf][\\x80-\\xbf]|[\\xf1-\\xf3][\\x80-\\xbf][\\x80-\\xbf][\\x80-\\xbf]|\\xf4[\\x80-\\x8f][\\x80-\\xbf][\\x80-\\xbf])|`\"|``|`\\.|`:|`\\(|`\\))*\"))|(.)";
  static const reflex::Pattern PATTERN_INITIAL(REGEX_INITIAL);
  if (!has_matcher())
  {
    matcher(new Matcher(PATTERN_INITIAL, stdinit(), this));
  }
  while (true)
  {
        switch (matcher().scan())
        {
          case 0:
            if (matcher().at_end())
            {
              return ceammc::DataStringParser::symbol_type(0);
            }
            else
            {
              out().put(matcher().input());
            }
            break;
          case 1: // rule at line 54: (?:[\x09\x0a\x20]+)
#line 54 "data_string.l"
            break;
          case 2: // rule at line 55: null
#line 55 "data_string.l"
{ return token::TOK_NULL; }
            break;
          case 3: // rule at line 56: true
#line 56 "data_string.l"
{ return DataStringParser::make_FLOAT(1); }
            break;
          case 4: // rule at line 57: false
#line 57 "data_string.l"
{ return DataStringParser::make_FLOAT(0); }
            break;
          case 5: // rule at line 58: S(?=(?:"(?:(?:[\x00-!]|[\x23-_]|[a-\x7f]|[\xc2-\xdf][\x80-\xbf]|\xe0[\xa0-\xbf][\x80-\xbf]|[\xe1-\xec][\x80-\xbf][\x80-\xbf]|\xed[\x80-\x9f][\x80-\xbf]|[\xee\xef][\x80-\xbf][\x80-\xbf]|\xf0[\x90-\xbf][\x80-\xbf][\x80-\xbf]|[\xf1-\xf3][\x80-\xbf][\x80-\xbf][\x80-\xbf]|\xf4[\x80-\x8f][\x80-\xbf][\x80-\xbf])|`"|``|`\.|`:|`\(|`\))*"))
#line 58 "data_string.l"
{ return DataStringParser::make_DATA_TYPE_STRING(); }
            break;
          case 6: // rule at line 59: (?:[A-Z][A-Za-z]*)(?=(?:\())
#line 59 "data_string.l"
{ return DataStringParser::make_DATA_TYPE(text()); }
            break;
          case 7: // rule at line 60: (?:[A-Z][A-Za-z]*)(?=(?:\[))
#line 60 "data_string.l"
{ return DataStringParser::make_DATA_TYPE(text()); }
            break;
          case 8: // rule at line 61: (?:\[)
#line 61 "data_string.l"
{ return DataStringParser::make_OPEN_DICT_BRACKET(); }
            break;
          case 9: // rule at line 62: (?:\])
#line 62 "data_string.l"
{ return DataStringParser::make_CLOSE_DICT_BRACKET(); }
            break;
          case 10: // rule at line 63: (?:\()
#line 63 "data_string.l"
{ return DataStringParser::make_OPEN_LIST_BRACKET(); }
            break;
          case 11: // rule at line 64: (?:\))
#line 64 "data_string.l"
{ return DataStringParser::make_CLOSE_LIST_BRACKET(); }
            break;
          case 12: // rule at line 65: (?:(?:[\x2b\x2d]?(?:0|[1-9][0-9]*))(?:\.[0-9]+)?)
#line 65 "data_string.l"
{ return DataStringParser::make_FLOAT(std::strtod(text(), 0)); }
            break;
          case 13: // rule at line 66: (?:[\x2b\x2d]?0[Xx][0-9A-Fa-f]+)
#line 66 "data_string.l"
{
                          try {
                           long hex = std::stoi(text(), 0, 16);
                           return DataStringParser::make_FLOAT(hex);
                          }
                          catch(std::exception& e) {
                             LIB_ERR << "invalid hex: " << e.what() << " - " << text();
                             return token::TOK_LEXER_ERROR;
                          }
                        }
            break;
          case 14: // rule at line 76: (?:0b[01]+)
#line 76 "data_string.l"
{
                          std::string str(text() + 2);
                          try {
                           long bin = std::stoi(str.c_str(), 0, 2);
                           return DataStringParser::make_FLOAT(bin);
                          }
                          catch(std::exception& e) {
                             LIB_ERR << "invalid bin: " << e.what() << " - " << text();
                             return token::TOK_LEXER_ERROR;
                          }
                        }
            break;
          case 15: // rule at line 87: (?:(?:[\x23\x2e0-9@-Z_a-z])+:)
#line 87 "data_string.l"
{
                          std::string key(str());
                          key.pop_back(); // remove trailing ':'
                          return DataStringParser::make_KEY(key);
                        }
            break;
          case 16: // rule at line 92: (?:(?:[\x23\x2e0-9@-Z_a-z])(?:[\x23'*+\x2d-:<-Z_a-z\x7c])*)
#line 92 "data_string.l"
{ return DataStringParser::make_SYMBOL(text()); }
            break;
          case 17: // rule at line 93: (?:"(?:(?:[\x00-!]|[\x23-_]|[a-\x7f]|[\xc2-\xdf][\x80-\xbf]|\xe0[\xa0-\xbf][\x80-\xbf]|[\xe1-\xec][\x80-\xbf][\x80-\xbf]|\xed[\x80-\x9f][\x80-\xbf]|[\xee\xef][\x80-\xbf][\x80-\xbf]|\xf0[\x90-\xbf][\x80-\xbf][\x80-\xbf]|[\xf1-\xf3][\x80-\xbf][\x80-\xbf][\x80-\xbf]|\xf4[\x80-\x8f][\x80-\xbf][\x80-\xbf])|`"|``|`\.|`:|`\(|`\))*")
#line 93 "data_string.l"
{
                          std::string str(text() + 1);
                          str.pop_back();
                          return DataStringParser::make_STRING(str);
                        }

            break;
          case 18: // rule at line 99: .
#line 99 "data_string.l"
{
                          std::string line = matcher().line();
                          std::string err;

                          if(strcmp("`", text()) == 0)
                            err = " - stray ` found, use quotes: \"``\"";
                          else if(strcmp("\"", text()) == 0)
                            err = " - unmatched \" found, use quotes: \"`\"\"";
                          else if(strcmp("[", text()) == 0)
                            err = " - unmatched [ found, use quotes: \"[\"";
                          else if(strcmp("]", text()) == 0)
                            err = " - unmatched ] found, use quotes: \"]\"";
                          else if(strcmp("(", text()) == 0)
                            err = " - unmatched '(' found, use quotes: \"(\"";
                          else
                            err = fmt::format("unmatched lexer input: '{}'", text());

                          LIB_ERR << fmt::format("error while parsing '{}': {}", line, err);

                          return token::TOK_LEXER_ERROR; /* error */
                        }

            break;
        }
  }
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  SECTION 3: user code                                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#line 122 "data_string.l"

# include "fmt/format.h"
