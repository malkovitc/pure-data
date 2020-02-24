/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#ifndef TREEAT_H
#define TREEAT_H

#include "ceammc_object.h"
#include "datatype_tree.h"

using namespace ceammc;

class TreeAt : public BaseObject {
    IntProperty* index_;
    DataTPtr<DataTypeTree> ptr_;

public:
    TreeAt(const PdArgs& args);
    void onBang() final;
    void onInlet(size_t idx, const AtomList& lst) final;
    void onDataT(const DataTPtr<DataTypeTree>& dptr);
};

void setup_tree_at();

#endif // TREEAT_H
