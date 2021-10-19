//
// Created by 86157 on 2020/9/16.
//

#ifndef PAASSDK_MEDIASINK_H
#define PAASSDK_MEDIASINK_H

#include "MediaSource.h"
#include "MediaProcessDefine.h"

namespace rz {

class VideoSinkEventHandler {
protected:
    virtual void onError(MODULE_TYPE type, int errCode, const std::string& errMsg) = 0;

    virtual void onWarn(MODULE_TYPE type, int warnCode, const std::string& warnMsg) = 0;

public:
    virtual void onError(int errCode, const std::string& errMsg) final { onError(MODULE_VIDEO_SINK, errCode, errMsg); }

    virtual void onWarn(int warnCode, const std::string& warnMsg) final {
        onWarn(MODULE_VIDEO_SINK, warnCode, warnMsg);
    }
};

//视频渲染
class VideoSink : VideoFrameConsumer {
public:
    /**
        *初始化渲染器。
        *Media Engine 初始化渲染器的时候调用这个方法。
        *开发者可以在这个方法中做渲染器的初始化工作。如果是耗时操作，也可以提前初始化好，然后在这个方法中告知 Media Engine 自定义渲染器已初始化好。该方法需要开发者自己输入 true 或 false，以告知 Media Engine 自定义渲染器的状态。
        *
        *return value
        *true：Media Engine 会认为自定义的渲染器已经初始化好
        *false：Media Engine 会认为自定义的渲染器初始化失败，不继续往下运行
        */
    virtual bool onInitialize() = 0;

    /**
        *启动渲染器。
        *Media Engine 在开启渲染功能的时候会回调这个方法。开发者可以在这个方法中启动渲染器。该方法需要开发者输入 true 或 false，Media Engine 会根据返回值做对应的动作。
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
    void consumeVideoData(VideoData*) override = 0;

    ~VideoSink() override = default;
};

//视频渲染生成器接口
class VideoSinkProducer {
public:
    //释放接口
    virtual void Release() = 0;

    //获取渲染器接口
    virtual VideoSink* GetVideoSink(const VideoSinkConfig&, VideoSinkEventHandler* event) = 0;

    //获取渲染器接口
    virtual void DelVideoSink(VideoSink*) = 0;

    /**
         * 为videosink 配置画板 videocanvas
         * @param videoSink 要配置的videosink
         * @param videoCanvas 画板参数
         */
    virtual void setVideoCanvas(const VideoSink* videoSink, const VideoCanvas& videoCanvas) = 0;

    /**
         * 更新显示模式
         * @param videoSink 要配置的videosink
         * @param renderMode
         */
    virtual void setVideoRenderMode(const VideoSink* videoSink, RENDER_MODE_TYPE renderMode) = 0;

    /**
        * 对用户传入的view指针进行保护，避免被释放
        * 
        * */
    virtual void HoldView(void* view) = 0;

    /**
        * SDK内部将view指针进行清除操作，避免view资源无法释放
        */
    virtual void ReleaseView(void* view) = 0;

    virtual ~VideoSinkProducer() = default;
};

class AudioSinkEventHandler {
protected:
    virtual void onError(MODULE_TYPE type, int errCode, const std::string& errMsg) = 0;

    virtual void onWarn(MODULE_TYPE type, int warnCode, const std::string& warnMsg) = 0;

public:
    //媒体处理项运行时错误回调输出
    virtual void onError(int errCode, const std::string& errMsg) final { onError(MODULE_AUDIO_SINK, errCode, errMsg); }

    virtual void onWarn(int warnCode, const std::string& warnMsg) final {
        onWarn(MODULE_AUDIO_SINK, warnCode, warnMsg);
    }
};

//音频渲染
class AudioSink : public AudioFrameConsumer {
public:
    /**
        * 初始化渲染器。
        * Media Engine 初始化渲染器的时候调用这个方法。
        * 开发者可以在这个方法中做渲染器的初始化工作。如果是耗时操作，也可以提前初始化好，然后在这个方法中告知 Media Engine 自定义渲染器已初始化好。该方法需要开发者自己输入 true 或 false，以告知 Media Engine 自定义渲染器的状态。
        *
        * return value
        * true：Media Engine 会认为自定义的渲染器已经初始化好
        * false：Media Engine 会认为自定义的渲染器初始化失败，不继续往下运行
        */
    virtual bool onInitialize() = 0;

    /**
        * 启动渲染器。
        * Media Engine 在开启渲染功能的时候会回调这个方法。开发者可以在这个方法中启动渲染器。该方法需要开发者输入 true 或 false，Media Engine 会根据返回值做对应的动作。
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

    /**
        * 进行数据的抛出
        * sdk接收调用该接口将数据进行抛出
        */
    void consumeAudioData(AudioData*) override = 0;

    ~AudioSink() override = default;
};

//视频渲染生成器接口
class AudioSinkProducer {
public:
    //释放接口
    virtual void Release() = 0;

    //获取渲染器接口
    virtual AudioSink* GetAudioSink(const AudioSinkConfig&, AudioSinkEventHandler* event) = 0;

    //获取渲染器接口
    virtual void DelAudioSink(AudioSink*) = 0;

    //查询扬声器启用状态
    virtual bool isSpeakerphoneEnabled() = 0;

    //切换默认设备到扬声器 Android iOS
    virtual int setEnableSpeakerphone(bool enable) = 0;

    //设置默认的语音路由 Android iOS
    virtual int setDefaultAudioRouteToSpeakerphone(bool defaultToSpeaker) = 0;

    virtual ~AudioSinkProducer() = default;
};

}  // namespace rz

#endif  //PAASSDK_MEDIASINK_H
