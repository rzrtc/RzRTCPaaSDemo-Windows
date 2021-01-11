#pragma once

#include <QWidget>
#include "ui_VideoItem.h"
#include <QMouseEvent>
#include "common.h"
#include "UserStateSingleton.h"
#include <QPaintEngine>
class VideoItem : public QWidget
{
	Q_OBJECT
public:
	VideoItem(QWidget* parent = Q_NULLPTR);
	~VideoItem();
public:
	const WId& GetWid();
	E_VIDEO_ITEM_TYPE GetType() { return m_type; }
	QString GetChannelId() { return m_strChannelId; }
	QString GetUid() { return m_strUserId; }
	QString GetStreamName() { return m_strStreamName; }


	void Init(E_VIDEO_ITEM_TYPE type, int row, int column);
	void Reset();
	void ResetOfMain();
	void SetChannelId(QString id) { m_strChannelId = id; }
	void SetUid(QString id) { m_strUserId = id; }
	void SetStreamName(QString name) { m_strStreamName = name; }
	void UpdateStreamName(QString streamName);
	void SetVideoCanvas();
	void SetShowMainState(bool state) {
		m_isShowMain = state;
	}

	bool GetShowMainState() {
		return m_isShowMain;
	}
signals:
	//void SigShowMain(E_VIDEO_ITEM_TYPE , QString channelid = "", QString uid = "", QString streamName = "");
	void SigShowMain(E_VIDEO_ITEM_TYPE, QString, QString, int, int);
	void SigEnabled();
public slots:
	void SlotCam();
	void SlotMic();
	void SlotHd();

	void SlotLocalVideoStateChanged(LOCAL_VIDEO_STREAM_STATE state, LOCAL_VIDEO_STREAM_ERROR error);
	void SlotLocalAudioStateChanged(LOCAL_AUDIO_STREAM_STATE state, LOCAL_AUDIO_STREAM_ERROR error);
	void SlotAudioVolumeIndication(const AUDIOVOLUMEINFO& speakers, unsigned int speakerNumber, int totalVolume);
	void SlotFirstLocalVideoFrame(int width, int height, int elapsed);
	void SlotFirstLocalVideoFramePublished(int elapsed);
	void SlotMediaDeviceStateChanged(const QString& deviceId, MEDIA_DEVICE_TYPE deviceType, int deviceState);
	
	void SlotVideoSizeChanged(int width, int height);
	void SlotFirstLocalAudioFramePublished(int elapsed);
	void SlotAudioRouteChanged(AUDIO_ROUTE_TYPE routing);
	void SlotNetworkTypeChanged(NETWORK_TYPE type);
	void SlotLocalAudioStats(const LocalAudioStats& stats);
	void SlotLocalVideoStats(const LocalVideoStats& stats);
	//remote
	
	void SlotError(const QString& channelid, int err, const QString& msg);
	
	void SlotAudioPublishStateChanged(const QString& channelid, PUBLISH_STREAM_STATE state, int elapsed);
	void SlotVideoPublishStateChanged(const QString& channelid, PUBLISH_STREAM_STATE state, int elapsed);
	void SlotAudioSubscribeStateChanged(const QString& channelid, const QString& uid, SUBSCRIBE_STREAM_STATE state, int elapsed);
	void SlotVideoSubscribeStateChanged(const QString& channelid, const QString& uid, const QString& streamName, SUBSCRIBE_STREAM_STATE state, int elapsed);
	void SlotFirstRemoteVideoFrame(const QString& channelid, const QString& uid, const QString& streamName, int width, int height, int elapsed);
	
	//多路流
	void SlotVideoPublishStateChanged(const QString& channelid, const QString& streamName, PUBLISH_STREAM_STATE state, int elapsed);
	void SlotLocalVideoStateChanged(const QString& channelid, const QString& streamName, LOCAL_VIDEO_STREAM_STATE state, LOCAL_VIDEO_STREAM_ERROR error, int elapsed);
	void SlotFirstVideoFramePublished(const QString& channelid, const QString& streamName, int elapsed);
	void SlotVideoSizeChanged(const QString& channelid, const QString& streamName, int width, int height);

	//第二路流处理

	//处理切换摄像头
	void SlotSwitchCam();
protected:
	void showEvent(QShowEvent* event) override;
	//QPaintEngine* paintEngine()const override;
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
	Ui::VideoItem ui;
	E_VIDEO_ITEM_TYPE m_type = e_init;
	QString m_strChannelId;
	QString m_strUserId;
	QString m_strStreamName;
	bool m_isSelfSream = false;	//是否自定义推流
	bool m_isSelfRender = false;	//是否自定义渲染
	bool m_isOpenglWidget = false;

	int m_nRow = -1;
	int m_nColumn = -1;
	bool m_isShowMain = false;
};
