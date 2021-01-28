#pragma once
#include "RtcEngineEventHandler.h"

class RZEngineEventHandler : public rz::IRtcEngineEventHandler
{
public:
    RZEngineEventHandler();
    ~RZEngineEventHandler();

    // 本地音频状态发生改变回调。。
    // 本地音频的状态发生改变时（包括本地麦克风录制状态和音频编码状态）， SDK 会触发该回调报告当前的本地音频状态。
    virtual void onLocalAudioStateChanged(rz::LOCAL_AUDIO_STREAM_STATE state, rz::LOCAL_AUDIO_STREAM_ERROR error) override;

    // 已发布本地音频首帧回调.
    virtual void onFirstLocalAudioFramePublished(int elapsed) override;
};