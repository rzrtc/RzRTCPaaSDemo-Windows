//
// Created by 86157 on 2020/9/16.
//

#ifndef PAASSDK_MEDIASOURCE_H
#define PAASSDK_MEDIASOURCE_H

#include <memory>
#include <cstdint>

#include "MediaDefine.h"
#include "../IRtcEngineState.h"

namespace rz {

    class VideoSourceEventHandler{
    protected:
        virtual void onError(MODULE_TYPE type,int errCode,const std::string &errMsg) = 0;

        virtual void onWarn(MODULE_TYPE type,int warnCode,const std::string &warnMsg) = 0;
    public:
        virtual void onError(int errCode,const std::string &errMsg) final {
            onError(MODULE_VIDEO_SOURCE,errCode,errMsg);
        };

        virtual void onWarn(int warnCode,const std::string &warnMsg) final {
            onWarn(MODULE_VIDEO_SOURCE,warnCode,warnMsg);
        }
    };

//视频采集 由平台层实现
    class VideoSource {
    public:
        /**
        *视频源初始化回调
        *SDK 传给你的一个由SDK实现的 IVideoFrameConsumer 对象。保存该对象，并在视频源启动后，通过这个对象把视频帧传给 SDK。详见 IVideoFrameConsumer。
        *
        *return value
        *true: 自定义的视频源已经完成了初始化工作。
        *false: 自定义的视频源设备没准备好或者初始化失败，SDK 会停下来并上报错误。
        */
        virtual bool onInitialize(VideoFrameConsumer *consumer) = 0;

        /**
        *释放视频源回调。
        *SDK 触发该回调提醒你关闭视频源设备。该回调通知你 SDK 即将销毁 IVideoFrameConsumer 对象。 收到该回调后，请你确保不再使用 IVideoFrameConsumer 对象
        */
        virtual void onDispose() = 0;

        /**
        *启动视频源回调。
        *SDK 触发该回调提醒你启动视频帧采集。启动成功后，SDK 会打开 IVideoFrameConsumer 的开关，接收你采集的视频帧。 你需要通过返回值告知 SDK 自定义的视频源是否已经成功启动。
        */
        virtual int onStart() = 0;

        /**
        *停止视频源回调。
        *SDK 触发该回调提醒你停止视频帧采集。该回调通知你 IVideoFrameConsumer 的开关的即将关闭，SDK 不会接收你之后采集的视频帧。
        */
        virtual void onStop() = 0;

        virtual ~VideoSource() = default;
    };

//视频采集生成器接口
    class VideoSourceProducer {
    public:
        //释放接口
        virtual void Release() = 0;

        //获取采集器接口
        virtual VideoSource *GetVideoSource(const VideoConfig &videoConfig,VideoSourceEventHandler *event) = 0;

        //获取采集器接口
        virtual void DelVideoSource(VideoSource *) = 0;

        //切换前后置摄像头 Android iOS
        virtual void switchCamera(VideoSource *) = 0;

        /**
         * 重置视频采集格式
         * @param videoConfig
         */
        virtual void setVideoConfig(VideoSource *,const VideoConfig &videoConfig) = 0;

        /**
         * 更新视频方向模式。
         * @param orientationMode
         */
        virtual void setVideoOrientationMode(VideoSource *,ORIENTATION_MODE orientationMode) = 0;


        virtual ~VideoSourceProducer() = default;
    };


    class AudioSourceEventHandler{
    protected:
        virtual void onError(MODULE_TYPE type,int errCode,const std::string &errMsg) = 0;

        virtual void onWarn(MODULE_TYPE type,int warnCode,const std::string &warnMsg) = 0;
    public:
        virtual void onError(int errCode,const std::string &errMsg) final {
            onError(MODULE_AUDIO_SOURCE,errCode,errMsg);
        };

        virtual void onWarn(int warnCode,const std::string &warnMsg) final {
            onWarn(MODULE_AUDIO_SOURCE,warnCode,warnMsg);
        }
    };
//音频采集结构 由平台模块实现
    class AudioSource {
    public:
        /**
        *音频源初始化回调，
        *SDK 传给你的一个 IAudioFrameConsumer 对象。保存该对象，并在音频源启动后，通过这个对象把音频帧传给 SDK。详见 IAudioFrameConsumer。
        *
        *return value
        *true: 自定义的音频源已经完成了初始化工作。
        *false: 自定义的音频源设备没准备好或者初始化失败，SDK 会停下来并上报错误。
        */
        virtual bool onInitialize(AudioFrameConsumer *consumer) = 0;

        /**
        *释放视频源回调。
        *SDK 触发该回调提醒你关闭音频源设备。该回调通知你 SDK 即将销毁 IAudioFrameConsumer 对象。 收到该回调后，请你确保不再使用 IAudioFrameConsumer.
        */
        virtual void onDispose() = 0;

        /**
        *启动视频源回调。
        *SDK 触发该回调提醒你启动音频帧采集。启动成功后，SDK 会打开 IAudioFrameConsumer 的开关，接收你采集的音频帧。 你需要通过返回值告知 SDK 自定义的音频源是否已经成功启动。
        */
        virtual int onStart() = 0;

        /**
        *停止视频源回调。
        *SDK 触发该回调提醒你停止音频帧采集。该回调通知你 IAudioFrameConsumer 的开关的即将关闭，SDK 不会接收你之后采集的音频帧。
        */
        virtual void onStop() = 0;


        virtual ~AudioSource() = default;
    };

//视频采集生成器接口
    class AudioSourceProducer {
    public:
        //释放接口
        virtual void Release() = 0;

        /**
         * 生成默认音频采集源
         * @param audioConfig 建议的音频采集配置
         * @return 音频采集接口
         */
        virtual AudioSource *GetAudioSource(const AudioConfig &audioConfig,AudioSourceEventHandler *event) = 0;

        virtual void DelAudioSource(AudioSource *) = 0;

        /**
         * 重置音频采集格式
         * @param audioConfig
         */
        virtual void setAudioConfig(AudioSource *,const AudioConfig &audioConfig) = 0;

        virtual ~AudioSourceProducer() = default;
    };


}

#endif //PAASSDK_MEDIASOURCE_H
