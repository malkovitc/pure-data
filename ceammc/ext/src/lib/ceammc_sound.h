/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#ifndef CEAMMC_SOUND_H
#define CEAMMC_SOUND_H

#include <boost/shared_ptr.hpp>
#include <m_pd.h>
#include <string>
#include <vector>

namespace ceammc {
namespace sound {
    typedef std::vector<std::string> StringList;
    class SoundFile;
    typedef boost::shared_ptr<SoundFile> SoundFilePtr;

    class SoundFile {
        std::string fname_;

    public:
        SoundFile(const std::string& fname);
        virtual ~SoundFile();

        virtual std::string filename();
        virtual bool close() = 0;

        /**
         * @brief size in samples
         */
        virtual size_t sampleCount() const = 0;

        virtual size_t sampleRate() const = 0;

        virtual size_t channels() const = 0;

        /**
         * @brief read samples to given buffer
         * @param sz - destination buffer size
         * @param dest - pointer to destination
         * @return
         */
        virtual long read(t_word* dest, size_t sz, size_t channel, long offset = 0) = 0;

        virtual bool isOpened() const = 0;
    };

    class SoundFileLoader {
    public:
        static StringList supportedFormats();
        static bool isSupported(const std::string& format);

        static SoundFilePtr open(const std::string& path);
    };
}
}

#endif // CEAMMC_SOUND_H
