//
// Created by yhy on 11/1/20.
//

#ifndef PAASSDK_RTCENGINECONTEXT_H
#define PAASSDK_RTCENGINECONTEXT_H

#include <map>
#include <mutex>
#include <memory>
#include <string>
#include <atomic>

#include "../IMediaSource.h"
#include "../IMediaSink.h"
#include "RtcEngineEventHandler.h"
#include "../IRtcStreamEventHandler.h"
#include "IChannel/IChannel.h"
#include "MediaStream/SubAudioStream.h"
#include "MediaStream/SubVideoStream.h"
#include "MediaStream/PubVideoStream.h"
#include "MediaStream/PubAudioStream.h"
#include "../LogCollector/GlobalInfoCache.h"
#include "../Src/Fetching/RZFetchServers.h"


namespace rz{

    struct IRtcChannelContext;


    enum STREAM_CONTEXT_TYPE {
        STREAM_CONTEXT_LOCAL_VIDEO,
        STREAM_CONTEXT_REMOTE_VIDEO,
        STREAM_CONTEXT_LOCAL_AUDIO,
        STREAM_CONTEXT_REMOTE_AUDIO
    };
    struct BaseStreamContext {
        STREAM_CONTEXT_TYPE streamContextType;
        BaseStreamContext(STREAM_CONTEXT_TYPE type) {
            streamContextType = type;
        }
    };


    struct localVideoStreamContext : public BaseStreamContext {
        IRtcChannelContext *channelContext = nullptr;

        std::shared_ptr<StreamInfoCache> streamInfo;
        std::shared_ptr<StreamInfoCache> lowStreamInfo;

        std::string streamName;
        std::string streamId;

        bool enable = true;
        bool mute = false;
        bool publish = false;

        //是否已经开始preview
        bool preview = false;

        VIDEO_STREAM_TYPE streamType = VIDEO_STREAM_FRAME;
        IVideoSource *videoSource = nullptr;
        VideoEncoderConfiguration videoConfig;

        bool enabledDual = false;

        IVideoSink *videoSink = nullptr; //是否配置自定义的本地视频回显渲染器
        RENDER_MODE_TYPE renderMode = RENDER_MODE_HIDDEN;
        VIDEO_MIRROR_MODE_TYPE mirrorMode = VIDEO_MIRROR_MODE_AUTO;
        bool haveVideoCanvas = false;
        VideoCanvas videoCanvas;    //本地视频的canvas

        IRtcStreamEventHandler *eventHandler = nullptr;
        CONNECTION_STATE_TYPE connectionStateType = CONNECTION_STATE_DISCONNECTED;

        bool firstPublished = false;
        LOCAL_VIDEO_STREAM_STATE localVideoState = LOCAL_VIDEO_STREAM_STATE_STOPPED;
        PUBLISH_STREAM_STATE videoStreamPublishState = PUBLISH_STREAM_STREAM_STATE_NO_PUBLISH;

        std::shared_ptr<PubVideoStream> pubVideoStream;

        IVideoSource* lowVideoStreamSource = nullptr;
        std::shared_ptr<PubVideoStream> lowPubVideoStream;

        localVideoStreamContext():BaseStreamContext(STREAM_CONTEXT_LOCAL_VIDEO) {}

        ~localVideoStreamContext() {
            streamInfo.reset();
            lowStreamInfo.reset();

            GlobalOperation::ReleaseView(videoCanvas.view);
            GlobalInfoCache::DelStreamInfo(streamId);
            GlobalInfoCache::DelStreamInfo(streamId + "-low");
        }
    };

    inline localVideoStreamContext* getStreamContext(localVideoStreamContext* ptr) {
        if (ptr->streamContextType != STREAM_CONTEXT_LOCAL_VIDEO) {
            LOG_ERROR("StreamContextConstant") << "指针转换错误";
            return nullptr;
        }
        return (localVideoStreamContext*)ptr;
    }
    struct localAudioStreamContext : public BaseStreamContext {
        IRtcChannelContext *channelContext = nullptr;

        std::shared_ptr<StreamInfoCache> streamInfo;

        std::string streamName;
        std::string streamId;

        bool enable = true;
        bool mute = false;
        bool publish = false;
        bool enableAec = true;

        AudioSourceConfig sourceConfig;
        IAudioSource *audioSource = nullptr;
        AUDIO_PROFILE_TYPE audioProfileType = AUDIO_PROFILE_DEFAULT;
        AUDIO_SCENARIO_TYPE audioScenarioType = AUDIO_SCENARIO_DEFAULT;

        CONNECTION_STATE_TYPE connectionStateType = CONNECTION_STATE_DISCONNECTED;

