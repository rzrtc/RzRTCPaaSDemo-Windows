//
// Created by yhy on 10/28/20.
//

#ifndef PAASSDK_RTCENGINEEVENTHANDLER_H
#define PAASSDK_RTCENGINEEVENTHANDLER_H

#include <map>
#include <mutex>
#include <string>

#include "../IRtcEngineState.h"
#include "../IRtcEngineEventHandler.h"
#include "../IRtcChannelEventHandler.h"
#include "../IRtcStreamEventHandler.h"

namespace rz{

    class RtcEngineEventHandler {
        std::recursive_mutex eventHandlerMX;
        IRtcEngineEventHandler * eventHandler = nullptr;                     //本地相关事件回调eventhandler 包含本地默认的音视频流
        std::map<std::string,IRtcChannelEventHandler *> channelEventHandlerPool; //channel相关事件回调eventHandler 包含远端音视频流事件 频道内用户上下线
        std::map<std::string,IRtcChannel *>channelPool;
        std::map<std::string,IRtcStreamEventHandler *> streamEventHandlerPool;   //本地另推的视频流的相应事件回调eventHandler
        std::map<std::string,IRtcStream *> streamPool;
    public:

        explicit RtcEngineEventHandler(IRtcEngineEventHandler *event) : eventHandler(event){}

        bool addChannelEventHandler(const std::string &channelName,IRtcChannelEventHandler *event,IRtcChannel *);

        bool addStreamEventHandler(const std::string &streamFlag,IRtcStreamEventHandler *event,IRtcStream *);

        void removeChannelEventHandler(const std::string &channelName);

        void removeStreamEventHandler(const std::string &streamFlag);

        void release();

        /**
         * 发生警告回调。
         * @param warn
         * @param msg
         */
        void onWarning(int warn, const std::string &msg) ;

        /**
         * 发生错误回调。
         * @param err
         * @param msg
         */
        void onError(int err, const std::string &msg) ;

        /**
         * 发生警告回调。
         * @param warn
         * @param msg
         */
        void onWarning(const std::string &channelName,int warn, const std::string &msg) ;

        /**
         * 发生错误回调。
         * @param err
         * @param msg
         */
        void onError(const std::string &channelName,int err, const std::string &msg) ;

        /**
         * 加入频道回调。
         * @param channel
         * @param uid
         * @param elapsed
         */
        void onJoinChannelSuccess(const std::string &channelName, const std::string &uid, int elapsed) ;

        /**
         * 重新加入频道回调。
         * @param channel
         * @param uid
         * @param elapsed
         */
        void onRejoinChannelSuccess(const std::string &channelName, const std::string &uid, int elapsed) ;

        /**
         * 离开频道回调。
         * @param stats
         */
        void onLeaveChannel(const std::string &channelName, const RtcStats &stats) ;

        /**
         * 直播场景下用户角色已切换回调。
         * @param oldRole
         * @param newRole
         * @param elapsed
         */
        void onClientRoleChanged(const std::string &channelName,CLIENT_ROLE_TYPE oldRole, CLIENT_ROLE_TYPE newRole, int elapsed);

        /**
         * 远端用户（通信场景）/主播（直播场景）加入当前频道回调。
         * @param channelName
         * @param uid
         * @param elapsed
         */
        void onUserJoined(const std::string &channelName,const std::string &userId, int elapsed);

        /**
         * 远端用户（通信场景）/主播（直播场景）离开当前频道回调。
         * @param uid
         * @param reason
         */
        void onUserOffline(const std::string &channelName,const std::string &uid, USER_OFFLINE_REASON_TYPE reason);

        void onLocalVideoStateChanged(const std::string &channelName,const std::string &streamName, LOCAL_VIDEO_STREAM_STATE state,LOCAL_VIDEO_STREAM_ERROR error, int elapsed);

        void onLocalAudioStateChanged(const std::string &channelName,const std::string &streamName,LOCAL_AUDIO_STREAM_STATE state, LOCAL_AUDIO_STREAM_ERROR error, int elapsed);

        void onAudioPublishStateChanged(const std::string &channelName,const std::string &streamName, PUBLISH_STREAM_STATE state, int elapsed);

        void onVideoPublishStateChanged(const std::string &channelName,const std::string &streamName, PUBLISH_STREAM_STATE state, int elapsed);

        void onAudioSubscribeStateChanged(const std::string &channelName,const std::string &uid,const std::string &streamFlag, SUBSCRIBE_STREAM_STATE oldState, int elapsed);

