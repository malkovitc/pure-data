#ifndef MLIST_IFACE_H_
#define MLIST_IFACE_H_

#include "ceammc_data.h"
#include "ceammc_object.h"
#include "data_protocol.h"
#include "datatype_mlist.h"

using namespace ceammc;

template <class T>
class DataMListIFace : public ListIFace<T> {
public:
    DataMListIFace(const PdArgs& a)
        : ListIFace<T>(a)
    {
    }

    void dump() const override
    {
        BaseObject::dump();
        OBJ_DBG << "list: " << mlist();
    }

    void onBang() override
    {
        this->atomTo(0, mlist());
    }

    void onList(const AtomList& lst) override
    {
        mlist()->setRaw(lst);
        onBang();
    }

    void onAny(t_symbol* s, const AtomList& lst) override
    {
        Message msg(s, lst);
        if (s->s_name[0] != '\(') {
            OBJ_ERR << "opening parenthesis expected: " << to_string(msg);
            return;
        }

        mlist()->setParsed(msg.anyValue());
    }

    void onDataT(const MListAtom& ml)
    {
        mlist() = ml;
        // this is important detach
        mlist().detachData();
        onBang();
    }

    void proto_set(const AtomList& lst) override
    {
        mlist()->setRaw(lst);
    }

    void proto_clear() override
    {
        mlist()->clear();
    }

    void proto_append(const AtomList& lst) override
    {
        mlist()->append(lst);
    }

    void proto_prepend(const AtomList& lst) override
    {
        mlist()->prepend(lst);
    }

    bool proto_insert(size_t idx, const AtomList& lst) override
    {
        return mlist()->insert(idx, lst);
    }

    bool proto_pop() override
    {
        auto n = mlist()->size();
        if (n < 1)
            return false;

        return mlist()->remove(n - 1);
    }

    bool proto_removeAt(size_t pos) override
    {
        return mlist()->remove(pos);
    }

    size_t proto_size() const override
    {
        return mlist()->size();
    }

    void proto_sort() override
    {
        mlist()->sort();
    }

    void proto_reverse() override
    {
        mlist()->reverse();
    }

    void proto_shuffle() override
    {
        mlist()->shuffle();
    }

    void proto_fill(const Atom& v) override
    {
        std::fill(mlist()->begin(), mlist()->end(), v);
    }

    virtual const MListAtom& mlist() const = 0;
    virtual MListAtom& mlist() = 0;
};

#endif
