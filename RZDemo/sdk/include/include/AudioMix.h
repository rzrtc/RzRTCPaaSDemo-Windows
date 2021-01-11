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

namespace rz {

#define MAX_AUDIO_NUM 32

    struct AudioMixConfig {

    };


    class AudioMixEventHandle{
    protected:
        virtual void onError(MODULE_TYPE type,int errCode, const std::string &errMsg) = 0;
        virtual void onWarn(MODULE_TYPE type,int warnCode, const std::string &warnMsg) = 0;
    public:
        //媒体处理项运行时错误回调输出
        virtual void onError(int errCode, const std::string &errMsg) final {
            onError(MODULE_AUDIO_MIX,errCode,errMsg);
        }

        virtual void onWarn(int warnCode, const std::string &warnMsg) final {
            onWarn(MODULE_AUDIO_MIX,warnCode,warnMsg);
        }

        virtual ~AudioMixEventHandle() = default;
    };


    class AudioMix : public DataProducer<AudioData>,public AudioDataOutputAdapter {
    private:
        struct AudioStream {
            std::list<std::shared_ptr<AudioData>> dataPool;
            bool isPush = false;
            bool isStart = false;
        };
        std::mutex AudioDataPoolMX;
        std::map<void *, std::shared_ptr<AudioStream>> AudioDataPool;
        std::list<std::shared_ptr<AudioData>> mixDataPool;
    protected:

        virtual void GetAudioData(std::list<std::shared_ptr<AudioData>> &dataList) final {
            std::lock_guard<std::mutex> lk(AudioDataPoolMX);
            for (auto &iter : AudioDataPool) {
                if (iter.second->isStart && !iter.second->isPush) {
                    if (!iter.second->dataPool.empty()) {
                        auto dataIter = iter.second->dataPool.begin();
                        auto data = *dataIter;
                        iter.second->dataPool.erase(dataIter);
                        mixDataPool.push_back(data);
                    }
                    else {
                        iter.second->isStart = false;
                    }
                }
                iter.second->isPush = false;
            }

            dataList.splice(dataList.begin(), mixDataPool);
        }

    public:
        explicit AudioMix():DataProducer<AudioData>("AudioMix"){}

        //实现该函数以进行混音操作
        virtual void WorkFun() = 0;


        int registAudioStream(void *flag,const SubAudioStreamInfo &streamInfo) final {
            AudioDataPoolMX.lock();
            if (AudioDataPool.find(flag) != AudioDataPool.end()) {

                AudioDataPoolMX.unlock();
                return -1;
            }

            bool isstart = AudioDataPool.empty();
            auto stream = std::make_shared<AudioStream>();
            AudioDataPool.insert(std::pair<void *, std::shared_ptr<AudioStream>>(flag, stream));

            AudioDataPoolMX.unlock();
            if(isstart){
                //委托任务执行器20ms执行一次任务,并立即开始
                DelegatManager::registTimedTask("AudioPlayBack_AudioMixModule",std::bind(&AudioMix::WorkFun,this),20,true);
            }
            return 0;
        }

        int removeAudioStream(void *flag) final {
            AudioDataPoolMX.lock();
            AudioDataPool.erase(flag);

            bool isstop = AudioDataPool.empty();
            AudioDataPoolMX.unlock();
            if(isstop){
                //取消任务执行器的定时任务
                DelegatManager::removeTimedTask("AudioPlayBack_AudioMixModule");
            }
            return 0;
        }

        int pushAudioData(void *f, std::shared_ptr<AudioData> &data) final {
            std::lock_guard<std::mutex> lk(AudioDataPoolMX);
            auto iter = AudioDataPool.find(f);
            iter->second->dataPool.push_back(data);

            if (!iter->second->isStart) {
                if (iter->second->dataPool.size() > 1) {
                    iter->second->isStart = true;
                }
                return 0;
            }

            auto dataIter = iter->second->dataPool.begin();
            if (!iter->second->isPush) {
                auto audioData = *dataIter;
                iter->second->dataPool.erase(dataIter);
                mixDataPool.push_back(audioData);
                iter->second->isPush = true;
            }
            else if (iter->second->dataPool.size() > 3) {
                iter->second->dataPool.erase(dataIter);
            }

            return 0;
        }

        virtual int Reset(AudioMixConfig &cfg) = 0;

        virtual int Release() = 0;

        ~AudioMix() override = default;
    };



    class AudioMixProducer {
    public:
        virtual AudioMix *GetAudioMix(AudioMixConfig &cfg, AudioMixEventHandle *eventHandler) = 0;

        virtual void DelAudioMix(AudioMix *) = 0;

        virtual int Release() = 0;

        virtual ~AudioMixProducer() = default;
    };
}

#endif //PAASSDK_AUDIOMIX_H
