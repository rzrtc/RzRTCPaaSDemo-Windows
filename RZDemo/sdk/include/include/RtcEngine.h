//
// Created by 86157 on 2020/9/21.
//

#ifndef PAASSDK_RTCENGINE_H
#define PAASSDK_RTCENGINE_H

#include <mutex>
#include <atomic>
#include <string>



#include "RZLog.h"
#include "RZTime.h"
#include "IChannel.h"
#include "ThreadPool.h"
#include "RZLogging++.h"
#include "PubAudioStream.h"
#include "PubVideoStream.h"
#include "SubAudioStream.h"
#include "SubVideoStream.h"
#include "MediaStreamInfo.h"
#include "../IMediaSink.h"
#include "../IMediaSource.h"
#include "../IRtcEngineState.h"
#include "RtcEngineContext.h"
#include "../IRtcEngine.h"

namespace rz {

    class RtcEngine : public IRtcEngine{
    private:
    protected:
    public:
        static std::atomic<RtcEngine *>rtcEngine;
        IRtcEngineContext *engineContext = nullptr;

        std::map<std::string ,IRtcChannel*> channelPool;

        explicit RtcEngine(IRtcEngineContext *engine);

        ~RtcEngine() override = default;

        int enableBitratePrediction(bool enablePrediction, bool enableAutoAdjust) override;

        int enableLocalAudio(bool enabled) override ;

        int setAudioProfile(AUDIO_PROFILE_TYPE profile, AUDIO_SCENARIO_TYPE scenario) override ;

        int muteLocalAudioStream(bool mute) override ;

        IRtcChannel* createChannel(CHANNEL_PROFILE_TYPE profile,const char *channelId,IRtcChannelEventHandler *event) override ;

        IAudioDeviceManager *getAudioDeviceManager() override ;

        IVideoDeviceManager *getVideoDeviceManager() override ;

        int setVideoEncoderConfiguration(const VideoEncoderConfiguration &config) override ;

        int muteLocalVideoStream(bool mute) override ;

        int enableDualStreamMode(bool enabled) override ;

        int setLocalRenderMode(RENDER_MODE_TYPE renderMode, VIDEO_MIRROR_MODE_TYPE mirrorMode) override ;

        int enableLocalVideo(bool enabled) override ;

        int setupLocalVideo(const VideoCanvas &canvas) override ;

        int startPreview() override ;

        int stopPreview() override ;

        int setLocalVideoSource(IVideoSource *source,VIDEO_STREAM_TYPE streamType) override ;

        int setLocalAudioSource(IAudioSource *source,const AudioSourceConfig &sourceConfig)  override;

        int setLocalVideoSink(IVideoSink *sink) override ;

        int registerAudioObserver(AudioObserverInfo&) override ;

        int removeAudioObserver() override ;

        int registerVideoObserver(VideoObserverInfo&) override ;

        int removeVideoObserver() override ;

        int enableAudioVolumeIndication(int interval, int smooth,bool report_vad) override ;

        int switchCamera() override ;

        int startLastmileProbeTest(const LastmileProbeConfig &config) override ;

        int stopLastmileProbeTest() override ;

        int setDefaultAudioRouteToSpeakerphone(bool defaultToSpeaker) override ;

        int setEnableSpeakerphone(bool speakerOn) override ;

        bool isSpeakerphoneEnabled() override ;

        int setLogFile(const char *filePath) override ;

        int setLogFilter(unsigned int filter) override ;

        int setLogFileSize(unsigned int fileSizeInKBytes) override ;
    };
}
#endif //PAASSDK_RTCENGINE_H

