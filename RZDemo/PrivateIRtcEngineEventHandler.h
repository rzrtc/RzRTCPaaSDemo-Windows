#pragma once
#include <IRtcEngineEventHandler.h>
using namespace rz;

class PrivateIRtcEngineEventHandler : public IRtcEngineEventHandler
{
public:
    void onWarning(int warn, const char* msg) override;
    void onError(int err, const char* msg) override;
    void onLocalVideoStateChanged(LOCAL_VIDEO_STREAM_STATE state, LOCAL_VIDEO_STREAM_ERROR error) override;
    void onLocalAudioStateChanged(LOCAL_AUDIO_STREAM_STATE state, LOCAL_AUDIO_STREAM_ERROR error) override;
    void onAudioVolumeIndication(const AudioVolumeInfo* speakers, unsigned int speakerNumber, int totalVolume) override;
    void onFirstLocalVideoFrame(int width, int height, int elapsed) override;
    void onFirstLocalVideoFramePublished(int elapsed) override;
    void onMediaDeviceStateChanged(const char* deviceId, MEDIA_DEVICE_TYPE deviceType, int deviceState) override;
    void onAudioDeviceVolumeChanged(MEDIA_DEVICE_TYPE deviceType, int volume, bool muted) override;
    void onVideoSizeChanged(int width, int height) override;
    void onFirstLocalAudioFramePublished(int elapsed) override;
    void onAudioRouteChanged(AUDIO_ROUTE_TYPE routing) override;
    void onNetworkTypeChanged(NETWORK_TYPE type) override;
    void onLocalAudioStats(const LocalAudioStats& stats) override;
    void onLocalVideoStats(const LocalVideoStats& stats) override;
    void onLocalNetworkQuality(QUALITY_TYPE txQuality, QUALITY_TYPE rxQuality) override;
    void onLastmileProbeResult(const LastmileProbeResult& result) override;
};
