#include "PrivateIRtcChannelEventHandler.h"
#include "UserStateSingleton.h"
#include <IRtcChannel.h>
#include <QDebug>
#include "Settings.h"
void PrivateIRtcChannelEventHandler::onWarning(IRtcChannel* channel, int warn, const char* msg)
{
	if (!channel) {
		qDebug() << "PrivateIRtcChannelEventHandler::onWarning";
		return;
	}

	QString channelid = channel->getChannelId();
	QString strmsg = msg;
	QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigWarning", Q_ARG(const QString&, channelid), Q_ARG(int, warn), Q_ARG(const QString&, strmsg));
}

void PrivateIRtcChannelEventHandler::onError(IRtcChannel* channel, int err, const char* msg)
{
	if (!channel) {
		qDebug() << "PrivateIRtcChannelEventHandler::onError";
		return;
	}
	qDebug() << "PrivateIRtcChannelEventHandler::onError" << err <<"  "<< msg;
	QString channelid = channel->getChannelId();
	QString strmsg = msg;
	bool ret = QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigError", Q_ARG(const QString&, channelid), Q_ARG(int, err), Q_ARG(const QString&, strmsg));
	//UserStateSingleton::instance()->SetProcessData(QString("PrivateIRtcChannelEventHandler::onError err = %1 msg = %2").arg(err).arg(msg));
}

void PrivateIRtcChannelEventHandler::onJoinChannelSuccess(IRtcChannel* channel, const char* uid, int elapsed)
{
	if (!channel) {
		qDebug() << "PrivateIRtcChannelEventHandler::onJoinChannelSuccess";
		return;
	}

	QString channelid = QString::fromUtf8(channel->getChannelId());
	QString struid = QString::fromUtf8(uid);
	QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigJoinChannelSuccess", Q_ARG(QString, channelid), Q_ARG(QString, struid), Q_ARG(int, elapsed));
	UserStateSingleton::instance()->SetProcessData(QString("PrivateIRtcChannelEventHandler::onJoinChannelSuccess uid = %1").arg(uid));
}

void PrivateIRtcChannelEventHandler::onRejoinChannelSuccess(IRtcChannel* channel, const char* uid, int elapsed)
{
	if (!channel) {
		qDebug() << "PrivateIRtcChannelEventHandler::onRejoinChannelSuccess";
		return;
	}

	QString channelid = QString::fromUtf8(channel->getChannelId());
	QString struid = QString::fromUtf8(uid);
	QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigRejoinChannelSuccess", Q_ARG(QString, channelid), Q_ARG(QString, struid), Q_ARG(int, elapsed));
	//UserStateSingleton::instance()->SetProcessData(QString("PrivateIRtcChannelEventHandler::onRejoinChannelSuccess uid = %1").arg(uid));
}

void PrivateIRtcChannelEventHandler::onLeaveChannel(IRtcChannel* channel, const RtcStats& stats)
{
	if (!channel) {
		qDebug() << "PrivateIRtcChannelEventHandler::onLeaveChannel";
		return;
	}

	QString channelid = QString::fromUtf8(channel->getChannelId());
	QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigLeaveChannel", Q_ARG(QString, channelid), Q_ARG(const RtcStats&, stats));
	//UserStateSingleton::instance()->SetProcessData(QString("PrivateIRtcChannelEventHandler::onLeaveChannel"));
}

void PrivateIRtcChannelEventHandler::onClientRoleChanged(IRtcChannel* channel, CLIENT_ROLE_TYPE oldRole, CLIENT_ROLE_TYPE newRole, int elapsed)
{
	if (!channel) {
		qDebug() << "PrivateIRtcChannelEventHandler::onClientRoleChanged";
		return;
	}

	QString channelid = QString::fromUtf8(channel->getChannelId());
	QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigClientRoleChanged", Q_ARG(const QString&, channelid), Q_ARG(CLIENT_ROLE_TYPE, oldRole), Q_ARG(CLIENT_ROLE_TYPE, newRole), Q_ARG(int, elapsed));
	//UserStateSingleton::instance()->SetProcessData(QString("PrivateIRtcChannelEventHandler::onClientRoleChanged oldRole = %1 newRole = %2").arg(oldRole).arg(newRole));
}

void PrivateIRtcChannelEventHandler::onUserJoined(IRtcChannel* channel, const char* uid, int elapsed)
{
	if (!channel) {
		qDebug() << "PrivateIRtcChannelEventHandler::onUserJoined";
		return;
	}

	QString channelid = QString::fromUtf8(channel->getChannelId());
	QString struid = QString::fromUtf8(uid);
	UserStateSingleton::instance()->AddRemoteUser(struid.toStdString());
	QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigUserJoined", Q_ARG(QString, channelid), Q_ARG(QString, struid), Q_ARG(int, elapsed));
	UserStateSingleton::instance()->SetProcessData(QString("PrivateIRtcChannelEventHandler::onUserJoined uid = %1").arg(uid));
	qDebug() << "onUserJoined uid = " << struid;
}

