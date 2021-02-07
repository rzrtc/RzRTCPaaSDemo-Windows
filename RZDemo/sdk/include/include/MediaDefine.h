//
// Created by 86157 on 2020/9/16.
//

#ifndef PAASSDK_MEDIADEFINE_H
#define PAASSDK_MEDIADEFINE_H


#include <cstdint>
#include <cstring>
#include <memory>

#include "RZStreamHandler.h"
#include "MediaProcessDefine.h"
#include "../IRtcEngineState.h"

namespace rz {
    class IRtcEngineEventHandler;


//音频编码器码率控制项
    enum AUDIO_BIT_RATE_CTRL_TYPE {
        AUDIO_BIT_RATE_CTRL_CBR,
        AUDIO_BIT_RATE_CTRL_VBR
    };

//码率控制方式
    enum BIT_RATE_CTRL_TYPE {
        BIT_RATE_CTRL_CBR,   //固定码率
        BIT_RATE_CTRL_VBR,   //可变码率
        BIT_RATE_CTRL_ABR    //平均码率
    };
    //视频编解码器类型
    enum VIDEO_CODEC_TYPE{
        VIDEO_CODEC_HARD_H264,
        VIDEO_CODEC_SOFT_H264,
    };
    enum AUDIO_CODEC_TYPE{
        AUDIO_CODEC_OPUS,
    };

//视频流格式配置
    struct VideoConfig {
        VIDEO_PIXEL_FORMAT pixelFormat = VIDEO_PIXEL_FORMAT::VIDEO_PIXEL_I420; //视频像素格式
        int bitDepth = 8;                                      //位深
        ORIENTATION_MODE orientationMode = ORIENTATION_MODE_ADAPTIVE;//采集方向模式
        uint32_t frameRate = 0;
        int height = 0;
        int width = 0;
    };

#define MAX_VIDEO_PLANE 4

    static const int VideoPixelHeigth[VIDEO_PIXEL_COUNT][MAX_VIDEO_PLANE] = {
            { 2, 1, 1, 0 },
            { 1, 1, 1, 1 },
            { 2, 1, 0, 0 },
            { 1, 1, 1, 1 },
            { 2, 1, 1, 0 },
            { 1, 1, 1, 1 },
            { 2, 1, 0, 0 },
            { 1, 1, 1, 0 },
    };
    static const int VideoPixelLinesize[VIDEO_PIXEL_COUNT][MAX_VIDEO_PLANE] = {
            { 2, 1, 1, 0 },
            { 1, 1, 1, 1 },
            { 2, 1, 0, 0 },
            { 1, 1, 1, 1 },
            { 2, 1, 1, 0 },
            { 1, 1, 1, 1 },
            { 2, 1, 0, 0 },
            { 2, 1, 1, 0 },
    };


//视频帧结构
    struct VideoData : public MediaData{
    private:
        VIDEO_STREAM_TYPE videoStreamType = VIDEO_STREAM_TYPE::VIDEO_STREAM_FRAME;   //标识该视频帧结构数据是否已经经过编码
        VIDEO_PIXEL_FORMAT format = VIDEO_PIXEL_I420;                          //标识该视频帧结构的像素格式
        uint32_t timestamp = 0;                                                    //该帧数据的时间戳 进程内严格沿系统时间单向递增

        bool mirrorMode = false;

        uint32_t width = 0;                     //该帧数据的分辨率
        uint32_t height = 0;

        uint32_t index = 0;                        //数据接收libsol提供的数据帧序列号 用于判断是否存在丢帧
        bool isKey = false;                     //标识该帧数据是否是关键帧 由编解码器负责修改

        uint8_t *Plane[MAX_VIDEO_PLANE] = {nullptr};           //数据内容 plane
        uint32_t lineSize[MAX_VIDEO_PLANE] = {0};             //数据步长

        uint8_t *Data = nullptr;
        uint32_t DataSize = 0;

    public:

        VIDEO_STREAM_TYPE getVideoStreamType() {
            return videoStreamType;
        }

        VIDEO_PIXEL_FORMAT getVideoPixelFormat() {
            return format;
        }

        uint32_t getIndex() {
            return index;
        }

        uint32_t getTimeStamp() {
            return timestamp;
        }

