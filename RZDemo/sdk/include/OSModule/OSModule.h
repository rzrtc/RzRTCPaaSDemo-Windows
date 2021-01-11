//
// Created by 86157 on 2020/9/16.
//

#ifndef PAASSDK_OSMODULE_H
#define PAASSDK_OSMODULE_H

#include "MediaSink.h"
#include "VideoCodec.h"
#include "MediaSource.h"
#include "RZSystemInfo.h"
#include "DataStatistics.h"
#include "DeviceColloection.h"
#include "DeviceCollectionProducer.h"
#include "RZLogProcess.h"

//平台层与C++模块连接接口 平台层实现
namespace rz {
    class OSModule {
    public:
        //获取H264硬编解码器Producer
        static VideoCodecProducer *createH264HardCodecProducer();

        //释放H264硬编解码器Producer
        static void deleteH264HardCodecProducer(VideoCodecProducer *);

        //获取H265硬编解码器Producer
        static VideoCodecProducer *createH265HardCodeceProducer();

        //释放H265硬编解码器Producer
        static void deleteH265HardCodecProducer(VideoCodecProducer *);

        //获取视频渲染Producer
        static VideoSinkProducer *createVideoSinkProducer();

        //释放视频渲染Producer
        static void deleteVideoSinkProducer(VideoSinkProducer *);

        //获取音频渲染Producer
        static AudioSinkProducer *createAudioSinkProducer();

        //释放音频渲染Producer
        static void deleteAudioSinkProducer(AudioSinkProducer *);

        //获取视频采集Producer
        static VideoSourceProducer *createVideoSourceProducer();

        //释放视频采集Producer
        static void deleteVideoSourceProducer(VideoSourceProducer *);

        //获取音频采集Producer
        static AudioSourceProducer *createAudioSourceProducer();

        //释放音频采集Producer
        static void deleteAudioSourceProducer(AudioSourceProducer *);

        //创建媒体设备收集Producer
        static MediaDeviceColloectionProduce *createMediaDeviceColloectionProduce();

        //释放媒体设备收集Producer
        static void deleteMediaDeviceColloectionProduce(MediaDeviceColloectionProduce *);

        //创建打点数据发送结构
        static DataStatistics *createDataStatistics(const char *url);

        //释放打点数据发送结构
        static void deleteDataStatistics(DataStatistics *);

        //创建Http传输结构
        static RZHttpClient *createHttpClient();

        //释放Http传输结构
        static void deleteHttpClient(RZHttpClient *ptr);

        //创建系统信息获取接口
        static systemInfoCollection *createSystemInfoCollection(int loadInfoInterval,systemInfoCollectionEventHandler *event);

        //释放系统信息获取接口
        static void deleteSystemInfoCollection(systemInfoCollection *);

        //创建日志写入模块
        static RZLog *createRZLogImpl();

        //释放日志写入模块
        static void deleteRZLogImpl(RZLog *ptr);

        //平台配置是否使用SDK内置回音消除
        static bool isAec;

        //平台配置是否使用SDK内置降噪
        static bool isAnc;

        //平台配置是否使用SDK内置啸叫抑制
        static bool isHowling;
    };
}


#endif //PAASSDK_OSMODULE_H
