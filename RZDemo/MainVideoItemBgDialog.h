#pragma once

#include <QWidget>
#include "ui_MainVideoItemBgDialog.h"
#include <QMouseEvent>
#include "common.h"
#include "UserStateSingleton.h"
#include <QPaintEngine>
class MainVideoItemBgDialog : public QDialog
{
	Q_OBJECT
public:
	MainVideoItemBgDialog(E_VIDEO_ITEM_TYPE type = e_init, QString uid = "", QString streamName = "", QWidget* parent = Q_NULLPTR);
	~MainVideoItemBgDialog();
public:
	E_VIDEO_ITEM_TYPE GetType() { return m_type; }
	QString GetChannelId() { return m_strChannelId; }
	QString GetUid() { return m_strUserId; }
	QString GetStreamName() { return m_strStreamName; }


	void SetChannelId(QString id) { m_strChannelId = id; }
	void SetUid(QString id) { m_strUserId = id; }
	void SetStreamName(QString name) { m_strStreamName = name; }
	void UpdateStreamName(QString streamName);
public slots:
	void SlotCam();
	void SlotMic();
	void SlotHd();
	void SlotClose();

	void SlotLocalVideoStateChanged(LOCAL_VIDEO_STREAM_STATE state, LOCAL_VIDEO_STREAM_ERROR error);
	void SlotLocalAudioStateChanged(LOCAL_AUDIO_STREAM_STATE state, LOCAL_AUDIO_STREAM_ERROR error);
	void SlotAudioVolumeIndication(const AUDIOVOLUMEINFO& speakers, unsigned int speakerNumber, int totalVolume);
	void SlotAudioSubscribeStateChanged(const QString& channelid, const QString& uid, SUBSCRIBE_STREAM_STATE state, int elapsed);
	void SlotVideoSubscribeStateChanged(const QString& channelid, const QString& uid, const QString& streamName, SUBSCRIBE_STREAM_STATE state, int elapsed);
	void SlotFirstRemoteVideoFrame(const QString& channelid, const QString& uid, const QString& streamName, int width, int height, int elapsed);
	void SlotLocalVideoStateChanged(const QString& channelid, const QString& streamName, LOCAL_VIDEO_STREAM_STATE state, LOCAL_VIDEO_STREAM_ERROR error, int elapsed);
protected:
	void showEvent(QShowEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* ev) override;
	void paintEvent(QPaintEvent* event) override;
private:
	void initStyleSheetFile();
	void initControl();
	void initConnect();
	void disConnect();

	void initUserStates();	//初始化用户状态
	void initDefLocalUserStates();	//默认推流
	void initMultiStreamsLocalUserStates();	//自定义推流
	void initRemoteUserStates();	//远端用户
private:
	void setRemoteUserAudioState(SUBSCRIBE_STREAM_STATE state);
	void setRemoteUserVideoState(SUBSCRIBE_STREAM_STATE state);
public:

private:
	Ui::MainVideoItemBgDialog ui;
	E_VIDEO_ITEM_TYPE m_type = e_init;
	QString m_strChannelId;
	QString m_strUserId;
	QString m_strStreamName;
	bool m_isSelfSream = false;	//是否自定义推流
	bool m_isSelfRender = false;	//是否自定义渲染
	bool m_isOpenglWidget = false;
};
