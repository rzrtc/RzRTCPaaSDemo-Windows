//
// Created by 86157 on 2020/9/23.
//

#ifndef PAASSDK_AUDIOPLAYBACKBUF_H
#define PAASSDK_AUDIOPLAYBACKBUF_H

#include <memory>
#include <mutex>

#include "MediaDefine.h"

namespace rz {

class AudioPlaybackBuf final{
    std::shared_ptr<AudioData> playbackData;

private:
    AudioPlaybackBuf() {}
    AudioPlaybackBuf(const AudioPlaybackBuf&) = delete;
    AudioPlaybackBuf(const AudioPlaybackBuf&&) = delete;
    AudioPlaybackBuf& operator=(const AudioPlaybackBuf&) = delete;
    AudioPlaybackBuf& operator=(const AudioPlaybackBuf&&) = delete;

public:
    ~AudioPlaybackBuf() = default;

public:
    static AudioPlaybackBuf& GetInstance() {
        static AudioPlaybackBuf audioPlaybackBuf;
        return audioPlaybackBuf;
    }
    inline std::shared_ptr<AudioData> getPlaybackAudioData() {
        return std::atomic_load(&this->playbackData);
    }

    inline void pushPlaybackAudioData(std::shared_ptr<AudioData>& data) {
        std::atomic_store(&(this->playbackData), data);
    }
};

}  // namespace rz
#endif  //PAASSDK_AUDIOPLAYBACKBUF_H