        void onVideoSubscribeStateChanged(const std::string &channelName,const std::string &uid,const std::string &streamFlag, SUBSCRIBE_STREAM_STATE oldState, int elapsed);

        /**
         * 提示频道内谁正在说话、说话者音量及本地用户是否在说话的回调。
         * @param speakers
         * @param speakerNumber
         * @param totalVolume
         */
        void
        onAudioVolumeIndication(const AudioVolumeInfo *speakers, unsigned int speakerNumber, int totalVolume) ;


        /**
         * 已显示本地视频首帧回调。
         * @param width
         * @param height
         * @param elapsed
         */
        void onFirstLocalVideoFrame(const std::string &channelName,const std::string &streamName,int width, int height, int elapsed) ;

        /**
         * 已发布本地视频首帧回调。
         * @param elapsed
         */
        void onFirstLocalVideoFramePublished(const std::string &channelName,const std::string &streamName, int elapsed) ;

        /**
         * 已显示首帧远端视频回调。
         * @param streamInfo
         * @param width
         * @param height
         * @param elapsed
         * @return
         */
        void onFirstRemoteVideoFrame(const std::string &channelName,const std::string &uid,const std::string &streamName, int width, int height, int elapsed) ;

        /**
         * 音频设备变化回调。
         * @param deviceId
         * @param deviceType
         * @param deviceState
         */
        void onMediaDeviceStateChanged(const std::string &deviceId, MEDIA_DEVICE_TYPE deviceType, int deviceState) ;

        /**
         * 回放、录音设备、或 App 的音量发生改变。
         * @param deviceType
         * @param volume
         * @param muted
         */
        void onAudioDeviceVolumeChanged(MEDIA_DEVICE_TYPE deviceType, int volume, bool muted) ;

        /**
         * 本地或远端视频大小和旋转信息发生改变回调。
         * @param streamInfo
         * @param width
         * @param height
         * @param rotation
         */
        void onLocalVideoSizeChanged(const std::string &channelName , const std::string &uid,const std::string &streamName, int width, int height, int elapsed) ;

        /**
         * 远端视频大小和旋转信息发生改变回调。
         * @param streamInfo
         * @param width
         * @param height
         * @param rotation
         */
        void onRemoteVideoSizeChanged(const std::string &channelName , const std::string &uid,const std::string &streamName, int width, int height, int elapsed) ;

        /**
         * 已发布本地音频首帧回调。
         * @param elapsed
         */
        void onFirstLocalAudioFramePublished(const std::string &channelName,const std::string &streamName, int elapsed) ;

        /**
         * 语音路由已发生变化回调。
         * @param routing
         */
        void onAudioRouteChanged(AUDIO_ROUTE_TYPE routing) ;

        /**
         * 网络连接状态已改变回调。
         * @param state
         * @param reason
         */
        void onConnectionStateChanged(const std::string &channelName,CONNECTION_STATE_TYPE state, CONNECTION_CHANGED_REASON_TYPE reason) ;

        /**
         * 本地网络类型发生改变回调。
         * @param type
         */
        void onNetworkTypeChanged(NETWORK_TYPE type) ;

        /**
         * 通话中远端音频流的统计信息回调
         * @param stats
         */
        void onRemoteAudioStats(const std::string &channelName,const RemoteAudioStats &stats) ;

        /**
         * 通话中本地音频流的统计信息回调。
         * @param stats
         */
        void onLocalAudioStats(const LocalAudioStats &stats) ;

        /**
         * 通话中远端视频流的统计信息回调。
         * @param stats
         */
        void onRemoteVideoStats(const std::string &channelName,const RemoteVideoStats &stats) ;

        /**
         * 本地视频流统计信息回调。
         * @param stats
         */
        void onLocalVideoStats(const std::string& channelName, const std::string& streamName, const LocalVideoStats &stats) ;

        /**
         * 通话中每个用户的网络上下行 last mile 质量报告回调。
         * @param uid
         * @param txQuality
         * @param rxQuality
         */
        void onNetworkQuality(const std::string &channelName,const std::string &uid, int txQuality, int rxQuality) ;

        /**
         * 当前通话统计回调。
         * @param stats
         */
        void onRtcStats(const std::string &channelName,const RtcStats &stats) ;

        /**
         *网络连接丢失回调。
         */
        void onConnectionLost(const std::string &channelName) ;


        /**
         * 通话前网络质量探测报告回调。
         * @param result
         */
        void onLastmileProbeResult(const LastmileProbeResult &result) ;

        ~RtcEngineEventHandler() = default;

    };

}

#endif //PAASSDK_RTCENGINEEVENTHANDLER_H
