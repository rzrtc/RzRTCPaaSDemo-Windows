#include "PrivateIRtcEngineEventHandler.h"
#include "UserStateSingleton.h"
#include <QMetaObject>
#include <QString>
#include <QDebug>
#include "Settings.h"
void PrivateIRtcEngineEventHandler::onWarning(int warn, const char* msg)
{
	QString strmsg = msg;
	QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigWarning", Q_ARG(int, warn), Q_ARG(QString, strmsg));
}

void PrivateIRtcEngineEventHandler::onError(int err, const char* msg)
{
	QString strmsg = msg;
	QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigError", Q_ARG(int, err), Q_ARG(QString, strmsg));
	//UserStateSingleton::instance()->SetProcessData(QString("PrivateIRtcEngineEventHandler::onError err = %1 msg = %2").arg(err).arg(msg));
}

void PrivateIRtcEngineEventHandler::onLocalVideoStateChanged(LOCAL_VIDEO_STREAM_STATE state, LOCAL_VIDEO_STREAM_ERROR error)
{
	
	switch (state)
	{
	case rz::LOCAL_VIDEO_STREAM_STATE_STOPPED:
	{
		UserStateSingleton::instance()->SetLocalVideoDeviceState(state, error);
		break;
	}
	case rz::LOCAL_VIDEO_STREAM_STATE_CAPTURING:
	{
		UserStateSingleton::instance()->SetLocalVideoDeviceState(state, error);
		break;
	}
	case rz::LOCAL_VIDEO_STREAM_STATE_FAILED:
	{
		QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigError", Q_ARG(int, 1504), Q_ARG(QString, QString::fromLocal8Bit("打开相机失败")));
		break;
	}
	case rz::LOCAL_VIDEO_STREAM_STATE_SENDING:
	{
		UserStateSingleton::instance()->SetLocalVideoState(state, error);
		break;
	}
	case rz::LOCAL_VIDEO_STREAM_STATE_NO_SEND:
	{
		UserStateSingleton::instance()->SetLocalVideoState(state, error);
		break;
	}
	default:
		break;
	}
	qDebug() << "888888888888888888888888 = " << state;
	
	QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigLocalVideoStateChanged", Q_ARG(LOCAL_VIDEO_STREAM_STATE, state), Q_ARG(LOCAL_VIDEO_STREAM_ERROR, error));
	//UserStateSingleton::instance()->SetProcessData(QString("PrivateIRtcEngineEventHandler::onLocalVideoStateChanged state = %1 error = %2").arg(state).arg(error));
}

void PrivateIRtcEngineEventHandler::onLocalAudioStateChanged(LOCAL_AUDIO_STREAM_STATE state, LOCAL_AUDIO_STREAM_ERROR error)
{
	switch (state)
	{
	case rz::LOCAL_AUDIO_STREAM_STATE_STOPPED:
	{
		UserStateSingleton::instance()->SetLocalAudioDeviceState(state, error);
		break;
	}
	case rz::LOCAL_AUDIO_STREAM_STATE_RECORDING:
	{
		UserStateSingleton::instance()->SetLocalAudioDeviceState(state, error);
		break;
	}
	case rz::LOCAL_AUDIO_STREAM_STATE_FAILED:
		break;
	case rz::LOCAL_AUDIO_STREAM_STATE_SENDING:
	{
		UserStateSingleton::instance()->SetLocalAudioState(state, error);
		break;
	}
	case rz::LOCAL_AUDIO_STREAM_STATE_NO_SEND:
	{
		UserStateSingleton::instance()->SetLocalAudioState(state, error);
		break;
	}
	default:
		break;
	}

	QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigLocalAudioStateChanged", Q_ARG(LOCAL_AUDIO_STREAM_STATE, state), Q_ARG(LOCAL_AUDIO_STREAM_ERROR, error));
	//UserStateSingleton::instance()->SetProcessData(QString("PrivateIRtcEngineEventHandler::onLocalAudioStateChanged state = %1 error = %2").arg(state).arg(error));
}

