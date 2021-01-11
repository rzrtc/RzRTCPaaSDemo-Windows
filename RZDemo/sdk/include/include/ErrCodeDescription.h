//
// Created by yhy on 11/5/20.
//

#ifndef PAASSDK_ERRCODEDESCRIPTION_H
#define PAASSDK_ERRCODEDESCRIPTION_H

namespace rz{
    enum ERROR_CODE_TYPE{
        /** 0: No error occurs. */
        ERR_OK = 0,
        /** 1: A general error occurs (no specified reason).
        */
        ERR_FAILED = 1,
        /** 2: An invalid parameter is used. For example, the specific channelId includes illegal characters.
        */
        ERR_INVALID_ARGUMENT = 2,
        /** 3: The request is rejected.
         * The specified channelId is already in use  when creating the RtcChannel instance. Please reset the channelId.
         * The specified streamName is already in use  when creating the RtcStream instance. Please reset the channelId.
         * A user whose role is audience calls publish or unPublish
         */
        ERR_REFUSED = 3,
        /** 4: The SDK is not initialized before calling this method.
         */
        ERR_NOT_INITIALIZED = 4,
        /** 5: The request to join the channel is rejected.
         *
         * - This error usually occurs when the user is already in the channel, and still calls the method to join the channel.
         */
        ERR_JOIN_CHANNEL_REJECTED = 5,
        /** 6: The request to leave the channel is rejected.
         This error usually occurs:
         - When the user has left the channel and still calls leaveChannel to leave the channel.
         - When the user has not joined the channel and still calls leaveChannel to leave the channel.
         */
        ERR_LEAVE_CHANNEL_REJECTED = 6,

        /** 101: Lookup schedule server timed out (server no response). This error usually occurs when the network condition is too poor for the SDK to connect to the server.
         */
        ERR_LOOKUP_SCHEDULE_SERVER_TIMEOUT  = 101,

        /** 102: The specified App ID is invalid. Please try to rejoin the channel with a valid App ID.
         */
        ERR_INVALID_APP_ID = 102,
        /** 103: The specified channel Id is invalid. Please try to rejoin the channel with a valid channel ID.
         */
        ERR_INVALID_CHANNEL_ID = 103,
        /** 104: Fails to lookup schedule server.
         */
        ERR_LOOKUP_SCHEDULE_SERVER_FAILED = 104,
        /** 105: No available schedule server resources.
         */
        ERR_NO_SCHEDULE_SERVER_RESOURCES  = 105,
        /** 106: Lookup server time out
         */
        ERR_LOOKUP_SERVER_TIMEOUT = 106,
        /** 107: No available server
         */
        ERR_NO_AVAILABLE_SERVER_RESOURCES = 107,
        /** 108: The specified stream Id is invalid. Please try to create the rtcStream with a valid stream ID.
         */
        ERR_INVALID_STREAM_ID = 108,

        /** 1001: A general error occurs in the Audio Device Module (no specified reason). Check if the audio device is used by another application, or try rejoining the channel.
         */
        ERR_ADM_GENERAL_ERROR = 1001,

        ERR_ADM_RECORD_UNKNOWN_ERROR = 1011,
        ERR_ADM_NO_PERMISSION = 1012,
        ERR_ADM_DEVICE_IN_USE = 1013,
        ERR_ADM_INIT_RECORDING = 1014,
        ERR_ADM_START_RECORDING = 1015,
        ERR_ADM_RECORD_AUDIO_FAILED = 1016,
        /** 1017: Audio Device Module: An error occurs in setting the sampling frequency.
        */
        ERR_ADM_SAMPLE_RATE = 1017,
        /** 1022: Audio Device Module: A recording error occurs.
         */
        ERR_ADM_RUNTIME_RECORDING_ERROR = 1022,
        /** 1031: Audio Device Module: An error occurs in stopping recording device.
         */
        ERR_ADM_STOP_RECORDING = 1031,
        /** 1036: Audio Device Module: No recording device exists. **/
        ERR_ADM_NO_RECORDING_DEVICE = 1036,

