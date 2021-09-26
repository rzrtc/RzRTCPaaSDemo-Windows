//
// Created by yhy on 11/1/20.
//

#ifndef PAASSDK_IRTCENGINE_H
#define PAASSDK_IRTCENGINE_H

#include <atomic>

#include "IMediaDeviceManager.h"
#include "IMediaSink.h"
#include "IMediaSource.h"
#include "IRtcChannel.h"
#include "IRtcChannelEventHandler.h"
#include "IRtcEngineEventHandler.h"
#include "IRtcEngineState.h"

namespace rz {

class IRtcEngine;

struct RtcEngineContext {
    const char* appID = nullptr;                     // APPID
    IRtcEngineEventHandler* eventHandler = nullptr;  // IRtcEngine 的事件回调句柄
    const char* envConfig = nullptr;
    REGION_CODE regionCode = REGION_GLOBAL;
    CODEC_PRIORITY codecPriority = PRIORITY_AUTO;
};
RZ_API IRtcEngine* RZ_CALL createRZRtcEngine(const RtcEngineContext& context);

class IRtcEngine {
protected:
    virtual ~IRtcEngine() = default;

public:
    /**
   * 销毁 IRtcEngine 对象。
   * @param sync
   *       true:该方法为同步调用。需要等待 IRtcEngine 资源释放后才能执行其他操作，
   *       false:该方法为异步调用。不需要等待 IRtcEngine 资源释放后就能执行其他操作。使用异步调用时要注意，不要在该调用后立即卸载 SDK
   * 动态库，否则可能会因为 SDK 的清理线程还没有退出而崩溃。
   * @return
   */
    RZ_CPP_API static void release(bool sync = false);

    /**
   * 预测带宽功能是否开启及是否启用自动调整功能
   * @param enablePrediction 是否开启码率预测功能, 默认为false
   *				true: 开启
   *                false: 不开启
   * @param enableAutoAdjust 是否使用自动调整功能
   * @return
   * 0: 方法调用成功
   * < 0: 方法调用失败
   */
    virtual int enableBitratePrediction(bool enablePrediction, bool enableAutoAdjust) = 0;
    /**
   * 开关本地音频采集。
   * @param enabled true: 重新开启本地语音功能，即开启本地语音采集（默认）；
   *                false: 关闭本地语音功能，即停止本地语音采集。
   * @return
   * 0: 方法调用成功
   * < 0: 方法调用失败
   */
    virtual int enableLocalAudio(bool enabled) = 0;

    /**
   * 设置音频编码属性。
   * @param profile  设置采样率，码率，编码模式和声道数:
   * @param scenario 设置音频应用场景
   * @return
   * 0: 方法调用成功
   * < 0: 方法调用失败
   */
    virtual int setAudioProfile(AUDIO_PROFILE_TYPE profile, AUDIO_SCENARIO_TYPE scenario) = 0;

    /**
   * 开关本地音频发送。 joinChannel 前设置不执行实际的发送操作 只执行标记 joinchannel时执行动作
   * @param mute true: 停止 false: 继续发送
   * @return
   * 0: 方法调用成功
   * < 0: 方法调用失败
   */
    virtual int muteLocalAudioStream(bool mute) = 0;

    /**
   * 创建频道。
   * @param profile 频道使用场景
   * @param channelId 频道Id
   * @param eventHandler IRtcChannelEventHandler的指针
   * @return
   * 0(ERR_OK): 方法调用成功。
   * < 0: 方法调用失败。
   *   -2 (ERR_INVALID_ARGUMENT): 参数无效。
   */
    virtual IRtcChannel* createChannel(CHANNEL_PROFILE_TYPE profile, const char* channelId,
                                       IRtcChannelEventHandler* eventHandler) = 0;

    virtual IAudioDeviceManager* getAudioDeviceManager() = 0;

    virtual IVideoDeviceManager* getVideoDeviceManager() = 0;

    /**
   * 设置视频编码属性。
   * @param config 视频编码参数配置
   * @return
   * 0: 方法调用成功
   * < 0: 方法调用失败
   */
    virtual int setVideoEncoderConfiguration(const VideoEncoderConfiguration& config) = 0;

    /**
   * 开关本地视频发送。
   * @param mute
   * @return
   * 0: 方法调用成功
   * < 0: 方法调用失败
   */
    virtual int muteLocalVideoStream(bool mute) = 0;

    /**
   * 开关双流模式
   * @param enabled
   * @return
   * 0: 方法调用成功
   * < 0: 方法调用失败
   */
    virtual int enableDualStreamMode(bool enabled) = 0;

