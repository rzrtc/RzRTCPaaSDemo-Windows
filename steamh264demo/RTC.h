#pragma once

#include <IRtcEngine.h>
#include <IRtcChannel.h>
#include <RZEngineEventHandler.h>
#include <RZRtcChannelEventHandler.h>
#include <IRtcStreamEventHandler.h>
#include <CustomizedVideoSink.h>
#include <CutomizedVideoSource.h>
#include "VideoConverter.h"
#include "VideoDecoder.h"
#include "TipDialog.h"

using namespace rz;

class RTC
{
public:
    static RTC* instance();
    bool init(const char* appid);
    bool createChannel(const char* uid);
    int joinChannel(const char* channelId);
    void publish();
    void show();
    void setRemoteVideoSink(const char* uid);
    void leaveChannel();
    void release();
    VideoConverter* getEncoder() { return encoder.get(); };
    VideoDecoder* getDecoder() { return decoder.get(); };
    static void showTipDialog(const char* title,const char* content);
private:
    static RTC* m_instance;

    IRtcEngine* rtcEngine = nullptr;
    IRtcChannel* rtcChannel = nullptr;
    IRtcStream* rtcStream = nullptr;
    CutomizedVideoSource* videoSource = nullptr;
    CustomizedVideoSink* videoSink = nullptr;

    // CALLBACK
    RZEngineEventHandler* engineEventHandler = nullptr;
    RZRtcChannelEventHandler* channelEventHandler = nullptr;
    IRtcStreamEventHandler* streamEventHandler = nullptr;

    std::shared_ptr<VideoConverter> encoder;
    std::shared_ptr<VideoDecoder> decoder;

    RTC();
    ~RTC();
};
