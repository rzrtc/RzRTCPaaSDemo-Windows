//
// Created by 86157 on 2020/9/22.
//

#ifndef PAASSDK_MEDIAPROCESSDEFINE_H
#define PAASSDK_MEDIAPROCESSDEFINE_H

#include <string>
#include <functional>
#include <memory>
//#include "RtcEngineContext.h"


namespace rz {

    struct MediaData{
        //私有域，process模块单独使用，不可跨process使用
        void *privateData = nullptr;

        virtual ~MediaData() = default;
    };

    enum DECODER_TYPE {
        HARDWARE_DECODER = 0,
        SOFTWARE_DECODER,
    };

    enum MODULE_TYPE{
        MODULE_UNKNOW = -1,

        MODULE_VIDEO_ENCODER_MANAGE,
        MODULE_VIDEO_DECODER_MANAGE,
        MODULE_VIDEO_MIRRORMODEPROCESS,
        MODULE_VIDEO_SCALECROP,
        MODULE_VIDEO_SOLSEND,
        MODULE_VIDEO_SOLRECV,
        MODULE_VIDEO_SOURCE,
        MODULE_VIDEO_SINK,
        MODULE_VIDEO_DUALSTREAM,


        MODULE_PUBVIDEO,
        MODULE_PUBVIDEO_LOW,
        MODULE_PUBVIDEO_PERVIEW,
        MODULE_PUBVIDEO_PERVIEW_SINK,
        MODULE_PUBVIDEO_PREVIEW_SCALECROP,
        MODULE_PUBVIDEO_ENCODER_MANAGE,
        MODULE_PUBVIDEO_LOW_ENCODER_MANAGE,
        MODULE_PUBVIDEO_MIRRORMODEPROCESS,
        MODULE_PUBVIDEO_SCALECROP,
        MODULE_PUBVIDEO_LOW_SCALECROP,
        MODULE_PUBVIDEO_SOLSEND,
        MODULE_PUBVIDEO_LOW_SOLSEND,
        MODULE_PUBVIDEO_SOURCE,

        MODULE_SUBVIDEO,
        MODULE_SUBVIDEO_LOW,
        MODULE_SUBVIDEO_DECODER_MANAGE,
        MODULE_SUBVIDEO_SCALECROP,
        MODULE_SUBVIDEO_SOLRECV,
        MODULE_SUBVIDEO_LOW_SOLRECV,
        MODULE_SUBVIDEO_SINK,
        MODULE_SUBVIDEO_DUALSTREAM,


        MODULE_AUDIO_SOURCE,
        MODULE_AUDIO_MIX,
        MODULE_AUDIO_AEC,
        MODULE_AUDIO_ANC,
        MODULE_AUDIO_DECODER,
        MODULE_AUDIO_ENCODER,
        MODULE_AUDIO_HOWLING,
        MODULE_AUDIO_RESAMPLE,
        MODULE_AUDIO_VOLUMEDETECT,
        MODULE_AUDIO_SOLSEND,
        MODULE_AUDIO_SOLRECV,
        MODULE_AUDIO_SINK,

        MODULE_AUDIO_PLAYBACK,
        MODULE_AUDIO_PLAYBACK_MIX,
        MODULE_AUDIO_PLAYBACK_SINK,
        MODULE_AUDIO_PLAYBACK_RESAMPLE,

        MODULE_PUBAUDIO,
        MODULE_PUBAUDIO_SOURCE,
        MODULE_PUBAUDIO_AEC,
        MODULE_PUBAUDIO_ANC,
        MODULE_PUBAUDIO_ENCODER,
        MODULE_PUBAUDIO_VOLUMEDETECT,
        MODULE_PUBAUDIO_RESAMPLE,
        MODULE_PUBAUDIO_SOLSEND,

        MODULE_SUBAUDIO,
        MODULE_SUBAUDIO_DECODER,
        MODULE_SUBAUDIO_HOWLING,
        MODULE_SUBAUDIO_VOLUMEDETECT,
        MODULE_SUBAUDIO_RESAMPLE,
        MODULE_SUBAUDIO_SOLRECV,

        MODULE_AUDIO_DEVICE_COLLOCTION,
        MODULE_VIDEO_DEVICE_COLLOCTION,

        MODULE_CHANNEL,

        MODULE_MEDIA_DEVICETEST,

        MODULE_DEVICETEST_VIDEO_SINK,
        MODULE_DEVICETEST_VIDEO_SCALECROP,
        MODULE_DEVICETEST_AUDIO_VOLUMEDETECT,
        MODULE_DEVICETEST_AUDIO_SOURCE,
        MODULE_DEVICETEST_VIDEO_SOURCE,

        MODULE_MEDIA_STREAMSYNC,
    };

    enum AUDIO_PROCESS_LEVEL {
        AUDIO_PROCESS_LEVEL_LOW = 0,
        AUDIO_PROCESS_LEVEL_MEDIUM,
        AUDIO_PROCESS_LEVEL_HIGH,
        AUDIO_PROCESS_LEVEL_AGGRESSIVE
    };
}
#endif //PAASSDK_MEDIAPROCESSDEFINE_H