        ERR_ADM_PLAYBACK_UNKNOWN_ERROR = 1051,
        ERR_ADM_INIT_PLAYBACK = 1052,
        ERR_ADM_START_PLAYBACK = 1053,
        ERR_ADM_PLAYBACK_AUDIO_FAILED = 1054,
        /** 1062: Audio Device Module: a error occurs in the playback device. */
        ERR_ADM_RUNTIME_PLAYBACK_ERROR = 1062,
        /** 1071: Audio Device Module: An error occurs in stopping playback device.
         */
        ERR_ADM_STOP_PLAYBACK = 1071,
        /** 1076: Audio Device Module: No playback device exists. **/
        ERR_ADM_NO_PLAYBACK_DEVICE = 1076,
        /** 1091: Audio Device Module: An error occurs in initializing the
         * loopback device.
        */
        ERR_ADM_INIT_LOOPBACK = 1091,
        /** 1092: Audio Device Module: An error occurs in starting the loopback
         * device.
         */
        ERR_ADM_START_LOOPBACK = 1092,
        /** 1093: Audio Device Module: An error occurs in stopping the loopback
         * device.
        */
        ERR_ADM_STOP_LOOPBACK = 1093,
        /** 1101: Audio Device Module: A fatal exception occurs.
        */
        ERR_ADM_ANDROID_JNI_JAVA_RESOURCE = 1101,
        /** 1121: Audio Device Module: The recording frequency is lower than 50.
         * 0 indicates that the recording is not yet started. We recommend
         * checking your recording permission.
        */
        ERR_ADM_ANDROID_JNI_LOW_RECORD_FREQUENCY = 1121,
        /** 1122: Audio Device Module: AudioRecord fails to start up. A ROM system
         * error occurs. We recommend the following options to debug:
         * - Restart your App.
         * - Restart your cellphone.
         * - Check your recording permission.
         */
        ERR_ADM_ANDROID_JNI_JAVA_START_RECORD = 1122,
        /** 1123: Audio Device Module: AudioRecord returns error. The SDK will
         * automatically restart AudioRecord. */
        ERR_ADM_ANDROID_JNI_JAVA_RECORD_ERROR = 1123,
        /** 1161: Audio Device Module: The playback frequency is lower than 50.
         * 0 indicates that the playback is not yet started. We recommend
         * checking if you have created too many AudioTrack instances.
        */
        ERR_ADM_ANDROID_JNI_LOW_PLAYBACK_FREQUENCY =1161,
        /** 1162: Audio Device Module: AudioTrack fails to start up. A ROM system
        * error occurs. We recommend the following options to debug:
        * - Restart your App.
        * - Restart your cellphone.
        * - Check your playback permission.
        */
        ERR_ADM_ANDROID_JNI_JAVA_START_PLAYBACK =1162,

        /** 1201: Audio Device Module: The current device does not support audio
         * input, possibly because you have mistakenly configured the audio session
         *  category, or because some other app is occupying the input device. We
         * recommend terminating all background apps and re-joining the channel. */
        ERR_ADM_IOS_INPUT_NOT_AVAILABLE = 1201,
        /** 1202: Audio Device Module: Cannot activate the Audio Session.*/
        ERR_ADM_IOS_ACTIVATE_SESSION_FAIL = 1202,
        /** 1203: Audio Device Module: Fails to initialize the audio device,
        * normally because the audio device parameters are wrongly set.*/
        ERR_ADM_IOS_VPIO_INIT_FAIL = 1203,
        /** 1204: Audio Device Module: Fails to re-initialize the audio device,
         * normally because the audio device parameters are wrongly set.*/
        ERR_ADM_IOS_VPIO_REINIT_FAIL = 1204,
        /** 1205: Audio Device Module: Fails to re-start up the Audio Unit,
         * possibly because the audio session category is not compatible
         * with the settings of the Audio Unit.
        */
        ERR_ADM_IOS_VPIO_RESTART_FAIL = 1205,

        ERR_ADM_IOS_RECORDING_RESET_BY_SYSTEM = 1221,
        ERR_ADM_IOS_PLAYBACK_RESET_BY_SYSTEM = 1261,

        // VDM error code starts from 1501
        ERR_VDM_UNKNOWN_ERROR = 1501,
        ERR_VDM_CAMERA_NO_PERMISSION= 1502,
        ERR_VDM_DEVICE_IN_USE = 1503,
        ERR_VDM_CAPTURE_FAILED = 1504,
        /** 1512: Video Device Module: An error occurs in the video capture device.
         */
        ERR_VDM_RUNTIME_VIDEO_CAPTURE_ERROR = 1512,
        /** 1531: Video Device Module: An error occurs in stopping the video capture device.
         */
        ERR_VDM_STOP_FAILED = 1531,
        /** 1541: Video Device Module: No camera exists. **/
        ERR_VDM_NO_CAMERA = 1541,
        /** 1601: Fails to start the video rendering module.
         */
        ERR_START_VIDEO_RENDER = 1601,

