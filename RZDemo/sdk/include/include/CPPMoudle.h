//
// Created by 86157 on 2020/9/30.
//

#ifndef PAASSDK_CPPMOUDLE_H
#define PAASSDK_CPPMOUDLE_H

#include "AudioAEC.h"
#include "AudioANC.h"
#include "AudioMix.h"
#include "VideoCodec.h"
#include "StreamSync.h"
#include "AudioCodec.h"
#include "AudioHowling.h"
#include "AudioResample.h"
#include "VideoScaleCrop.h"
#include "VideoCodecManage.h"
#include "AudioVolumeDetect.h"
#include "MediaObserverProducer.h"
#include "VideoMirrorModeProcess.h"
#include "FileAudioSource.h"

namespace rz {

class CPPMoudle {
public:
    //创建 AudioAECProducer
    static AudioAECProducer* createAudioAECProducer();

    //释放 AudioAECProducer
    static void deleteAudioAECProducer(AudioAECProducer*);

    //创建 AudioANCProducer
    static AudioANCProducer* createAudioANCProducer();

    //释放 AudioANCProducer
    static void deleteAudioANCProducer(AudioANCProducer*);

    //创建 AudioCodecProducer
    static AudioCodecProducer* createAudioCodecProducer();

    //释放 AudioCodecProducer
    static void deleteAudioCodecProducer(AudioCodecProducer*);

    //创建 AudioHowlingProducer
    static AudioHowlingProducer* createAudioHowlingProducer();

    //释放 AudioHowlingProducer
    static void deleteAudioHowlingProducer(AudioHowlingProducer*);

    //创建 AudioMixProducer
    static AudioMixProducer* createAudioMixProducer();

    //释放 AudioMixProducer
    static void deleteAudioMixProducer(AudioMixProducer*);

    //创建 AudioResampleProducer
    static AudioResampleProducer* createAudioResampleProducer();

    //释放 AudioResampleProducer
    static void deleteAudioResampleProducer(AudioResampleProducer*);

    //创建 AudioVolumeDetectProducer
    static AudioVolumeDetectProducer* createAudioVolumeDetectProducer();

    //释放 AudioVolumeDetectProducer
    static void deleteAudioVolumeDetectProducer(AudioVolumeDetectProducer*);

    //创建 MediaObserverProduce
    static MediaObserverProducer* createMediaObserverProduce();

    //释放 MediaObserverProduce
    static void deleteMediaObserverProduce(MediaObserverProducer*);

    //创建 MediaStreamSyncProducer
    static MediaStreamSyncProducer* createMediaStreamSyncProducer();

    //释放 MediaStreamSyncProducer
    static void deleteMediaStreamSyncProducer(MediaStreamSyncProducer*);

    //创建 streamSyncProcessProducer
    static StreamSyncProcessProducer* createstreamSyncProcessProducer();

    //释放 streamSyncProcessProducer
    static void deletestreamSyncProcessProducer(StreamSyncProcessProducer*);

    //创建 VideoCodecProducer
    static VideoCodecProducer* createH264VideoCodecProducer();

    //释放 VideoCodecProducer
    static void deleteH264VideoCodecProducer(VideoCodecProducer*);

    //创建 VideoCodecProducer
    static VideoCodecProducer* createH265VideoCodecProducer();

    //释放 VideoCodecProducer
    static void deleteH265VideoCodecProducer(VideoCodecProducer*);

    //创建 VideoScaleCropProducer
    static VideoScaleCropProducer* createVideoScaleCropProducer();

    //释放 VideoScaleCropProducer
    static void deleteVideoScaleCropProducer(VideoScaleCropProducer*);

    static VideoCodecManageProducer* createVideoCodecManageProducer();

    static void deleteVideoCodecManageProducer(VideoCodecManageProducer* ptr);

    static VideoMirrorModeProcessProducer* createVideoMirrorModeProcessProducer();

    static void deleteVideoMirrorModeProcessProducer(VideoMirrorModeProcessProducer* ptr);

    static FileAudioSourceProducer* createFileAudioSourceProducer();

    static void deleteFileAudioSourceProducer(FileAudioSourceProducer* ptr);
};

}  // namespace rz

#endif  //PAASSDK_CPPMOUDLE_H
