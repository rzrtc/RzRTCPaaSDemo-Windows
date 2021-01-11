//
// Created by yhy on 11/4/20.
//

#ifndef PAASSDK_IRTCSTREAM_H
#define PAASSDK_IRTCSTREAM_H

#include "IMediaSource.h"
#include "IRtcEngineState.h"

namespace rz {

    class IRtcStream {
    protected:
        virtual ~IRtcStream() = default;
    public:
        virtual const char *getStreamName() = 0;

        virtual const char *getChannelId() = 0;

        virtual void release() = 0;

        virtual int publish() = 0;

        virtual int unPublish() = 0;

        virtual int muteVideoStream(bool mute) = 0;

        virtual int enableDualStreamMode(bool enabled) = 0;

        virtual int setVideoEncoderConfiguration(const VideoEncoderConfiguration &config) = 0;

        virtual int setLowVideoStream(IVideoSource*) = 0;
    };

}


#endif //PAASSDK_IRTCSTREAM_H
