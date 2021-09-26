//
// Created by yhy on 10/28/20.
//

#ifndef PAASSDK_RTCENGINEBASE_H
#define PAASSDK_RTCENGINEBASE_H

#include <Base/bitRateCalculation.h>

#include "../IMediaSink.h"
#include "../IRtcEngine.h"
#include "../IRtcEngineState.h"
#include "IChannel.h"
#include "MediaDeviceTest.h"
#include "MediaStreamInfo.h"
#include "PubAudioStream.h"
#include "PubVideoStream.h"
#include "RZAudioVolumeIndication.h"
#include "RZLog.h"
#include "RZLogging++.h"
#include "RZTime.h"
#include "RtcEngineContext.h"
#include "StatisticsInfoManager.h"
#include "StructureFactory.h"
#include "SubAudioStream.h"
#include "SubVideoStream.h"
#include "ThreadPool.h"
#include "lastmileProbe/RZLastmileProbeCalculate.h"

namespace rz {

class RtcEngineBase
    : public LastmileProbeEventHandler
    , public systemInfoCollectionEventHandler
    , public ChannelEventHandler
    , public SubVideoStreamEventHandler
    , public PubVideoStreamEventHandler
    , public SubAudioStreamEventHandler
    , public PubAudioStreamEventHandler
    , public AudioDeviceCollectionalEventHandler
    , public VideoDeviceCollectionalEventHandler
    , public AudioPlaybackEventHandler
    , public MediaDeviceTestEventHandler
    , public StatisticsEventHandler
    , public AudioVolumeIndicationEventHandler {
private:
    std::shared_ptr<RZLog> rzLogImpl;

    std::shared_ptr<AudioDeviceManager> audioDeviceManage;
    std::shared_ptr<VideoDeviceManager> videoDeviceManage;

    std::shared_ptr<VideoSourceProducer> videoSourceProducer;
    std::shared_ptr<AudioPlayback> audioPlayBack;

    std::shared_ptr<AudioSinkProducer> audioSinkProducer;
    std::shared_ptr<RZAudioVolumeIndication> audioVolumeIndication;

    std::shared_ptr<MediaDeviceTest> mediaDeviceTest;

    std::shared_ptr<RZLastmileProbeCalculate> lastmileProbeCal;

    std::shared_ptr<RZLastmileProbeTest> lastmileProbeTest;

    std::shared_ptr<systemInfoCollection> systemColloction;

    std::shared_ptr<SystemInfoCache> systemInfo;

    std::shared_ptr<StatisticsInfoManager> statisticsManage;
    std::shared_ptr<RZHttpClient> httpClient;
    std::shared_ptr<DataStatistics> dataStatisticsWriter;

    // engine 上下文信息
    IRtcEngineContext* engineContext = nullptr;

    static std::string logFilePath;

    static LOG_FILTER_TYPE logLevel;

    static uint32_t LogFileSize;

    static rzl::Configurations conf;
    /**
   * 目前对于值不感兴趣, 随便填写, 以后考虑换成map, 本次改动只优化变量名称
   */
    std::map<void*, void*> streamContextLivingPool;

    static std::mutex EngineBaseLivingMX;

private:
    static void TransportLoglevel(LOG_FILTER_TYPE filter);
    void InitDataStatistics();
    void AutoAdjustVideoStreamBitrate(const void* streamContext, MODULE_TYPE type, int newBitrate);
protected:
    inline static std::string getLowVideoStreamID(const std::string& bigVideoStreamId);
    
  static StreamConfig getStreamConnectConfig(const std::shared_ptr<LocalVideoStreamContext> &streamCtx);

    static StreamConfig getStreamConnectConfig(const std::shared_ptr<LocalAudioStreamContext>& streamCtx);

    static void createPubAudioStreamInfo(std::shared_ptr<LocalAudioStreamContext>& streamCtx);

    static void createSubAudioStreamInfo(std::shared_ptr<RemoteAudioStreamContext>& streamCtx,
                                         std::shared_ptr<IRtcChannelContext>& channelCtx,
                                         MEDIA_STREAM_TYPE mediaStreamType, MediaStreamSync* sync);

    static void createPubVideoStreamInfo(std::shared_ptr<LocalVideoStreamContext>& streamCtx);

    static void createSubVideoStreamInfo(std::shared_ptr<RemoteVideoStreamContext>& streamCtx,
                                         std::shared_ptr<IRtcChannelContext>& channelCtx,
                                         MEDIA_STREAM_TYPE mediaStreamType, MediaStreamSync* sync);

    static void localStreamCreate(std::shared_ptr<LocalAudioStreamContext> channelCtx);

    static void localStreamCreate(std::shared_ptr<LocalVideoStreamContext> streamCtx);

    static SUBSCRIBE_STREAM_STATE getRemoteStreamState(std::shared_ptr<RemoteAudioStreamContext>& streamCtx,
                                                       bool allMute);

    static SUBSCRIBE_STREAM_STATE getRemoteStreamState(std::shared_ptr<RemoteVideoStreamContext>& streamCtx,
                                                       bool allMute);

protected:
    void onError(MODULE_TYPE type, int errCode, const std::string& errMsg) override;

    void onWarn(MODULE_TYPE type, int warnCode, const std::string& warnMsg) override;

public:
    static RtcEngineBase* rtcEngineBase;

    RtcEngineEventHandler* eventHandler = nullptr;

    explicit RtcEngineBase(IRtcEngineContext* context);

    void onError(const std::string& channelName, MODULE_TYPE type, int errCode, const std::string& errMsg) override;

    void onWarn(const std::string& channelName, MODULE_TYPE type, int warnCode, const std::string& warnMsg) override;

    void onError(const void* streamContext, MODULE_TYPE type, int errCode, const std::string& errMsg) override;

    void onWarn(const void* streamContext, MODULE_TYPE type, int warnCode, const std::string& warnMsg) override;

public:
    //系统负载信息回调
    void onSystemLoadInfo(const systemLoadInfo& loadInfo) override;

    //音量综合回调
    void onAudioVolumeIndication(const AudioVolumeInfo* speakers, unsigned int speakerNumber, int totalVolume) override;

    //设备测试
    void onVolume(MODULE_TYPE type, int volume, int vad) override;

    //设备事件
    void onAudioDeviceVolumeChanged(MEDIA_DEVICE_TYPE deviceType, int volume, bool muted) override;

    void onDeviceWorkStateChanged(const char* deviceID, MEDIA_DEVICE_TYPE deviceType,
                                  MEDIA_DEVICE_STATE_TYPE deviceState) override;

    void onAudioRouteChanged(AUDIO_ROUTE_TYPE routing) override;

    //设备网络类型检测回调
    void onNetTypeChanged(NETWORK_TYPE oldNetType, NETWORK_TYPE newNetType) override;

    //网络参数检测结果回调
    void onLastmileQuality(rz::QUALITY_TYPE upNetQuality, rz::QUALITY_TYPE downNetQuality) override;

    void onLastmileProbeCallback(rz::LastmileProbeResult& probeResult) override;

  // 第一帧被渲染回调
  void onFirstVideoFrameSink(const void *streamContext, MODULE_TYPE type, int width, int height) override;

    // 渲染时间戳回调
    void onFrameSinkTimeStamps(const void* streamContext, MODULE_TYPE type, int32_t index,
                               int64_t sinkTimeStamp) override;

    void onVideoSizeChanged(const void* streamContext, MODULE_TYPE type, int width, int height) override;

    void onStreamFrozen(const void* streamContext, MODULE_TYPE type, bool forzen, uint64_t frameCount,
                        uint64_t ts) override;

    void onStreamRecvFirstVideoKeyFrame(const void* streamContext, MODULE_TYPE type, uint32_t width, uint32_t height,
                                        uint32_t index) override;

    void onStreamRecvFirstVideoFrame(const void* streamContext, MODULE_TYPE type, uint32_t width, uint32_t height,
                                     uint32_t index, bool isKey) override;

    void onStreamRecvFirstAudioFrame(const void* streamContext, MODULE_TYPE type) override;

    void onCodecSwitchWarn(const void* streamContext, MODULE_TYPE type, VIDEO_CODEC_TYPE oldDecoderType,
                           VIDEO_CODEC_TYPE newDecoderType) override;

    void onStreamConnectStateChanged(const void* streamContext, MODULE_TYPE type,
                                     CONNECTION_STATE_TYPE stateType) override;

    void onStreamNetInfo(const void* streamContext, MODULE_TYPE type, rzrtc::NetworkQuality& networkQuality) override;

    void onLossFrame(const void* streamContext, MODULE_TYPE type, int Num) override;

    void onAudioStreamVolume(const void* streamContext, MODULE_TYPE type, int volume, int vad) override;

    void onFirstVideoFramePublished(const void* streamContext, MODULE_TYPE type, uint32_t width, uint32_t height,
                                    uint32_t index, bool isKey) override;
 
    void onFirstKeyFramePublished(const void* streamContext, MODULE_TYPE type, uint32_t width, uint32_t height,
                                  uint32_t index) override;

  void onFirstAudioFramePublished(const void* streamContext, MODULE_TYPE type) override; 

    void onFirstVideoFrameDecode(const void* streamContext, MODULE_TYPE m_type, uint32_t index, uint32_t width,
                                 uint32_t height, DECODER_TYPE d_type) override;

    void onStreamStartSuccess(const void* streamContext, MODULE_TYPE type) override;

    void onPredictedBitrateChanged(const void* streamContext, MODULE_TYPE type, int newBitrate) override;

    // channelEventHandler
    void onRemoteVideoStreamMuteChanged(const std::string& channelName, const std::string& streamId,
                                        bool mute) override;

    void onRemoteAudioStreamMuteChanged(const std::string& channelName, const std::string& streamId,
                                        bool mute) override;
    void onRemoteVideoStreamDualChanged(const std::string &channelName, const std::string &streamId, bool dual) override;

    // 设置远端视频大小流回调
    void onRemoteVideoStreamTypeChanged(const void* streamContext, REMOTE_VIDEO_STREAM_TYPE type) override;

    void onRemoteVideoStreamOnline(const std::string& channelName, const std::string& uid,
                                   std::shared_ptr<MediaStreamSync> sync,
                                   const std::shared_ptr<MediaStreamInfo>& streamInfo) override;

    void onRemoteVideoStreamOffline(const std::string& channelName, const std::string& streamId) override;

    void onRemoteAudioStreamOnline(const std::string& channelName, const std::string& uid,
                                   std::shared_ptr<MediaStreamSync> sync,
                                   const std::shared_ptr<MediaStreamInfo>& streamInfo) override;

    void onRemoteAudioStreamOffline(const std::string& channelName, const std::string& streamId) override;

    void onRemoteUserOnline(const std::string& channelName, const std::string& userId) override;

    void onRemoteUserOffline(const std::string& channelName, const std::string& userId,
                             rzrtc::MN_RET_CODE reason) override;

    void onChannelConnectStateChanged(const std::string& channelName, CONNECTION_STATE_TYPE stateType,
                                      CONNECTION_CHANGED_REASON_TYPE reason) override;

    void onConnectionLost(const std::string& channelName) override;

    void onJoinChannelSuccess(const std::string& channelName, const std::string& uid) override;

    void onRejoinChannelSuccess(const std::string& channelName, const std::string& uid) override;

    void onLeaveChannel(const std::string& channelName, bool kick) override;

    void onUserCountChangeCallBack(const std::string& channelName, int broadcasterNum, int audientceNum) override;

    void onNetWorkQualityChanged(const std::string& channelName, const std::string& uid, rz::QUALITY_TYPE upNetQuality,
                                 rz::QUALITY_TYPE downNetQuality) override;

    //统计信息回调 StatisticsInfoManage

    void onRtcStats(const std::string& channelName, const RtcStats& stats) override;

    void onRemoteAudioStats(const std::string& channelName, const std::string& streamId,
                            RemoteAudioStats& stats) override;

    void onRemoteVideoStats(const std::string& channelName, const std::string& streamId,
                            RemoteVideoStats& stats) override;

    void onLocalAudioStats(const std::string& streamName, LocalAudioStats& stats) override;

    void onLocalVideoStats(const std::string& channelName, const std::string& streamName,
                           LocalVideoStats& stats) override;

    //外部操作
    static void enableBitratePrediction(bool enablePrediction, bool enableAutoAdjust);

    static void registerAudioObserver(const AudioObserverInfo&);

    static void removeAudioObserver();

    static void registerVideoObserver(const VideoObserverInfo&);

    static void removeVideoObserver();

    static void channelRelease(const std::string& channelName);

    static void streamRelease(const std::string& channelName, const std::string& streamName);

    static void publish(bool isVideo, const std::string& channelName, const std::string& streamName);

    static void unpublish(bool isVideo, const std::string& channelName, const std::string& streamName);

    static void muteLocalVideoStream(const std::string& channelName, const std::string& streamName, bool mute);

    static void muteLocalAudioStream(const std::string& channelName, const std::string& streamName, bool mute);

    static void muteRemoteVideoStream(const std::string& channelName, const std::string& uid,
                                      const std::string& streamName, bool mute);

    static void muteRemoteAudioStream(const std::string& channelName, const std::string& uid,
                                      const std::string& streamName, bool mute);

    static void muteAllRemoteAudioStreams(const std::string& channelName, bool mute);

    static void muteAllRemoteVideoStreams(const std::string& channelName, bool mute);

    static void joinChannel(const std::string& channelName);

    static void leaveChannel(const std::string& channelName, bool kike = false);

    static void setClientRole(const std::string& channelName, CLIENT_ROLE_TYPE type);

    static void setLocalVideoSink(const std::string& channelName, const std::string& streamName, IVideoSink* sink);

    static void setRemoteVideoSink(const std::string& channelName, const std::string& uid,
                                   const std::string& streamName, IVideoSink* sink);

    static void setAudioSink(IAudioSink* sink);

    static void enableLocalAudio(const std::string& channelName, const std::string& streamFlag, bool enabled);

    static void setRemoteVideoStreamType(const std::string& channelName, const std::string& uid,
                                         const std::string& streamName, REMOTE_VIDEO_STREAM_TYPE streamType);

    static void setRemoteRenderMode(const std::string& channelName, const std::string& uid,
                                    const std::string& streamName, RENDER_MODE_TYPE renderMode,
                                    VIDEO_MIRROR_MODE_TYPE mirrorMode);

    static void setLocalRenderMode(const std::string& channelName, const std::string& streamName,
                                   RENDER_MODE_TYPE renderMode, VIDEO_MIRROR_MODE_TYPE mirrorMode);

    static void enableDualStreamMode(const std::string& channelName, const std::string& streamName, bool enabled);

    static void setupRemoteVideo(const std::string& channelName, const std::string& uid, const std::string& streamName,
                                 const VideoCanvas& canvas);

    static void enableLocalVideo(const std::string& channelName, const std::string& streamName, bool enable);

    static void setupLocalVideo(const std::string& channelName, const std::string& streamName,
                                const VideoCanvas& videoCanvas);

    static void startPreview(const std::string& channelName, const std::string& streamName);

    static void stopPreview(const std::string& channelName, const std::string& streamName);

    static void setVideoEncoderConfiguration(const std::string& channelName, const std::string& streamName,
                                             const VideoEncoderConfiguration& config);

    static void enableAudioVolumeIndication(int interval, int smooth, bool report_vad);

    static void switchCamera();

    static void startLastmileProbeTest(const LastmileProbeConfig& config);

    static void stopLastmileProbeTest();

    static void setDefaultAudioRouteToSpeakerphone(bool defaultToSpeaker);

    static void setEnableSpeakerphone(bool speakerOn);

    static bool isSpeakerphoneEnabled();

    static IVideoDeviceManager* getVideoDeviceManager();

    static IAudioDeviceManager* getAudioDeviceManager();

    static void setLogFile(const std::string& filePath);

    static void setLogFilter(LOG_FILTER_TYPE filter);

    static void setLogFileSize(unsigned int fileSizeInKBytes);

    static void release();

    static void startDeviceTest(void* hwnd);

    static void stopDeviceTest();

    static void startRecordingDeviceTest(int indicationInterval);

    static void stopRecordingDeviceTest();

    static void startPlaybackDeviceTest(const char* testAudioFilePath);

    static void stopPlaybackDeviceTest();

    static void startAudioDeviceLoopbackTest(int indicationInterval);

    static void stopAudioDeviceLoopbackTest();
};
}  // namespace rz

#endif  // PAASSDK_RTCENGINEBASE_H
