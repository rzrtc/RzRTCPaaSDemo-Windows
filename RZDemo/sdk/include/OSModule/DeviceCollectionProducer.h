//
// Created by yhy on 11/1/20.
//

#ifndef PAASSDK_DEVICECOLLECTIONPRODUCER_H
#define PAASSDK_DEVICECOLLECTIONPRODUCER_H

#include "DeviceCollection.h"
#include "DeviceCollectionEventHandler.h"

namespace rz{

    class MediaDeviceColloectionProduce {
    public:
        virtual IAudioDeviceCollection *GetAudioPlaybackDeviceCollection(AudioDeviceCollectionalEventHandler *event) = 0;

        virtual IAudioDeviceCollection *GetAudioRecordingDeviceCollection(AudioDeviceCollectionalEventHandler *event) = 0;

        virtual IVideoDeviceCollection *GetVideoDeviceColloection(VideoDeviceCollectionalEventHandler *event) = 0;

        virtual void DelAudioPlaybackDeviceCollection(IAudioDeviceCollection *) = 0;

        virtual void DelAudioRecordingDeviceCollection(IAudioDeviceCollection *) = 0;

        virtual void DelVideoDeviceColloection(IVideoDeviceCollection *) = 0;

        virtual void Release() = 0;

        virtual ~MediaDeviceColloectionProduce() = default;
    };
}

#endif //PAASSDK_DEVICECOLLECTIONPRODUCER_H
