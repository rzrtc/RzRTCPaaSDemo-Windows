//
// Created by yhy on 11/20/20.
//

#ifndef PAASSDK_SHAREDEVENTSMANAGE_H
#define PAASSDK_SHAREDEVENTSMANAGE_H

#include <mutex>
#include <map>

#include "DelegatManager.h"
#include "IRtcEngineState.h"


namespace rz {

    //流类型 用于事件接收方区分流
    enum STREAM_TYPE{
        PUB_VIDEO_STREAM,       //本地视频流
        PUB_VIDEO_LOW_STREAM,   //本地视频小流
        PUB_AUDIO_STREAM,       //本地音频流
        SUB_VIDEO_STREAM,       //远端视频流
        SUB_VIDEO_LOW_STREAM,   //远端视频小流
        SUB_AUDIO_STREAM,       //远端音频流
    };

    //本地流事件
    enum LOCAL_STREAM_STATE{
        LOCAL_STREAM_STATE_STOPPED = 0,            //0: 本地媒体流默认初始状态。
        LOCAL_STREAM_STATE_STARTSUCCESS = 1,       //1: 本地媒体流启动成功。
        LOCAL_STREAM_STATE_FAILED = 2,             //2: 本地媒体流启动失败。
        LOCAL_STREAM_STATE_SENDING = 3,            //3: 正在发送媒体数据
        LOCAL_STREAM_STATE_NO_SEND = 4,            //4: 不发送媒体数据
    };

    //定义事件接收器
    class SharedEventsRecver {
    public:
        virtual void onNetTypeChanged(uint64_t unixTm, NETWORK_TYPE oldNetType, NETWORK_TYPE newNetType) {
            (void)unixTm;
            (void)oldNetType;
            (void)newNetType;
        }

        virtual void onChannelConnectState(uint64_t unixTm, const std::string& channelId, CONNECTION_STATE_TYPE state, CONNECTION_CHANGED_REASON_TYPE reason) {
            (void)unixTm;
            (void)channelId;
            (void)state;
            (void)reason;
        }

        virtual void onStreamConnectState(uint64_t unixTm, const std::string& streamId, STREAM_TYPE streamType, CONNECTION_STATE_TYPE state) {
            (void)unixTm;
            (void)streamId;
            (void)streamType;
            (void)state;
        }

        virtual void onJoinChannel(uint64_t unixTm, const std::string& channelId) {
            (void)unixTm;
            (void)channelId;
        }

        virtual void onJoinChannelResult(uint64_t unixTm, const std::string& channelId, bool isSuccess, int reson) {
            (void)unixTm;
            (void)channelId;
            (void)isSuccess;
            (void)reson;
        }

        virtual void onLeaveChannel(uint64_t unixTm, const std::string& channelId) {
            (void)unixTm;
            (void)channelId;
        }

        virtual void onVideoSizeChanged(uint64_t unixTm, const std::string& streamId, STREAM_TYPE streamType, int w, int h) {
            (void)unixTm;
            (void)streamId;
            (void)streamType;
            (void)w;
            (void)h;
        }

        virtual void onFirstFrameRecved(uint64_t unixTm, const std::string& streamId, STREAM_TYPE streamType) {
            (void)unixTm;
            (void)streamId;
            (void)streamType;
        }

        virtual void onFirstVideoStreamSink(uint64_t unixTm, const std::string& streamId, STREAM_TYPE streamType, int w, int h) {
            (void)unixTm;
            (void)streamId;
            (void)streamType;
            (void)w;
            (void)h;
        }


        virtual void onStreamForzen(uint64_t unixTm, const std::string& streamId, STREAM_TYPE streamType, bool forzen) {
            (void)unixTm;
            (void)streamId;
            (void)streamType;
            (void)forzen;
        }

        virtual void onStreamSubscribeState(uint64_t unixTm,const std::string &streamId,STREAM_TYPE streamType,SUBSCRIBE_STREAM_STATE state){
            (void)unixTm;
            (void)streamId;
            (void)streamType;
            (void)state;
        }

        virtual void onStreamPublishState(uint64_t unixTm,const std::string &streamId,STREAM_TYPE streamType,PUBLISH_STREAM_STATE state){
            (void)unixTm;
            (void)streamId;
            (void)streamType;
            (void)state;
        }

        virtual void onLocalStreamState(uint64_t unixTm,const std::string &streamId,STREAM_TYPE streamType,LOCAL_STREAM_STATE state){
            (void)unixTm;
            (void)streamId;
            (void)streamType;
            (void)state;
        }
    };

    //定义共享事件管理器
    class SharedEventsManage {
    private:
        static std::mutex sharedEventsRecverMX;
        static std::map<std::string,std::shared_ptr<SharedEventsRecver>> sharedEventsRecver;

    public:
        static void registSharedEventsRecver(const std::string &recvName,const std::shared_ptr<SharedEventsRecver> &recver);

        static void removeSharedEventsRecver(const std::string &recvName);


        static void onNetTypeChanged(NETWORK_TYPE oldNetType,NETWORK_TYPE newNetType) ;

        static void onChannelConnectState(const std::string &channelId,CONNECTION_STATE_TYPE state, CONNECTION_CHANGED_REASON_TYPE reason);

        static void onStreamConnectState(const std::string &streamId,STREAM_TYPE streamType,CONNECTION_STATE_TYPE state);

        static void onJoinChannel(const std::string &channelId);

        static void onJoinChannelResult(const std::string &channelId,bool isSuccess,int reson);

        static void onLeaveChannel(const std::string &channelId);

        static void onVideoSizeChanged(const std::string &streamId,STREAM_TYPE streamType,int w,int h);

        static void onFirstFrameRecved(const std::string &streamId,STREAM_TYPE streamType);

        static void onFirstVideoStreamSink(const std::string& streamId, STREAM_TYPE streamType, int w, int h);

        static void onStreamPublishState(const std::string &streamId,STREAM_TYPE streamType,PUBLISH_STREAM_STATE state);

        static void onStreamSubscribeState(const std::string &streamId,STREAM_TYPE streamType,SUBSCRIBE_STREAM_STATE state);

        static void onStreamForzen(const std::string& streamId, STREAM_TYPE streamType, bool forzen);

        static void onLocalStreamState(const std::string &streamId,STREAM_TYPE streamType,LOCAL_STREAM_STATE state);

    };

}

#endif //PAASSDK_SHAREDEVENTSMANAGE_H
