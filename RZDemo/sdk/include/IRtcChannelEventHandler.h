//
// Created by yhy on 11/1/20.
//

#ifndef PAASSDK_IRTCCHANNELEVENTHANDLER_H
#define PAASSDK_IRTCCHANNELEVENTHANDLER_H

#include "IRtcEngineState.h"

namespace rz {
    class IRtcChannel;

    class IRtcChannelEventHandler{
    public:

        /**
         * 发生警告回调。
         * @param channel
         * @param warn
         * @param msg
         */
        virtual void onWarning(IRtcChannel *channel,int warn, const char *msg) {
            (void) channel;
            (void) warn;
            (void) msg;
        }

        /**
         * 发生错误回调。
         * @param channel
         * @param err
         * @param msg
         */
        virtual void onError(IRtcChannel *channel,int err, const char *msg) {
            (void) channel;
            (void) err;
            (void) msg;
        }

        /**
         * 加入频道成功的回调。
         * @param channel
         * @param uid
         * @param elapsed
         */
        virtual void onJoinChannelSuccess(IRtcChannel *channel, const char *uid, int elapsed) {
            (void) channel;
            (void) uid;
            (void) elapsed;
        }

        /**
         * 重新加入频道成功的回调。
         * @param channel
         * @param uid
         * @param elapsed
         */
        virtual void onRejoinChannelSuccess(IRtcChannel *channel, const char *uid, int elapsed) {
            (void) channel;
            (void) uid;
            (void) elapsed;
        }

        /**
         * 离开频道回调。
         * @param channel
         * @param stats
         */
        virtual void onLeaveChannel(IRtcChannel *channel, const RtcStats &stats) {
            (void) channel;
            (void) stats;
        }

        /**
         * 直播场景下用户角色已切换回调。
         * @param channel
         * @param oldRole
         * @param newRole
         * @param elapsed
         */
        virtual void onClientRoleChanged(IRtcChannel *channel,CLIENT_ROLE_TYPE oldRole, CLIENT_ROLE_TYPE newRole,int elapsed) {
            (void) channel;
            (void) oldRole;
            (void) newRole;
            (void) elapsed;
        }

        /**
         * 远端用户（通信场景）/主播（直播场景）加入当前频道回调。
         * @param channel
         * @param uid
         * @param elapsed
         */
        virtual void onUserJoined(IRtcChannel *channel, const char *uid, int elapsed) {
            (void) channel;
            (void) uid;
            (void) elapsed;
        }

        /**
         * 远端用户（通信场景）/主播（直播场景）离开当前频道回调。
         * @param channel
         * @param uid
         * @param reason
         */
        virtual void onUserOffline(IRtcChannel *channel, const char *uid, USER_OFFLINE_REASON_TYPE reason) {
            (void) channel;
            (void) uid;
            (void) reason;
        }


        virtual void onAudioPublishStateChanged(IRtcChannel *channel,PUBLISH_STREAM_STATE state, int elapsed) {
            (void) channel;
            (void) state;
            (void) elapsed;
        }

        virtual void onVideoPublishStateChanged(IRtcChannel *channel,PUBLISH_STREAM_STATE state, int elapsed) {
            (void) channel;
            (void) state;
            (void) elapsed;
        }

        virtual void onAudioSubscribeStateChanged(IRtcChannel *channel,const char *uid, SUBSCRIBE_STREAM_STATE state, int elapsed) {
            (void) channel;
            (void) uid;
            (void) state;
            (void) elapsed;
        }

        virtual void onVideoSubscribeStateChanged(IRtcChannel *channel,const char *uid, const char *streamName, SUBSCRIBE_STREAM_STATE state, int elapsed) {
            (void) channel;
            (void) uid;
            (void) streamName;
            (void) state;
            (void) elapsed;
        }

        /**
         * 已显示首帧远端视频回调。
         * @param channel
         * @param width
         * @param height
         * @param elapsed
         * @return
         */
        virtual void onFirstRemoteVideoFrame(IRtcChannel *channel,const char *uid, const char *streamName, int width, int height, int elapsed) {
            (void) channel;
            (void) uid;
            (void) streamName;
            (void) width;
            (void) height;
            (void) elapsed;
        }

        /**
         * 远端视频大小发生改变回调。
         * @param channel
         * @param uid
         * @param streamName
         * @param width
         * @param height
         * @param rotation
         */
        virtual void onVideoSizeChanged(IRtcChannel *channel,const char *uid, const char *streamName, int width, int height, int elapsed) {
            (void) channel;
            (void) uid;
            (void) streamName;
            (void) width;
            (void) height;
            (void) elapsed;
        }

        /**
         * 通话中每个用户的网络上下行 last mile 质量报告回调。
         * @param channel
         * @param uid
         * @param txQuality
         * @param rxQuality
         */
        virtual void onNetworkQuality(IRtcChannel *channel,const char *uid, QUALITY_TYPE txQuality, QUALITY_TYPE rxQuality) {
            (void) channel;
            (void) uid;
            (void) txQuality;
            (void) rxQuality;
        }

        /**
         *网络连接丢失回调。
         * @param channel
         */
        virtual void onConnectionLost(IRtcChannel *channel) {
            (void) channel;
        }

        virtual void onConnectionStateChanged(IRtcChannel *channel,CONNECTION_STATE_TYPE state,CONNECTION_CHANGED_REASON_TYPE reason) {
            (void) channel;
            (void) state;
            (void) reason;
        }


        /**
         * 当前通话统计回调。
         * @param channel
         * @param stats
         */
        virtual void onRtcStats(IRtcChannel *channel,const RtcStats &stats) {
            (void) channel;
            (void) stats;
        }

        /**
         * 通话中远端音频流的统计信息回调
         * @param channel
         * @param stats
         */
        virtual void onRemoteAudioStats(IRtcChannel *channel,const RemoteAudioStats &stats) {
            (void) channel;
            (void) stats;
        }

        /**
         * 通话中远端视频流的统计信息回调。
         * @param channel
         * @param stats
         */
        virtual void onRemoteVideoStats(IRtcChannel *channel,const RemoteVideoStats &stats) {
            (void) channel;
            (void) stats;
        }


        virtual ~IRtcChannelEventHandler() = default;
    };

}
#endif //PAASSDK_IRTCCHANNELEVENTHANDLER_H