        void getResolution(uint32_t &w, uint32_t &h) {
            w = width;
            h = height;
        }

        void setResolution(uint32_t w,uint32_t h){
            width = w;
            height = h;
        }

        void setMirrorMode(bool mirror){
            mirrorMode = mirror;
        }

        bool isMirror(){
            return mirrorMode;
        }

        bool isKeyFrame() {
            return isKey;
        }

        void setKeyFrame(bool key){
            isKey = key;
        }

        void getFrameData(uint8_t *plane[4], uint32_t linesize[4]) {
            for (int i = 0; i < 4; i++) {
                plane[i] = this->Plane[i];
                linesize[i] = this->lineSize[i];
            }
        }

        void getData(uint8_t **data, uint32_t &dataLen) {
            *data = this->Data;
            dataLen = this->DataSize;
        }

        unsigned char * getRawVideoFrame(){

            int bufferLen = 0;
            int lineCount = height / VideoPixelHeigth[format][0];
            int lieCount = width / VideoPixelLinesize[format][0];

            for(int i = 0;i < MAX_VIDEO_PLANE;i++){
                if(!this->Plane[i])
                    break;
                bufferLen += lieCount * VideoPixelLinesize[format][i] * lineCount * VideoPixelHeigth[format][i];
            }

            auto *buffer = new unsigned char[bufferLen];
            unsigned char *buffImple = buffer;


            for(int i = 0;i < MAX_VIDEO_PLANE;i++){
                for(int j = 0;j < lineCount * VideoPixelHeigth[format][i];j++){
                    memcpy(buffImple,this->Plane[i] + j * lineSize[i],lieCount * VideoPixelLinesize[format][i]);
                    buffImple += lieCount * VideoPixelLinesize[format][i];
                }
            }
            return buffer;
        }

        void updateData(VideoFrame &frame) {

            width = frame.width;
            height = frame.height;
            for (int i = 0; i < MAX_VIDEO_PLANE; i++) {
                delete[]Plane[i];
                Plane[i] = nullptr;
            }
            Plane[0] = new uint8_t[frame.yStride];
            memcpy(Plane[0], frame.yBuffer, frame.yStride);
            Plane[1] = new uint8_t[frame.uStride];
            memcpy(Plane[1], frame.uBuffer, frame.yStride);
            Plane[2] = new uint8_t[frame.vStride];
            memcpy(Plane[2], frame.vBuffer, frame.yStride);
            timestamp = frame.timeStamp;
        }

        VideoData(const unsigned char *data,VIDEO_STREAM_TYPE streamType,VIDEO_PIXEL_FORMAT frameType, int w, int h, long ts){
            if (data == nullptr)
                return;
            width = w;
            height = h;
            format = frameType;
            timestamp = ts;
            videoStreamType = streamType;

            unsigned char* pDataImple = (unsigned char*)data;
            int heightCount = height / VideoPixelHeigth[format][0];
            int widthCount = width / VideoPixelLinesize[format][0];
            for (int i = 0; i < MAX_VIDEO_PLANE; i++) {
                this->lineSize[i] = widthCount * VideoPixelLinesize[format][i];
                if(this->lineSize[i] == 0)
                    continue;
                int buffSize = this->lineSize[i] * heightCount * VideoPixelHeigth[format][i];
                this->Plane[i] = new uint8_t[buffSize];
                memcpy(this->Plane[i], pDataImple, buffSize);
                pDataImple += buffSize ;
            }
        }


        VideoData(const VideoData &data){

            videoStreamType = data.videoStreamType;
            format = data.format;
            timestamp = data.timestamp;
            width = data.width;
            height = data.height;
            index = data.index;
            isKey = data.isKey;
            mirrorMode = data.mirrorMode;

            privateData = data.privateData;

            int lineCount = height / VideoPixelHeigth[format][0];

            for (int i = 0; i < MAX_VIDEO_PLANE; i++) {
                if (data.Plane[i] == nullptr)
                    break;
                this->Plane[i] = new uint8_t[data.lineSize[i] * lineCount * VideoPixelHeigth[format][i]];
                memcpy(this->Plane[i], data.Plane[i], data.lineSize[i] * lineCount * VideoPixelHeigth[format][i]);
                this->lineSize[i] = data.lineSize[i];
            }

            if(data.Data) {
                this->Data = new uint8_t[data.DataSize];
                this->DataSize = data.DataSize;
                memcpy(this->Data, data.Data, data.DataSize);
            }
        }

