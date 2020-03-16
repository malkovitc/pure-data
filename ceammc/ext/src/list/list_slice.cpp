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
#include "list_slice.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

ListSlice::ListSlice(const PdArgs& a)
    : BaseObject(a)
    , from_(nullptr)
    , step_(nullptr)
    , to_(nullptr)
{
    createOutlet();

    from_ = new IntProperty("@from", 0);
    from_->setArgIndex(0);
    to_ = new IntProperty("@to", -1);
    to_->setArgIndexNext(from_);
    step_ = new SizeTProperty("@step", 1);
    step_->setArgIndexNext(to_);

    createProperty(from_);
    createProperty(to_);
    createProperty(step_);
}

void ListSlice::onList(const AtomList& l)
{
    listTo(0, l.slice(from_->value(), to_->value(), step_->value()));
}

void ListSlice::onDataT(const DataTypeMList& l)
{
    DataTypeMList* res = new DataTypeMList(l.slice(from_->value(), to_->value(), step_->value()));
    dataTo(0, DataPtr(res));
}

void setup_list_slice()
{
    ObjectFactory<ListSlice> obj("list.slice");
}
