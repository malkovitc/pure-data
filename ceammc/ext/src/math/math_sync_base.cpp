/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#include "math_sync_base.h"

#include <cstdint>
#include <exception>
#include <type_traits>

namespace ceammc {

MathSyncBase::MathSyncBase(FloatBinFn fn, const PdArgs& args)
    : BaseObject(args)
    , prop_int_(nullptr)
    , v1_(0)
    , v2_(positionalFloatArgument(0, 0))
    , fn_(fn)
{
    prop_int_ = new FlagProperty("@int");
    createProperty(prop_int_);

    createInlet();
    createOutlet();
}

void MathSyncBase::onBang()
{
    try {

        if (prop_int_->value())
            floatTo(0, static_cast<IntType>(fn_(static_cast<IntType>(v1_), static_cast<IntType>(v2_))));
        else
            floatTo(0, fn_(v1_, v2_));

    } catch (std::exception& e) {
        OBJ_ERR << e.what();
    }
}

void MathSyncBase::onFloat(t_float f)
{
    v1_ = f;
    onBang();
}

void MathSyncBase::onInlet(size_t n, const AtomList& lst)
{
    if (n != 1)
        return;

    if (!lst.empty())
        v2_ = lst[0].asFloat();

    onBang();
}

void MathSyncBase::onList(const AtomList& lst)
{
    const auto N = lst.size();

    if (N > 0)
        v1_ = lst[0].asFloat();

    if (N > 1)
        v2_ = lst[1].asFloat();

    onBang();
}

enum BoolType {
    BOOL_FALSE = 0,
    BOOL_TRUE,
    BOOL_FLOAT
};

static BoolType float2bool(t_float f)
{
    if (f == 0)
        return BOOL_FALSE;
    else if (f == 1)
        return BOOL_TRUE;
    else
        return BOOL_FLOAT;
}

MathSyncBool::MathSyncBool(BoolBinFn fn, const PdArgs& args)
    : BaseObject(args)
    , v1_(false)
    , v2_(float2bool(positionalFloatArgument(0, 0)) != BOOL_FALSE)
    , fn_(fn)
{
    createInlet();
    createOutlet();

    if (float2bool(positionalFloatArgument(0, 0) == BOOL_FLOAT))
        OBJ_ERR << "1 or 0 expected";
}

void MathSyncBool::onBang()
{
    floatTo(0, fn_(v1_, v2_) ? 1 : 0);
}

void MathSyncBool::onFloat(t_float f)
{
    v1_ = checkBool(f);
    onBang();
}

void MathSyncBool::onInlet(size_t n, const AtomList& lst)
{
    if (n != 1)
        return;

    if (!lst.empty())
        v2_ = checkBool(lst[0].asFloat());

    onBang();
}

void MathSyncBool::onList(const AtomList& lst)
{
    const auto N = lst.size();

    if (N > 0)
        v1_ = checkBool(lst[0].asFloat());

    if (N > 1)
        v2_ = checkBool(lst[1].asFloat());

    onBang();
}

bool MathSyncBool::checkBool(t_float f) const
{
    auto t = float2bool(f);
    if (t == BOOL_FLOAT)
        OBJ_ERR << "1 or 0 expected";

    return t != BOOL_FALSE;
}
}