         /**
         * videoData 原始视频数据构造函数
          * @param fmt        视频帧像素格式
          * @param ts         帧数据时间戳
          * @param w          视频长宽
          * @param h
          * @param mirror     是否需要镜像 不可以用自动镜像模式
          * @param plane      plane 指针
          * @param pitch      plane pitch
          */
        VideoData(VIDEO_PIXEL_FORMAT fmt, uint32_t ts, uint32_t w, uint32_t h,bool mirror,
                  uint8_t *plane[4],const uint32_t linesize[4]) {
            videoStreamType = VIDEO_STREAM_FRAME;
            format = fmt;
            timestamp = ts;
            width = w;
            height = h;
            mirrorMode = mirror;

            int lineCount = height / VideoPixelHeigth[format][0];
            for (int i = 0; i < 4; i++) {
                if(plane[i] == nullptr){
                    delete[]this->Plane[i];
                    this->Plane[i] = nullptr;
                    this->lineSize[i] = 0;
                    continue;
                }
                this->Plane[i] = new uint8_t[linesize[i] * VideoPixelHeigth[format][i] * lineCount];
                memcpy(this->Plane[i], plane[i], linesize[i] * VideoPixelHeigth[format][i] * lineCount);
                this->lineSize[i] = linesize[i];
            }
        }

        /**
         * 裸视频数据构造函数
         * @param streamType
         * @param ts
         * @param isKey
         * @param data
         * @param length
         */
        VideoData(VIDEO_STREAM_TYPE streamType, uint32_t ts,bool isKey,
                   uint8_t *data, uint32_t length) {
            videoStreamType = streamType;
            timestamp = ts;
            this->isKey = isKey;
            mirrorMode = false;

            this->Data = new uint8_t[length];
            memcpy(this->Data,data,length);
            this->DataSize = length;
        }

        VideoData(rzrtc::AVData &data){

            if(data.dataType == rzrtc::AVData::AV_DATA_TYPE_H264)
                videoStreamType = VIDEO_STREAM_H264;
            else
                videoStreamType = VIDEO_STREAM_CUSTOM;
            timestamp = data.timestemp;
            index = data.index;
            isKey = data.isKey;


            mirrorMode = false;
            this->Data = new uint8_t[data.len];
            memcpy(this->Data,data.data,data.len);
            this->DataSize = data.len;
        }

        void fillEncodeData(VIDEO_STREAM_TYPE streamType, uint8_t *data, int length, bool key) {
            isKey = key;
            videoStreamType = streamType;
            this->DataSize = length;

            int lineCount = height / VideoPixelHeigth[format][0];
            if (length > this->lineSize[0] * VideoPixelHeigth[format][0] * lineCount) {
                for (int i = 0; i < MAX_VIDEO_PLANE; i++) {
                    if (this->Plane[i] == nullptr)
                        break;
                    delete[]this->Plane[i];
                    this->Plane[i] = nullptr;
                    this->lineSize[i] = 0;
                }

                this->Data = new uint8_t[length];

                memcpy(this->Data, data, length);
            } else {
                this->Data = this->Plane[0];
                this->Plane[0] = nullptr;
                this->lineSize[0] = 0;
                memcpy(this->Data, data, length);
            }
        }

        void fillDecodeData(VIDEO_PIXEL_FORMAT fmt, uint32_t w, uint32_t h, uint8_t *plane[4],const uint32_t linesize[4]) {
            videoStreamType = VIDEO_STREAM_FRAME;

            format = fmt;
            width = w;
            height = h;

            delete[]this->Data;
            this->Data = nullptr;
            DataSize = 0;

            int lineCount = height / VideoPixelHeigth[format][0];
            for (int i = 0; i < 4; i++) {
                if(plane[i] == nullptr){
                    delete[]this->Plane[i];
                    this->Plane[i] = nullptr;
                    this->lineSize[i] = 0;
                    continue;
                }
                this->Plane[i] = new uint8_t[linesize[i] * VideoPixelHeigth[format][i] * lineCount];
                memcpy(this->Plane[i], plane[i], linesize[i] * VideoPixelHeigth[format][i] * lineCount);
                this->lineSize[i] = linesize[i];
            }
        }

