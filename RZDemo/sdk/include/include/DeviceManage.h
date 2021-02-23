//
// Created by 86157 on 2020/9/25.
//

#ifndef PAASSDK_DEVICEMANAGE_H
#define PAASSDK_DEVICEMANAGE_H

#include <memory>

#include "../IMediaDeviceManager.h"
#include "DeviceCollection.h"
#include "DeviceCollectionEventHandler.h"

namespace rz {

    class AudioDeviceManager : public IAudioDeviceManager{
        std::shared_ptr<IAudioDeviceCollection> audioPlaybackCollection = nullptr;
        std::shared_ptr<IAudioDeviceCollection> audioRecordingCollection = nullptr;
        AudioDeviceCollectionalEventHandler *eventHandler = nullptr;
    public:

        explicit AudioDeviceManager(AudioDeviceCollectionalEventHandler *event) ;

        IAudioDeviceCollection *enumeratePlaybackDevices() override ;

        IAudioDeviceCollection *enumerateRecordingDevices() override ;

        int setPlaybackDevice(const char deviceId[MAX_DEVICE_ID_LENGTH]) override ;

        int setRecordingDevice(const char deviceId[MAX_DEVICE_ID_LENGTH]) override ;

        int setPlaybackDeviceVolume(int volume) override ;

        int getPlaybackDeviceVolume(int *volume) override ;

        int setRecordingDeviceVolume(int volume) override ;

        int getRecordingDeviceVolume(int *volume) override ;

        int setPlaybackDeviceMute(bool mute) override ;

        int getPlaybackDeviceMute(bool *mute) override ;

        int setRecordingDeviceMute(bool mute) override ;

        int getRecordingDeviceMute(bool *mute) override ;

        int startRecordingDeviceTest(int indicationInterval) override ;

        int stopRecordingDeviceTest() override ;

        int startPlaybackDeviceTest(const char *testAudioFilePath) override ;

        int stopPlaybackDeviceTest() override ;

        int getPlaybackDevice(char deviceId[MAX_DEVICE_ID_LENGTH]) override ;

        int getPlaybackDeviceInfo(char deviceId[MAX_DEVICE_ID_LENGTH], char deviceName[MAX_DEVICE_ID_LENGTH]) override ;

        int getRecordingDevice(char deviceId[MAX_DEVICE_ID_LENGTH]) override ;

        int getRecordingDeviceInfo(char deviceId[MAX_DEVICE_ID_LENGTH], char deviceName[MAX_DEVICE_ID_LENGTH]) override ;

        int startAudioDeviceLoopbackTest(int indicationInterval) override ;

        int stopAudioDeviceLoopbackTest() override ;
    };


    class VideoDeviceManager : public IVideoDeviceManager {
        std::shared_ptr<IVideoDeviceCollection> videoDeviceCollection = nullptr;
    public:
        explicit VideoDeviceManager(VideoDeviceCollectionalEventHandler *event) ;

        IVideoDeviceCollection *enumerateVideoDevices() override ;

        int startDeviceTest(void *hwnd) override ;

        int stopDeviceTest() override ;

        int setDevice(const char deviceId[MAX_DEVICE_ID_LENGTH]) override ;

        int getDevice(char deviceId[MAX_DEVICE_ID_LENGTH]) override ;

    };
}

#endif //PAASSDK_DEVICEMANAGE_H
