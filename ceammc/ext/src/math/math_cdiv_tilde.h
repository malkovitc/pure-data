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
#ifndef MATH_CDIV_TILDE_H
#define MATH_CDIV_TILDE_H

#include "ceammc_sound_external.h"

using namespace ceammc;

class MathComplexDivTilde : public SoundExternal {
public:
    MathComplexDivTilde(const PdArgs& args);
    void processBlock(const t_sample** in, t_sample** out) override;
};

void setup_math_cdiv_tilde();

#endif // MATH_CDIV_TILDE_H
