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
#include "system_shell.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "datatype_string.h"
#include "process.hpp"

#include <array>
#include <cstdio>
#include <exception>
#include <iostream>
#include <list>
#include <memory>

extern "C" {
#include "s_stuff.h"
}

enum TerminateMethod {
    METHOD_NONE = 0,
    METHOD_TERM,
    METHOD_KILL
};

typedef int pipe_fd[2];

class ShellTask : public thread::Task {
    pipe_fd* stdout_fd_;
    std::string cmd_;

public:
    volatile TerminateMethod kill_;

public:
    ShellTask()
        : thread::Task()
        , stdout_fd_(nullptr)
        , kill_(METHOD_NONE)
    {
    }

    void setFd(pipe_fd* fd)
    {
        stdout_fd_ = fd;
    }

    void setCommand(const std::string& str)
    {
        cmd_ = str;
    }

    int run() override
    {
        using namespace TinyProcessLib;

        kill_ = METHOD_NONE;

        Process p(cmd_, {},
            [&](const char* bytes, size_t n) { write((*stdout_fd_)[1], bytes, n); },
            [&](const char* bytes, size_t n) { writeError(std::string(bytes, n).c_str()); });

        int rc = 0;
        while (!p.try_get_exit_status(rc)) {
            switch (kill_) {
            case METHOD_KILL:
                p.kill(true);
                break;
            case METHOD_TERM:
                p.kill(false);
                break;
            default:
                break;
            }
        }

        return rc;
    }
};

SystemShell::SystemShell(const PdArgs& args)
    : ThreadExternal(args, new ShellTask())
    , poll_stdout_(this, &SystemShell::readSubprocesOutput)
{
    task()->setFd(&poll_stdout_.fd);

    createOutlet();
    createOutlet();
}

SystemShell::~SystemShell()
{
    terminate(true);
}

void SystemShell::onList(const AtomList& lst)
{
    ShellTask* t = static_cast<ShellTask*>(task_);
    t->setCommand(to_string(lst));
    start();
}

void SystemShell::onThreadExit(int rc)
{
    floatTo(1, rc);
}

void SystemShell::readSubprocesOutput(int fd)
{
    std::array<char, 1024> buf;

    ssize_t n = read(fd, buf.data(), buf.size());
    if (n < 1)
        return;

    char* s = buf.data();

    for (size_t i = 0; i < n; i++) {
        if (s[i] != '\n') {
            line_buf_ += s[i];
        } else {
            line_buf_ += '\0';
            dataTo(0, DataPtr(new DataTypeString(line_buf_)));
            line_buf_.clear();
        }
    }
}

void SystemShell::m_terminate(t_symbol*, const AtomList&)
{
    terminate();
}

ShellTask* SystemShell::task()
{
    return static_cast<ShellTask*>(task_);
}

void SystemShell::terminate(bool force)
{
    ((ShellTask*)task_)->kill_ = force ? METHOD_KILL : METHOD_TERM;
}

void setup_system_shell()
{
    ObjectFactory<SystemShell> obj("system.shell");
    obj.addMethod("terminate", &SystemShell::m_terminate);
}
