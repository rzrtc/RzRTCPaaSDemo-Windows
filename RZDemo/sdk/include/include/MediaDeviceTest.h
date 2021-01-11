//
// Created by yhy on 10/18/20.
//

#ifndef PAASSDK_MEDIADEVICETEST_H
#define PAASSDK_MEDIADEVICETEST_H

#include <memory>

#include "DataFlow.h"
#include "MediaDefine.h"
#include "MediaSource.h"
#include "DataProducer.h"
#include "DataConsumer.h"
#include "VideoScaleCrop.h"
#include "FileAudioSource.h"
#include "AudioVolumeDetect.h"
#include "AudioDataOutputConsumer.h"
#include "AudioResample.h"

namespace rz{

    class MediaDeviceTestEventHandler{
    protected:

        virtual void onError(MODULE_TYPE type, int errCode, const std::string &errMsg) = 0;

        virtual void onWarn(MODULE_TYPE type, int warnCode, const std::string &warnMsg) = 0;
    public:

        virtual void _onError(MODULE_TYPE type, int errCode, const std::string &errMsg) final {
            onError(type,errCode,errMsg);
        }

        virtual void _onWarn(MODULE_TYPE type, int warnCode, const std::string &warnMsg) final {
            onWarn(type,warnCode,warnMsg);
        }

        virtual void onVolume(MODULE_TYPE type, int volume,int vad) = 0;
    };


    class MediaDeviceTest : public VideoSinkEventHandler,public VideoScaleCropEventHandler,public AudioSourceDataProducerEventHandler,
            public VideoSinkDataConsumerEventHandler,public VideoSourceDataProducerEventHandler,public AudioSinkEventHandler,
            public AudioResampleEventHandler,
            public AudioVolumeDetectEventHandler,public AudioSourceEventHandler,public VideoSourceEventHandler{
    private:
        //视频测试流及中间处理项
        std::shared_ptr<DataFlowMonitor> videoTestDataFlowMonitor;
        std::shared_ptr<DataFlow<VideoData>> videoTestDataFlow;
        std::shared_ptr<VideoSource> videoSource;
        std::shared_ptr<VideoSourceDataProducer> videoDataProducer;
        std::shared_ptr<VideoScaleCrop> videoScaleCrop;
        std::shared_ptr<VideoSinkDataConsumer> videoDataConsumer;
        std::shared_ptr<VideoSink> videoSink;

        //音频播放设备测试流及中间处理项
        std::shared_ptr<DataFlowMonitor> audioPlaybackTestDataFlowMonitor;
        std::shared_ptr<DataFlow<AudioData>> audioPlaybackTestDataFlow;
        std::shared_ptr<AudioSource> audioFileSource;
        std::shared_ptr<AudioSourceDataProducer> audioPlaybackDataProducer;
        std::shared_ptr<AudioVolumeDetect> audioPlaybackVolumeDetect;
        std::shared_ptr<AudioSinkDataConsumer> audioPlaybackDataConsumer;
        std::shared_ptr<AudioSink> audioSink;

        //音频录制设备测试流及中间处理项
        std::shared_ptr<DataFlowMonitor> audioRecordTestDataFlowMonitor;
        std::shared_ptr<DataFlow<AudioData>> audioRecordTestDataFlow;
        std::shared_ptr<AudioSource> audioRecordSource;
        std::shared_ptr<AudioSourceDataProducer> audioRecordDataProducer;
        std::shared_ptr<AudioVolumeDetect> audioRecordVolumeDetect;
        std::shared_ptr<AudioResample> audioResample;
        std::shared_ptr<DataConsumerAdapter<AudioData>> audioRecordDataConsumer;


        //音频回路测试流
        std::shared_ptr<DataFlowMonitor> audioLoopBackTestDataFlowMonitor;
        std::shared_ptr<DataFlow<AudioData>> audioLoopBackTestDataFlow;

        MediaDeviceTestEventHandler *eventHandler = nullptr;
    public:

        explicit MediaDeviceTest(MediaDeviceTestEventHandler *event):eventHandler(event){}

        ~MediaDeviceTest();

        int startVideoDeviceTest(void *hwnd);

        int stopVideoDeviceTest();

        int startAudioDeviceLoopbackTest(int indicationInterval);

        int stopAudioDeviceLoopbackTest();

        int startRecordingDeviceTest(int indicationInterval);

        int stopRecordingDeviceTest();

        int startPlaybackDeviceTest(const char *testAudioFilePath);

        int stopPlaybackDeviceTest();

    protected:
        void onError(MODULE_TYPE type, int errCode, const std::string &errMsg) override ;

        void onWarn(MODULE_TYPE type, int warnCode, const std::string &warnMsg) override ;

    public:

        void onFirstVideoFrameSink(int width, int height) override ;

        void onVolume(int volume,int vad) override ;

        void onVideoStartSuccess() override ;

        void onAudioStartSuccess() override ;

        void onVideoForzen(bool forzen) override ;
    };

}


#endif //PAASSDK_MEDIADEVICETEST_H