        ERR_ACM_ENCODER_UNKNOWN_ERROR = 1701,
        ERR_ACM_ENCODER_INIT_ERROR = 1702,
        ERR_ACM_ENCODER_ENCODE_ERROR = 1703,
        ERR_ACM_ENCODER_SET_ERROR = 1704,
        ERR_ACM_DECODER_UNKNOWN_ERROR = 1751,
        ERR_ACM_DECODER_INIT_ERROR = 1752,
        ERR_ACM_DECODER_ENCODE_ERROR = 1753,
        ERR_ACM_DECODER_SET_ERROR = 1754,

        // VCM error code starts from 1801
        /** 1801: Video Codec Module: An unknown error occurs.
         */
        ERR_VCM_ENCODER_UNKNOWN_ERROR = 1801,
        /** 1802: Video Codec Module: An error occurs in initializing the video encoder.
        */
        ERR_VCM_ENCODER_INIT_ERROR = 1802,
        /** 1803: Video Codec Module: An error occurs in encoding.
         */
        ERR_VCM_ENCODER_ENCODE_ERROR = 1803,
        /** 1804: Video Codec Module: An error occurs in setting the video encoder.
         */
        ERR_VCM_ENCODER_SET_ERROR = 1804,

        ERR_VCM_DECODER_UNKNOWN_ERROR = 1851,
        ERR_VCM_DECODER_INIT_ERROR = 1852,
        ERR_VCM_DECODER_DECODE_ERROR = 1853,
        ERR_VCM_DECODER_SET_ERROR = 1854,

        /** 2021: Audio AEC Module: An unknown error occurs.*/
        ERR_APM_AEC_UNKNOWN_ERROR = 2021,
        /** 2022: Audio AEC Module: An error occurs in initializing the aec*/
        ERR_APM_AEC_INIT_ERROR = 2022,
        /** 2023: Audio AEC Module: An error occurs in exec the aec*/
        ERR_APM_AEC_FAILED_ERROR = 2023,
        /** 2024: Audio AEC Module: An error occurs in reset the aec*/
        ERR_APM_AEC_RESET_ERROR = 2024,

        /** 2041: Audio ANC Module: An unknown error occurs.*/
        ERR_APM_ANC_UNKNOWN_ERROR = 2041,
        /** 2042: Audio ANC Module: An error occurs in initializing the anc*/
        ERR_APM_ANC_INIT_ERROR = 2042,
        /** 2043: Audio ANC Module: An error occurs in exec the anc*/
        ERR_APM_ANC_FAILED_ERROR = 2043,
        /** 2044: Audio ANC Module: An error occurs in reset the anc*/
        ERR_APM_ANC_RESET_ERROR = 2044,

        /** 2061: Audio VAD Module: An unknown error occurs.*/
        ERR_APM_VAD_UNKNOWN_ERROR = 2061,
        /** 2062: Audio VAD Module: An error occurs in initializing the vad*/
        ERR_APM_VAD_INIT_ERROR = 2062,
        /** 2063: Audio VAD Module: An error occurs in exec the vad*/
        ERR_APM_VAD_FAILED_ERROR = 2063,
        /** 2064: Audio VAD Module: An error occurs in reset the vad*/
        ERR_APM_VAD_RESET_ERROR = 2064,

        /** 2081: Audio DETECT Module: An unknown error occurs.*/
        ERR_APM_DETECT_UNKNOWN_ERROR = 2081,
        /** 2082: Audio DETECT Module: An error occurs in initializing the audio detect*/
        ERR_APM_DETECT_INIT_ERROR = 2082,
        /** 2083: Audio DETECT Module: An error occurs in exec the audio detect*/
        ERR_APM_DETECT_FAILED_ERROR = 2083,
        /** 2084: Audio DETECT Module: An error occurs in reset the audio detect*/
        ERR_APM_DETECT_RESET_ERROR = 2084,

