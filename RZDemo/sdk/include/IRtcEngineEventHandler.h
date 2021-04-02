//
// Created by 86157 on 2020/9/21.
//

#ifndef PAASSDK_IRTCENGINEEVENTHANDLER_H
#define PAASSDK_IRTCENGINEEVENTHANDLER_H

#include "IRtcEngineState.h"

namespace rz {
class IRtcEngineEventHandler {
 public:
  /**
   * 发生警告回调。
   * @param warn
   * @param msg
   */
  virtual void onWarning(int warn, const char *msg) {
    (void)warn;
    (void)msg;
  }

  /**
   * 发生错误回调。
   * @param err
   * @param msg
   */
  virtual void onError(int err, const char *msg) {
    (void)err;
    (void)msg;
  }

  virtual void onLocalVideoStateChanged(LOCAL_VIDEO_STREAM_STATE state, LOCAL_VIDEO_STREAM_ERROR error) {
    (void)state;
    (void)error;
  }

  virtual void onLocalAudioStateChanged(LOCAL_AUDIO_STREAM_STATE state, LOCAL_AUDIO_STREAM_ERROR error) {
    (void)state;
    (void)error;
  }

  /**
   * 提示频道内谁正在说话、说话者音量及本地用户是否在说话的回调。
   * @param speakers
   * @param speakerNumber
   * @param totalVolume
   */
  virtual void onAudioVolumeIndication(const AudioVolumeInfo *speakers, unsigned int speakerNumber, int totalVolume) {
    (void)speakers;
    (void)speakerNumber;
    (void)totalVolume;
  }

  /**
   * 已显示本地视频首帧回调。
   * @param width
   * @param height
   * @param elapsed
   */
  virtual void onFirstLocalVideoFrame(int width, int height, int elapsed) {
    (void)width;
    (void)height;
    (void)elapsed;
  }

  /**
   * 已发布本地视频首帧回调。
   * @param elapsed
   */
  virtual void onFirstLocalVideoFramePublished(int elapsed) { (void)elapsed; }

  /**
   * 音频设备变化回调。
   * @param deviceId
   * @param deviceType
   * @param deviceState
   */
  virtual void onMediaDeviceStateChanged(const char *deviceId, MEDIA_DEVICE_TYPE deviceType, int deviceState) {
    (void)deviceId;
    (void)deviceType;
    (void)deviceState;
  }

  /**
   * 回放、录音设备、或 App 的音量发生改变。
   * @param deviceType
   * @param volume
   * @param muted
   */
  virtual void onAudioDeviceVolumeChanged(MEDIA_DEVICE_TYPE deviceType, int volume, bool muted) {
    (void)deviceType;
    (void)volume;
    (void)muted;
  }

  /**
   * 本地视频大小发生改变回调。
   * @param width
   * @param height
   */
  virtual void onVideoSizeChanged(int width, int height) {
    (void)width;
    (void)height;
  }

  /**
   * 已发布本地音频首帧回调。
   * @param elapsed
   */
  virtual void onFirstLocalAudioFramePublished(int elapsed) { (void)elapsed; }

  /**
   * 语音路由已发生变化回调。
   * @param routing
   */
  virtual void onAudioRouteChanged(AUDIO_ROUTE_TYPE routing) { (void)routing; }

  /**
   * 本地网络类型发生改变回调。
   * @param type
   */
  virtual void onNetworkTypeChanged(NETWORK_TYPE type) { (void)type; }

  /**
   * 通话中本地音频流的统计信息回调。
   * @param stats
   */
  virtual void onLocalAudioStats(const LocalAudioStats &stats) { (void)stats; }

  /**
   * 本地视频流统计信息回调。
   * @param stats
   */
  virtual void onLocalVideoStats(const LocalVideoStats &stats) { (void)stats; }

  /**
   * 通话中本地的网络上下行 last mile 质量报告回调。
   * @param txQuality
   * @param rxQuality
   */
  virtual void onLocalNetworkQuality(QUALITY_TYPE txQuality, QUALITY_TYPE rxQuality) {
    (void)txQuality;
    (void)rxQuality;
  }

  /**
   * 通话前网络质量探测报告回调。
   * @param result
   */
  virtual void onLastmileProbeResult(const LastmileProbeResult &result) { (void)result; }

  virtual ~IRtcEngineEventHandler() = default;
};

}  // namespace rz
#endif  // PAASSDK_IRTCENGINEEVENTHANDLER_H
