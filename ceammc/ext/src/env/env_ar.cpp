#include "env_ar.h"
#include "ceammc_clock.h"
#include "ceammc_factory.h"
#include "datatype_env.h"

using namespace ceammc;

class EnvAr : public faust_env_ar_tilde {
    ClockMemberFunction<EnvAr> auto_release_;
    ClockMemberFunction<EnvAr> done_;

    UIProperty* prop_attack_;
    UIProperty* prop_release_;
    UIProperty* prop_gate_;

public:
    EnvAr(const PdArgs& args)
        : faust_env_ar_tilde(args)
        , auto_release_(this, &EnvAr::release)
        , done_(this, &EnvAr::done)
        , prop_attack_((UIProperty*)property(gensym("@attack")))
        , prop_release_((UIProperty*)property(gensym("@release")))
        , prop_gate_((UIProperty*)property(gensym("@gate")))
    {
        bindPositionalArgsToProps({ gensym("@attack"), gensym("@release") });
        createProperty(new CombinedProperty("@ar", { property(gensym("@attack")), property(gensym("@release")) }));
        createCbProperty("@length", &EnvAr::propLength);

        createOutlet();
    }

    bool processAnyProps(t_symbol* sel, const AtomList& lst) override
    {
        if (sel == gensym("@gate") && atomlistToValue<bool>(lst, false)) {
            auto_release_.unset();
            done_.delay(length());
        }

        return faust_env_ar_tilde::processAnyProps(sel, lst);
    }

    void onBang() override
    {
        prop_gate_->setValue(1);
        auto_release_.delay(10);
        done_.delay(length());
    }

    void onList(const AtomList& l) override
    {
        if (!checkArgs(l, ARG_FLOAT, ARG_FLOAT)) {
            OBJ_ERR << "ATTACK RELEASE values expected: " << l;
            return;
        }

        if (!set(l[0].asFloat(), l[1].asFloat()))
            OBJ_ERR << "can't set envelope";
    }

    void onDataT(const DataTPtr<DataTypeEnv>& dptr)
    {
        if (!dptr->isAR()) {
            OBJ_ERR << "not an AR envelope: " << *dptr;
            return;
        }

        float attack = dptr->pointAt(1).timeMs() - dptr->pointAt(0).timeMs();
        float release = dptr->pointAt(2).timeMs() - dptr->pointAt(1).timeMs();

        if (!set(attack, release))
            OBJ_ERR << "can't set envelope";
    }

    void m_reset(t_symbol*, const AtomList&)
    {
        prop_gate_->setValue(0);
        dsp_->instanceClear();
        unsetClocks();
    }

    void m_click(t_symbol*, const AtomList& l)
    {
        onBang();
    }

    void m_play(t_symbol*, const AtomList&)
    {
        onBang();
    }

    AtomList propLength() const
    {
        return Atom(length());
    }

private:
    void release()
    {
        prop_gate_->setValue(0);
    }

    void done()
    {
        bangTo(1);
    }

    bool checkValues(float a, float r)
    {
        return a >= 0 && r >= 0;
    }

    void unsetClocks()
    {
        done_.unset();
        auto_release_.unset();
    }

    bool set(float a, float r)
    {
        if (!checkValues(a, r)) {
            OBJ_ERR << "invalid values: " << a << ", " << r;
            return false;
        }

        prop_attack_->setValue(a);
        prop_release_->setValue(r);
        return true;
    }

    float length() const
    {
        return prop_attack_->value() + prop_release_->value();
    }
};

void setup_env_ar_tilde()
{
    SoundExternalFactory<EnvAr> obj("env.ar~");
    obj.processData<DataTypeEnv>();
    obj.addMethod("play", &EnvAr::m_play);
    obj.addMethod("reset", &EnvAr::m_reset);
    obj.addClick(&EnvAr::m_click);
}