void PrivateIRtcChannelEventHandler::onUserOffline(IRtcChannel* channel, const char* uid, USER_OFFLINE_REASON_TYPE reason)
{
	if (!channel) {
		qDebug() << "PrivateIRtcChannelEventHandler::onUserOffline";
		return;
	}

	QString channelid = QString::fromUtf8(channel->getChannelId());
	QString struid = QString::fromUtf8(uid);
	UserStateSingleton::instance()->DelRemoteUser(struid.toStdString());
	QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigUserOffline", Q_ARG(QString, channelid), Q_ARG(QString, struid), Q_ARG(USER_OFFLINE_REASON_TYPE, reason));
	UserStateSingleton::instance()->SetProcessData(QString("PrivateIRtcChannelEventHandler::onUserOffline uid = %1 reason = %2").arg(uid).arg(reason));
	qDebug() << "onUserOffline uid = " << struid;
}

void PrivateIRtcChannelEventHandler::onAudioPublishStateChanged(IRtcChannel* channel, PUBLISH_STREAM_STATE state, int elapsed)
{
	if (!channel) {
		qDebug() << "PrivateIRtcChannelEventHandler::onAudioPublishStateChanged";
		return;
	}

	QString channelid = QString::fromUtf8(channel->getChannelId());
	QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigAudioPublishStateChanged", Q_ARG(QString, channelid), Q_ARG(PUBLISH_STREAM_STATE, state), Q_ARG(int, elapsed));

}

void PrivateIRtcChannelEventHandler::onVideoPublishStateChanged(IRtcChannel* channel, PUBLISH_STREAM_STATE state, int elapsed)
{
	if (!channel) {
		qDebug() << "PrivateIRtcChannelEventHandler::onVideoPublishStateChanged";
		return;
	}

	QString channelid = QString::fromUtf8(channel->getChannelId());
	QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigVideoPublishStateChanged", Q_ARG(QString, channelid), Q_ARG(PUBLISH_STREAM_STATE, state), Q_ARG(int, elapsed));
}

void PrivateIRtcChannelEventHandler::onAudioSubscribeStateChanged(IRtcChannel* channel, const char* uid, SUBSCRIBE_STREAM_STATE state, int elapsed)
{
	if (!channel) {
		qDebug() << "PrivateIRtcChannelEventHandler::onAudioSubscribeStateChanged";
		return;
	}

	
	QString channelid = QString::fromUtf8(channel->getChannelId());
	QString struid = QString::fromUtf8(uid);
	UserStateSingleton::instance()->SetAudioSubscribeState(channelid.toStdString(), struid.toStdString(), state);
	QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigAudioSubscribeStateChanged", Q_ARG(QString, channelid), Q_ARG(QString, struid), Q_ARG(SUBSCRIBE_STREAM_STATE, state), Q_ARG(int, elapsed));
	//UserStateSingleton::instance()->SetProcessData(QString("PrivateIRtcChannelEventHandler::onAudioSubscribeStateChanged uid = %1 state = %2").arg(uid).arg(state));
}

void PrivateIRtcChannelEventHandler::onVideoSubscribeStateChanged(IRtcChannel* channel, const char* uid, const char* streamName, SUBSCRIBE_STREAM_STATE state, int elapsed)
{
	if (!channel) {
		qDebug() << "PrivateIRtcChannelEventHandler::onVideoSubscribeStateChanged";
		return;
	}

	QString channelid = QString::fromUtf8(channel->getChannelId());
	QString struid = QString::fromUtf8(uid);
	QString strStreamName = QString::fromUtf8(streamName);

	UserStateSingleton::instance()->UpdateRemoteUserVideoStreamMap(struid.toStdString(), strStreamName.toStdString(), state);
	QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigVideoSubscribeStateChanged", Q_ARG(QString, channelid), Q_ARG(QString, struid), Q_ARG(QString, strStreamName), Q_ARG(SUBSCRIBE_STREAM_STATE, state), Q_ARG(int, elapsed));
	//UserStateSingleton::instance()->SetProcessData(QString("PrivateIRtcChannelEventHandler::onVideoSubscribeStateChanged uid = %1 streamName = %2 state = %3").arg(uid).arg(streamName).arg(state));
}

