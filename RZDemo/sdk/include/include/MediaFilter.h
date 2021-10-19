#ifndef PAASSDK_MEDIAFILTER_H
#define PAASSDK_MEDIAFILTER_H

#include <functional>

#include "DataFlow.h"

namespace rz {

template<class DataType>
class MediaFilter : public DataProcess<DataType> {
protected:
    virtual bool FilterDataFun(std::shared_ptr<DataType>& data) = 0;

public:
    //构造函数
    MediaFilter(const ProcessInfo& info) : DataProcess<DataType>(info) {}

    void WorkFun(std::shared_ptr<DataType>& data) final {
        if (FilterDataFun(data)) {
            this->SendData(data);
        }
    }
};

class VideoFilterAdapter : public MediaFilter<VideoData> {
private:
    std::string channelId;
    std::string uid;
    std::string streamName;
    VIDEO_OBSERVER_POSITION observerPosition;
    VideoObserver* videoObserver = nullptr;

protected:
    bool FilterDataFun(std::shared_ptr<VideoData>& data) override;
    void generateVideoFrame(VideoFrame& videoFrame, std::shared_ptr<VideoData>& data);

public:
    VideoFilterAdapter(const std::string& cId, const std::string& id, const std::string& name,
                       VIDEO_OBSERVER_POSITION position, VideoObserver* observer)
        : MediaFilter<VideoData>(VideoFilterProcessInfo[(int)position]) {
        channelId = cId;
        uid = id;
        streamName = name;
        observerPosition = position;
        videoObserver = observer;
    }
};

class AudioFilterAdapter : public MediaFilter<AudioData> {
private:
    std::string channelId;
    std::string uid;
    std::string streamName;
    AUDIO_OBSERVER_POSITION observerPosition;
    AudioObserver* audioObserver = nullptr;

protected:
    bool FilterDataFun(std::shared_ptr<AudioData>& data) override;
    void generateAudioFrame(AudioFrame& audioFrame, std::shared_ptr<AudioData>& data);

public:
    AudioFilterAdapter(const std::string& cId, const std::string& id, const std::string& name,
                       AUDIO_OBSERVER_POSITION position, AudioObserver* observer)
        : MediaFilter<AudioData>(AudioFilterProcessInfo[(int)position]) {
        channelId = cId;
        uid = id;
        streamName = name;
        observerPosition = position;
        audioObserver = observer;
    }
};
}  // namespace rz

#endif  //PAASSDK_MEDIAFILTER_H
