#pragma once

#include <QObject>

class UserStateNotifySingleton : public QObject
{
    Q_OBJECT

public:
    static UserStateNotifySingleton* instance();
signals:
    void SigJoinChannelSuccess(const QString& channelid, const QString& uid, int elapsed);
    void SigUserJoined(const QString& channelid, const QString& uid, int elapsed);
    void SigUserOffline();
    void SigLeaveChannel();
    void SigChannelError(const QString channelid, int err, const QString& errmsg);

private:
    static UserStateNotifySingleton* m_pUserStateNotifySingleton;
};
