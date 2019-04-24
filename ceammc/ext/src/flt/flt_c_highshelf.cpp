#include "flt_c_highshelf.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FltCHighShelf : public faust_flt_c_highshelf_tilde {
public:
    FltCHighShelf(const PdArgs& args)
        : faust_flt_c_highshelf_tilde(args)
    {
        bindPositionalArgsToProps({ gensym("@freq"), gensym("@gain") });
    }
};

void setup_flt_c_highshelf_tilde()
{
    SoundExternalFactory<FltCHighShelf> obj("flt.c_highshelf~", OBJECT_FACTORY_DEFAULT);
}
