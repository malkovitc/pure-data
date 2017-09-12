#ifndef PARAM_FLOAT_H
#define PARAM_FLOAT_H

#include "ceammc_object.h"

#include <string>
#include <vector>

using namespace ceammc;

class PresetFloat : public BaseObject {
    SizeTProperty* max_;
    FloatProperty* init_;
    SymbolProperty* name_;
    t_float current_value_;
    t_canvas* cnv_;
    t_symbol* bind_addr_;

public:
    PresetFloat(const PdArgs& args);
    ~PresetFloat();

    void onFloat(float f);

    void m_clear(t_symbol*, const AtomList&);
    void m_store(t_symbol*, const AtomList& l);
    void m_load(t_symbol*, const AtomList& l);

private:
    std::string makeParamFilePath() const;
};

void setup_preset_float();

#endif // PARAM_FLOAT_H