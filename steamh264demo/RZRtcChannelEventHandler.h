#pragma once
#include "IRtcChannelEventHandler.h"

class RZRtcChannelEventHandler : public rz::IRtcChannelEventHandler
{
public:
    RZRtcChannelEventHandler();
    ~RZRtcChannelEventHandler();

    // 注册 onJoinChannelSuccess 回调。
    // 本地用户成功加入频道时，会触发该回调。
    virtual void onJoinChannelSuccess(rz::IRtcChannel* rtcChannel, const char* uid, int elapsed) override;

    // 注册 onLeaveChannel 回调。
    // 本地用户成功离开频道时，会触发该回调。
    virtual void onLeaveChannel(rz::IRtcChannel* rtcChannel, const rz::RtcStats& stat) override;

    // 注册 onFirstRemoteVideoDecoded 回调。
    // SDK 接收到第一帧远端视频并成功解码时，会触发该回调。
    // 可以在该回调中调用 setupRemoteVideo 方法设置远端视图。
    virtual void onFirstRemoteVideoFrame(rz::IRtcChannel* rtcChannel, const char* uid, const char* streamName, int width, int height, int elapsed) override;

    // 注册 onUserOffline 回调。
    // 远端用户离开频道或掉线时，会触发该回调。
    virtual void onUserOffline(rz::IRtcChannel* rtcChannel, const char* uid, rz::USER_OFFLINE_REASON_TYPE reason) override;


    virtual void onUserJoined(rz::IRtcChannel* channel, const char* uid, int elapsed) override;


    virtual void onError(rz::IRtcChannel* channel, int err, const char* msg) override;
};