        void fillConvertData(VIDEO_PIXEL_FORMAT fmt, uint32_t w, uint32_t h, uint8_t *plane[4],const uint32_t linesize[4]) {

            int lineCount = height / VideoPixelHeigth[format][0];
            int lineC = h / VideoPixelHeigth[fmt][0];
            for (int i = 0; i < 4; i++) {
                if(plane[i] == nullptr){
                    delete[]this->Plane[i];
                    this->Plane[i] = nullptr;
                    this->lineSize[i] = 0;
                    continue;
                }
                if (linesize[i] * lineC * VideoPixelHeigth[fmt][i] > this->lineSize[i] * lineCount * VideoPixelHeigth[format][i]) {

                    delete[]this->Plane[i];
                    this->Plane[i] = new uint8_t[linesize[i] * lineC * VideoPixelHeigth[fmt][i]];
                }
                this->lineSize[i] = linesize[i];
                memcpy(this->Plane[i], plane[i], linesize[i] * lineC * VideoPixelHeigth[fmt][i]);
            }

            format = fmt;
            width = w;
            height = h;
        }

        ~VideoData() override {
            for (auto &i : this->Plane) {
                delete[]i;
            }
            delete[]Data;
        }
    };

//流信息结构
    struct StreamInfo {
        const char *channelID = nullptr;     //channelID 对频道进行标识
        const char *userID = nullptr;        //userID 对用户进行标识
        const char *streamName = nullptr;    //流名称 对流进行标识
    };



    //视频帧数据传输结构
    class VideoFrameConsumer {

    public:
        //视频帧数据传输函数
        virtual void consumeVideoData(VideoData *videodata) = 0;

        //视频帧数据传输函数
        virtual void consumeVideoData(std::shared_ptr<VideoData> &data) {};

        virtual ~VideoFrameConsumer() = default;
    };

//音频相关配置项
    struct AudioConfig {
        int channelCount = 1;                           //声道数
        uint32_t sampleRate = 16000;                    //采样率
        AUDIO_PCM_FORMAT pcmFormat = AUDIO_PCM_S16;     //音频采集PCM位深格式
        AUDIO_STREAM_TYPE streamType = AUDIO_STREAM_PCM;//音频流类型
        uint32_t timeInterval = 20;                     //数据采集间隔 单位毫秒
        uint32_t frameRate = 50;
    };

#define MAX_AUDIO_CHANNEL 8

//音频帧结构
    struct AudioData :public MediaData{
    private:
        AUDIO_STREAM_TYPE audioStreamType = AUDIO_STREAM_TYPE::AUDIO_STREAM_PCM;   //标识该帧数据是否经过编码
        AUDIO_PCM_FORMAT format = AUDIO_PCM_FORMAT::AUDIO_PCM_S16;               //标识该帧结构的PCM位深
        uint32_t sampleRate = 0;
        uint32_t sampleSize = 0;                    //每采样占据数据位
        uint32_t timestamp = 0;                     //标识该帧数据的时间戳
        uint32_t channelCount = 1;                  //标识该帧数据的声道数
        uint32_t frameRate = 50;


        bool enableAEC = false;                     //标识该帧数据是否需要进行回音消除处理
        bool enableNS = false;                      //标识该帧数据是否需要进行降噪处理
        bool enableAGC = false;                     //标识该帧数据是否需要进行增益处理
        bool enableHowling = false;                 //标识该帧数据是否需要进行啸叫抑制

        uint32_t index = 0;                        //数据接收libsol提供的数据帧序列号 用于判断是否存在丢帧

        uint8_t *Plane[MAX_AUDIO_CHANNEL] = {nullptr};              //数据存储结构 plane
        uint32_t lineSize[MAX_AUDIO_CHANNEL] = {0};                 //数据长度 pitch
        uint32_t planeLen[MAX_AUDIO_CHANNEL] = {0};                 //内存长度
        uint32_t planeUseNum = 0;                                   //使用的plane数量

        uint8_t *Data = nullptr;                                   //编码后数据
        uint32_t DataSize = 0;                                     //编码后数据长度
        uint32_t MemLen = 0;                                       //内存长度

