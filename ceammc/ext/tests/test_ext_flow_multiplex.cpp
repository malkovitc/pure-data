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
#include "../flow/flow_multiplex.h"
#include "catch.hpp"
#include "ceammc_pd.h"
#include "test_base.h"

#include <stdio.h>

typedef TestExternal<FlowMultiplex> FlowMultiplexTest;

static CanvasPtr cnv = PureData::instance().createTopCanvas("test_canvas");

TEST_CASE("flow.multiplex", "[externals]")
{
    test::pdPrintToStdError();

    SECTION("init")
    {
        FlowMultiplexTest t("flow.multiplex");
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE_PROPERTY(t, @index, 0.f);

        // invalid
        SECTION("number")
        {
            FlowMultiplexTest t("flow.multiplex", LF(0.f));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @index, 0.f);
        }

        // invalid
        SECTION("number")
        {
            FlowMultiplexTest t("flow.multiplex", LF(-1));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @index, 0.f);
        }

        // invalid
        SECTION("number")
        {
            FlowMultiplexTest t("flow.demultiplex", LF(1000));
            REQUIRE(t.numInlets() == 24);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @index, 0.f);
        }

        // invalid
        SECTION("number")
        {
            FlowMultiplexTest t("flow.multiplex", LF(1));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @index, 0.f);
        }

        // valid
        SECTION("number")
        {
            FlowMultiplexTest t("flow.multiplex", LF(8));
            REQUIRE(t.numInlets() == 8);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @index, 0.f);
        }
    }

    SECTION("process")
    {
        FlowMultiplexTest t("flow.multiplex", LF(3));

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_BANG_AT_OUTLET(0, t);
        WHEN_SEND_FLOAT_TO(0, t, 100);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 100);
        WHEN_SEND_SYMBOL_TO(0, t, "A");
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "A");
        WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 2, 3));
        WHEN_SEND_ANY_TO(t, "test", LF(1, 2));
        REQUIRE_ANY_AT_OUTLET(0, t, LA("test", 1, 2));
        DataPtr dp(new IntData(123));
        WHEN_SEND_DATA_TO(0, t, dp);
        REQUIRE_DATA_AT_OUTLET(0, t, dp);

        WHEN_SEND_BANG_TO(1, t);
        REQUIRE_NO_MSG(t);
        WHEN_SEND_FLOAT_TO(1, t, 100);
        REQUIRE_NO_MSG(t);
        WHEN_SEND_SYMBOL_TO(1, t, "A");
        REQUIRE_NO_MSG(t);

        t.setProperty("@index", LF(1));

        WHEN_SEND_BANG_TO(1, t);
        REQUIRE_BANG_AT_OUTLET(0, t);
        WHEN_SEND_FLOAT_TO(1, t, 100);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 100);
        WHEN_SEND_SYMBOL_TO(1, t, "A");
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "A");
        WHEN_SEND_LIST_TO(1, t, LF(1, 2, 3));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 2, 3));
        WHEN_SEND_DATA_TO(1, t, dp);
        REQUIRE_DATA_AT_OUTLET(0, t, dp);

        t.setProperty("@index", LF(2));
        WHEN_SEND_BANG_TO(2, t);
        REQUIRE_BANG_AT_OUTLET(0, t);
        WHEN_SEND_FLOAT_TO(2, t, 100);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 100);
        WHEN_SEND_SYMBOL_TO(2, t, "A");
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "A");
        WHEN_SEND_LIST_TO(2, t, LF(1, 2, 3));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 2, 3));
        WHEN_SEND_DATA_TO(2, t, dp);
        REQUIRE_DATA_AT_OUTLET(0, t, dp);

        t.setProperty("@index", LF(3));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MSG(t);

        t.setProperty("@index", LF(-1));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MSG(t);
    }
}
