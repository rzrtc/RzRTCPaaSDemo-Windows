//
// Created by yhy on 11/1/20.
//

#ifndef PAASSDK_IMEDIASINK_H
#define PAASSDK_IMEDIASINK_H

#include "IMediaFrameConsumer.h"
#include "IRtcEngineState.h"

namespace rz {

//视频渲染
class IVideoSink : IVideoFrameConsumer {
 public:
  /**
   *初始化渲染器。
   *Media Engine 初始化渲染器的时候调用这个方法。
   *开发者可以在这个方法中做渲染器的初始化工作。如果是耗时操作，也可以提前初始化好，然后在这个方法中告知 Media Engine
   *自定义渲染器已初始化好。该方法需要开发者自己输入 true 或 false，以告知 Media Engine 自定义渲染器的状态。
   *
   *return value
   *true：Media Engine 会认为自定义的渲染器已经初始化好
   *false：Media Engine 会认为自定义的渲染器初始化失败，不继续往下运行
   */
  virtual bool onInitialize() = 0;

  /**
   *启动渲染器。
   *Media Engine 在开启渲染功能的时候会回调这个方法。开发者可以在这个方法中启动渲染器。该方法需要开发者输入 true 或 false，Media Engine
   *会根据返回值做对应的动作。
   *
   *返回
   *true：Media Engine 继续进行渲染
   *false：Media Engine 认为出错而停止渲染器的功能
   */
  virtual bool onStart() = 0;

  /**
   *停止渲染器。
   *Media Engine 在停止渲染功能的时候会回调这个方法。开发者可以在这个方法中停止渲染。
   */
  virtual void onStop() = 0;

  /**
   *释放渲染器。
   *Media Engine 通知开发者渲染器即将被废弃。在 onDispose() 返回之后，开发者就可以释放掉资源了。
   */
  virtual void onDispose() = 0;

  /**
   * 获取渲染器需要的数据格式
   * @return 视频渲染器需要的数据格式
   */
  virtual VideoSinkConfig getSinkConfig() = 0;

  /**
   * 进行数据的抛出
   * sdk调用该接口将视频数据给到平台层进行渲染
   */
  void consumeVideoFrame(const unsigned char *buffer, VIDEO_PIXEL_FORMAT frameType, int width, int height, long timestamp) override = 0;

  /**
   * streamType = VIDEO_STREAM_H264 || streamType = VIDEO_STREAM_CUSTOM
   * 进行数据的抛出
   * sdk调用该接口将视频数据给到平台层进行渲染
   */
  void consumeVideoPacket(const unsigned char *buffer, long length, VIDEO_STREAM_TYPE streamType, bool isKey, long timestamp) override = 0;

  ~IVideoSink() override = default;
};

//音频渲染
class IAudioSink : public IAudioFrameConsumer {
 public:
  /**
   * 初始化渲染器。
   * Media Engine 初始化渲染器的时候调用这个方法。
   * 开发者可以在这个方法中做渲染器的初始化工作。如果是耗时操作，也可以提前初始化好，然后在这个方法中告知 Media Engine
   * 自定义渲染器已初始化好。该方法需要开发者自己输入 true 或 false，以告知 Media Engine 自定义渲染器的状态。
   *
   * return value
   * true：Media Engine 会认为自定义的渲染器已经初始化好
   * false：Media Engine 会认为自定义的渲染器初始化失败，不继续往下运行
   */
  virtual bool onInitialize() = 0;

  /**
   * 启动渲染器。
   * Media Engine 在开启渲染功能的时候会回调这个方法。开发者可以在这个方法中启动渲染器。该方法需要开发者输入 true 或 false，Media Engine
   * 会根据返回值做对应的动作。
   *
   * return value
   * true：Media Engine 继续进行渲染
   * false：Media Engine 认为出错而停止渲染器的功能
   */
  virtual bool onStart() = 0;

  /**
   * 停止渲染器。
   * Media Engine 在停止渲染功能的时候会回调这个方法。开发者可以在这个方法中停止渲染。
   */
  virtual void onStop() = 0;

  /**
   * 释放渲染器。
   * Media Engine 通知开发者渲染器即将被废弃。在 onDispose() 返回之后，开发者就可以释放掉资源了。
   */
  virtual void onDispose() = 0;

  /**
   * 获取渲染器需要的数据格式
   * @return 音频渲染器需要的数据格式
   */
  virtual AudioSinkConfig getSinkConfig() = 0;

  ~IAudioSink() override = default;
};
}  // namespace rz

#endif  // PAASSDK_IMEDIASINK_H