        /** 2101: Audio MIX Module: An unknown error occurs.*/
        ERR_APM_MIX_UNKNOWN_ERROR = 2101,
        /** 2102: Audio MIX Module: An error occurs in initializing the audio mix*/
        ERR_APM_MIX_INIT_ERROR = 2102,
        /** 2103: Audio MIX Module: An error occurs in exec the audio mix*/
        ERR_APM_MIX_FAILED_ERROR = 2103,
        /** 2104: Audio MIX Module: An error occurs in reset the audio mix*/
        ERR_APM_MIX_RESET_ERROR = 2104,

        /** 2121: Audio howlling Module: An unknown error occurs.*/
        ERR_APM_HOWLLING_UNKNOWN_ERROR = 2121,
        /** 2122: Audio howlling Module: An error occurs in initializing the audio howlling*/
        ERR_APM_HOWLLING_INIT_ERROR = 2122,
        /** 2123: Audio howlling Module: An error occurs in exec the audio howlling*/
        ERR_APM_HOWLLING_FAILED_ERROR = 2123,
        /** 2124: Audio howlling Module: An error occurs in reset the audio howlling*/
        ERR_APM_HOWLLING_RESET_ERROR = 2124,
    };
    enum WARN_CODE_TYPE{

        /** 1006: The audio playback or recording is interrupted by system events (such as a phone call).
        */
        WARN_ADM_CALL_INTERRUPTION = 1006,
        /** 1021: Audio Device Module: a warning occurs in the recording device.
        */
        WARN_ADM_RUNTIME_RECORDING_WARNING = 1021,
        /** 1023: Audio Device Module: the audio recording frequency is abnormal, which may cause audio freezes. This abnormality is caused by high CPU usage. We recommends stopping other apps.
        */
        WARN_ADM_RECORD_MALFUNCTION = 1023,
        /** 1024: Audio Device Module: no valid audio data is recorded.
        */
        WARN_ADM_RECORD_AUDIO_SILENCE = 1024,
        /** 1032: Audio Device Module: The recorded audio voice is too low.
        */
        WARN_ADM_RECORD_AUDIO_LOWLEVEL = 1032,
        /** 1061: Audio Device Module: A warning occurs in the playback device.
        */
        WARN_ADM_RUNTIME_PLAYBACK_WARNING = 1061,
        /** 1072: Audio Device Module: The playback audio voice is too low.
        */
        WARN_ADM_PLAYBACK_AUDIO_LOWLEVEL= 1072,
        /** 1206: During a call, the audio session category should be set to
         * AVAudioSessionCategoryPlayAndRecord, and RtcEngine monitors this value.
         * If the audio session category is set to other values, this warning code
         * is triggered and RtcEngine will forcefully set it back to
         * AVAudioSessionCategoryPlayAndRecord.
        */
        WARN_ADM_IOS_CATEGORY_NOT_PLAYANDRECORD = 1206,
        /** 1512: Video Device Module: An warnning occurs in the video capture device.
         */
        WARN_VDM_RUNTIME_VIDEO_CAPTURE_WARNING = 1511,
        /** 1602: The specified view is invalid. Specify a view when using the video call function.
        */
        WARN_INVAILED_VIEW = 1602,
        WARN_VCM_ENCODER_HW_FAILED = 1805,
        WARN_VCM_DECODER_HW_FAILED = 1855,
        WARN_VPM_DUAL_NO_LOW_STREAM = 3041,
        WARN_VPM_DUAL_SWITCH_LOW_FAILED = 3042,
        WARN_VPM_DUAL_SWITCH_HIGH_FAILED = 3043
    };

