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
#ifndef DATATYPE_MLIST_H
#define DATATYPE_MLIST_H

#include "ceammc_abstractdata.h"
#include "ceammc_atomlist.h"

#include <boost/optional.hpp>

struct mlist_node;

namespace ceammc {

class DataTypeMList : public AbstractData {
    AtomList data_;

public:
    using iterator = AtomList::iterator;
    using const_iterator = AtomList::const_iterator;
    using filter_iterator = AtomList::atom_filter_iterator;
    using const_filter_iterator = AtomList::const_atom_filter_iterator;

public:
    DataTypeMList();
    DataTypeMList(const std::string& str);
    DataTypeMList(const AtomList& lst);
    DataTypeMList(AtomList&& lst) noexcept;

    template <typename... Args>
    explicit DataTypeMList(Args... args)
        : DataTypeMList(AtomList(args...))
    {
    }

    // copy/move
    DataTypeMList(const DataTypeMList& mlist);
    DataTypeMList(DataTypeMList&& mlist) noexcept;
    DataTypeMList& operator=(const DataTypeMList& mlist);
    DataTypeMList& operator=(DataTypeMList&& mlist);

    int type() const final;
    DataTypeMList* clone() const final;
    bool isEqual(const AbstractData* cmp) const final;
    void dump() override;
    std::string toString() const final;

    AtomList& data() { return data_; }
    const AtomList& data() const { return data_; }
    template <class Fn>
    AtomList toList(Fn pred) const;

    // main
    bool empty() const;
    size_t size() const;
    const Atom& at(size_t n) const;
    Atom& at(size_t n);
    const Atom& operator[](size_t n) const;
    Atom& operator[](size_t n);
    void append(const Atom& a);
    void append(const AtomList& lst);
    void clear();
    bool insert(size_t idx, const AtomList& lst);
    void prepend(const Atom& a);
    void prepend(const AtomList& lst);
    bool pop();
    bool remove(size_t idx);
    void reserve(size_t n);
    void set(const AtomList& lst);

    const_iterator begin() const { return data_.begin(); }
    iterator begin() { return data_.begin(); }
    const_iterator end() const { return data_.end(); }
    iterator end() { return data_.end(); }

    const_filter_iterator begin_filter(AtomPredicate pred) const { return data_.begin_atom_filter(pred); }
    filter_iterator begin_filter(AtomPredicate pred) { return data_.begin_atom_filter(pred); }
    const_filter_iterator end_filter() const { return data_.end_atom_filter(); }
    filter_iterator end_filter() { return data_.end_atom_filter(); }

    // fn
    DataTypeMList rotateLeft(int steps) const;
    DataTypeMList flatten() const;
    DataTypeMList slice(int start, int end = -1, size_t step = 1) const;
    // sort only floats and symbols
    void sort();
    void reverse();
    void shuffle();
    bool contains(const Atom& a) const;
    bool contains(const DataTypeMList& l) const;

public:
    typedef std::vector<DataTypeMList*> MListStack;
    typedef boost::optional<DataTypeMList> MaybeList;

public:
    static int dataType;
    static MaybeList parse(const AtomList& lst);
    static MaybeList parse(const std::string& lst);
    static void traverse(mlist_node* node, DataTypeMList* data, MListStack* stack, int act, const char* txt);
};

template <class Fn>
AtomList DataTypeMList::toList(Fn pred) const
{
    AtomList res;
    res.reserve(size());

    for (auto& el : data_) {
        if (pred(el))
            res.append(el);
    }

    return res;
}

}

#endif // DATATYPE_MLIST_H
