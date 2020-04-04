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
#include "list_length.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListLength, list, length)

using TExt = TestExtListLength;
using TObj = TestListLength;

TEST_CASE("list.length", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        TestListLength t("list.length");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
    }

    SECTION("do")
    {
        TestExtListLength t("list.length");

        t << L();
        REQUIRE_THAT(t, outputFloat(&t, 0));
        t << LF(1);
        REQUIRE_THAT(t, outputFloat(&t, 1));
        t << LF(1, 2);
        REQUIRE_THAT(t, outputFloat(&t, 2));

        t.send(MLA("a", "b", "c"));
        REQUIRE_THAT(t, outputFloat(&t, 3));
        t.send(MLA());
        REQUIRE_THAT(t, outputFloat(&t, 0));
    }

    SECTION("alias")
    {
        TestExtListLength t1("list.length");
        TestExtListLength t2("list.size");

        REQUIRE(t1.object());
        REQUIRE(t2.object());
    }
}
