//
// Created by 86157 on 2020/9/23.
//

#ifndef PAASSDK_AUDIOPLAYBACKBUF_H
#define PAASSDK_AUDIOPLAYBACKBUF_H

#include <memory>
#include <mutex>

#include "MediaDefine.h"

namespace rz {

    class AudioPlaybackBuf {
        static std::mutex playbackDataMX;
        static std::shared_ptr<AudioData> playbackData;
    public:
        static std::shared_ptr<AudioData> getPlaybackAudioData() {
            std::lock_guard<std::mutex> lk(playbackDataMX);
            return playbackData;
        }

        static void pushPlaybackAudioData(std::shared_ptr<AudioData> &data) {
            std::lock_guard<std::mutex> lk(playbackDataMX);
            playbackData.swap(data);
        }
    };

}
#endif //PAASSDK_AUDIOPLAYBACKBUF_H
