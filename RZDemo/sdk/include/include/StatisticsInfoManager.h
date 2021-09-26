//
// Created by yhy on 11/25/20.
//

#ifndef PAASSDK_STATISTICSINFOMANAGER_H
#define PAASSDK_STATISTICSINFOMANAGER_H

#include <thread>
#include <mutex>

#include "RZPaaSConfig.h"
#include "InteractiveManage.h"
#include "SharedEventsManage.h"
#include "../LogCollector/GlobalInfoCache.h"

namespace rz {

//统计信息计算回调接口
class StatisticsEventHandler {
public:
    virtual void onRtcStats(const std::string& channelName, const RtcStats& stats) = 0;

    virtual void onRemoteAudioStats(const std::string& channelName, const std::string& streamId,
                                    RemoteAudioStats& stats) = 0;

    virtual void onRemoteVideoStats(const std::string& channelName, const std::string& streamId,
                                    RemoteVideoStats& stats) = 0;

    virtual void onLocalAudioStats(const std::string& streamName, LocalAudioStats& stats) = 0;

    virtual void onLocalVideoStats(const std::string& channelName, const std::string& streamName,
                                   LocalVideoStats& stats) = 0;
};

//各种统计信息的事件的产生 statsProducer基类
//主要负责任务注册及注销
class StatsProducer {
protected:
    StatisticsEventHandler* eventHandler = nullptr;

    std::string statsProducerId;

    virtual void WorkFun() = 0;

public:
    StatsProducer(const std::string& ID, StatisticsEventHandler* event) {
        eventHandler = event;
        statsProducerId = ID;

        InteractiveManage::registTimedTask(statsProducerId + "statsProducer", std::bind(&StatsProducer::WorkFun, this),
                                           RZPaaSConfig::statisticsInfoInterval * 1000, false);
    }

    virtual ~StatsProducer() { InteractiveManage::removeTimedTask(statsProducerId + "statsProducer"); }
};

//负责RtcStats事件的产生
class RtcStatsProducer : public StatsProducer {
    uint32_t startTime = 0;
    RtcStats lastStats;

protected:
    void WorkFun() override;

public:
    RtcStatsProducer(const std::string& channelId, StatisticsEventHandler* event) : StatsProducer(channelId, event) {
        startTime = RZTime::opts();

        GlobalInfoCache::ForEachStreamInfos([this](std::shared_ptr<StreamInfoCache> info) {
            if (info->Static.streamType == PUB_VIDEO_STREAM || info->Static.streamType == PUB_VIDEO_LOW_STREAM) {
                lastStats.txVideoBytes += (info->Dynamic.TXBytes());
            } else if (info->Static.streamType == SUB_VIDEO_STREAM || info->Static.streamType == SUB_VIDEO_LOW_STREAM) {
                lastStats.rxVideoBytes += (info->Dynamic.TXBytes());
            } else if (info->Static.streamType == PUB_AUDIO_STREAM) {
                lastStats.txAudioBytes += (info->Dynamic.TXBytes());
            } else if (info->Static.streamType == SUB_AUDIO_STREAM) {
                lastStats.rxAudioBytes += (info->Dynamic.TXBytes());
            }
        });
        lastStats.txBytes = lastStats.txAudioBytes + lastStats.txVideoBytes;
        lastStats.rxBytes = lastStats.rxAudioBytes + lastStats.rxVideoBytes;
    }

    RtcStats getRtcStats();
};

//负责远端音频流Stats事件的产生
class RemoteAudioStatsProducer : public StatsProducer {
private:
    uint32_t startTime = 0;

    uint32_t stateChangedTime = 0;
    SUBSCRIBE_STREAM_STATE streamState = SUBSCRIBE_STREAM_STATE_ONLINE;

    int lastLossCount = 0;
    int lastRecvCount = 0;

    int lastByteCount = 0;

    //卡顿时长
    int totalFrozenTime = 0;

    // 卡顿状态标识
    bool lastForzenState = false;