    public:
        AUDIO_STREAM_TYPE getAudioStreamType() {
            return audioStreamType;
        }

        AUDIO_PCM_FORMAT getAudioPcmFormat() {
            return format;
        }

        int getAudioBitDepth(){
            return 8 * sampleSize;
        }

        uint32_t getAudioSampleRate() {
            return sampleRate;
        }

        uint32_t getSampleSize() {
            return sampleSize;
        }

        uint32_t getChannelCount() {
            return channelCount;
        }

        uint32_t getTimeStamp() {
            return timestamp;
        }
        void setTimeStamp(uint32_t ts) {
            timestamp = ts;
        }

        uint32_t getFrameRate(){
            return frameRate;
        }

        bool isEnableAEC() {
            return enableAEC;
        }

        bool isEnableNS() {
            return enableNS;
        }

        bool isEnableAGC() {
            return enableAGC;
        }

        bool isEnableHowling() {
            return enableHowling;
        }

        uint32_t getIndex() {
            return index;
        }

        int audioTimeStampAline(std::shared_ptr<AudioData> &data){

            int frameSize = (sampleRate * sampleSize * channelCount) / (frameRate * planeUseNum);

            if(this->lineSize[0] != frameSize){
                if(data == nullptr){
                    data = std::make_shared<AudioData>(*this);
                    for (int i = 0;i < data->planeUseNum;i++){
                        if(data->Plane[i] != nullptr) {
                            delete []data->Plane[i];
                            data->Plane[i] = nullptr;
                            data->planeLen[i] = 0;
                            data->lineSize[i] = 0;
                        }
                    }
                }
            }
            else if(data == nullptr || data->lineSize[0] == 0){
                return 1;
            }

            for (int i = 0;i < data->planeUseNum;i++){
                if(data->Plane[i] != nullptr) {
                    if (data->planeLen[i] < frameSize) {
                        auto ptr = data->Plane[i];
                        data->Plane[i] = new uint8_t[frameSize];
                        memcpy(data->Plane[i],ptr,data->lineSize[i]);
                        delete []ptr;
                        data->planeLen[i] = frameSize;
                    }
                }
                else{
                    data->Plane[i] = new uint8_t[frameSize];
                    data->planeLen[i] = frameSize;
                }
            }

            if(data->lineSize[0] + this->lineSize[0] >= frameSize) {
                for (int i = 0; i < planeUseNum; i++) {
                    memcpy(data->Plane[i] + data->lineSize[i], this->Plane[i],
                           frameSize - data->lineSize[i]);
                    memmove(this->Plane[i], this->Plane[i] + frameSize - data->lineSize[i],
                           this->lineSize[i] - frameSize + data->lineSize[i]);
                    this->lineSize[i] = this->lineSize[i] - frameSize + data->lineSize[i];
                    data->lineSize[i] = frameSize;
                }
                return 2;
            }else{
                for (int i = 0; i < channelCount; i++) {
                    memcpy(data->Plane[i] + data->lineSize[i], this->Plane[i],
                           this->lineSize[i]);
                    data->lineSize[i] = data->lineSize[i] + this->lineSize[i];
                }
                return -1;
            }

            return 2;
        }

        AudioData(const AudioData &data){
            audioStreamType = data.audioStreamType;
            format = data.format;
            sampleRate = data.sampleRate;
            sampleSize = data.sampleSize;
            frameRate = data.frameRate;
            timestamp = data.timestamp;
            channelCount = data.channelCount;

            enableAEC = data.enableAEC;
            enableNS = data.enableNS;
            enableAGC = data.enableAGC;
            enableHowling =data.enableHowling;
            index = data.index;
            this->planeUseNum = data.planeUseNum;

            for (int i = 0; i < planeUseNum; i++) {
                this->Plane[i] = new uint8_t[data.planeLen[i]];
                memcpy(this->Plane[i], data.Plane[i], data.lineSize[i]);
                this->lineSize[i] = data.lineSize[i];
                this->planeLen[i] = data.planeLen[i];
            }

            if(data.Data) {
                this->Data = new uint8_t[data.MemLen];
                this->DataSize = data.DataSize;
                this->MemLen = data.MemLen;
                memcpy(this->Data, data.Data, data.DataSize);
            }
        }

