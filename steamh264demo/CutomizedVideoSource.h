#pragma once
/*
* 自定义视频渲染数据接口
* 
*/
#include <mutex>
#include <thread>
#include <queue>
#include "IMediaSource.h"
using namespace rz;

class CutomizedVideoSource : public IVideoSource
{
public:
    /**
      *视频源初始化回调
      *SDK 传给你的一个由SDK实现的 IVideoFrameConsumer 对象。保存该对象，并在视频源启动后，通过这个对象把视频帧传给 SDK。详见 IVideoFrameConsumer。
      *
      *return value
      *true: 自定义的视频源已经完成了初始化工作。
      *false: 自定义的视频源设备没准备好或者初始化失败，SDK 会停下来并上报错误。
      */
    virtual bool onInitialize(IVideoFrameConsumer* consumer) override;

    /**
    *释放视频源回调。
    *SDK 触发该回调提醒你关闭视频源设备。该回调通知你 SDK 即将销毁 IVideoFrameConsumer 对象。 收到该回调后，请你确保不再使用 IVideoFrameConsumer 对象
    */
    virtual void onDispose() override;

    /**
    *启动视频源回调。
    *SDK 触发该回调提醒你启动视频帧采集。启动成功后，SDK 会打开 IVideoFrameConsumer 的开关，接收你采集的视频帧。 你需要通过返回值告知 SDK 自定义的视频源是否已经成功启动。
    */
    virtual int onStart() override;

    /**
    *停止视频源回调。
    *SDK 触发该回调提醒你停止视频帧采集。该回调通知你 IVideoFrameConsumer 的开关的即将关闭，SDK 不会接收你之后采集的视频帧。
    */
    virtual void onStop() override;
    void SetParams(const std::string& channelId, const std::string &streamName);
public:
    void onVideoData(uint8_t* y, int w, int h);

private:
    IVideoFrameConsumer* m_pConsumer = nullptr;
    std::mutex m_mutex;
    volatile bool m_isStart = false;
    std::condition_variable m_cv;
    uint64_t m_last = 0;
    std::string m_strChannelId;
    std::string m_strStreamName;
};
