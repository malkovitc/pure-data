/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#include "array_deviation.h"
#include "ceammc_factory.h"

#include <cmath>
#include <numeric>

ArrayDeviation::ArrayDeviation(const PdArgs& args)
    : ArrayBase(args)
{
    createOutlet();
}

void ArrayDeviation::onBang()
{
    if (!checkArray())
        return;

    const size_t N = array_.size();
    if (N < 2) {
        OBJ_ERR << "array size should be > 1";
        return;
    }

    t_sample sum = 0;
    t_sample sum2 = std::accumulate(array_.begin(), array_.end(), t_sample(0),
        [&sum](t_sample accum, t_sample x) {
            sum += x;
            return accum + x * x;
        });

    auto variance = (sum2 - (sum * sum) / N) / (N - 1);

    floatTo(0, std::sqrt(variance));
}

void setup_array_deviation()
{
    ObjectFactory<ArrayDeviation> obj("array.stddev");
}
