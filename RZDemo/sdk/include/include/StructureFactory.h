//
// Created by 86157 on 2020/9/30.
//

#ifndef PAASSDK_STRUCTUREFACTORY_H
#define PAASSDK_STRUCTUREFACTORY_H

#include <memory>
#include "ProcessInterface/VideoDualStream.h"
#include "lastmileProbe/RZLastmileProbeTest.h"
#include "lastmileProbe/RZLastmileProbeCalculate.h"
#include "VideoMirrorModeProcess.h"

#include "OSModule.h"
#include "CPPMoudle.h"
#include "DeviceCollectionProducer.h"

#include "DataConsumer.h"
#include "DataProducer.h"
#include "DataFlow.h"
#include "DeviceManage.h"
#include "AudioDataOutputConsumer.h"
#include "AudioPlayback.h"
#include "AudioCodecManage.h"

#include "RZChannelHandler.h"
#include "RZStreamHandler.h"
#include "VideoCodecManage.h"
#include "MediaStreamInfo.h"

#include "IChannel.h"
#include "PubAudioStream.h"
#include "SubAudioStream.h"
#include "SubVideoStream.h"
#include "PubVideoStream.h"
#include "MediaDeviceTest.h"
#include "RZAudioVolumeIndication.h"
#include "StatisticsInfoManager.h"
#include "MediaFilter.h"
#include "../Src/Fetching/RZFetchServers.h"

namespace rz {
class StructureFactory {
        private:
    //各种结构Producer
    //平台层提供的Producer
    static std::shared_ptr<VideoCodecProducer> H264HardCodecProducer;
    static std::shared_ptr<VideoCodecProducer> H265HardCodecProducer;
    static std::shared_ptr<VideoSinkProducer> videoSinkProducer;
    static std::shared_ptr<VideoSourceProducer> videoSourceProducer;
    static std::shared_ptr<AudioSinkProducer> audioSinkProducer;
    static std::shared_ptr<AudioSourceProducer> audioSourceProducer;
    static std::shared_ptr<MediaDeviceColloectionProduce> deviceColloectionProducer;

    static std::shared_ptr<VideoCodecProducer> H264CodecProducer;
    static std::shared_ptr<VideoCodecProducer> H265CodecProducer;
    static std::shared_ptr<AudioAECProducer> audioAecProducer;
    static std::shared_ptr<AudioANCProducer> audioAncProducer;
    static std::shared_ptr<AudioCodecProducer> audioCodecProducer;
    static std::shared_ptr<AudioHowlingProducer> audioHowlingProducer;
    static std::shared_ptr<AudioMixProducer> audioMixProducer;
    static std::shared_ptr<AudioResampleProducer> audioResampleProducer;
    static std::shared_ptr<AudioVolumeDetectProducer> audioVolumeDetectProducer;
    static std::shared_ptr<MediaObserverProducer> mediaObserverProducer;
    static std::shared_ptr<MediaStreamSyncProducer> mediaStreamSyncProducer;
    static std::shared_ptr<StreamSyncProcessProducer> streamSyncProcessProducer;
    static std::shared_ptr<VideoScaleCropProducer> videoScaleCropProducer;
    static std::shared_ptr<VideoCodecManageProducer> videoCodecManageProducer;
    static std::shared_ptr<VideoMirrorModeProcessProducer> videoMirrorModeProcessProducer;
    static std::shared_ptr<FileAudioSourceProducer> fileAudioSourceProducer;

    static std::shared_ptr<rzrtc::RZChannelHandlerProducer> baseChannelProducer;
    static std::shared_ptr<rzrtc::RZStreamHandlerProducer> baseStreamProducer;

        public:
    static int Start();

    static void Stop();

    static std::shared_ptr<AudioSourceProducer> getAudioSourceProducer();

    static std::shared_ptr<AudioSinkProducer> getAudioSinkProducer();

    static std::shared_ptr<VideoSinkProducer> getVideoSinkProducer();

    static std::shared_ptr<VideoSourceProducer> getVideoSourceProducer();

    static std::shared_ptr<RZAudioVolumeIndication> createAudioVolumeIndication(
        int interval, AudioVolumeIndicationEventHandler *event);

    static std::shared_ptr<MediaDeviceTest> createMediaDeviceTest(MediaDeviceTestEventHandler *event);

    static std::shared_ptr<RZHttpClient> createHttpClient();

    static std::shared_ptr<RZLog> createRZLogImpl();

    static std::shared_ptr<StatisticsInfoManager> createStatisticsInfoManage(StatisticsEventHandler *event);

    static std::shared_ptr<systemInfoCollection> createSystemInfoCollection(int interval,
                                                                            systemInfoCollectionEventHandler *event);

