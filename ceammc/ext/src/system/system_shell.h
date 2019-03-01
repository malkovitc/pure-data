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
#ifndef SYSTEM_SHELL_H
#define SYSTEM_SHELL_H

#include "ceammc_thread.h"

using namespace ceammc;

class ShellTask;

class SystemShell : public ThreadExternal {
    std::string line_buf_;
    FlagProperty* no_split_;
    std::unique_ptr<thread::Pipe> pipe_stdout_;

public:
    SystemShell(const PdArgs& args);
    ~SystemShell();

    void onSymbol(t_symbol* s) override;
    void onList(const AtomList& lst) override;
    void onAny(t_symbol* s, const AtomList& lst) override;
    void onThreadDone(int rc) override;
    bool onThreadCommand(int code) override;

    void m_terminate(t_symbol*, const AtomList&);
    void m_kill(t_symbol*, const AtomList&);

private:
    ShellTask* task();
    void sendSignal(int sig);
};

void setup_system_shell();

#endif // SYSTEM_SHELL_H