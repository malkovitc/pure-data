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
#include "base_extension_test.h"
#include "ceammc_music_theory.h"

#include "catch.hpp"

#include <sstream>

using namespace ceammc;
using namespace ceammc::music;

#define REQUIRE_STR(P, STR)      \
    {                            \
        std::ostringstream s;    \
        s << PitchName::P;       \
        REQUIRE(s.str() == STR); \
    }

#define REQUIRE_DISTANCE(p1, p2, d) REQUIRE(PitchName::distance(PitchName::p1, PitchName::p2) == d)
#define REQUIRE_MIN_DISTANCE(p1, p2, d) REQUIRE(PitchName::minDistance(PitchName::p1, PitchName::p2) == d)

TEST_CASE("MusicTheory::PitchName", "[ceammc::music]")
{
    SECTION("equal")
    {
        REQUIRE(PitchName::C == PitchName::C);
        REQUIRE(PitchName::D == PitchName::D);
        REQUIRE(PitchName::E == PitchName::E);
        REQUIRE(PitchName::F == PitchName::F);
        REQUIRE(PitchName::G == PitchName::G);
        REQUIRE(PitchName::A == PitchName::A);
        REQUIRE(PitchName::B == PitchName::B);

        REQUIRE(PitchName::C != PitchName::D);
        REQUIRE(PitchName::D != PitchName::E);
        REQUIRE(PitchName::E != PitchName::F);
    }

    SECTION("add")
    {
        REQUIRE(PitchName::C + 1 == PitchName::D);
        REQUIRE(PitchName::C + 2 == PitchName::E);
        REQUIRE(PitchName::C + 3 == PitchName::F);
        REQUIRE(PitchName::C + 4 == PitchName::G);
        REQUIRE(PitchName::C + 5 == PitchName::A);
        REQUIRE(PitchName::C + 6 == PitchName::B);
        REQUIRE(PitchName::C + 7 == PitchName::C);
        REQUIRE(PitchName::C + 50 == PitchName::D);
        REQUIRE(PitchName::C + 702 == PitchName::E);

        REQUIRE(PitchName::C + -1 == PitchName::B);
        REQUIRE(PitchName::C + -2 == PitchName::A);
        REQUIRE(PitchName::C + -3 == PitchName::G);
        REQUIRE(PitchName::C + -4 == PitchName::F);
        REQUIRE(PitchName::C + -5 == PitchName::E);
        REQUIRE(PitchName::C + -6 == PitchName::D);
        REQUIRE(PitchName::C + -7 == PitchName::C);
        REQUIRE(PitchName::C + -50 == PitchName::B);
        REQUIRE(PitchName::C + -702 == PitchName::A);

        REQUIRE(PitchName::C - 1 == PitchName::B);
        REQUIRE(PitchName::C - 2 == PitchName::A);
        REQUIRE(PitchName::C - 3 == PitchName::G);
        REQUIRE(PitchName::C - 4 == PitchName::F);
        REQUIRE(PitchName::C - 5 == PitchName::E);
        REQUIRE(PitchName::C - 6 == PitchName::D);
        REQUIRE(PitchName::C - 7 == PitchName::C);
        REQUIRE(PitchName::C - 50 == PitchName::B);
        REQUIRE(PitchName::C - 702 == PitchName::A);

        REQUIRE(PitchName::C - -1 == PitchName::D);
        REQUIRE(PitchName::C - -2 == PitchName::E);
        REQUIRE(PitchName::C - -3 == PitchName::F);
        REQUIRE(PitchName::C - -4 == PitchName::G);
        REQUIRE(PitchName::C - -5 == PitchName::A);
        REQUIRE(PitchName::C - -6 == PitchName::B);
        REQUIRE(PitchName::C - -7 == PitchName::C);
        REQUIRE(PitchName::C - -50 == PitchName::D);
        REQUIRE(PitchName::C - -702 == PitchName::E);
    }

    SECTION("operator<<")
    {
        REQUIRE_STR(C, "C");
        REQUIRE_STR(D, "D");
        REQUIRE_STR(E, "E");
        REQUIRE_STR(F, "F");
        REQUIRE_STR(G, "G");
        REQUIRE_STR(A, "A");
        REQUIRE_STR(B, "B");
    }

    SECTION("distance")
    {
        REQUIRE_DISTANCE(C, C, 0);
        REQUIRE_DISTANCE(C, D, 1);
        REQUIRE_DISTANCE(D, C, 1);
        REQUIRE_DISTANCE(C, E, 2);
        REQUIRE_DISTANCE(C, B, 6);
        REQUIRE_DISTANCE(B, C, 6);
    }

    SECTION("minDistance")
    {
        REQUIRE_MIN_DISTANCE(C, C, 0);
        REQUIRE_MIN_DISTANCE(C, D, 1);
        REQUIRE_MIN_DISTANCE(D, C, 1);
        REQUIRE_MIN_DISTANCE(C, E, 2);
        REQUIRE_MIN_DISTANCE(C, B, 1);
        REQUIRE_MIN_DISTANCE(B, C, 1);
        REQUIRE_MIN_DISTANCE(C, F, 3);
        REQUIRE_MIN_DISTANCE(C, G, 3);
        REQUIRE_MIN_DISTANCE(C, A, 2);
    }
}