void PrivateIRtcChannelEventHandler::onFirstRemoteVideoFrame(IRtcChannel* channel, const char* uid, const char* streamName, int width, int height, int elapsed)
{
	if (!channel) {
		qDebug() << "PrivateIRtcChannelEventHandler::onFirstRemoteVideoFrame";
		return;
	}

	QString channelid = QString::fromUtf8(channel->getChannelId());
	QString struid = QString::fromUtf8(uid);
	QString strStreamName = QString::fromUtf8(streamName);

	UserStateSingleton::instance()->SetFirstRemoteVideoFrame(channelid, struid, strStreamName, width, height);

	QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigFirstRemoteVideoFrame", Q_ARG(QString, channelid), Q_ARG(QString, struid), Q_ARG(QString, strStreamName), Q_ARG(int, width), Q_ARG(int, height), Q_ARG(int, elapsed));
}

void PrivateIRtcChannelEventHandler::onVideoSizeChanged(IRtcChannel* channel, const char* uid, const char* streamName, int width, int height, int elapsed)
{
	if (!channel) {
		qDebug() << "PrivateIRtcChannelEventHandler::onVideoSizeChanged";
		return;
	}

	QString channelid = QString::fromUtf8(channel->getChannelId());
	QString struid = QString::fromUtf8(uid);
	QString strStreamName = QString::fromUtf8(streamName);
	QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigVideoSizeChanged", Q_ARG(QString, channelid), Q_ARG(QString, struid), Q_ARG(QString, strStreamName), Q_ARG(int, width), Q_ARG(int, height), Q_ARG(int, elapsed));
}

void PrivateIRtcChannelEventHandler::onNetworkQuality(IRtcChannel* channel, const char* uid, QUALITY_TYPE txQuality, QUALITY_TYPE rxQuality)
{
	if (!channel) {
		qDebug() << "PrivateIRtcChannelEventHandler::onNetworkQuality";
		return;
	}

	QString channelid = channel->getChannelId();
	QString struid = uid;
	UserStateSingleton::instance()->SetRemoteNetQulity(struid, (int)rxQuality);

	QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigNetworkQuality", Q_ARG(const QString&, channelid), Q_ARG(QString, struid), Q_ARG(QUALITY_TYPE, txQuality), Q_ARG(QUALITY_TYPE, rxQuality));
	//UserStateSingleton::instance()->SetProcessData(QString("PrivateIRtcChannelEventHandler::onVideoSubscribeStateChanged uid = %1 txQuality = %2 rxQuality = %3").arg(uid).arg(txQuality).arg(rxQuality));
}

void PrivateIRtcChannelEventHandler::onConnectionLost(IRtcChannel* channel)
{
	if (!channel) {
		qDebug() << "PrivateIRtcChannelEventHandler::onConnectionLost";
		return;
	}

	QString channelid = QString::fromUtf8(channel->getChannelId());
	QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigConnectionLost", Q_ARG(QString, channelid));
	//UserStateSingleton::instance()->SetProcessData(QString("PrivateIRtcChannelEventHandler::onConnectionLost"));
}

void PrivateIRtcChannelEventHandler::onConnectionStateChanged(IRtcChannel* channel, CONNECTION_STATE_TYPE state, CONNECTION_CHANGED_REASON_TYPE reason)
{
	if (!channel) {
		qDebug() << "PrivateIRtcChannelEventHandler::onConnectionStateChanged";
		return;
	}

	QString channelid = QString::fromUtf8(channel->getChannelId());
	QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigConnectionStateChanged", Q_ARG(const QString&, channelid), Q_ARG(CONNECTION_STATE_TYPE, state), Q_ARG(CONNECTION_CHANGED_REASON_TYPE, reason));
	//UserStateSingleton::instance()->SetProcessData(QString("PrivateIRtcChannelEventHandler::onConnectionStateChanged state = %1 reason = %2").arg(state).arg(reason));
}

void PrivateIRtcChannelEventHandler::onRtcStats(IRtcChannel* channel, const RtcStats& stats)
{
	if (!channel) {
		qDebug() << "PrivateIRtcChannelEventHandler::onRtcStats";
		return;
	}

	QString channelid = channel->getChannelId();
	UserStateSingleton::instance()->SetRtcStats(channelid.toStdString(), stats);
	bool ret = QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigRtcStats", Q_ARG(QString, channelid), Q_ARG(const RtcStats&, stats));
}

void PrivateIRtcChannelEventHandler::onRemoteAudioStats(IRtcChannel* channel, const RemoteAudioStats& stats)
{
	if (!channel) {
		qDebug() << "PrivateIRtcChannelEventHandler::onRemoteAudioStats";
		return;
	}

	QString channelid = QString::fromUtf8(channel->getChannelId());
	//TODO
	//QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigRemoteAudioStats", Q_ARG(QString, channelid), Q_ARG(const RemoteAudioStats&, stats));
}

void PrivateIRtcChannelEventHandler::onRemoteVideoStats(IRtcChannel* channel, const RemoteVideoStats& stats)
{
}