    static std::shared_ptr<Channel> createChannelImpl(const std::string &mnIp, int port, ChannelEventHandler *event);

    static std::shared_ptr<net::FetchServersImpl> createFetchServersImpl(const net::FetchConfigure &);

    static std::shared_ptr<RZLastmileProbeCalculate> createLastmileProbeCalculate(
        LastmileProbeEventHandler *eventHandler, int calculateIntervalSec = 3);

    static std::shared_ptr<DataFlow<VideoData>> createVideoDataFlow(
        const std::string &flowName, const std::shared_ptr<DataFlowMonitor> &monitor,
        const std::shared_ptr<DataProducer<VideoData>> &producer,
        const std::shared_ptr<DataConsumer<VideoData>> consumer);

    static std::shared_ptr<RZLastmileProbeTest> createLastmileProbeTest(const net::FetchConfigure &netConfig);

    static std::shared_ptr<DataFlow<AudioData>> createAudioDataFlow(
        const std::string &flowName, const std::shared_ptr<DataFlowMonitor> &monitor,
        const std::shared_ptr<DataProducer<AudioData>> &producer,
        const std::shared_ptr<DataConsumer<AudioData>> consumer);

    static std::shared_ptr<AudioFilterAdapter> createAudioFilter(const std::string &cId, const std::string &id,
                                                                 const std::string &name,
                                                                 AUDIO_OBSERVER_POSITION position,
                                                                 AudioObserver *observer);

    static std::shared_ptr<VideoFilterAdapter> createVideoFilter(const std::string &cId, const std::string &id,
                                                                 const std::string &name,
                                                                 VIDEO_OBSERVER_POSITION position,
                                                                 VideoObserver *observer);

    static std::shared_ptr<AudioAEC> createAudioAEC(const AudioAECConfig &cfg, AudioAECEventHandler *Event);

    static std::shared_ptr<AudioANC> createAudioANC(AudioANCConfig &cfg, AudioANCEventHandler *errorEvent);

    static std::shared_ptr<AudioEncoder> createAudioEncoder(AUDIO_CODEC_TYPE codecType, AudioCodecConfig &cfg,
                                                            AudioEncoderEventHandler *eventHandler);

    static std::shared_ptr<AudioDecoder> createAudioDecoder(AUDIO_CODEC_TYPE codecType, AudioCodecConfig &cfg,
                                                            AudioDecoderEventHandler *eventHandler);

    static std::shared_ptr<AudioEncoderManage> createAudioEncoderManage(AUDIO_CODEC_TYPE codecType,
                                                                        AudioCodecConfig &cfg,
                                                                        AudioEncoderManagerEventHandler *eventHandler);

    static std::shared_ptr<AudioDecoderManage> createAudioDecoderManage(AudioCodecConfig &cfg,
                                                                        AudioDecoderEventHandler *eventHandler);

    static std::shared_ptr<AudioHowling> createAudioHowling(AudioHowlingConfig &cfg,
                                                            AudioHowlingEventHandler *errorEvent);

    static std::shared_ptr<AudioMix> createAudioMix(AudioMixConfig &cfg, AudioMixEventHandle *eventHandler);

    static std::shared_ptr<AudioResample> createAudioResample(AudioResampleConfig &cfg,
                                                              AudioResampleEventHandler *errorEvent);

    static std::shared_ptr<AudioVolumeDetect> createAudioVolumeDetect(AudioVolumeDetectConfig &cfg,
                                                                      AudioVolumeDetectEventHandler *eventHandler);

    static std::shared_ptr<AudioSinkDataConsumer> createAudioSinkDataConsumer(AudioSink *sink);

    static std::shared_ptr<VideoSinkDataConsumer> createVideoSinkDataConsumer(VideoSink *sink,
                                                                              VIDEO_MIRROR_MODE_TYPE mirror,
                                                                              VideoSinkDataConsumerEventHandler *event);

    static std::shared_ptr<VideoSinkDataConsumer> createVideoSinkDataConsumer(IVideoSink *sink,
                                                                              VIDEO_MIRROR_MODE_TYPE mirror,
                                                                              VideoSinkDataConsumerEventHandler *event);

    static std::shared_ptr<AudioSolSendDataConsumer> createAudioSolSendDataConsumer(
        AudioSolSendDataConsumerEventHandler *event);

    static std::shared_ptr<VideoSolSendDataConsumer> createVideoSolSendDataConsumer(
        VideoSolSendDataConsumerEventHandler *event);

    static std::shared_ptr<rzrtc::RZStreamHandler> createAudioSolPubStream(rzrtc::RZStreamConfig &context,
                                                                           rzrtc::RZStreamHandlerEvent *event);