        void getPCMData(uint8_t *plane[MAX_AUDIO_CHANNEL], uint32_t linesize[MAX_AUDIO_CHANNEL]) {
            for (int i = 0; i < MAX_AUDIO_CHANNEL; i++) {
                plane[i] = this->Plane[i];
                linesize[i] = this->lineSize[i];
            }
        }

        void getData(uint8_t **data, uint32_t &dataLen) {
            *data = this->Data;
            dataLen = this->DataSize;
        }


        void updateData(AudioFrame& frame) {
            sampleSize = frame.bytesPerSample;                 //每个采样占有的byte数量
            channelCount = frame.channels;                 //声道数
            sampleRate = frame.samplesRate;             //采样率

            for (int i = 0; i < MAX_AUDIO_CHANNEL; i++) {
                delete[]Plane[i];
                Plane[i] = nullptr;
            }
            Plane[0] = new uint8_t[sampleSize * frame.samples];
            memcpy(Plane[0], frame.buffer, sampleSize * frame.samples);
            timestamp = frame.timeStamp;
        }

         /**
          * audioData 使用PCM数据的 构造函数
          * @param fmt          audio PCM的数据格式
          * @param ts           帧时间戳 ms
          * @param channel      声道数
          * @param samplerate   采样率
          * @param samplesize   每个采样占据的Bite数量
          * @param framerate    帧率
          * @param AEC          是否需要做回声消除处理
          * @param NS           是否需要做降噪处理
          * @param AGC          是否需要做增益处理
          * @param plane        各plane指针
          * @param linesize     各plane pitch
          */
        AudioData(AUDIO_PCM_FORMAT fmt, uint32_t ts,uint32_t samplerate,uint32_t samplesize,
                uint32_t channel, bool AEC, bool NS,bool AGC,
                uint8_t *plane[MAX_AUDIO_CHANNEL], uint32_t linesize[MAX_AUDIO_CHANNEL]) {
            audioStreamType = AUDIO_STREAM_PCM;

            format = fmt;
            sampleRate = samplerate;
            sampleSize = samplesize;
            frameRate = 50;

            timestamp = ts;
            enableAEC = AEC;
            enableAGC = AGC;
            enableNS = NS;
            this->index = 0;

            channelCount = channel;
            if(fmt > AUDIO_PCM_DBL)
                planeUseNum = channel;
            else
                planeUseNum = 1;

            int frameSize = (sampleRate * sampleSize * channelCount) / (frameRate * planeUseNum);

            for (int i = 0; i < planeUseNum; i++) {
                if(linesize[0] > frameSize)
                    this->Plane[i] = new uint8_t[linesize[0]];
                else
                    this->Plane[i] = new uint8_t[frameSize];

                memcpy(this->Plane[i], plane[i], linesize[0]);
                planeLen[i] = linesize[0] > frameSize ? linesize[0] : frameSize;
                this->lineSize[i] = linesize[0];
            }
        }

        /**
         * 使用编码后的数据构造AudioData
         * @param streamType 类型
         * @param fmt        数据格式
         * @param samplerate 采样率
         * @param channel    声道数
         * @param ts         时间戳
         * @param data       数据
         * @param dataLen    数据长度
         */
        AudioData(AUDIO_STREAM_TYPE streamType,AUDIO_PCM_FORMAT fmt, uint32_t samplerate,uint32_t channel,uint32_t ts,uint8_t *data,uint32_t dataLen){
            audioStreamType = streamType;

            format = fmt;
            sampleRate = samplerate;
            channelCount = channel;
            timestamp = ts;
            frameRate = 50;

            this->Data = new uint8_t[dataLen];
            memcpy(this->Data, data, dataLen);
            this->DataSize = dataLen;
            this->MemLen = dataLen;
        }

        /**
         * 使用libSol接收的数据构造AudioData
         * @param data          libSol返回的数据结构
         * @param streamType    类型
         * @param fmt           数据格式
         * @param samplerate    采样率
         * @param channel       声道数
         */
        AudioData(rzrtc::AVData &data){

            if(data.dataType == rzrtc::AVData::AV_DATA_TYPE_OPUS)
                audioStreamType = AUDIO_STREAM_OPUS;
            else
                audioStreamType = AUDIO_STREAM_CUSTOM;

            format = AUDIO_PCM_S16;
            sampleRate = data.audioSampleRate;
            channelCount = data.audioChannelCount;
            timestamp = data.timestemp;
            index = data.index;
            frameRate = 50;

            this->Data = new uint8_t[data.len];
            memcpy(this->Data, data.data, data.len);
            this->DataSize = data.len;
            this->MemLen = data.len;
        }




