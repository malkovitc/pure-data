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
#include "catch.hpp"
#include "ceammc_data.h"
#include "test_datatypes.h"

using namespace ceammc;

using IntA = DataAtom<IntData>;
using StrA = DataAtom<StrData>;

static Atom copyFirst(const AtomList& l) { return l.at(0); }

TEST_CASE("DataAtom", "[core]")
{
    REQUIRE(IntData::constructor_called == 0);
    REQUIRE(IntData::destructor_called == 0);

    SECTION("direct instantiation")
    {
        AtomList l0(IntA(10), StrA("test string"));
        AtomList l1({ IntA(10), StrA("test string") });

        AtomList l3 = l0;
        Atom a0 = copyFirst(l1);
        IntA i0(IntA(20));
        Atom a1 = IntA(Atom(new IntData(100)));
    }

    REQUIRE(IntData::constructor_called == 4);
    REQUIRE(IntData::destructor_called == 4);
}
