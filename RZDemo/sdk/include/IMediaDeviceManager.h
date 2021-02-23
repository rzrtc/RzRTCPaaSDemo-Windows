//
// Created by yhy on 11/1/20.
//

#ifndef PAASSDK_IMEDIADEVICEMANAGER_H
#define PAASSDK_IMEDIADEVICEMANAGER_H

#include "DeviceCollection.h"

#define MAX_DEVICE_ID_LENGTH 64

namespace rz{

    class IAudioDeviceManager{
    protected:
        virtual ~IAudioDeviceManager() = default;
    public:

        virtual IAudioDeviceCollection *enumeratePlaybackDevices() = 0;

        virtual IAudioDeviceCollection *enumerateRecordingDevices() = 0;

        virtual int setPlaybackDevice(const char deviceId[MAX_DEVICE_ID_LENGTH]) = 0;

        virtual int setRecordingDevice(const char deviceId[MAX_DEVICE_ID_LENGTH]) = 0;

        virtual int setPlaybackDeviceVolume(int volume) = 0;

        virtual int getPlaybackDeviceVolume(int *volume) = 0;

        virtual int setRecordingDeviceVolume(int volume) = 0;

        virtual int getRecordingDeviceVolume(int *volume) = 0;

        virtual int setPlaybackDeviceMute(bool mute) = 0;

        virtual int getPlaybackDeviceMute(bool *mute) = 0;

        virtual int setRecordingDeviceMute(bool mute) = 0;

        virtual int getRecordingDeviceMute(bool *mute) = 0;

        virtual int startRecordingDeviceTest(int indicationInterval) = 0;

        virtual int stopRecordingDeviceTest() = 0;

        virtual int startPlaybackDeviceTest(const char *testAudioFilePath) = 0;

        virtual int stopPlaybackDeviceTest() = 0;

        virtual int getPlaybackDevice(char deviceId[MAX_DEVICE_ID_LENGTH]) = 0;

        virtual int getPlaybackDeviceInfo(char deviceId[MAX_DEVICE_ID_LENGTH], char deviceName[MAX_DEVICE_ID_LENGTH]) = 0;

        virtual int getRecordingDevice(char deviceId[MAX_DEVICE_ID_LENGTH]) = 0;

        virtual int getRecordingDeviceInfo(char deviceId[MAX_DEVICE_ID_LENGTH], char deviceName[MAX_DEVICE_ID_LENGTH]) = 0;

        virtual int startAudioDeviceLoopbackTest(int indicationInterval) = 0;

        virtual int stopAudioDeviceLoopbackTest() = 0;

    };


    class IVideoDeviceManager {
    protected:
        virtual ~IVideoDeviceManager() = default;
    public:

        virtual IVideoDeviceCollection *enumerateVideoDevices() = 0;

        virtual int startDeviceTest(void *hwnd) = 0;

        virtual int stopDeviceTest() = 0;

        virtual int setDevice(const char deviceId[MAX_DEVICE_ID_LENGTH]) = 0;

        virtual int getDevice(char deviceId[MAX_DEVICE_ID_LENGTH]) = 0;
    };

}

#endif //PAASSDK_IMEDIADEVICEMANAGER_H
