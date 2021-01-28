#pragma once
#include "IMediaSink.h"

class CustomizedVideoSink : public rz::IVideoSink
{
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
    virtual bool onInitialize() override;

    /**
        *启动渲染器。
        *Media Engine 在开启渲染功能的时候会回调这个方法。开发者可以在这个方法中启动渲染器。该方法需要开发者输入 true 或 false，Media Engine 会根据返回值做对应的动作。
        *
        *返回
        *true：Media Engine 继续进行渲染
        *false：Media Engine 认为出错而停止渲染器的功能
        */
    virtual bool onStart() override;

    /**
        *停止渲染器。
        *Media Engine 在停止渲染功能的时候会回调这个方法。开发者可以在这个方法中停止渲染。
        */
    virtual void onStop() override;

    /**
        *释放渲染器。
        *Media Engine 通知开发者渲染器即将被废弃。在 onDispose() 返回之后，开发者就可以释放掉资源了。
        */
    virtual void onDispose() override;

    /**
         * 获取渲染器需要的数据格式
         * @return 视频渲染器需要的数据格式
         */
    virtual rz::VideoSinkConfig getSinkConfig() override;

    virtual void consumeVideoFrame(const unsigned char* buffer, rz::VIDEO_PIXEL_FORMAT frameType, int width, int height, long timestamp) override;

    void consumeVideoPacket(const unsigned char* buffer, long length, rz::VIDEO_STREAM_TYPE streamType, bool isKey, long timestamp) override;
    
};
