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
#include "../flow/flow_group.h"
#include "catch.hpp"
#include "test_base.h"

#include <stdio.h>

#define REQUIRE_SIZE(obj, n) REQUIRE_PROPERTY(t, @size, float(n));

#define REQUIRE_LIST(obj, lst)               \
    {                                        \
        WHEN_SEND_BANG_TO(0, obj);           \
        REQUIRE_SIZE(obj, lst.size());       \
        REQUIRE_LIST_AT_OUTLET(0, obj, lst); \
    }

typedef TestExternal<FlowGroup> FlowGroupTest;

TEST_CASE("flow.group", "[externals]")
{
    SECTION("create")
    {
        SECTION("empty")
        {
            FlowGroupTest t("flow.group");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY(t, @by, 1);
            REQUIRE_PROPERTY(t, @free, 0.f);
        }

        SECTION("raw args")
        {
            FlowGroupTest t("flow.group", LF(4, 5));
            REQUIRE_PROPERTY(t, @by, 4);
        }

        SECTION("props")
        {
            FlowGroupTest t("flow.group", LA("@by", 5));
            REQUIRE_PROPERTY(t, @by, 5);
        }
    }

    SECTION("onFloat")
    {
        FlowGroupTest t("flow.group", LF(2));

        WHEN_SEND_FLOAT_TO(0, t, 12);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_FLOAT_TO(0, t, 13);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_PROPERTY(t, @free, 2);

        WHEN_SEND_FLOAT_TO(0, t, 14);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(12, 13));
        REQUIRE_PROPERTY(t, @free, 1);

        WHEN_SEND_FLOAT_TO(0, t, 15);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_FLOAT_TO(0, t, 16);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(14, 15));
    }

    SECTION("onSymbol")
    {
        FlowGroupTest t("flow.group", LF(1));

        WHEN_SEND_SYMBOL_TO(0, t, "A");
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_SYMBOL_TO(0, t, "B");
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "A");

        WHEN_SEND_SYMBOL_TO(0, t, "C");
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "B");
    }

    SECTION("onList")
    {
        FlowGroupTest t("flow.group", LF(3));

        WHEN_SEND_LIST_TO(0, t, LA("A"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_LIST_TO(0, t, LA("B", "C"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_LIST_TO(0, t, LA("D", "E", "F"));
        REQUIRE_LIST_AT_OUTLET(0, t, LA("A", "B", "C"));

        WHEN_SEND_LIST_TO(0, t, LA("G", "H", "I", "J"));
        REQUIRE_LIST_AT_OUTLET(0, t, LA("G", "H", "I"));
    }

    SECTION("onData")
    {
        DataPtr d0(new IntData(123));
        DataPtr d1(new StrData("test"));
        DataPtr d2(new StrData("test2"));

        FlowGroupTest t("flow.group", LF(2));

        WHEN_SEND_DATA_TO(0, t, d0);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_DATA_TO(0, t, d1);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
    }
}
