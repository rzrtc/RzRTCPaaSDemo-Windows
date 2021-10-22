//
// Created by 86157 on 2020/9/22.
//

#ifndef PAASSDK_AUDIOMIX_H
#define PAASSDK_AUDIOMIX_H

#include <list>
#include <mutex>
#include <map>

#include "DataFlow.h"
#include "DelegatManager.h"
#include "MediaProcessDefine.h"
#include "AudioDataOutputAdapter.h"
#include "MediaTool/Audio/AudioResample.h"

namespace rz {

#define MAX_AUDIO_NUM 32

struct AudioMixConfig {};

class AudioMixEventHandle {
protected:
    virtual void onError(MODULE_TYPE type, int errCode, const std::string& errMsg) = 0;
    virtual void onWarn(MODULE_TYPE type, int warnCode, const std::string& warnMsg) = 0;

public:
    //媒体处理项运行时错误回调输出
    virtual void onError(int errCode, const std::string& errMsg) final { onError(MODULE_AUDIO_MIX, errCode, errMsg); }

    virtual void onWarn(int warnCode, const std::string& warnMsg) final { onWarn(MODULE_AUDIO_MIX, warnCode, warnMsg); }

    virtual ~AudioMixEventHandle() = default;
};
/**
 * 混音模块目前只支持单声道16000采样,s16采样类型的音频数据
 */
class AudioMix
    : public DataProducer<AudioData>
    , public AudioDataOutputAdapter {
private:
    struct AudioStream {
        std::list<std::shared_ptr<AudioData>> dataPool;
        bool isPush = false;
        bool isStart = false;
    };
    std::mutex AudioDataPoolMX;
    std::map<void*, std::shared_ptr<AudioStream>> AudioDataPool;
    std::list<std::shared_ptr<AudioData>> mixDataPool;
    DuoBei::AudioResampleInitCfg initCfg {};
    // 音频帧数据最大缓存数量
    static const uint32_t MAX_CACHE_SIZE {10};
    // 每帧采样数量
    static const uint32_t FRAME_SAMPLE_SIZE {320};
    // 单个采样字节数
    static const uint32_t SAMPLE_SIZE {2};
    // 采样率
    static const uint32_t SAMPLE_RATE {16000};

protected:
    virtual void GetAudioData(std::list<std::shared_ptr<AudioData>>& dataList) final;

public:
    explicit AudioMix();
    virtual ~AudioMix();

    //实现该函数以进行混音操作
    virtual void WorkFun() = 0;

    int registAudioStream(void* flag, const SubAudioStreamInfo& streamInfo) final;

    int removeAudioStream(void* flag) final;

    int pushAudioData(void* f, std::shared_ptr<AudioData>& data) final;

    virtual int Reset(AudioMixConfig& cfg) = 0;

    virtual int Release() = 0;
};

class AudioMixProducer {
public:
    virtual AudioMix* GetAudioMix(AudioMixConfig& cfg, AudioMixEventHandle* eventHandler) = 0;

    virtual void DelAudioMix(AudioMix*) = 0;

    virtual int Release() = 0;

    virtual ~AudioMixProducer() = default;
};
}  // namespace rz

#endif  //PAASSDK_AUDIOMIX_H
