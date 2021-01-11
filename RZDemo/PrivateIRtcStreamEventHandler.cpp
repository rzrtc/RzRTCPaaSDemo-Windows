#include "PrivateIRtcStreamEventHandler.h"
#include "UserStateSingleton.h"
#include <QMetaObject>
#include <QString>
#include <IRtcStream.h>
void PrivateIRtcStreamEventHandler::onVideoPublishStateChanged(IRtcStream* stream, PUBLISH_STREAM_STATE state, int elapsed)
{

}

void PrivateIRtcStreamEventHandler::onLocalVideoStateChanged(IRtcStream* stream, LOCAL_VIDEO_STREAM_STATE state, LOCAL_VIDEO_STREAM_ERROR error, int elapsed)
{
	std::string strStreamName = stream->getStreamName();
	std::string strChannelId = stream->getChannelId();
	UserStateSingleton::instance()->SetLocalVideoState(state, error, strStreamName);
	//自定义推流在此处获取本端视频流状态
	//SlotLocalVideoStateChanged(const QString & channelid, const QString & streamName, LOCAL_VIDEO_STREAM_STATE state, LOCAL_VIDEO_STREAM_ERROR error, int elapsed);
	QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigLocalVideoStateChanged", Q_ARG(const QString&, QString::fromStdString(strChannelId)),\
		Q_ARG(const QString&, QString::fromStdString(strStreamName)), Q_ARG(LOCAL_VIDEO_STREAM_STATE, state), Q_ARG(LOCAL_VIDEO_STREAM_ERROR, error),\
		Q_ARG(int, elapsed));
	
}

void PrivateIRtcStreamEventHandler::onFirstVideoFramePublished(IRtcStream* stream, int elapsed)
{

}

void PrivateIRtcStreamEventHandler::onVideoSizeChanged(IRtcStream* stream, int width, int height)
{
	/*QString strmsg = QString::fromUtf8(msg);
	QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigWarning", Q_ARG(int, warn), Q_ARG(QString, strmsg));*/
}