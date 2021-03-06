#include "list_reduce.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

ListReduce::ListReduce(const PdArgs& a)
    : BaseObject(a)
{
    createInlet();
    createOutlet();
    createOutlet();
}

void ListReduce::onList(const AtomList& l)
{
    if (l.empty())
        return;

    accum_ = l[0];
    AtomList pair(0.f, 0.f);

    for (size_t i = 1; i < l.size(); i++) {
        pair[0] = accum_;
        pair[1] = l[i];
        listTo(1, pair);
    }

    atomTo(0, accum_);
}

void ListReduce::onInlet(size_t n, const AtomList& l)
{
    if (n != 1)
        return;

    accum_ = atomlistToValue<Atom>(l, Atom(0.f));
}

void ListReduce::onDataT(const MListAtom& ml)
{
    onList(ml->data());
}

void setup_list_reduce()
{
    ObjectFactory<ListReduce> obj("list.reduce");
    obj.addAlias("list.foldl");
    obj.useDefaultPdFloatFn();
    obj.useDefaultPdSymbolFn();
    obj.processData<DataTypeMList>();
}
