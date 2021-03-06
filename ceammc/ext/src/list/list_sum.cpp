#include "list_sum.h"
#include "datatype_mlist.h"
#include "ceammc_factory.h"

ListSum::ListSum(const PdArgs& a)
    : ListBase(a)
{
    createOutlet();
}

void ListSum::onFloat(t_float f)
{
    floatTo(0, f);
}

void ListSum::onList(const AtomList& l)
{
    auto s = l.sum();
    if (s == boost::none)
        return;

    floatTo(0, *s);
}

void setup_list_sum()
{
    ObjectFactory<ListSum> obj("list.sum");
    obj.processData<DataTypeMList>();
}
