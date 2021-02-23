//
// Created by yhy on 11/3/20.
//

#ifndef PAASSDK_RTCCHANNEL_H
#define PAASSDK_RTCCHANNEL_H

#include "RtcEngineContext.h"
#include "../IRtcChannel.h"

namespace rz{

    class RtcChannel : public IRtcChannel{
        IRtcChannelContext *channelContext = nullptr;
    protected:
        ~RtcChannel() override = default;
    public:
        std::map<std::string ,IRtcStream*> pubVideoStreamPool;
        explicit RtcChannel(CHANNEL_PROFILE_TYPE profileType,const char *channelId,IRtcChannelContext *context);

        void release() override ;

        const char * getChannelId() override ;

        int setClientRole(CLIENT_ROLE_TYPE role) override ;

        int setRemoteVideoSink(const char *uid,const char *streamName,IVideoSink *sink) override ;

        int joinChannel(const char *uid) override ;

        int leaveChannel() override ;

        /**
         * 所有参数必须传递, 不能为空
         */
        IRtcStream * createVideoStream(const char *streamName, IVideoSource *videoSource,VIDEO_STREAM_TYPE  streamType,IRtcStreamEventHandler *event,const VideoEncoderConfiguration &videoCfg) override ;

        int publish(PUBLISH_MEDIA_TYPE publishType = PUBLISH_AUDIO_VIDEO) override ;

        int unPublish(PUBLISH_MEDIA_TYPE publishType = PUBLISH_AUDIO_VIDEO) override ;

        int setupRemoteVideo(const char *uid,const char *streamName, const VideoCanvas &canvas) override ;

        int setDefaultMuteAllRemoteVideoStreams(bool mute) override ;

        int muteAllRemoteVideoStreams(bool mute) override ;

        int muteRemoteVideoStream(const char *uid,const char *streamName, bool mute) override ;

        int setRemoteRenderMode(const char *uid,const char *streamName, RENDER_MODE_TYPE renderMode,VIDEO_MIRROR_MODE_TYPE mirrorMode) override ;

        int setRemoteDefaultVideoStreamType(REMOTE_VIDEO_STREAM_TYPE streamType) override ;

        int setRemoteVideoStreamType(const char *uid,const char *streamName, REMOTE_VIDEO_STREAM_TYPE streamType) override ;

        int muteAllRemoteAudioStreams(bool mute) override ;

        int setDefaultMuteAllRemoteAudioStreams(bool mute) override ;

        int muteRemoteAudioStream(const char *uid, bool mute) override ;

        CONNECTION_STATE_TYPE getConnectionState() override ;
    };
}
#endif //PAASSDK_RTCCHANNEL_H