        bool firstPublished = false;
        LOCAL_AUDIO_STREAM_STATE localAudioState = LOCAL_AUDIO_STREAM_STATE_STOPPED;
        PUBLISH_STREAM_STATE audioStreamPublishState = PUBLISH_STREAM_STREAM_STATE_NO_PUBLISH;

        std::shared_ptr<PubAudioStream> pubAudioStream;

        localAudioStreamContext() :BaseStreamContext(STREAM_CONTEXT_LOCAL_AUDIO) {}

        ~localAudioStreamContext() {
            streamInfo.reset();

            GlobalInfoCache::DelStreamInfo(streamId);
        }
    };

    inline localAudioStreamContext* getStreamContext(localAudioStreamContext* ptr) {
        if (ptr->streamContextType != STREAM_CONTEXT_LOCAL_AUDIO) {
            LOG_ERROR("StreamContextConstant") << "指针转换错误";
            return nullptr;
        }
        return (localAudioStreamContext*)ptr;
    }
    struct remoteVideoStreamContext : public BaseStreamContext {
        IRtcChannelContext *channelContext = nullptr;

        std::shared_ptr<StreamInfoCache> streamInfo;
        std::shared_ptr<StreamInfoCache> lowStreamInfo;

        std::string uid;
        std::string streamName;
        std::string streamId;
        bool haveLocalMute = false;
        bool localMute = false;
        RENDER_MODE_TYPE renderMode = RENDER_MODE_HIDDEN;
        VIDEO_MIRROR_MODE_TYPE mirrorMode = VIDEO_MIRROR_MODE_AUTO;
        bool setStreamType = false;
        REMOTE_VIDEO_STREAM_TYPE videoStreamType = REMOTE_VIDEO_STREAM_HIGH;
        IVideoSink *videoSink = nullptr; //是否配置自定义的本地视频回显渲染器
        bool haveVideoCanvas = false;
        VideoCanvas videoCanvas;    //本地视频的canvas

        bool isDual = false;                   //推流端是否开启了双流模式
        bool remoteMute = false;

        CONNECTION_STATE_TYPE connectionStateType = CONNECTION_STATE_DISCONNECTED;
        SUBSCRIBE_STREAM_STATE videoStreamSubscribeState = SUBSCRIBE_STREAM_STATE_ONLINE;

        bool firstrecv = false;
        std::shared_ptr<SubVideoStream> subVideoStream;

        std::shared_ptr<MediaStreamSync> streamSync;

        remoteVideoStreamContext() :BaseStreamContext(STREAM_CONTEXT_REMOTE_VIDEO) {}

        ~remoteVideoStreamContext() {
            subVideoStream.reset();
            streamSync.reset();
            streamInfo.reset();
            lowStreamInfo.reset();

            GlobalOperation::ReleaseView(videoCanvas.view);
            GlobalInfoCache::DelStreamInfo(streamId);
            GlobalInfoCache::DelStreamInfo(streamId + "-low");
        }
    };

    inline remoteVideoStreamContext* getStreamContext(remoteVideoStreamContext* ptr) {
        if (ptr->streamContextType != STREAM_CONTEXT_REMOTE_VIDEO) {
            LOG_ERROR("StreamContextConstant") << "指针转换错误";
            return nullptr;
        }
        return (remoteVideoStreamContext*)ptr;
    }
    struct remoteAudioStreamContext : public BaseStreamContext {
        IRtcChannelContext *channelContext = nullptr;

        std::shared_ptr<StreamInfoCache> streamInfo;

        std::string uid;
        std::string streamName;
        std::string streamId;
        bool haveLocalMute = false;
        bool localMute = false;
        IAudioSink *audioSink = nullptr;

        bool remoteMute = false;

        CONNECTION_STATE_TYPE connectionStateType = CONNECTION_STATE_DISCONNECTED;
        SUBSCRIBE_STREAM_STATE audioStreamSubscribeState = SUBSCRIBE_STREAM_STATE_ONLINE;

        bool firstrecv = false;
        std::shared_ptr<SubAudioStream> subAudioStream;
        std::shared_ptr<MediaStreamSync> streamSync;

        remoteAudioStreamContext() :BaseStreamContext(STREAM_CONTEXT_REMOTE_AUDIO) {}

        ~remoteAudioStreamContext() {
            subAudioStream.reset();
            streamSync.reset();
            streamInfo.reset();

            GlobalInfoCache::DelStreamInfo(streamId);
        }
    };

    inline remoteAudioStreamContext* getStreamContext(remoteAudioStreamContext* ptr) {
        if (ptr->streamContextType != STREAM_CONTEXT_REMOTE_AUDIO) {
            LOG_ERROR("StreamContextConstant") << "指针转换错误";
            return nullptr;
        }
        return (remoteAudioStreamContext*)ptr;
    }

