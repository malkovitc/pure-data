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
#include "../replace.h"
#include "base_extension_test.h"
#include "catch.hpp"

#include <stdio.h>

typedef TestExtension<Replace> ReplaceTest;

TEST_CASE("replace", "[PureData]")
{
    obj_init();

    SECTION("test create with:")
    {
        SECTION("empty arguments")
        {
            AtomList args;
            ReplaceTest t("replace", args);
            REQUIRE(t.numInlets() == 3);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE(t.hasProperty("@from"));
            REQUIRE(t.hasProperty("@to"));

            REQUIRE_PROPERTY_NONE(t, @from);
            REQUIRE_PROPERTY_NONE(t, @to);

            WHEN_SEND_FLOAT_TO(0, t, 12);
            REQUIRE_FLOAT_AT_OUTLET(0, t, 12);

            WHEN_SEND_SYMBOL_TO(0, t, "abc");
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "abc");

            WHEN_SEND_LIST_TO(0, t, AtomList(10, 12));
            REQUIRE_LIST_AT_OUTLET(0, t, AtomList(10, 12));
        }

        SECTION("properties")
        {
            AtomList args;
            args.append(AtomList(gensym("@from"), 1));
            args.append(AtomList(gensym("@to"), 1000));

            ReplaceTest t("replace", args);

            REQUIRE_PROP(t, from, Atom(1));
            REQUIRE_PROP(t, to, Atom(1000));

            WHEN_SEND_FLOAT_TO(0, t, 12);
            REQUIRE_FLOAT_AT_OUTLET(0, t, 12);

            WHEN_SEND_FLOAT_TO(0, t, 1);
            REQUIRE_FLOAT_AT_OUTLET(0, t, 1000);

            WHEN_SEND_SYMBOL_TO(0, t, "abc");
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "abc");

            WHEN_SEND_LIST_TO(0, t, AtomList(13, 1));
            REQUIRE_LIST_AT_OUTLET(0, t, AtomList(13, 1000));
        }

        SECTION("positional arguments")
        {
            AtomList args(gensym("a"), gensym("b"));
            ReplaceTest t("replace", args);

            REQUIRE_PROP(t, from, S("a"));
            REQUIRE_PROP(t, to, S("b"));

            WHEN_SEND_SYMBOL_TO(0, t, "a");
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "b");

            WHEN_SEND_SYMBOL_TO(0, t, "abc");
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "abc");
        }
    }

    SECTION("test cold inlets")
    {
        AtomList args;
        ReplaceTest t("replace", args);

        REQUIRE_PROPERTY_NONE(t, @from);
        REQUIRE_PROPERTY_NONE(t, @to);

        WHEN_SEND_FLOAT_TO(1, t, 123456789);
        REQUIRE_PROPERTY(t, @from, F(123456789));
        REQUIRE_PROPERTY_NONE(t, @to);

        WHEN_SEND_SYMBOL_TO(1, t, "a");
        REQUIRE_PROPERTY(t, @from, S("a"));
        REQUIRE_PROPERTY_NONE(t, @to);

        WHEN_SEND_FLOAT_TO(2, t, 777);
        REQUIRE_PROPERTY(t, @to, F(777));
        REQUIRE_PROPERTY(t, @from, S("a"));

        WHEN_SEND_SYMBOL_TO(2, t, "b");
        REQUIRE_PROPERTY(t, @to, S("b"));
        REQUIRE_PROPERTY(t, @from, S("a"));

        WHEN_SEND_LIST_TO(1, t, AtomList());
        REQUIRE_PROPERTY_NONE(t, @from);
        REQUIRE_PROPERTY(t, @to, S("b"));

        WHEN_SEND_LIST_TO(2, t, AtomList());
        REQUIRE_PROPERTY_NONE(t, @from);
        REQUIRE_PROPERTY_NONE(t, @to);
    }

    SECTION("replace:")
    {
        SECTION("float")
        {
            AtomList args;
            ReplaceTest t("replace", args);

            WHEN_SEND_FLOAT_TO(0, t, 1);
            REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

            // remove mode
            t.setProperty("@from", F(2));

            WHEN_SEND_FLOAT_TO(0, t, 1);
            REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

            WHEN_SEND_FLOAT_TO(0, t, 2);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            // replace mode
            t.setProperty("@from", F(2));
            t.setProperty("@to", F(2222));

            WHEN_SEND_FLOAT_TO(0, t, 1);
            REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

            WHEN_SEND_FLOAT_TO(0, t, 2);
            REQUIRE_FLOAT_AT_OUTLET(0, t, 2222);

            // self replace
            t.setProperty("@from", F(10));
            t.setProperty("@to", F(10));

            WHEN_SEND_FLOAT_TO(0, t, 1);
            REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

            WHEN_SEND_FLOAT_TO(0, t, 10);
            REQUIRE_FLOAT_AT_OUTLET(0, t, 10);

            // replace to other type
            t.setProperty("@from", F(2));
            t.setProperty("@to", S("2"));

            WHEN_SEND_FLOAT_TO(0, t, 1);
            REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

            WHEN_SEND_FLOAT_TO(0, t, 2);
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "2");

            // replace other types
            t.setProperty("@from", S("a"));
            t.setProperty("@to", S("b"));

            for (size_t i = 0; i < 10; i++) {
                WHEN_SEND_FLOAT_TO(0, t, i);
                REQUIRE_FLOAT_AT_OUTLET(0, t, i);
            }
        }
    }
}