        void fillEncodeData(AUDIO_STREAM_TYPE streamType, uint8_t *data, int length) {
            audioStreamType = streamType;

            DataSize = length;

            if (length > this->planeLen[0]) {
                for (int i = 0; i < planeUseNum; i++) {
                    delete []this->Plane[i];
                    planeLen[i] = 0;
                    this->Plane[i] = nullptr;
                    this->lineSize[i] = 0;
                }

                this->Data = new uint8_t[length];
                this->MemLen = length;

                memcpy(this->Data, data, length);
            } else {
                this->Data = this->Plane[0];
                this->MemLen = this->planeLen[0];
                this->Plane[0] = nullptr;
                this->planeLen[0] = 0;
                this->lineSize[0] = 0;
                for (int i = 1; i < planeUseNum; i++) {
                    delete []this->Plane[i];
                    this->Plane[i] = nullptr;
                    planeLen[i] = 0;
                    this->lineSize[i] = 0;
                }
                memcpy(this->Data, data, length);
            }
        }

        void fillDecodeData(AUDIO_PCM_FORMAT fmt, uint32_t samplerate, uint32_t channelcount,uint32_t samplesize,
                            uint8_t *plane[MAX_AUDIO_CHANNEL], uint32_t linesize[MAX_AUDIO_CHANNEL]) {
            audioStreamType = AUDIO_STREAM_PCM;
            format = fmt;
            this->sampleRate = samplerate;
            this->channelCount = channelcount;
            this->sampleSize = samplesize;
            frameRate = 50;

            delete[]Data;
            Data = nullptr;
            MemLen = 0;
            DataSize = 0;

            if(fmt > AUDIO_PCM_DBL)
                planeUseNum = channelcount;
            else
                planeUseNum = 1;

            int frameSize = (this->sampleRate * sampleSize * this->channelCount) / (frameRate * planeUseNum);

            for (int i = 0; i < planeUseNum; i++) {

                if(linesize[0] > frameSize)
                    this->Plane[i] = new uint8_t[linesize[0]];
                else
                    this->Plane[i] = new uint8_t[frameSize];

                memcpy(this->Plane[i], plane[i], linesize[0]);
                planeLen[i] = linesize[0] > frameSize ? linesize[0] : frameSize;
                this->lineSize[i] = linesize[0];
            }
        }

        void
        fillConvertData(AUDIO_PCM_FORMAT fmt, uint32_t samplerate,uint32_t samplesize, uint32_t channel, uint8_t *plane[MAX_AUDIO_CHANNEL],
                        uint32_t linesize[MAX_AUDIO_CHANNEL]) {

            format = fmt;
            channelCount = channel;
            sampleSize = samplesize;
            sampleRate = samplerate;

            auto num = planeUseNum;

            if(fmt > AUDIO_PCM_DBL)
                planeUseNum = channelCount;
            else
                planeUseNum = 1;

            for (int i = 0; i < num; i++) {
                if(i < planeUseNum) {
                    if (this->planeLen[i] < linesize[0]) {
                        delete []this->Plane[i];
                        this->Plane[i] = new uint8_t[linesize[0]];
                        this->planeLen[i] = linesize[0];
                    }

                    memcpy(this->Plane[i], plane[i], linesize[0]);
                    this->lineSize[i] = linesize[0];
                }
                else{
                    delete []this->Plane[i];
                    this->Plane[i] = nullptr;
                    this->lineSize[i] = 0;
                    this->planeLen[i] = 0;
                }
            }
        }

        ~AudioData() override {
            for (auto &i : this->Plane) {
                delete[]i;
            }
            delete[]Data;
        }
    };

//音频数据传输结构
    class AudioFrameConsumer {
    public:
        //音频数据传输函数
        virtual void consumeAudioData(AudioData *) = 0;

        virtual ~AudioFrameConsumer() = default;

    };
}
#endif //PAASSDK_MEDIADEFINE_H
