//
// Created by yhy on 11/1/20.
//

#ifndef PAASSDK_IMEDIAFRAMECONSUMER_H
#define PAASSDK_IMEDIAFRAMECONSUMER_H

#include "IRtcEngineState.h"

namespace rz{

    //视频帧数据传输结构
    class IVideoFrameConsumer {

    public:
        //streamType = VIDEO_STREAM_FRAME
        //视频帧数据传输函数
        virtual void consumeVideoFrame(const unsigned char *buffer,VIDEO_PIXEL_FORMAT frameType,
                                          int width,int height,long timestamp) = 0;

        //streamType = VIDEO_STREAM_H264 || streamType = VIDEO_STREAM_CUSTOM
        virtual void consumeVideoPacket(const unsigned char *buffer, long length, VIDEO_STREAM_TYPE streamType, bool isKey,
                                        long timestamp) = 0;

        virtual ~IVideoFrameConsumer() = default;
    };

    class IAudioFrameConsumer {
    public:

		virtual void consumePcmData(uint8_t* buffer[8], uint32_t lineSize[8], AUDIO_PCM_FORMAT frameType,
			uint32_t channel, uint32_t sampleRate, long timestamp) = 0;

        virtual ~IAudioFrameConsumer() = default;

    };
}

#endif //PAASSDK_IMEDIAFRAMECONSUMER_H
