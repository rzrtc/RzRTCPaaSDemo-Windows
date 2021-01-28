#include "RZRtcChannelEventHandler.h"
#include <QtCore/qstring.h>
#include <IRtcChannel.h>
#include "UserStateNotifySingleton.h"

RZRtcChannelEventHandler::RZRtcChannelEventHandler()
{
}

RZRtcChannelEventHandler::~RZRtcChannelEventHandler()
{
}

void RZRtcChannelEventHandler::onJoinChannelSuccess(rz::IRtcChannel* rtcChannel, const char* uid, int elapsed)
{
    QString channelid = QString::fromUtf8(rtcChannel->getChannelId());
    QString struid = QString::fromUtf8(uid);
    QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigJoinChannelSuccess", Q_ARG(QString, channelid), Q_ARG(QString, struid), Q_ARG(int, elapsed));
}

void RZRtcChannelEventHandler::onLeaveChannel(rz::IRtcChannel* rtcChannel, const rz::RtcStats& stat)
{
    //TODO:
    QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigLeaveChannel");
}

void RZRtcChannelEventHandler::onFirstRemoteVideoFrame(rz::IRtcChannel* rtcChannel, const char* uid, const char* streamName, int width, int height, int elapsed)
{
}

void RZRtcChannelEventHandler::onUserOffline(rz::IRtcChannel* rtcChannel, const char* uid, rz::USER_OFFLINE_REASON_TYPE reason)
{
    QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigUserOffline");
}

void RZRtcChannelEventHandler::onUserJoined(rz::IRtcChannel* channel, const char* uid, int elapsed)
{
    QString channelid = QString::fromUtf8(channel->getChannelId());
    QString struid = QString::fromUtf8(uid);
    QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigUserJoined", Q_ARG(QString, channelid), Q_ARG(QString, struid), Q_ARG(int, elapsed));
}

void RZRtcChannelEventHandler::onError(rz::IRtcChannel* channel, int err, const char* msg)
{
    QString channelid = QString::fromUtf8(channel->getChannelId());
    QString errmsg = QString::fromUtf8(msg);
    QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigChannelError", Q_ARG(QString, channelid), Q_ARG(int, err), Q_ARG(QString, errmsg));
    qDebug(msg);
}
