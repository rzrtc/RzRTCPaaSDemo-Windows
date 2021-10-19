//
// Created by yhy on 10/16/20.
//

#ifndef PAASSDK_AUDIODATAOUTPUTADAPTER_H
#define PAASSDK_AUDIODATAOUTPUTADAPTER_H

#include <map>
#include <mutex>
#include <atomic>

#include "MediaDefine.h"
#include "MediaStreamInfo.h"
//#include "StructureFactory.h"

namespace rz {

class AudioDataOutputAdapter {
public:
    virtual int registAudioStream(void* flag, const SubAudioStreamInfo& streamInfo) = 0;

    virtual int removeAudioStream(void* flag) = 0;

    virtual int pushAudioData(void* flag, std::shared_ptr<AudioData>& data) = 0;
};

class AudioPlayback;

class AudioDataOutput {
private:
    static AudioDataOutputAdapter* adapter;
    static AudioPlayback* audioPlayback;
    static std::atomic_int subAudioStreamNum;

public:
    static void Start(AudioDataOutputAdapter* ptr, AudioPlayback* playback);

    static void Stop();

    static int registAudioStream(void* flag, const SubAudioStreamInfo& streamInfo);

    static int removeAudioStream(void* flag);

    static int pushAudioData(void* flag, std::shared_ptr<AudioData>& data);
};

}  // namespace rz

#endif  //PAASSDK_AUDIODATAOUTPUTADAPTER_H
