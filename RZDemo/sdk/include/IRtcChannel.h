//
// Created by yhy on 11/1/20.
//

#ifndef PAASSDK_IRTCCHANNEL_H
#define PAASSDK_IRTCCHANNEL_H

#include <atomic>

#include "IMediaSink.h"
#include "IMediaSource.h"
#include "IRtcChannelEventHandler.h"
#include "IRtcEngineState.h"
#include "IRtcStream.h"
#include "IRtcStreamEventHandler.h"

namespace rz {
class IRtcChannel {
 protected:
  virtual ~IRtcChannel() = default;

 public:
  virtual void release() = 0;

  virtual const char *getChannelId() = 0;
  /**
   * 设置用户角色。
   * @param role
   * @return
   * 0(ERR_OK): 方法调用成功。
   * < 0: 方法调用失败。
   *      -1(ERR_FAILED): 一般性的错误（未明确归类）。
   *      * -2(ERR_INALID_ARGUMENT): 参数无效。
   */
  virtual int setClientRole(CLIENT_ROLE_TYPE role) = 0;

  virtual int setRemoteVideoSink(const char *uid, const char *streamName, IVideoSink *sink) = 0;

  /**
   * 加入频道。
   * @param uid 用户ID 标识用户
   * @return
   * 0(ERR_OK): 方法调用成功。
   * < 0: 方法调用失败。
   *      -2(ERR_INALID_ARGUMENT): 参数无效。
   *      -3(ERR_REFUSED): 调用被拒绝。可能有如下两个原因：
   *          已经加入了 channelId 频道。
   */
  virtual int joinChannel(const char *uid) = 0;

  /**
   * 离开频道。
   * @return
   * 0(ERR_OK): 方法调用成功。
   * < 0: 方法调用失败。
   *  -1(ERR_FAILED): 一般性的错误（未明确归类）。
   *  -2(ERR_INALID_ARGUMENT): 参数无效。
   */
  virtual int leaveChannel() = 0;

  /**
   * 创建一路视频流
   * @param streamName 流名字
   * @param videoSource 视频源
   * @param streamType 流的类型
   * @param event
   * @param videoCfg 编解码配置
   * @return
   */
  virtual IRtcStream *createVideoStream(const char *streamName, IVideoSource *videoSource, VIDEO_STREAM_TYPE streamType,
                                        IRtcStreamEventHandler *event, const VideoEncoderConfiguration &videoCfg) = 0;

  /**
   * 本地默认流发布到频道
   * @return
   */
  virtual int publish(PUBLISH_MEDIA_TYPE publishType = PUBLISH_AUDIO_VIDEO) = 0;

  /**
   * 删除一路默认流发布到频道
   * @return
   */
  virtual int unPublish(PUBLISH_MEDIA_TYPE publishType = PUBLISH_AUDIO_VIDEO) = 0;

  /**
   * 初始化远端用户视图。
   * @param uid
   * @param streamName
   * @param canvas视频画布信息
   * @return
   * 0: 方法调用成功
   * < 0: 方法调用失败
   */
  virtual int setupRemoteVideo(const char *uid, const char *streamName, const VideoCanvas &canvas) = 0;

  /**
   * 设置是否默认停止接收视频流。
   * @param mute
   * @return
   * 0: 方法调用成功
   * < 0: 方法调用失败
   */
  virtual int setDefaultMuteAllRemoteVideoStreams(bool mute) = 0;

  /**
   * 接收／停止接收所有远端视频流。
   * @param mute
   * @return
   * 0: 方法调用成功
   * < 0: 方法调用失败
   */
  virtual int muteAllRemoteVideoStreams(bool mute) = 0;

  /**
   * 接收／停止接收指定远端用户的视频流
   * @param uid
   * @param streamName
   * @param mute
   * @return
   * 0: 方法调用成功
   * < 0: 方法调用失败
   */
  virtual int muteRemoteVideoStream(const char *uid, const char *streamName, bool mute) = 0;

  /**
   * 更新远端视图显示模式。
   * @param uid
   * @param streamName
   * @param renderMode 远端用户视图的渲染模式
   * @param mirrorMode 远端用户视图的镜像模式
   * @return
   * 0: 方法调用成功
   * < 0: 方法调用失败
   */
  virtual int setRemoteRenderMode(const char *uid, const char *streamName, RENDER_MODE_TYPE renderMode, VIDEO_MIRROR_MODE_TYPE mirrorMode) = 0;

  /**
   * 设置默认订阅的视频流类型。
   * @param streamType 视频流类型
   * @return
   * 0: 方法调用成功
   * < 0: 方法调用失败
   */
  virtual int setRemoteDefaultVideoStreamType(REMOTE_VIDEO_STREAM_TYPE streamType) = 0;

  /**
   * 设置订阅的视频流类型。
   * @param uid
   * @param streamName
   * @param streamType 视频流类型
   * @return
   * 0: 方法调用成功
   * < 0: 方法调用失败
   */
  virtual int setRemoteVideoStreamType(const char *uid, const char *streamName, REMOTE_VIDEO_STREAM_TYPE streamType) = 0;

  /**
   * 接收／停止接收所有音频流。
   * @param mute true: 停止 false: 继续接收
   * @return
   * 0: 方法调用成功
   * < 0: 方法调用失败
   */
  virtual int muteAllRemoteAudioStreams(bool mute) = 0;

  /**
   *设置是否默认接收所有音频流。
   * @param mute true: 停止 false: 接收
   * @return
   * 0: 方法调用成功
   * < 0: 方法调用失败
   */
  virtual int setDefaultMuteAllRemoteAudioStreams(bool mute) = 0;

  /**
   * 接收／停止接收指定音频流。
   * @param uid
   * @param mute
   * @return
   * 0: 方法调用成功
   * < 0: 方法调用失败
   */
  virtual int muteRemoteAudioStream(const char *uid, bool mute) = 0;

  /**
   * 查询网络连接状态
   * @return
   */
  virtual CONNECTION_STATE_TYPE getConnectionState() = 0;
};
}  // namespace rz

#endif  // PAASSDK_IRTCCHANNEL_H
