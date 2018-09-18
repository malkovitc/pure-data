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
#ifndef NET_HOST_H
#define NET_HOST_H

#include "ceammc_thread.h"

using namespace ceammc;

class HostTask;

class NetHost : public ThreadExternal {

public:
    NetHost(const PdArgs& args);

    void onSymbol(t_symbol* s) override;
    void onAny(t_symbol* s, const AtomList&) override;
    void onThreadExit(int rc) override;

private:
    HostTask* task();
};

void setup_net_host();

#endif // NET_HOST_H
