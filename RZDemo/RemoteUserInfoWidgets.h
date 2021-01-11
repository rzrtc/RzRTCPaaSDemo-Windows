#pragma once

#include <QWidget>
#include "ui_RemoteUserInfoWidgets.h"
#include "common.h"
class RemoteUserInfoWidgets : public QWidget
{
	Q_OBJECT

public:
	RemoteUserInfoWidgets(QWidget *parent = Q_NULLPTR);
	~RemoteUserInfoWidgets();
public:
	E_REMOTE_INFO_TYPE GetType() { return m_type; }
	QString GetChannelId() { return m_strChannelId; }
	QString GetUid() { return m_strUserId; }
	QString GetStreamName() { return m_strStreamName; }

	void SetType(E_REMOTE_INFO_TYPE type) { m_type = type; }
	void SetChannelId(QString id) { m_strChannelId = id; }
	void SetUid(QString id) { m_strUserId = id; }
	void SetStreamName(QString name) { m_strStreamName = name; }
private:
	Ui::RemoteUserInfoWidgets ui;
	E_REMOTE_INFO_TYPE m_type;
	QString m_strChannelId;
	QString m_strUserId;
	QString m_strStreamName;
};
