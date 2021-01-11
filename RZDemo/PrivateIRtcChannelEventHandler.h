#pragma once

#include <IRtcChannelEventHandler.h>
using namespace rz;

class PrivateIRtcChannelEventHandler : public IRtcChannelEventHandler
{
public:
    void onWarning(IRtcChannel* channel, int warn, const char* msg) override;
    void onError(IRtcChannel* channel, int err, const char* msg) override;
    void onJoinChannelSuccess(IRtcChannel* channel, const char* uid, int elapsed) override;
    void onRejoinChannelSuccess(IRtcChannel* channel, const char* uid, int elapsed) override;
    void onLeaveChannel(IRtcChannel* channel, const RtcStats& stats) override;
    void onClientRoleChanged(IRtcChannel* channel, CLIENT_ROLE_TYPE oldRole, CLIENT_ROLE_TYPE newRole, int elapsed) override;
    void onUserJoined(IRtcChannel* channel, const char* uid, int elapsed) override;
    void onUserOffline(IRtcChannel* channel, const char* uid, USER_OFFLINE_REASON_TYPE reason) override;
    void onAudioPublishStateChanged(IRtcChannel* channel, PUBLISH_STREAM_STATE state, int elapsed) override; 
    void onVideoPublishStateChanged(IRtcChannel* channel, PUBLISH_STREAM_STATE state, int elapsed) override;
    void onAudioSubscribeStateChanged(IRtcChannel* channel, const char* uid, SUBSCRIBE_STREAM_STATE state, int elapsed) override;
    void onVideoSubscribeStateChanged(IRtcChannel* channel, const char* uid, const char* streamName, SUBSCRIBE_STREAM_STATE state, int elapsed) override;
    void onFirstRemoteVideoFrame(IRtcChannel* channel, const char* uid, const char* streamName, int width, int height, int elapsed) override;
    void onVideoSizeChanged(IRtcChannel* channel, const char* uid, const char* streamName, int width, int height, int elapsed) override;
    void onNetworkQuality(IRtcChannel* channel, const char* uid, QUALITY_TYPE txQuality, QUALITY_TYPE rxQuality) override;
    void onConnectionLost(IRtcChannel* channel) override;
    void onConnectionStateChanged(IRtcChannel* channel, CONNECTION_STATE_TYPE state, CONNECTION_CHANGED_REASON_TYPE reason) override;
    void onRtcStats(IRtcChannel* channel, const RtcStats& stats) override;
    void onRemoteAudioStats(IRtcChannel* channel, const RemoteAudioStats& stats) override;
    void onRemoteVideoStats(IRtcChannel* channel, const RemoteVideoStats& stats) override;
};