    static std::shared_ptr<rzrtc::RZStreamHandler> createVideoSolPubStream(rzrtc::RZStreamConfig &context,
                                                                           rzrtc::RZStreamHandlerEvent *event);

    static std::shared_ptr<AudioSourceDataProducer> createAudioSourceDataProducer(
        AudioSource *source, uint32_t frameRate, AudioSourceDataProducerEventHandler *event);

    static std::shared_ptr<AudioSourceDataProducer> createAudioSourceDataProducer(
        IAudioSource *source, uint32_t frameRate, AudioSourceDataProducerEventHandler *event);

    static std::shared_ptr<VideoSourceDataProducer> createVideoSourceDataProducer(
        VideoSource *source, uint32_t frameRate, VideoSourceDataProducerEventHandler *event);

    static std::shared_ptr<VideoSourceDataProducer> createVideoSourceDataProducer(
        IVideoSource *source, uint32_t frameRate, VideoSourceDataProducerEventHandler *event, bool isLowStream);

    static std::shared_ptr<AudioSolRecvDataProducer> createAudioSolRecvDataProducer(
        const StreamConfig &stream, AudioSolRecvDataProducerEventHandler *event);

    static std::shared_ptr<VideoSolRecvDataProducer> createVideoSolRecvDataProducer(
        const StreamConfig &stream, VideoSolRecvDataProducerEventHandler *event);

    static std::shared_ptr<rzrtc::RZStreamHandler> createAudioSolSubStream(rzrtc::RZStreamConfig &context,
                                                                           rzrtc::RZStreamHandlerEvent *event);

    static std::shared_ptr<rzrtc::RZStreamHandler> createVideoSolSubStream(rzrtc::RZStreamConfig &context,
                                                                           rzrtc::RZStreamHandlerEvent *event);

    static std::shared_ptr<DataStatistics> createDataStatistics(const DataStatisticsConfig &config);

    static std::shared_ptr<IVideoDeviceCollection> createVideoDeviceColloection(VideoDeviceCollectionalEventHandler *);

    static std::shared_ptr<IAudioDeviceCollection> createAudioRecordingDeviceCollection(
        AudioDeviceCollectionalEventHandler *);

    static std::shared_ptr<IAudioDeviceCollection> createAudioPlaybackDeviceCollection(
        AudioDeviceCollectionalEventHandler *);

    static std::shared_ptr<AudioDeviceManager> createAudioDeviceManage(AudioDeviceCollectionalEventHandler *event);

    static std::shared_ptr<VideoDeviceManager> createVideoDeviceManager(VideoDeviceCollectionalEventHandler *event);

    static std::shared_ptr<VideoEncoder> createVideoEncoder(VIDEO_CODEC_TYPE type, VideoEncoderConfig &encodeCfg,
                                                            VideoEncodeEventHandler *handler);

    static std::shared_ptr<VideoDecoder> createVideoDecoder(VIDEO_CODEC_TYPE type, VideoDecodeEventHandler *handler);

    static std::shared_ptr<VideoScaleCrop> createVideoScaleCrop(VideoScaleCropConfig &dst,
                                                                VideoScaleCropEventHandler *eventHandler);

    static std::shared_ptr<StreamSyncProcess<VideoData>> createVideoStreamSyncProcess(MediaStreamSync *sync);

    static std::shared_ptr<StreamSyncProcess<AudioData>> createAudioStreamSyncProcess(MediaStreamSync *sync);

    static std::shared_ptr<VideoPreviewObserver> createVideoPreviewObserver(const PubVideoStreamInfo &stream,
                                                                            IVideoSink *sink,
                                                                            VIDEO_MIRROR_MODE_TYPE mirror,
                                                                            VideoPreviewObserverEventHandler *event);

    static std::shared_ptr<VideoPreviewObserver> createVideoPreviewObserver(const PubVideoStreamInfo &stream,
                                                                            VIDEO_MIRROR_MODE_TYPE mirror,
                                                                            VideoPreviewObserverEventHandler *event);

    static std::shared_ptr<AudioDataOutputConsumer> createAudioDataOutputConsumer(
        const SubAudioStreamInfo &stream, AudioDataOutputConsumerEventHandler *event);

    static std::shared_ptr<AudioDataOutputConsumer> createAudioDataOutputConsumer(
        const SubAudioStreamInfo &stream, IAudioSink *sink, AudioDataOutputConsumerEventHandler *event);

    static std::shared_ptr<DataProducerAdapter<VideoData>> createVideoDataProducerAdapter();

    static std::shared_ptr<DataProducerAdapter<AudioData>> createAudioDataProducerAdapter();

