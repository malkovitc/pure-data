/// this file is autogenerated, do not edit!
#include <cmath>

#include "ceammc_factory.h"
#include "ceammc_object.h"
#include "ceammc_sound_external.h"

using namespace ceammc;
        

class MathAbs : public BaseObject
{
    using FloatUnaryFn = t_float(*)(t_float);
public:
    MathAbs(const PdArgs& a) : BaseObject(a)
    {
        createOutlet();
    }

    void onFloat(t_float f) final
    {
        floatTo(0, std::fabs(f));
    }

    void onList(const AtomList& l) final
    {
        listTo(0, l.mapFloat(static_cast<FloatUnaryFn>(std::fabs)));
    }

};


class MathAbsTilde : public SoundExternal
{
public:
    MathAbsTilde(const PdArgs& a) : SoundExternal(a)
    {
        createSignalOutlet();
    }

    void processBlock(const t_sample** in, t_sample** out) final
    {
        const auto BS = blockSize();
        for(size_t i = 0; i < BS; i += 8) {
            out[0][i+0] = std::fabs(in[0][i+0]);
            out[0][i+1] = std::fabs(in[0][i+1]);
            out[0][i+2] = std::fabs(in[0][i+2]);
            out[0][i+3] = std::fabs(in[0][i+3]);
            out[0][i+4] = std::fabs(in[0][i+4]);
            out[0][i+5] = std::fabs(in[0][i+5]);
            out[0][i+6] = std::fabs(in[0][i+6]);
            out[0][i+7] = std::fabs(in[0][i+7]);
        }
    }
};


void setup_math_abs() {
    ObjectFactory<MathAbs> obj0("math.abs");
    SoundExternalFactory<MathAbsTilde> obj1("math.abs~");
}
        
