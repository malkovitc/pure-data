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
#include "datatype_mlist.h"
#include "../list/list_last.h"
#include "test_base.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(ListLast, list, last);

TEST_CASE("list.last", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        TestListLast t("list.insert");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
    }

    SECTION("do")
    {
        TestExtListLast t("list.last");

        t << L();
        REQUIRE_FALSE(t.hasOutput());
        t << LF(1000);
        REQUIRE(t.outputFloatAt(0) == Approx(1000));
        t << LF(100, 200, 300);
        REQUIRE(t.outputFloatAt(0) == Approx(300));

        t.send(DataTypeMList("()"));
        REQUIRE_FALSE(t.hasOutput());
        t.send(DataTypeMList("(1)"));
        REQUIRE(t.outputFloatAt(0) == Approx(1));
        t.send(DataTypeMList("(a b c)"));
        REQUIRE(t.outputSymbolAt(0) == gensym("c"));

        t.send(DataTypeMList("(a b c ())"));
        REQUIRE(t.outputDataAt(0) == DataPtr(new DataTypeMList));
        t.send(DataTypeMList("(a b c (1 2))"));
        REQUIRE(t.outputDataAt(0) == DataPtr(new DataTypeMList("(1 2)")));
    }
}