//
// Created by yhy on 11/1/20.
//

#ifndef PAASSDK_DEVICECOLLOECTIONEVENTHANDLER_H
#define PAASSDK_DEVICECOLLOECTIONEVENTHANDLER_H

#include "MediaProcessDefine.h"
#include "../IRtcEngineState.h"

namespace rz{


    class AudioDeviceCollectionalEventHandler{
    protected:
        virtual void onError(MODULE_TYPE type,int errCode,const std::string &errMsg) = 0;

        virtual void onWarn(MODULE_TYPE type,int warnCode,const std::string &warnMsg) = 0;
    public:
        virtual void onError(int errCode,const std::string &errMsg) final {
            onError(MODULE_AUDIO_DEVICE_COLLOCTION,errCode,errMsg);
        };

        virtual void onWarn(int warnCode,const std::string &warnMsg) final {
            onWarn(MODULE_AUDIO_DEVICE_COLLOCTION,warnCode,warnMsg);
        }

        /**
         * 设备音量变化通知
         * @param deviceType
         * @param volume
         * @param muted
         */
        virtual void onAudioDeviceVolumeChanged(MEDIA_DEVICE_TYPE deviceType,int volume, bool muted) = 0;


        /**
         * 设备状态改变回调
         * @param deviceID
         * @param deviceType
         * @param deviceState
         */
        virtual void onDeviceWorkStateChanged(const char *deviceID,MEDIA_DEVICE_TYPE deviceType,
                                              MEDIA_DEVICE_STATE_TYPE deviceState) = 0;

        /**
         * 语音路由已发生变化回调。
         * @param routing
         */
        virtual void onAudioRouteChanged(AUDIO_ROUTE_TYPE routing) = 0;

        virtual ~AudioDeviceCollectionalEventHandler() = default;

    };

    class VideoDeviceCollectionalEventHandler{
    protected:
        virtual void onError(MODULE_TYPE type,int errCode,const std::string &errMsg) = 0;

        virtual void onWarn(MODULE_TYPE type,int warnCode,const std::string &warnMsg) = 0;
    public:
        virtual void onError(int errCode,const std::string &errMsg) final {
            onError(MODULE_VIDEO_DEVICE_COLLOCTION,errCode,errMsg);
        };

        virtual void onWarn(int warnCode,const std::string &warnMsg) final {
            onWarn(MODULE_VIDEO_DEVICE_COLLOCTION,warnCode,warnMsg);
        }
        /**
         * 设备状态改变回调
         * @param deviceID
         * @param deviceType
         * @param deviceState
         */
        virtual void onDeviceWorkStateChanged(const char *deviceID,MEDIA_DEVICE_TYPE deviceType,
                                              MEDIA_DEVICE_STATE_TYPE deviceState) = 0;
    };
}

#endif //PAASSDK_DEVICECOLLOECTIONEVENTHANDLER_H
