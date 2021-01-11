#pragma once
#include "IRtcStreamEventHandler.h"
using namespace rz;

class PrivateIRtcStreamEventHandler : public IRtcStreamEventHandler
{
public:
    void onVideoPublishStateChanged(IRtcStream* stream, PUBLISH_STREAM_STATE state, int elapsed) override;
    void onLocalVideoStateChanged(IRtcStream* stream, LOCAL_VIDEO_STREAM_STATE state, LOCAL_VIDEO_STREAM_ERROR error, int elapsed) override;
    void onFirstVideoFramePublished(IRtcStream* stream, int elapsed) override;
    void onVideoSizeChanged(IRtcStream* stream, int width, int height) override;
};
