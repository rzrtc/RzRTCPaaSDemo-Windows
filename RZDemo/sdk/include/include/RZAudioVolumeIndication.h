/**
 * Copyright (c) 2018 Duobei Brothers Information Technology Co.,Ltd. All rights reserved.
 *
 * Author: visa
 * 
 * Date: 2020-11-9
 *
 * About this file: This file holds the declaration/implementation of class RZAudioVolumeIndication
 */

#pragma once

/**
 * RZAudioVolumeIndication is a class represents
 */
#include <string>
#include <atomic>
#include <list>
#include <mutex>
#include <thread>
#include <unordered_map>
#include "../IRtcEngineState.h"
#include "InteractiveManage.h"

namespace rz {
class AudioVolumeIndicationEventHandler {
public:
    virtual void onAudioVolumeIndication(const AudioVolumeInfo* speakers, unsigned int speakerNumber, int totalVolume) {

    }
};

class RZAudioVolumeIndication {
public:
    struct StreamVolumeInfo {
        std::string uid;
        std::string channelId;
        int vad = 0;
        int totalVolume = 0;
        std::list<int> volumeList;
    };

public:
    RZAudioVolumeIndication(int calculateIntervalMillsec, AudioVolumeIndicationEventHandler* eventHandler);

    ~RZAudioVolumeIndication();

public:
    void start();

    void stop();

    void pushStreamVolumeInfo(std::string channelId, std::string uid, std::string streamId, int volume, int vad);

    void pushTotalVolume(int volume);

    void reset(int calculateIntervalMillsec);

private:
    InteractiveManage::TimerTaskID m_timerID {0};
    std::atomic_bool m_running = {false};

    std::atomic_int m_calculateIntervalMillsec = {200};

    AudioVolumeIndicationEventHandler* m_pEventHandler = nullptr;

    std::mutex m_totalVolumeListMx;
    std::list<int> m_totalVolumeList;

    std::mutex m_streamVolumeMapMx;
    std::unordered_map<std::string, StreamVolumeInfo> m_streamVolumeMap;

private:
    void calculateVolumeFunction();
};

};  // namespace rz
