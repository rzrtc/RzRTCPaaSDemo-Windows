//
// Created by yhy on 11/1/20.
//

#ifndef PAASSDK_IMEDIASOURCE_H
#define PAASSDK_IMEDIASOURCE_H

#include "IMediaFrameConsumer.h"
#include "IRtcEngineState.h"

namespace rz {

//视频采集 由平台层实现
class IVideoSource {
 public:
  /**
   *视频源初始化回调
   *SDK 传给你的一个由SDK实现的 IVideoFrameConsumer 对象。保存该对象，并在视频源启动后，通过这个对象把视频帧传给 SDK。详见 IVideoFrameConsumer。
   *
   *return value
   *true: 自定义的视频源已经完成了初始化工作。
   *false: 自定义的视频源设备没准备好或者初始化失败，SDK 会停下来并上报错误。
   */
  virtual bool onInitialize(IVideoFrameConsumer *consumer) = 0;

  /**
   *小流准备就绪回调
   *SDK 传给你的一个由SDK实现的 IVideoFrameConsumer 对象。保存该对象，并在视频源启动后，通过这个对象把小流视频帧传给
   *SDK。详见 IVideoFrameConsumer。创建流和设置自定义源的时候,需要将streamType指定为VIDEO_STREAM_H264类型, 
   *与rtcStream::setLowVideoStream接口冲突, 不可重复使用。
   *
   *return value
   *true: 自定义的视频源已经完成了初始化工作。
   *false: 自定义的视频源设备没准备好或者初始化失败。
   */
  virtual bool onLowStreamReady(IVideoFrameConsumer *consumer) = 0;

  /**
   *释放视频源回调。
   *SDK 触发该回调提醒你关闭视频源设备。该回调通知你 SDK 即将销毁 IVideoFrameConsumer 对象。 收到该回调后，请你确保不再使用 IVideoFrameConsumer 对象
   */
  virtual void onDispose() = 0;

  /**
   *启动视频源回调。
   *SDK 触发该回调提醒你启动视频帧采集。启动成功后，SDK 会打开 IVideoFrameConsumer 的开关，接收你采集的视频帧。 你需要通过返回值告知 SDK
   *自定义的视频源是否已经成功启动。
   */
  virtual int onStart() = 0;

  /**
   *停止视频源回调。
   *SDK 触发该回调提醒你停止视频帧采集。该回调通知你 IVideoFrameConsumer 的开关的即将关闭，SDK 不会接收你之后采集的视频帧。
   */
  virtual void onStop() = 0;

  virtual ~IVideoSource() = default;
};

//音频采集结构 由平台模块实现
class IAudioSource {
 public:
  /**
   *音频源初始化回调，
   *SDK 传给你的一个 IAudioFrameConsumer 对象。保存该对象，并在音频源启动后，通过这个对象把音频帧传给 SDK。详见 IAudioFrameConsumer。
   *
   *return value
   *true: 自定义的音频源已经完成了初始化工作。
   *false: 自定义的音频源设备没准备好或者初始化失败，SDK 会停下来并上报错误。
   */
  virtual bool onInitialize(IAudioFrameConsumer *consumer) = 0;

  /**
   *释放视频源回调。
   *SDK 触发该回调提醒你关闭音频源设备。该回调通知你 SDK 即将销毁 IAudioFrameConsumer 对象。 收到该回调后，请你确保不再使用 IAudioFrameConsumer.
   */
  virtual void onDispose() = 0;

  /**
   *启动视频源回调。
   *SDK 触发该回调提醒你启动音频帧采集。启动成功后，SDK 会打开 IAudioFrameConsumer 的开关，接收你采集的音频帧。 你需要通过返回值告知 SDK
   *自定义的音频源是否已经成功启动。
   */
  virtual int onStart() = 0;

  /**
   *停止视频源回调。
   *SDK 触发该回调提醒你停止音频帧采集。该回调通知你 IAudioFrameConsumer 的开关的即将关闭，SDK 不会接收你之后采集的音频帧。
   */
  virtual void onStop() = 0;

  virtual ~IAudioSource() = default;
};

}  // namespace rz

#endif  // PAASSDK_IMEDIASOURCE_H
