//
// Created by yhy on 10/16/20.
//

#ifndef PAASSDK_AUDIOPLAYBACK_H
#define PAASSDK_AUDIOPLAYBACK_H

#include "DataFlow.h"
#include "AudioMix.h"
#include "MediaSink.h"
#include "DataConsumer.h"
#include "AudioResample.h"
#include "AudioPlaybackBuf.h"

namespace rz {

class AudioPlaybackEventHandler {
protected:
    virtual void onError(MODULE_TYPE type, int errCode, const std::string& errMsg) = 0;
    virtual void onWarn(MODULE_TYPE type, int warnCode, const std::string& warnMsg) = 0;

public:
    virtual void _onError(MODULE_TYPE type, int errCode, const std::string& errMsg) final {
        onError(type, errCode, errMsg);
    }

    virtual void _onWarn(MODULE_TYPE type, int warnCode, const std::string& warnMsg) final {
        onWarn(type, warnCode, warnMsg);
    }

    virtual void onVolume(MODULE_TYPE type, int volume, int vad) = 0;

    virtual ~AudioPlaybackEventHandler() = default;
};

class AudioPlayback
    : public AudioMixEventHandle
    , public AudioResampleEventHandler
    , public AudioSinkEventHandler
    , public AudioVolumeDetectEventHandler {
private:
    std::shared_ptr<DataFlowMonitor> audioPlaybackMonitor;
    std::shared_ptr<DataFlow<AudioData>> audioPlaybackFlow;

    std::shared_ptr<AudioMix> audioMix;
    std::shared_ptr<AudioResample> audioResample;
    std::shared_ptr<AudioVolumeDetect> audioVolumeDetect;
    std::shared_ptr<AudioFilterAdapter> audioPlaybackFilter;
    std::shared_ptr<AudioSinkDataConsumer> audioSinkDataConsumer;

    std::shared_ptr<AudioSink> audioSink;

    AudioPlaybackEventHandler* eventHandler = nullptr;

public:
    explicit AudioPlayback(const AudioSinkConfig& sinkCfg, AudioPlaybackEventHandler* event);

    ~AudioPlayback() override;

    AudioDataOutputAdapter* getAudioOutputAdapter();

    void onVolume(int volume, int vad) override;

    int enableAudioVolumeIndication(int interval, int smooth, bool report_vad);

    void Start();

    void Stop();

    void setAudioPlaybackFilter(std::shared_ptr<AudioFilterAdapter>& filter);

    void removeAudioPlaybackFilter();

protected:
    void onError(MODULE_TYPE type, int errCode, const std::string& errMsg) override;

    void onWarn(MODULE_TYPE type, int warnCode, const std::string& warnMsg) override;
};

}  // namespace rz

#endif  //PAASSDK_AUDIOPLAYBACK_H