    //有效时长
    int totalActiveTime = 1;

protected:
    void WorkFun() override;

public:
    RemoteAudioStatsProducer(const std::string& streamId, StatisticsEventHandler* event)
        : StatsProducer(streamId, event) {
        startTime = RZTime::opts();
        auto streamInfo = GlobalInfoCache::GetStreamInfoByID(streamId);
        if (streamInfo) {
            lastLossCount = streamInfo->Dynamic.PacketLoss();
            lastRecvCount = streamInfo->Dynamic.FrameCount();
            lastByteCount = streamInfo->Dynamic.TXBytes();
        }
    }

    void SetStreamSubscribestate(SUBSCRIBE_STREAM_STATE state);

    void streamForzenStateChange(bool frozen);
};

//负责远端视频流Stats事件的产生
class RemoteVideoStatsProducer : public StatsProducer {
private:
    uint32_t startTime = 0;

    uint32_t stateChangedTime = 0;
    SUBSCRIBE_STREAM_STATE streamState = SUBSCRIBE_STREAM_STATE_ONLINE;

    int lastLossCount = 0;
    int lastRecvCount = 0;

    int lastByteCount = 0;

    //卡顿时长
    int totalFrozenTime = 0;

    // 卡顿状态标识
    bool lastForzenState = false;

    //有效时长
    int totalActiveTime = 1;

protected:
    void WorkFun() override;

public:
    RemoteVideoStatsProducer(const std::string& streamId, StatisticsEventHandler* event)
        : StatsProducer(streamId, event) {
        startTime = RZTime::opts();
        auto streamInfo = GlobalInfoCache::GetStreamInfoByID(streamId);
        if (streamInfo) {
            lastLossCount = streamInfo->Dynamic.PacketLoss();
            lastRecvCount = streamInfo->Dynamic.FrameCount();
            lastByteCount = streamInfo->Dynamic.TXBytes();
        }
    }

    void SetStreamSubscribestate(SUBSCRIBE_STREAM_STATE state);

    void streamForzenStateChange(bool frozen);
};

//负责本地音频流Stats事件的产生
class LocalAudioStatsProducer : public StatsProducer {
private:
    uint32_t lastByteCount = 0;

protected:
    void WorkFun() override;

public:
    LocalAudioStatsProducer(const std::string& streamId, StatisticsEventHandler* event)
        : StatsProducer(streamId, event) {
        auto streamInfo = GlobalInfoCache::GetStreamInfoByID(streamId);
        if (streamInfo) {
            lastByteCount = streamInfo->Dynamic.TXBytes();
        }
    }
};

//负责本地视频流Stats事件的产生
class LocalVideoStatsProducer : public StatsProducer {
private:
    uint32_t lastFrameCount = 0;
    uint32_t lastByteCount = 0;

protected:
    void WorkFun() override;

public:
    LocalVideoStatsProducer(const std::string& streamId, StatisticsEventHandler* event)
        : StatsProducer(streamId, event) {
        auto streamInfo = GlobalInfoCache::GetStreamInfoByID(streamId);
        if (streamInfo) {
            lastByteCount = streamInfo->Dynamic.TXBytes();
            lastFrameCount = streamInfo->Dynamic.FrameCount();
        }
    }
};

class StatisticsInfoManager : public SharedEventsRecver {
private:
    StatisticsEventHandler* eventHandler = nullptr;

    std::mutex statsProducerPoolMX;
    std::map<std::string, std::shared_ptr<StatsProducer>> statsProducerPool;

public:
    explicit StatisticsInfoManager(StatisticsEventHandler* event);

    RtcStats getRtcStats(const std::string& channelId);

    //事件收集
    void onJoinChannel(uint64_t unixTm, const std::string& channelId) override;

    void onLeaveChannel(uint64_t unixTm, const std::string& channelId) override;

    void onStreamSubscribeState(uint64_t unixTm, const std::string& streamId, STREAM_TYPE streamType,
                                SUBSCRIBE_STREAM_STATE state) override;

    void onLocalStreamState(uint64_t unixTm, const std::string& streamId, STREAM_TYPE streamType,
                            LOCAL_STREAM_STATE state) override;

    void onStreamFrozen(uint64_t unixTm, const std::string& streamId, STREAM_TYPE streamType, bool forzen,
                        uint64_t frameCount) override;
};

}  // namespace rz

#endif  //PAASSDK_STATISTICSINFOMANAGER_H
