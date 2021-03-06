/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "load_msg.h"
#include "ceammc_factory.h"

LoadMsg::LoadMsg(const PdArgs& args)
    : BaseObject(args)
{
    if (!args.args.empty()) {
        if (args.args.isFloat())
            msg_.setFloat(args.args[0].asFloat());
        else if (args.args[0].isFloat() && args.args.size() > 1)
            msg_.setList(args.args);
        else if (args.args[0].isSymbol())
            msg_.setAny(args.args[0].asSymbol(), args.args.slice(1));
    }

    createOutlet();
}

void LoadMsg::output()
{
    if (msg_.isNone())
        bangTo(0);
    else
        msg_.output(outletAt(0));
}

void LoadMsg::parseProperties()
{
}

bool LoadMsg::processAnyProps(t_symbol* /*sel*/, const AtomList& /*lst*/)
{
    return true;
}

void LoadMsg::onClick(t_floatarg /*xpos*/, t_floatarg /*ypos*/, t_floatarg /*shift*/, t_floatarg /*ctrl*/, t_floatarg /*alt*/)
{
    output();
}

void LoadMsg::onLoadBang()
{
    output();
}

void setup_load_msg()
{
    ObjectFactory<LoadMsg> obj("msg.onload", OBJECT_FACTORY_NO_DEFAULT_INLET);
    obj.addAlias("loadmsg");
    obj.useClick();
    obj.useLoadBang();
}
