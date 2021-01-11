//
// Created by yhy on 10/18/20.
//

#ifndef PAASSDK_FILEAUDIOSOURCE_H
#define PAASSDK_FILEAUDIOSOURCE_H

#include "MediaSource.h"

namespace rz {

    class FileAudioSourceProducer{
    public:
        virtual AudioSource *GetFileAudioSource(const char *fileName,AudioSourceEventHandler *event) = 0;

        virtual void DelFileAudioSource(AudioSource *ptr) = 0;

        virtual ~FileAudioSourceProducer() = default;
    };
}

#endif //PAASSDK_FILEAUDIOSOURCE_H