    inline const char* getError(int code){
        switch (code){
            case ERR_OK:
                return "no error";
            case ERR_FAILED:
                return "general failure";
            case ERR_INVALID_ARGUMENT:
                return "invalid argument";
            case ERR_REFUSED:
                return "request is refused";
            case ERR_NOT_INITIALIZED:
                return "Not initialized";
            case ERR_JOIN_CHANNEL_REJECTED:
                return "Request to join channel is rejected";
            case ERR_LEAVE_CHANNEL_REJECTED:
                return "Request to leave channel is rejected";
            case ERR_LOOKUP_SCHEDULE_SERVER_TIMEOUT:
                return "Lookup schedule server timed out (server no response)";
            case ERR_INVALID_APP_ID:
                return "appId is invaild";
            case ERR_INVALID_CHANNEL_ID:
                return "channelId is invalid";
            case ERR_LOOKUP_SCHEDULE_SERVER_FAILED:
                return "Fails to lookup schedule server";
            case ERR_NO_SCHEDULE_SERVER_RESOURCES :
                return "No available schedule server resources";
            case ERR_LOOKUP_SERVER_TIMEOUT:
                return "Lookup server time out";
            case ERR_NO_AVAILABLE_SERVER_RESOURCES:
                return "No available server resources";
            case ERR_INVALID_STREAM_ID:
                return "streamId is invalid";
            case ERR_ADM_GENERAL_ERROR:
                return "general error in the recording device";
            case ERR_ADM_RUNTIME_RECORDING_ERROR:
                return "An error occurs in the recording device";
            case ERR_ADM_STOP_RECORDING:
                return "An error occurs in stopping recording device";
            case ERR_ADM_NO_RECORDING_DEVICE:
                return "No recording device exists";
            case ERR_ADM_RUNTIME_PLAYBACK_ERROR:
                return "An error occurs in the playback device";
            case ERR_ADM_STOP_PLAYBACK:
                return "An error occurs in stopping playback device";
            case ERR_ADM_NO_PLAYBACK_DEVICE:
                return "No available playback device";
            case ERR_ADM_INIT_LOOPBACK:
                return "An error occurs in initializing the loopback device. ";
            case ERR_ADM_START_LOOPBACK:
                return "An error occurs in starting the loopback device";
            case ERR_ADM_STOP_LOOPBACK:
                return "An error occurs in stopping the loopback device";
            case ERR_ADM_ANDROID_JNI_JAVA_RESOURCE:
                return "A fatal exception occurs";
            case ERR_ADM_ANDROID_JNI_LOW_RECORD_FREQUENCY:
                return "The recording frequency is lower than 50";
            case ERR_ADM_ANDROID_JNI_JAVA_START_RECORD:
                return "AudioRecord fails to start up";
            case ERR_ADM_ANDROID_JNI_JAVA_RECORD_ERROR:
                return "AudioRecord returns error";
            case ERR_ADM_ANDROID_JNI_LOW_PLAYBACK_FREQUENCY:
                return "The playback frequency is lower than 50";
            case ERR_ADM_ANDROID_JNI_JAVA_START_PLAYBACK:
                return "AudioTrack fails to start up";
            case ERR_ADM_IOS_INPUT_NOT_AVAILABLE:
                return "The current device does not support audio input";
            case ERR_ADM_IOS_ACTIVATE_SESSION_FAIL:
                return "Cannot activate the Audio Session";
            case ERR_ADM_IOS_VPIO_INIT_FAIL:
                return "Fails to initialize the audio device";
            case ERR_ADM_IOS_VPIO_REINIT_FAIL:
                return "Fails to re-initialize the audio device";
            case ERR_ADM_IOS_VPIO_RESTART_FAIL:
                return "Fails to re-start up the Audio Unit";
            case ERR_ADM_IOS_RECORDING_RESET_BY_SYSTEM:
                return "The audio recording device is reset by system";
            case ERR_ADM_IOS_PLAYBACK_RESET_BY_SYSTEM:
                return "The audio playback device is reset by system";
            case ERR_VDM_RUNTIME_VIDEO_CAPTURE_ERROR:
                return "An error occurs in the video capture device";
            case ERR_VDM_STOP_FAILED:
                return "An error occurs in stopping the video capture device";
            case ERR_VDM_NO_CAMERA:
                return "No camera exists";
            case ERR_START_VIDEO_RENDER:
                return "Failed to start the video rendering module";
            case ERR_VCM_ENCODER_UNKNOWN_ERROR:
                return "An unknown error occurs";
            case ERR_VCM_ENCODER_INIT_ERROR:
                return "An error occurs in initializing the video encoder";
            case ERR_VCM_ENCODER_ENCODE_ERROR:
                return "An error occurs in encoding";
            case ERR_VCM_ENCODER_SET_ERROR:
                return "An error occurs in setting the video encoder";

         /* 以下部分错误码暂时不会暴露给用户。
            case ERR_APM_AEC_UNKNOWN_ERROR:
                return "Audio AEC Module: An unknown error occurs.";
            case ERR_APM_AEC_INIT_ERROR:
                return "Audio AEC Module: An error occurs in initializing the aec";
            case ERR_APM_AEC_FAILED_ERROR:
                return "Audio AEC Module: An error occurs in exec the aec";
            case ERR_APM_AEC_RESET_ERROR:
                return "Audio AEC Module: An error occurs in reset the aec";
            case ERR_APM_ANC_UNKNOWN_ERROR:
                return "Audio ANC Module: An unknown error occurs.";
            case ERR_APM_ANC_INIT_ERROR:
                return "Audio ANC Module: An error occurs in initializing the anc";
            case ERR_APM_ANC_FAILED_ERROR:
                return "Audio ANC Module: An error occurs in exec the anc";
            case ERR_APM_ANC_RESET_ERROR:
                return "Audio ANC Module: An error occurs in reset the anc";
            case ERR_APM_VAD_UNKNOWN_ERROR:
                return "Audio VAD Module: An unknown error occurs.";
            case ERR_APM_VAD_INIT_ERROR:
                return "Audio VAD Module: An error occurs in initializing the vad";
            case ERR_APM_VAD_FAILED_ERROR:
                return "Audio VAD Module: An error occurs in exec the vad";
            case ERR_APM_VAD_RESET_ERROR:
                return "Audio VAD Module: An error occurs in reset the vad";
            case ERR_APM_DETECT_UNKNOWN_ERROR:
                return "Audio DETECT Module: An unknown error occurs.";
            case ERR_APM_DETECT_INIT_ERROR:
                return "Audio DETECT Module: An error occurs in initializing the audio detect";
            case ERR_APM_DETECT_FAILED_ERROR:
                return "Audio DETECT Module: An error occurs in exec the audio detect";
            case ERR_APM_DETECT_RESET_ERROR:
                return "Audio DETECT Module: An error occurs in reset the audio detect";
            case ERR_APM_MIX_UNKNOWN_ERROR:
                return "Audio MIX Module: An unknown error occurs.";
            case ERR_APM_MIX_INIT_ERROR:
                return "Audio MIX Module: An error occurs in initializing the audio mix";
            case ERR_APM_MIX_FAILED_ERROR:
                return "Audio MIX Module: An error occurs in exec the audio mix";
            case ERR_APM_MIX_RESET_ERROR:
                return "Audio MIX Module: An error occurs in reset the audio mix";
            case ERR_APM_HOWLLING_UNKNOWN_ERROR:
                return "Audio howlling Module: An unknown error occurs.";
            case ERR_APM_HOWLLING_INIT_ERROR:
                return "Audio howlling Module: An error occurs in initializing the audio howllingx";
            case ERR_APM_HOWLLING_FAILED_ERROR:
                return "Audio howlling Module: An error occurs in reset the audio howlling";
            case ERR_APM_HOWLLING_RESET_ERROR:
                return "Audio howlling Module: An error occurs in reset the audio howlling";
        */
            case WARN_ADM_CALL_INTERRUPTION:
                return "The audio playback or recording is interrupted by system events";
            case WARN_ADM_RUNTIME_RECORDING_WARNING:
                return "A warning occurs in the recording device";
            case WARN_ADM_RECORD_MALFUNCTION:
                return "The audio recording frequency is abnormal";
            case WARN_ADM_RECORD_AUDIO_SILENCE:
                return "No valid audio data is recorded";
            case WARN_ADM_RECORD_AUDIO_LOWLEVEL:
                return "The recorded audio voice is too low";
            case WARN_ADM_RUNTIME_PLAYBACK_WARNING:
                return "A warning occurs in the playback device";
            case WARN_ADM_PLAYBACK_AUDIO_LOWLEVEL:
                return "The playback audio voice is too low";
            case WARN_ADM_IOS_CATEGORY_NOT_PLAYANDRECORD:
                return "The audio session category is not AVAudioSessionCategoryPlayAndRecord";
            case WARN_VDM_RUNTIME_VIDEO_CAPTURE_WARNING:
                return "An warnning occurs in the video capture device";
            case WARN_INVAILED_VIEW:
                return "The specified view is invalid";
            case WARN_VCM_ENCODER_HW_FAILED:
                return "An warning occurs in using hardware to encode";
            case WARN_VCM_DECODER_HW_FAILED:
                return "An warning occurs in using hardware to decode";
            case WARN_VPM_DUAL_NO_LOW_STREAM:
                return "No low-quality video stream";
            case WARN_VPM_DUAL_SWITCH_LOW_FAILED:
                return "Fails to switch low-quality video stream";
            case WARN_VPM_DUAL_SWITCH_HIGH_FAILED:
                return "Fails to switch high-quality video stream";
            default:
                return nullptr;
        }
    }
}

#endif //PAASSDK_ERRCODEDESCRIPTION_H
