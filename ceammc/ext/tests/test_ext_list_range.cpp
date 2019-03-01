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
#include "../list/list_range.h"
#include "test_base.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(ListRange, list, range);

TEST_CASE("list.range", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        TestListRange t("list.range");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
    }

    SECTION("do")
    {
        TestExtListRange t("list.range");

        t << L();
        REQUIRE_FALSE(t.hasOutput());
        t << LF(2);
        REQUIRE(t.outputListAt(0) == LX(2, 2));
    }

    SECTION("mlist")
    {
        TestExtListRange t("list.range");

        t.send(DataTypeMList());
        REQUIRE_FALSE(t.hasOutput());

        t.send(DataTypeMList("((200))"));
        REQUIRE_FALSE(t.hasOutput());

        t.send(DataTypeMList("(() 20)"));
        REQUIRE(t.outputListAt(0) == LF(20, 20));

        t.send(DataTypeMList("(() 20 40)"));
        REQUIRE(t.outputListAt(0) == LF(20, 40));
    }

    SECTION("aliases")
    {
        TestExtListRange t1("list.range");
        TestExtListRange t2("list.minmax");

        REQUIRE(t1.object());
        REQUIRE(t2.object());
    }
}