    struct IRtcEngineContext {
        std::recursive_mutex IRtcEngineMX;
        static std::atomic<int> haveJoinChannel;
        //标记本地流是否已发布到频道
        std::atomic<bool> publishLocalStream;
        std::string envConfig;
        map<std::string, std::string> envConfigMap;
        int regionCode = 0;
        std::string appId;
        int baseTime = 0;
        CODEC_PRIORITY codecPriority = PRIORITY_AUTO;
        RtcEngineEventHandler *eventHandler = nullptr;

        bool AudioVolumeIndication = false;  //音量提示标记符
        int interval = 1;    //回调间隔 ms
        int smooth = 10;     //音量检测平滑度
        bool report_vad = false;

        AudioObserver* audioObserver = nullptr;
        uint8_t audioFilterPosition = 0;
        VideoObserver* videoObserver = nullptr;
        uint8_t videoFilterPosition = 0;
        //当前发布流的频道
        std::shared_ptr<IRtcChannelContext> pushChannelContext;

        std::map<std::string,std::shared_ptr<IRtcChannelContext>> channelContextPool;

        std::shared_ptr<localVideoStreamContext> localVideoStreamCtx;
        std::shared_ptr<localAudioStreamContext> localAudioStreamCtx;
    };

    struct IRtcChannelContext {
        IRtcEngineContext *engineContext = nullptr;

        std::shared_ptr<ChannelInfoCache> channelInfo;
        std::shared_ptr<UserInfoCache> userInfo;

        std::shared_ptr<net::FetchServersImpl> fetchServersImpl;

        std::string mnIP;
        int mnPort;

        std::string solIP;
        int solPort;
        std::vector<std::string> solProxy;

        //频道场景 默认为通信场景
        CHANNEL_PROFILE_TYPE channelProfileType = CHANNEL_PROFILE_COMMUNICATION;
        CLIENT_ROLE_TYPE clientRoleType = CLIENT_ROLE_BROADCASTER; //用户角色 通信场景默认为主播 直播场景默认为观众

        std::string channelId;
        std::string channelName;

        std::string uid;

        bool isJoinChannel = false;

        //标记本地流是否发布到本频道
        PUBLISH_MEDIA_TYPE publishType = PUBLISH_INIT;

        CONNECTION_STATE_TYPE connectionStateType = CONNECTION_STATE_DISCONNECTED;  //网络连接状态

        REMOTE_VIDEO_STREAM_TYPE deafautRemoteVideoType = REMOTE_VIDEO_STREAM_HIGH;

        bool defaultAudioMute = false;
        bool defaultVideoMute = false;
        bool allAudioMute = false;
        bool allVideoMute = false;

        //记录joinchannel前要求推送的其他路视频流
        std::map<std::string, std::shared_ptr<localVideoStreamContext>> pubVideoStreamContextPool;

        std::map<std::string, std::shared_ptr<remoteVideoStreamContext>> subVideoStreamContextPool;

        std::map<std::string, std::shared_ptr<remoteAudioStreamContext>> subAudioStreamContexPool;
        //MN的封装, 主要负责和信令服务器的通信
        std::shared_ptr<Channel> channelImpl;


        ~IRtcChannelContext() {
            //缓存清理, 暂时先分开吧
			channelInfo.reset();
			userInfo.reset();

            //发布流和订阅流的上下文pool清理
            pubVideoStreamContextPool.clear();
            subVideoStreamContextPool.clear();
            subAudioStreamContexPool.clear();

			//缓存清理, 暂时先分开吧
            GlobalInfoCache::DelChannelInfo(channelId);
            GlobalInfoCache::DelUserInfo();
        }

        int getSolAddress(std::string &solIp,int &port) {
            auto log = RZLOG_GUARD("fetchServer", " getLibSolSrvAddress ", 300);
            if(solIP.empty()){
                int errcode = 0;
                auto solAds = fetchServersImpl->GetSolServerAddr(errcode);

                if (solAds == nullptr) {
                    return errcode;
                }
                solIP = solAds->proxy.ip;
                solPort = solAds->proxy.port;
                solProxy = solAds->proxy.proxyList;
            }
            solIp = solIP;
            port = solPort;
            return 0;
        }

        int getMNAddress(std::string &mnIp,int &port){
            auto log = RZLOG_GUARD("fetchServer", " getLibMnSrvAddress ", 300);
            if(mnIP.empty()) {
                int errcode = 0;
                auto mnAds = fetchServersImpl->GetMnServerAddr(errcode);
                if (mnAds == nullptr) {
                    return errcode;
                }

                mnIP = mnAds->mnAddr.ip;
                mnPort = mnAds->mnAddr.port;
            }
            mnIp = mnIP;
            port = mnPort;
            return 0;
        }
    };
}


#endif //PAASSDK_RTCENGINECONTEXT_H
