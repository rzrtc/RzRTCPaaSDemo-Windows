//
// Created by yhy on 10/27/20.
//

#ifndef PAASSDK_RZPAASCONFIG_H
#define PAASSDK_RZPAASCONFIG_H

#include "MediaDefine.h"

namespace rz{

    class RZPaaSConfig{
    public:
        //版本号
        const static char *PAASSDK_CORE_VERSION;

        const static char *defaultDomain;
        
        const static char *defaultLogDomain;

        const static AUDIO_PROCESS_LEVEL audioAecLevel = AUDIO_PROCESS_LEVEL_AGGRESSIVE;

        const static AUDIO_PROCESS_LEVEL audioAncLevel = AUDIO_PROCESS_LEVEL_AGGRESSIVE;

        const static AUDIO_PROCESS_LEVEL audioHowlingLevel = AUDIO_PROCESS_LEVEL_AGGRESSIVE;

        const static int minKeyFrameTime = 2;

        //libsol的网络状况回调间隔  /ms
        const static int netQosInterval = 500;

        //dataFlow监控日志的时间间隔 /ms
        const static int DataFlowMonitorInterval = 5000;

        //dataFlow 各process 缓冲区大小监控界限  超过该大小则进行日志输出
        const static int dataProcessCache = 10;

        //线程池线程数量
        const static int dataFlowThreadNum = 16;

        //sdk 统计信息回调间隔 /s
        const static int statisticsInfoInterval = 2;

        //系统负载信息获取间隔 /ms
        const static int systemLoadInfoUpdateInterval = 2000;

        //默认的视频编解码器
        const static VIDEO_CODEC_TYPE defaultVideoCodecType = VIDEO_CODEC_HARD_H264;

        //默认的音频编解码器
        const static AUDIO_CODEC_TYPE defaultAudioCodecType = AUDIO_CODEC_OPUS;

        //默认的音频帧率 (作为基础数据 不可更改)
        const static int defaultAudioFrameRate = 50;

        //默认的系统内流转的视频像素格式
        const static VIDEO_PIXEL_FORMAT defaultVideoPixelFormat = VIDEO_PIXEL_I420;

        //默认的系统内流转的音频像素格式
        const static AUDIO_PCM_FORMAT defaultAudioPcmFormat = AUDIO_PCM_S16;

        //默认的音频声道数 (只能为 1 或 2 ) 底层协议相关
        const static int defaultAudioChannelCount = 1;

        //默认的音频采样率 底层协议支持 只有 8K 16K 32K 44.1K 48K
        const static int defaultAudioResample = 16000;

        //默认的音频回音消除级别
        const static AUDIO_PROCESS_LEVEL defaultAecLevel = AUDIO_PROCESS_LEVEL_MEDIUM;

        //默认的音频降噪级别
        const static AUDIO_PROCESS_LEVEL defaultAncLevel = AUDIO_PROCESS_LEVEL_MEDIUM;

        //默认的音频啸叫抑制级别
        const static AUDIO_PROCESS_LEVEL defaultHowlingLevel = AUDIO_PROCESS_LEVEL_MEDIUM;

        //默认的视频码率控制方式
        const static BIT_RATE_CTRL_TYPE defaultVideoBitRateCtrlType = BIT_RATE_CTRL_ABR;

        //默认的音频码率控制方式
        const static AUDIO_BIT_RATE_CTRL_TYPE defaultAudioBitRateCtrlType = AUDIO_BIT_RATE_CTRL_CBR;

        //默认的视频分辨率
        const static int defaultVideoWidth = 640;
        const static int defaultVideoHeight = 360;

        //默认的通信场景视频码率 bit
        const static int defaultSignlVideoBiteRate = 500000;

        //默认的直播场景视频码率 bit
        const static int defaultLiveVideoBiteRate = 1000000;

        //默认的通信场景音频码率 bit
        const static int defaultSignalAudioBiteRate = 16000;

        //默认的直播场景音频码率 bit
        const static int defaultLiveAudioBiteRate = 16000;
        //帧率的计算因子，分别对应
        /*
         * fps < 7: 0.2
         * 7<= fps < 10: 0.67
         * 10<= fps < 15: 0.8
         * 15<= fps < 24: 1
         * 24<= fps < 30: 1.3
         * 30<=  fps < 60: 1.5
         * fps > 60 : 2.25
         * */
        constexpr  static int fpsLevel[6] = {7, 10, 15, 24, 30, 60};
        constexpr static float fpsFactorList[7]= {0.2, 0.67, 0.8, 1, 1.3, 1.5, 2.25};
        /*
         * len <=  120: 3.0
         * len <=  240: 2.4
         * len <=  360: 1.6
         * len <=  480: 1.4
         * len <=  720: 1.2
         * len <=  1080: 1
         * len > 1080: 0.8
         * */
        constexpr  static float baseLenFactorList[7] = {3.0, 2.4, 1.6, 1.4, 1.2, 1, 0.8};
    };

}

#endif //PAASSDK_RZPAASCONFIG_H
