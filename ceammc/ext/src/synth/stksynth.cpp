#include "ADSR.h"
#include "Instrmnt.h"
#include "SineWave.h"

#include "ceammc_convert.h"
#include "stksynth.h"

#include "stksynth_p.h"

extern "C" {
#include "m_imp.h"
}

class RawpathInitializer {
    bool done_;
    RawpathInitializer()
        : done_(false)
    {
    }

public:
    static RawpathInitializer& instance()
    {
        static RawpathInitializer instance_;
        return instance_;
    }

    void init(t_class* c)
    {
        if (done_)
            return;

        stk::Stk::setRawwavePath(std::string(c->c_externdir->s_name) + "/stk/");
        done_ = true;
    }
};

StkBase::StkBase(const PdArgs& args, stk::Instrmnt* instr)
    : SoundExternal(args)
    , synth_(instr)
{
    createSignalOutlet();
}

StkBase::~StkBase()
{
    delete synth_;
}

void StkBase::setupDSP(t_signal** sp)
{
    SoundExternal::setupDSP(sp);
    stk::Stk::setSampleRate(samplerate());
}

void StkBase::processBlock(const t_sample**, t_sample** out)
{
    const size_t BS = blockSize();
    const size_t N = synth_->channelsOut();

    if (N == 1) {
        for (size_t i = 0; i < BS; i++)
            out[0][i] = synth_->tick();
    } else {
        for (size_t i = 0; i < BS; i++) {
            out[0][i] = synth_->tick();

            size_t T = std::min<size_t>(numOutputChannels(), N);
            for (size_t j = 0; j < T; j++)
                out[j][i] = synth_->lastFrame()[j];
        }
    }
}

StkSynth::StkSynth(const PdArgs& args, stk::Instrmnt* instr)
    : StkBase(args, instr)
    , freq_(nullptr)
    , gate_(0)
{
    freq_ = new FloatProperty("@freq", mtof(57));
    freq_->checkNonNegative();
    freq_->setUnitsHz();
    addProperty(freq_);

    createCbFloatProperty(
        "@gate",
        [this]() -> t_float { return gate_; },
        [this](t_float f) -> bool { return propSetGate(f); })
        ->setFloatCheck(PropValueConstraints::CLOSED_RANGE, 0, 1);
}

bool StkSynth::propSetGate(t_float f)
{
    if (f > 0)
        synth_->noteOn(freq_->value(), f);
    else
        synth_->noteOff(freq_->value());

    gate_ = f;
    return true;
}

void StkBase::m_cc(t_symbol* s, const AtomList& lst)
{
    if (!checkArgs(lst, ARG_INT, ARG_FLOAT, s))
        return;

    controlChange(lst[0].asInt(), lst[1].asFloat());
}

void StkBase::controlChange(int n, t_float val)
{
    OBJ_DBG << n << " -> " << val;

    synth_->controlChange(n, val);
}

void StkBase::initRawWaves(t_class* c)
{
    RawpathInitializer::instance().init(c);
}

class FreqGetter : public stk::SineWave {
public:
    FreqGetter(const stk::SineWave* ptr)
        : stk::SineWave(*ptr)
    {
    }

    t_float freq() const
    {
        return (rate_ * stk::Stk::sampleRate()) / TABLE_SIZE;
    }
};

class ADSRGetter : public stk::ADSR {
public:
    ADSRGetter(const stk::ADSR* ptr)
        : stk::ADSR(*ptr)
    {
    }

    t_float target() const
    {
        return target_;
    }
};

SinWaveFreq::SinWaveFreq(const stk::SineWave* ptr)
    : ptr_(ptr)
{
}

t_float SinWaveFreq::getFrequency() const
{
    return FreqGetter(ptr_).freq();
}

ADSRProp::ADSRProp(const stk::ADSR* ptr)
    : ptr_(ptr)
{
}

t_float ADSRProp::getTarget() const
{
    return ADSRGetter(ptr_).target();
}
