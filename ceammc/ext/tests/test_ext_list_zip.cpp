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
#include "../list/list_zip.h"
#include "test_base.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(ListZip, list, zip);

TEST_CASE("list.zip", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("default") {
            TestListZip t("list.zip");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
        }

        SECTION("args") {
            TestListZip t("list.zip", LF(4));
            REQUIRE(t.numInlets() == 4);
            REQUIRE(t.numOutlets() == 1);
        }

        SECTION("args") {
            TestListZip t("list.zip", LF(20));
            REQUIRE(t.numInlets() == 20);
            REQUIRE(t.numOutlets() == 1);
        }

        SECTION("invalid") {
            SECTION("1") {
                TestListZip t("list.zip", LF(1));
                REQUIRE(t.numInlets() == 2);
                REQUIRE(t.numOutlets() == 1);
            }

            SECTION("-10") {
                TestListZip t("list.zip", LF(-10));
                REQUIRE(t.numInlets() == 2);
                REQUIRE(t.numOutlets() == 1);
            }

            SECTION("21") {
                TestListZip t("list.zip", LF(20));
                REQUIRE(t.numInlets() == 20);
                REQUIRE(t.numOutlets() == 1);
            }
        }
    }

    SECTION("act") {
        SECTION("default") {
            TestListZip t("list.zip");

            // default @min method
            WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
            REQUIRE_LIST_AT_OUTLET(0, t, L());
            REQUIRE_PROPERTY_LIST(t, @l0, LF(1, 2, 3));

            // interleave with 4,5,6
            WHEN_SEND_LIST_TO(1, t, LF(4, 5, 6));
            REQUIRE_NO_MSG(t);
            REQUIRE_PROPERTY_LIST(t, @l1, LF(4, 5, 6));

            WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
            REQUIRE_LIST_AT_OUTLET(0, t, LA(1, 4, 2, 5, 3, 6));

            WHEN_SEND_LIST_TO(0, t, LF(1, 2));
            REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 4, 2, 5));

            WHEN_SEND_LIST_TO(0, t, LF(1));
            REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 4));

            WHEN_SEND_LIST_TO(0, t, L());
            REQUIRE_LIST_AT_OUTLET(0, t, L());

            // interleave with 4,5
            WHEN_SEND_LIST_TO(1, t, LF(4, 5));
            REQUIRE_NO_MSG(t);

            WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
            REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 4, 2, 5));

            WHEN_SEND_LIST_TO(0, t, LF(1, 2));
            REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 4, 2, 5));

            WHEN_SEND_LIST_TO(0, t, LF(1));
            REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 4));

            WHEN_SEND_LIST_TO(0, t, L());
            REQUIRE_LIST_AT_OUTLET(0, t, L());

            // interleave with 4
            WHEN_SEND_LIST_TO(1, t, LF(4));
            REQUIRE_NO_MSG(t);

            WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
            REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 4));

            WHEN_SEND_LIST_TO(0, t, LF(1, 2));
            REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 4));

            WHEN_SEND_LIST_TO(0, t, LF(1));
            REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 4));

            WHEN_SEND_LIST_TO(0, t, L());
            REQUIRE_LIST_AT_OUTLET(0, t, L());

            // interleave with ()
            WHEN_SEND_LIST_TO(1, t, L());
            REQUIRE_NO_MSG(t);

            WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
            REQUIRE_LIST_AT_OUTLET(0, t, L());

            WHEN_SEND_LIST_TO(0, t, LF(1, 2));
            REQUIRE_LIST_AT_OUTLET(0, t, L());

            WHEN_SEND_LIST_TO(0, t, LF(1));
            REQUIRE_LIST_AT_OUTLET(0, t, L());

            WHEN_SEND_LIST_TO(0, t, L());
            REQUIRE_LIST_AT_OUTLET(0, t, L());
        }

        SECTION("multiple") {
            TestListZip t("list.zip", LF(3));

            // default @min method
            WHEN_SEND_LIST_TO(0, t, LF(1, 2));
            REQUIRE_LIST_AT_OUTLET(0, t, L());
            REQUIRE_PROPERTY_LIST(t, @l0, LF(1, 2));

            // interleave with 3,4 and 5,6
            WHEN_SEND_LIST_TO(1, t, LF(3, 4));
            REQUIRE_PROPERTY_LIST(t, @l1, LF(3, 4));
            REQUIRE_NO_MSG(t);
            WHEN_SEND_LIST_TO(2, t, LF(5, 6));
            REQUIRE_PROPERTY_LIST(t, @l2, LF(5, 6));
            REQUIRE_NO_MSG(t);

            WHEN_SEND_LIST_TO(0, t, LF(1, 2));
            REQUIRE_LIST_AT_OUTLET(0, t, LA(1, 3, 5, 2, 4, 6));
        }
    }
}