    /**
   *更新本地视图显示模式。
   * @param renderMode 本地视图的渲染模式
   * @param mirrorMode 本地视图的镜像模式
   * @return
   * 0: 方法调用成功
   * < 0: 方法调用失败
   */
    virtual int setLocalRenderMode(RENDER_MODE_TYPE renderMode, VIDEO_MIRROR_MODE_TYPE mirrorMode) = 0;

    /**
   * 开关本地视频采集。
   * @param enabled
   * @return
   * 0: 方法调用成功
   * < 0: 方法调用失败
   */
    virtual int enableLocalVideo(bool enabled) = 0;

    /**
   * 初始化本地视图。
   * @param canvas 视频画布信息
   * @return
   *   0: 方法调用成功
   * < 0: 方法调用失败
   */
    virtual int setupLocalVideo(const VideoCanvas& canvas) = 0;

    /**
   * 开启本地视频预览。
   * @return
   * 0: 方法调用成功
   * < 0: 方法调用失败
   */
    virtual int startPreview() = 0;

    /**
   * 停止本地视频预览。
   * @return
   * 0: 方法调用成功
   * < 0: 方法调用失败
   */
    virtual int stopPreview() = 0;

    //不可更换 joinchannel前配置
    virtual int setLocalVideoSource(IVideoSource* source, VIDEO_STREAM_TYPE streamType) = 0;

    //设置自定义音频源, 不可更换 joinchannel前配置.
    virtual int setLocalAudioSource(IAudioSource* source, const AudioSourceConfig& sourceConfig) = 0;

    virtual int setLocalVideoSink(IVideoSink* sink) = 0;

    // observer配置接口 只能调用一次
    virtual int registerAudioObserver(AudioObserverInfo&) = 0;

    // remove 要求收到onLeaveChannel后再进行remove
    virtual int removeAudioObserver() = 0;

    // observer配置接口 只能调用一次
    virtual int registerVideoObserver(VideoObserverInfo&) = 0;

    // remove 要求收到onLeaveChannel后再进行remove
    virtual int removeVideoObserver() = 0;

    /**
   * 切换前置/后置摄像头
   * @return
   * 0: 方法调用成功
   * < 0: 方法调用失败
   */
    virtual int switchCamera() = 0;

    /**
   * 启用说话者音量提示。
   * @param interval 指定音量提示的时间间隔：ms
   * @param smooth 平滑系数，指定音量提示的灵敏度。
   * @param report_vad 是否开启人声检测
   * @return
   * 0: 方法调用成功
   * < 0: 方法调用失败
   */
    virtual int enableAudioVolumeIndication(int interval, int smooth, bool report_vad) = 0;

    /**
   * 开始通话前网络质量探测。
   * @param config 网络探测配置
   * @return
   * 0: 方法调用成功
   * < 0: 方法调用失败
   */
    virtual int startLastmileProbeTest(const LastmileProbeConfig& config) = 0;

    /**
   * 停止通话前网络质量探测。
   * @return
   * 0: 方法调用成功
   * < 0: 方法调用失败
   */
    virtual int stopLastmileProbeTest() = 0;

    /**
   * 设置默认的语音路由
   * @param defaultToSpeaker
   * @return
   * 0: 方法调用成功
   * < 0: 方法调用失败
   */
    virtual int setDefaultAudioRouteToSpeakerphone(bool defaultToSpeaker) = 0;

    /**
   * 启用/关闭扬声器播放
   * @param speakerOn
   * @return
   * 0: 方法调用成功
   * < 0: 方法调用失败
   */
    virtual int setEnableSpeakerphone(bool speakerOn) = 0;

    /**
   * 查询扬声器启用状态
   * @param speakerOn
   * @return
   * 0: 方法调用成功
   * < 0: 方法调用失败
   */
    virtual bool isSpeakerphoneEnabled() = 0;

    /**
   * 设置 SDK 输出的日志文件。
   * @param filePath
   * @return
   * 0: 方法调用成功
   * < 0: 方法调用失败
   */
    virtual int setLogFile(const char* filePath) = 0;

    /**
   * 设置日志输出等级
   * @param filter
   * @return
   * 0: 方法调用成功
   * < 0: 方法调用失败
   */
    virtual int setLogFilter(unsigned int filter) = 0;

    /**
   * 设置 SDK 输出的单个日志文件大小。
   * @param fileSizeInKBytes
   * @return
   * 0: 方法调用成功
   * < 0: 方法调用失败
   */
    virtual int setLogFileSize(unsigned int fileSizeInKBytes) = 0;

    /**
   * 获取警告或错误描述。
   * @param code
   * @return
   */
    RZ_CPP_API static const char* getErrorDescription(int code);

    /**
   * 查询 SDK 版本号。
   * @param build
   * @return
   */
    RZ_CPP_API static const char* getVersion();
};

}  // namespace rz

#endif  // PAASSDK_IRTCENGINE_H
