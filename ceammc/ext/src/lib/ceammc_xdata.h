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
#ifndef CEAMMC_XDATA_H
#define CEAMMC_XDATA_H

#include "ceammc_abstractdata.h"

namespace ceammc {

class DataPtr {
    DataDesc desc_;
    const AbstractData* data_;
    DataPtr();

public:
    DataPtr(AbstractData* data);
    DataPtr(const Atom& data);
    DataPtr(const DataPtr& d);
    DataPtr& operator=(const DataPtr& d);
    ~DataPtr();

    bool isValid() const;
    ceammc::DataDesc desc() const;
    size_t refCount() const;

    const AbstractData* data() const;
    const AbstractData* operator->() const;

    template <class T>
    const T* as() const;

    Atom asAtom() const;

    bool operator==(const DataPtr& d);
    bool operator!=(const DataPtr& d);
};

template <class T>
const T* DataPtr::as() const
{
    if (data_ == 0)
        return 0;

    return data_->as<T>();
}
}

#endif // CEAMMC_XDATA_H
