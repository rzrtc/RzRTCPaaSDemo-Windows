//
// Created by 86157 on 2020/9/25.
//

#ifndef PAASSDK_DEVICECOLLOECTION_H
#define PAASSDK_DEVICECOLLOECTION_H

#include "DeviceColloectionEventHandler.h"

namespace rz {

#define MAX_DEVICE_ID_LENGTH 64


//平台层实现
    class IAudioDeviceCollection {
    protected:
        //释放接口
        virtual void Release() = 0;

        virtual ~IAudioDeviceCollection() = default;

    public:
        //获取设备数量
        virtual int getCount() = 0;

        //获取序列index的设备ID、name
        virtual int
        getDevice(int index, char deviceName[MAX_DEVICE_ID_LENGTH], char deviceId[MAX_DEVICE_ID_LENGTH]) = 0;

        //获取当前设备的ID
        virtual int getDevice(char deviceId[MAX_DEVICE_ID_LENGTH]) = 0;

        //获取当前设备的ID、name
        virtual int getDevice(char deviceId[MAX_DEVICE_ID_LENGTH], char deviceName[MAX_DEVICE_ID_LENGTH]) = 0;

        //设置deviceID为系统的使用设备
        virtual int setDevice(const char deviceId[MAX_DEVICE_ID_LENGTH]) = 0;

        //设置APP音量  [0,100]。
        virtual int setApplicationVolume(int volume) = 0;

        //获取APP音量 [0,100]。
        virtual int getApplicationVolume(int &volume) = 0;

        //设置静音
        virtual int setApplicationMute(bool mute) = 0;

        //获取静音状态
        virtual int isApplicationMute(bool &mute) = 0;

    };



    //平台层实现
    class IVideoDeviceCollection {
    protected:
        //释放接口
        virtual void Release() = 0;

        virtual ~IVideoDeviceCollection() = default;
    public:

        //获取设备数量
        virtual int getCount() = 0;

        //获取序列index的设备ID、name
        virtual int
        getDevice(int index, char deviceName[MAX_DEVICE_ID_LENGTH], char deviceId[MAX_DEVICE_ID_LENGTH]) = 0;

        //获取当前设备的ID
        virtual int getDevice(char deviceId[MAX_DEVICE_ID_LENGTH]) = 0;

        //获取当前设备的ID、name
        virtual int getDevice(char deviceId[MAX_DEVICE_ID_LENGTH], char deviceName[MAX_DEVICE_ID_LENGTH]) = 0;

        //设置deviceID为系统的使用设备
        virtual int setDevice(const char deviceId[MAX_DEVICE_ID_LENGTH]) = 0;
    };

}

#endif //PAASSDK_DEVICECOLLOECTION_H