void PrivateIRtcEngineEventHandler::onAudioVolumeIndication(const AudioVolumeInfo* speakers, unsigned int speakerNumber, int totalVolume)
{
	QVariant value;
	Settings::instance()->Read("user", value, "Info");
	QString strLocalUid = value.toString();
	//Settings::instance()->Read("enablestream2", value, "SelfStreamSwitch");

	for (int i = 0; i < speakerNumber; i++)
	{
		AUDIOVOLUMEINFO info;
		info.uid = speakers[i].uid;
		info.channelid = speakers[i].channelId;
		info.volume = speakers[i].volume;
		info.vad = speakers[i].vad;
		/*if (info.uid.isEmpty() == false)
		{
			qDebug() << info.uid;
		}*/

		if (info.uid.isEmpty())
			info.uid = strLocalUid;
		else
			UserStateSingleton::instance()->SetRemoteVolume(info.uid, info.volume);
		QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigAudioVolumeIndication", Q_ARG(const AUDIOVOLUMEINFO&, info), Q_ARG(unsigned int, speakerNumber), Q_ARG(int, totalVolume));
	}
	
	if(speakerNumber == 0)
		QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigAudioVolumeIndication", Q_ARG(const AUDIOVOLUMEINFO&, AUDIOVOLUMEINFO()), Q_ARG(unsigned int, speakerNumber), Q_ARG(int, totalVolume));
}

void PrivateIRtcEngineEventHandler::onFirstLocalVideoFrame(int width, int height, int elapsed)
{
	QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigFirstLocalVideoFrame", Q_ARG(int, width), Q_ARG(int, height), Q_ARG(int, elapsed));
	//UserStateSingleton::instance()->SetProcessData(QString("PrivateIRtcEngineEventHandler::onFirstLocalVideoFrame width = %1 height = %2").arg(width).arg(height));
}

void PrivateIRtcEngineEventHandler::onFirstLocalVideoFramePublished(int elapsed)
{
	QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigFirstLocalVideoFramePublished", Q_ARG(int, elapsed));
}

void PrivateIRtcEngineEventHandler::onMediaDeviceStateChanged(const char* deviceId, MEDIA_DEVICE_TYPE deviceType, int deviceState)
{
	QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigMediaDeviceStateChanged", Q_ARG(QString, QString::fromUtf8(deviceId)), Q_ARG(MEDIA_DEVICE_TYPE, deviceType), Q_ARG(int, deviceState));
	//UserStateSingleton::instance()->SetProcessData(QString("PrivateIRtcEngineEventHandler::onMediaDeviceStateChanged deviceId = %1 deviceType = %2 deviceState = %3").arg(deviceId).arg(deviceType).arg(deviceState));
}

void PrivateIRtcEngineEventHandler::onAudioDeviceVolumeChanged(MEDIA_DEVICE_TYPE deviceType, int volume, bool muted)
{
	QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigAudioDeviceVolumeChanged", Q_ARG(MEDIA_DEVICE_TYPE, deviceType), Q_ARG(int, volume), Q_ARG(bool, muted));
}

void PrivateIRtcEngineEventHandler::onVideoSizeChanged(int width, int height)
{
	QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigVideoSizeChanged", Q_ARG(int, width), Q_ARG(int, height));
}

void PrivateIRtcEngineEventHandler::onFirstLocalAudioFramePublished(int elapsed)
{
	QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigFirstLocalAudioFramePublished", Q_ARG(int, elapsed));
}

void PrivateIRtcEngineEventHandler::onAudioRouteChanged(AUDIO_ROUTE_TYPE routing)
{
	QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigAudioRouteChanged", Q_ARG(AUDIO_ROUTE_TYPE, routing));
}

void PrivateIRtcEngineEventHandler::onNetworkTypeChanged(NETWORK_TYPE type)
{
	QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigNetworkTypeChanged", Q_ARG(NETWORK_TYPE, type));
}

void PrivateIRtcEngineEventHandler::onLocalAudioStats(const LocalAudioStats& stats)
{
	QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigLocalAudioStats", Q_ARG(const LocalAudioStats&, stats));
}

void PrivateIRtcEngineEventHandler::onLocalVideoStats(const LocalVideoStats& stats)
{

	//TODO
	//QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigLocalVideoStats", Q_ARG(const LocalVideoStats&, stats));
}

void PrivateIRtcEngineEventHandler::onLocalNetworkQuality(QUALITY_TYPE txQuality, QUALITY_TYPE rxQuality)
{
	QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigLocalNetworkQuality", Q_ARG(QUALITY_TYPE, txQuality), Q_ARG(QUALITY_TYPE, rxQuality));
	//UserStateSingleton::instance()->SetProcessData(QString("PrivateIRtcEngineEventHandler::onLocalNetworkQuality txQuality = %1 rxQuality = %2").arg(txQuality).arg(rxQuality));
}

void PrivateIRtcEngineEventHandler::onLastmileProbeResult(const LastmileProbeResult& result)
{
	QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigLastmileProbeResult", Q_ARG(const LastmileProbeResult&, result));
}