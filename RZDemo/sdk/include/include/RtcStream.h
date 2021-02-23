//
// Created by yhy on 11/4/20.
//

#ifndef PAASSDK_RTCSTREAM_H
#define PAASSDK_RTCSTREAM_H

#include <string>

#include "../IRtcStream.h"
#include "RtcEngineContext.h"

namespace rz{

    class RtcChannel;

    class RtcStream : public IRtcStream{
    private:

        localVideoStreamContext *streamContext;
        RtcChannel* channel = nullptr;
    protected:
        ~RtcStream() override = default;
    public:

        explicit RtcStream(localVideoStreamContext*context ,RtcChannel *cnl):streamContext(context),channel(cnl){}

        const char *getStreamName() override ;

        const char *getChannelId() override ;

        void release() override ;

        int publish() override ;

        int unPublish() override ;

        int muteVideoStream(bool mute) override ;

        int enableDualStreamMode(bool enabled) override ;

        int setVideoEncoderConfiguration(const VideoEncoderConfiguration &config) override ;
        
        int setLowVideoStream(IVideoSource*) override ;
    };

}



#endif //PAASSDK_RTCSTREAM_H
