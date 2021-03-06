/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#ifndef ARG_IS_SYMBOL_H
#define ARG_IS_SYMBOL_H

#include "arg_checker_single.h"

#include "m_pd.h"

#include <cstdint>
#include <memory>

namespace re2 {
class RE2;
}

namespace ceammc {

class ArgIsSymbol : public ArgCheckerSingle {
public:
    enum CheckType : uint8_t {
        SYM_ANY = 0,
        SYM_EQUAL,
        SYM_NOT_EQUAL,
        SYM_BEGINS_WITH,
        SYM_ENDS_WITH,
        SYM_CONTAINS,
        SYM_MATCH
    };

public:
    ArgIsSymbol();
    ~ArgIsSymbol();

    void setCheck(CheckType t, const char* s);
    bool checkAtom(const Atom& a, CheckerContext& ctx) const final;
    std::string name() const final;

private:
    t_symbol* pattern_;
    CheckType check_type_;
    std::unique_ptr<re2::RE2> re_;
};

}

#endif // ARG_IS_SYMBOL_H