    static std::shared_ptr<DataConsumerAdapter<VideoData>> createVideoDataConsumerAdapter(
        std::function<void(std::shared_ptr<VideoData> &data)> fun);

    static std::shared_ptr<DataConsumerAdapter<AudioData>> createAudioDataConsumerAdapter(
        std::function<void(std::shared_ptr<AudioData> &data)> fun);

    static std::shared_ptr<VideoEncodecManage> createVideoEncoderManage(CODEC_PRIORITY codecPriority,
                                                                        VideoEncoderConfig cfg,
                                                                        VideoEncodeManageEventHandler *event);

    static std::shared_ptr<VideoDecodecManage> createVideoDecoderManage(const std::string &streamId,
                                                                        CODEC_PRIORITY codecPriority,
                                                                        VideoDecodeManageEventHandler *event);

    static std::shared_ptr<VideoMirrorModeProcess> createVideoMirrorModeProcess(VideoMirrorModeProcessConfig &cfg,
                                                                                VideoMirrorModeProcessEventHandler *);

    static std::shared_ptr<PubVideoDualStream> createPubVideoDualStream(const PubVideoStreamInfo &streamInfo,
                                                                        PubVideoDualStreamEventHandler *event);

    static std::shared_ptr<SubVideoDualStream> createSubVideoDualStream(const SubVideoStreamInfo &streamInfo,
                                                                        MediaStreamSync *sync,
                                                                        SubVideoDualStreamEventHandler *event);

    static std::shared_ptr<VideoSink> createVideoSink(const VideoSinkConfig &cfg, VideoSinkEventHandler *event);

    static std::shared_ptr<AudioSink> createAudioSink(const AudioSinkConfig &cfg, AudioSinkEventHandler *event);

    static std::shared_ptr<VideoSource> createVideoSource(VideoConfig &cfg, VideoSourceEventHandler *event);

    static std::shared_ptr<AudioSource> createAudioSource(AudioConfig &cfg, AudioSourceEventHandler *event);

    static std::shared_ptr<PubVideoStream> createPubVideoStream(const PubVideoStreamInfo &stream,
                                                                const std::shared_ptr<VideoSource> &source,
                                                                PubVideoStreamEventHandler *event,
                                                                std::function<void()> fun = nullptr);

    static std::shared_ptr<PubVideoStream> createPubVideoStream(const PubVideoStreamInfo &stream, IVideoSource *source,
                                                                PubVideoStreamEventHandler *event,
                                                                std::function<void()> fun = nullptr,
                                                                bool isLowStream = false);

    static std::shared_ptr<SubVideoStream> createSubVideoStream(const SubVideoStreamInfo &stream, MediaStreamSync *sync,
                                                                IVideoSink *sink, SubVideoStreamEventHandler *event,
                                                                std::function<void()> fun = nullptr);

    static std::shared_ptr<SubVideoStream> createSubVideoStream(const SubVideoStreamInfo &stream, MediaStreamSync *sync,
                                                                SubVideoStreamEventHandler *event,
                                                                std::function<void()> fun = nullptr);

    static std::shared_ptr<PubAudioStream> createPubAudioStream(const PubAudioStreamInfo &stream,
                                                                const std::shared_ptr<AudioSource> &source,
                                                                PubAudioStreamEventHandler *event,
                                                                std::function<void()> fun = nullptr);

    static std::shared_ptr<PubAudioStream> createPubAudioStream(const PubAudioStreamInfo &stream,
                                                                const IAudioSource *source,
                                                                PubAudioStreamEventHandler *event,
                                                                std::function<void()> fun = nullptr);

    static std::shared_ptr<SubAudioStream> createSubAudioStream(const SubAudioStreamInfo &stream, MediaStreamSync *sync,
                                                                SubAudioStreamEventHandler *event,
                                                                std::function<void()> fun = nullptr);

    static std::shared_ptr<SubAudioStream> createSubAudioStream(const SubAudioStreamInfo &stream, MediaStreamSync *sync,
                                                                IAudioSink *sink, SubAudioStreamEventHandler *event,
                                                                std::function<void()> fun = nullptr);

    static std::shared_ptr<AudioPlayback> createAudioPlayback(const AudioSinkConfig &cfg,
                                                              AudioPlaybackEventHandler *event);

    static std::shared_ptr<AudioSource> createFileAudioSource(const char *fileName, AudioSourceEventHandler *event);

    static std::shared_ptr<MediaStreamSync> createMediaStreamSync(const mediaStreamSyncConfig &cfg);

    static std::shared_ptr<rzrtc::RZChannelHandler> createBaseChannel(rzrtc::IChannelHandlerEvent *event);
};
}  // namespace rz

#endif  // PAASSDK_STRUCTUREFACTORY_H
