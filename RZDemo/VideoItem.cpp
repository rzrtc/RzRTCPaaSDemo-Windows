#include "VideoItem.h"
#include <QFileInfo>
#include "Settings.h"
#include "RzObject.h"
#include <QDebug>
#include <QTimer>
#include <QElapsedTimer>
#include "TipDialog.h"

VideoItem::VideoItem(QWidget *parent)
{
	ui.setupUi(this);
	//自定义推流是否开启
	ui.widget_video->hide();
	ui.label_uid->hide();
	ui.pushButton_cam->hide();
	ui.pushButton_mic->hide();
	ui.pushButton_hd->hide();
	ui.label_loading_icon->hide();
	ui.label_loading_tip->hide();
	ui.label_state_tip->hide();
	ui.label_state_icon->hide();
	ui.label_state_tip_2->hide();
	ui.label_state_icon_2->hide();
	ui.label_uid_2->hide();
	ui.widget_video->raise();
	QVariant value;
	Settings::instance()->Read("enablestream2", value, "SelfStreamSwitch");
	m_isSelfSream = value.toBool();
	Settings::instance()->Read("RenderDevice", value, "ChannelSetUp");
	m_isSelfRender = value.toString() == QString::fromLocal8Bit("自定义渲染器") ? true : false;
	initStyleSheetFile();
}

VideoItem::~VideoItem()
{
}

const WId& VideoItem::GetWid()
{
	return ui.widget_video->winId();
}

void VideoItem::Init(E_VIDEO_ITEM_TYPE type, int row, int column)
{
	ui.label_uid->show();
	ui.pushButton_cam->show();
	ui.pushButton_mic->show();
	ui.pushButton_hd->show();
	ui.label_loading_icon->show();
	ui.label_loading_tip->show();
	ui.label_state_tip->show();
	ui.label_state_icon->show();
	ui.label_state_tip_2->show();
	ui.label_state_icon_2->show();
	ui.label_uid_2->show();
	ui.label_uid->raise();
	ui.pushButton_cam->raise();
	ui.pushButton_mic->raise();
	ui.pushButton_hd->raise();
	ui.label_loading_icon->raise();
	ui.label_loading_tip->raise();
	ui.label_state_tip->raise();
	ui.label_state_icon->raise();
	ui.label_state_tip_2->raise();
	ui.label_state_icon_2->raise();
	ui.label_uid_2->raise();
	if (!m_isShowMain) {
		initConnect();
		initControl();
	}
	m_type = type;
	QString str;

	if (type == e_local_user) {

		ui.pushButton_cam->hide();
		ui.pushButton_mic->hide();
		str = m_strStreamName.isEmpty() ? (m_strUserId + QString::fromLocal8Bit("（我）")) : (m_strUserId + "_" + m_strStreamName + QString::fromLocal8Bit("（我）"));
	}
	else
		str = m_strStreamName.isEmpty() ? m_strUserId : (m_strUserId + "_" + m_strStreamName);
	if (RzObject::instance()->isSelfRender())
	{
		ui.label_uid_2->setText(str);
	}
	else
		ui.label_uid_2->hide();

	if (RzObject::instance()->isSelfPubStream() && type == e_local_user)
	{
		ui.label_uid_2->show();
		ui.label_uid_2->setText(str);
	}

	ui.label_uid->setText(str);

	if (!m_isShowMain)
	{
		SetVideoCanvas();
		initUserStates();
	}
	m_nRow = row;
	m_nColumn = column;
}

void VideoItem::Reset()
{
	disConnect();
	m_type = e_init;
	m_strUserId = "";
	m_strStreamName = "";
	SetShowMainState(false);
	//connect(ui.openGLWidget, SIGNAL(SigHide()), this, SLOT(SLotHideOpenGL()));
	//ui.openGLWidget->Clear();
	delete ui.widget_video;
	ui.widget_video = nullptr;
	ui.widget_video = new VideoWidget(this);
	ui.widget_video->setObjectName(QString::fromUtf8("widget_video"));
	ui.widget_video->setGeometry(QRect(0, 0, 300, 224));
	ui.widget_video->lower();

	ui.widget_video->hide();
	ui.label_uid->hide();
	ui.pushButton_cam->hide();
	ui.pushButton_mic->hide();
	ui.pushButton_hd->hide();
	ui.label_loading_icon->hide();
	ui.label_loading_tip->hide();
	ui.label_state_tip->hide();
	ui.label_state_icon->hide();
	ui.label_state_tip_2->hide();
	ui.label_state_icon_2->hide();
	ui.label_uid_2->hide();
	initStyleSheetFile();
	m_nRow = -1;
	m_nColumn = -1;
}

void VideoItem::ResetOfMain()
{
	disConnect();
}

void VideoItem::UpdateStreamName(QString streamName)
{
	m_strStreamName = streamName;
	ui.label_uid->setText(m_strUserId + "_" + m_strStreamName);
	if(ui.label_uid_2->isHidden() == false)
		ui.label_uid_2->setText(m_strUserId + "_" + m_strStreamName);
}

void VideoItem::SetVideoCanvas()
{
	//TODO 自定义渲染；sdk渲染（区分local/remote）
	if (m_type == e_local_user)
	{
		VideoCanvas canvas;
		canvas.view = (void*)ui.widget_video->winId();
		ui.widget_video->show();
		RzObject::instance()->SetupLocalVideo(canvas, m_strStreamName);
	}
	else
	{
		ui.widget_video->show();
		VideoCanvas canvas;
		canvas.view = (void*)ui.widget_video->winId();
		RzObject::instance()->SetupRemoteVideo(m_strUserId, m_strStreamName, canvas);
	}
	
	
	//
}

/*
*/


void VideoItem::SlotCam()
{
	
	QString strState = ui.pushButton_cam->text();
	if (m_type == e_local_user)
	{
		
		
		if (strState == QString::fromLocal8Bit("推送"))
		{
			RzObject::instance()->MuteLocalVideoStream(true, m_strStreamName);
			//调用接口 mute true
		}
		else
		{
			//调用接口 mute false
			RzObject::instance()->MuteLocalVideoStream(false, m_strStreamName);
		}
	}
	else
	{
		if (strState == QString::fromLocal8Bit("拉取"))
		{
			RzObject::instance()->GetIRtcChannel()->muteRemoteVideoStream(m_strUserId.toStdString().c_str(),m_strStreamName.toStdString().c_str(),true);
			//调用接口 mute true
		}
		else
		{
			if (UserStateSingleton::instance()->GetRecvAllVideoState() == false)
			{
				TipDialog tip;
				QTimer::singleShot(3000, &tip, &QDialog::accept);
				tip.SetText(QString::fromLocal8Bit("已停止接收所有远端视频！"));
				tip.exec();
				return;
			}
			RzObject::instance()->GetIRtcChannel()->muteRemoteVideoStream(m_strUserId.toStdString().c_str(), m_strStreamName.toStdString().c_str(), false);
			//调用接口 mute false
		}
	}
}

void VideoItem::SlotMic()
{
	QString strState = ui.pushButton_mic->text();
	auto RZEngine = RzObject::instance();
	auto RZChannel = RZEngine->GetIRtcChannel();
	if (m_type == e_local_user)
	{
		if (strState == QString::fromLocal8Bit("推送"))
		{
			RZEngine->MuteLocalAudioStream(true);
			//调用接口 mute true
		}
		else
		{
			RZEngine->MuteLocalAudioStream(false);
			//调用接口 mute false
		}
	}
	else
	{
		if (strState == QString::fromLocal8Bit("拉取"))
		{
			RZChannel->muteRemoteAudioStream(m_strUserId.toStdString().c_str(), true);
			//调用接口 mute true
		}
		else
		{
			if (UserStateSingleton::instance()->GetRecvAllAudioState() == false)
			{
				TipDialog tip;
				QTimer::singleShot(3000, &tip, &QDialog::accept);
				tip.SetText(QString::fromLocal8Bit("已停止接收所有远端音频！"));
				tip.exec();
				return;
			}
			RZChannel->muteRemoteAudioStream(m_strUserId.toStdString().c_str(), false);
			//调用接口 mute false
		}
	}
}

void VideoItem::SlotHd()
{

	if (m_type == e_local_user)
		return;

	static bool isHd = true;
	int ret = RzObject::instance()->SetRemoteVideoStreamType(m_strUserId, m_strStreamName, isHd);
	//TODO怎么获取大小流状态
	isHd = !isHd;

}



void VideoItem::SlotLocalVideoStateChanged(LOCAL_VIDEO_STREAM_STATE state, LOCAL_VIDEO_STREAM_ERROR error)
{
	if (m_type != e_local_user)
		return;

	ui.pushButton_cam->setStyleSheet("QPushButton {background-image: url(./resource/image/item_cam_open.png); background-repeat: no-repeat;}");
	ui.label_state_tip->hide();	//远端视频停止发送
	ui.label_state_icon->hide();
	switch (state)
	{
	case rz::LOCAL_VIDEO_STREAM_STATE_STOPPED:
	{
		ui.label_state_tip->setText(QString::fromLocal8Bit("摄像头未开启"));
		ui.label_state_tip->show();	//远端视频停止发送
		ui.label_state_icon->show();
		ui.widget_video->hide();
		//ui.widget_video->hide();
		ui.pushButton_cam->setStyleSheet("QPushButton {background-image: url(./resource/image/item_cam_red.png); background-repeat: no-repeat;}");
		break;
	}
	case rz::LOCAL_VIDEO_STREAM_STATE_CAPTURING:
	{
		ui.widget_video->show();
		break;
	}
	case rz::LOCAL_VIDEO_STREAM_STATE_FAILED:
	{
		//TODO 发送提示信息信息
		break;
	}
	case rz::LOCAL_VIDEO_STREAM_STATE_SENDING:
	{
		//切换当前按钮状态为发送
		ui.pushButton_cam->setText(QString::fromLocal8Bit("推送"));
		break;
	}
	case rz::LOCAL_VIDEO_STREAM_STATE_NO_SEND:
	{
		//切换当前按钮状态为停止
		ui.pushButton_cam->setText(QString::fromLocal8Bit("停止"));
		break;
	}
	default:
	{
		break;
	}
	}

}

void VideoItem::SlotLocalAudioStateChanged(LOCAL_AUDIO_STREAM_STATE state, LOCAL_AUDIO_STREAM_ERROR error)
{
	if (m_type == e_remote_user)
		return;

	ui.pushButton_mic->setStyleSheet("QPushButton {background-image: url(./resource/image/item_mic_open.png); background-repeat: no-repeat;}");
	switch (state)
	{
	case rz::LOCAL_AUDIO_STREAM_STATE_STOPPED:
	{
		ui.pushButton_mic->setStyleSheet("QPushButton {background-image: url(./resource/image/item_mic_red.png); background-repeat: no-repeat;}");
		break;
	}

	case rz::LOCAL_AUDIO_STREAM_STATE_RECORDING:
	{
		ui.pushButton_mic->setStyleSheet("QPushButton {background-image: url(./resource/image/item_mic_open.png); background-repeat: no-repeat;}");
		break;
	}
	case rz::LOCAL_AUDIO_STREAM_STATE_FAILED:
	{
		//TODO 发送提示信息信息
		break;
	}
	case rz::LOCAL_AUDIO_STREAM_STATE_SENDING:
	{
		//切换当前按钮状态为发送
		ui.pushButton_mic->setText(QString::fromLocal8Bit("推送"));
		break;
	}
	case rz::LOCAL_AUDIO_STREAM_STATE_NO_SEND:
	{
		//切换当前按钮状态为停止
		ui.pushButton_mic->setText(QString::fromLocal8Bit("停止"));
		break;
	}
	default:
	{
		break;
	}
	}
}

void VideoItem::SlotAudioVolumeIndication(const AUDIOVOLUMEINFO& speakers, unsigned int speakerNumber, int totalVolume)
{
}

void VideoItem::SlotFirstLocalVideoFrame(int width, int height, int elapsed) 
{}
void VideoItem::SlotFirstLocalVideoFramePublished(int elapsed) 
{}
void VideoItem::SlotMediaDeviceStateChanged(const QString& deviceId, MEDIA_DEVICE_TYPE deviceType, int deviceState) 
{}

void VideoItem::SlotVideoSizeChanged(int width, int height) 
{}
void VideoItem::SlotFirstLocalAudioFramePublished(int elapsed) 
{}
void VideoItem::SlotAudioRouteChanged(AUDIO_ROUTE_TYPE routing) 
{}
void VideoItem::SlotNetworkTypeChanged(NETWORK_TYPE type) 
{}
void VideoItem::SlotLocalAudioStats(const LocalAudioStats& stats) 
{}
void VideoItem::SlotLocalVideoStats(const LocalVideoStats& stats) 
{}



//remote

void VideoItem::SlotError(const QString& channelid, int err, const QString& msg)
{}

void VideoItem::SlotAudioPublishStateChanged(const QString& channelid, PUBLISH_STREAM_STATE state, int elapsed) 
{}
void VideoItem::SlotVideoPublishStateChanged(const QString& channelid, PUBLISH_STREAM_STATE state, int elapsed)
{}
void VideoItem::SlotAudioSubscribeStateChanged(const QString& channelid, const QString& uid, SUBSCRIBE_STREAM_STATE state, int elapsed) 
{
	if (uid != m_strUserId || m_type == e_local_user)
		return;
	setRemoteUserAudioState(state);
}
void VideoItem::SlotVideoSubscribeStateChanged(const QString& channelid, const QString& uid, const QString& streamName, SUBSCRIBE_STREAM_STATE state, int elapsed) 
{
	if (uid != m_strUserId || m_strStreamName != streamName || m_type == e_local_user)
		return;
	qDebug() << "VideoItem::SlotVideoSubscribeStateChanged uid = " << uid << state;
	setRemoteUserVideoState(state);
}
void VideoItem::SlotFirstRemoteVideoFrame(const QString& channelid, const QString& uid, const QString& streamName, int width, int height, int elapsed)
{
	if (uid != m_strUserId || m_strStreamName != streamName || m_type == e_local_user)
		return;
	/*
	* 判断是否远端
	* 显示video
	*/
	ui.widget_video->show();
	ui.label_state_tip->hide();	//远端视频停止发送
	ui.label_state_icon->hide();
	ui.label_state_icon_2->hide();
	ui.label_state_tip_2->hide(); //已停止拉取远端视频
	ui.label_loading_icon->hide();
	ui.label_loading_tip->hide();
	ui.pushButton_cam->setStyleSheet("QPushButton {background-image: url(./resource/image/item_cam_open.png); background-repeat: no-repeat;}");
	ui.pushButton_cam->setText(QString::fromLocal8Bit("拉取"));
}

//多路流
void VideoItem::SlotVideoPublishStateChanged(const QString& channelid, const QString& streamName, PUBLISH_STREAM_STATE state, int elapsed)
{}
void VideoItem::SlotLocalVideoStateChanged(const QString& channelid, const QString& streamName, LOCAL_VIDEO_STREAM_STATE state, LOCAL_VIDEO_STREAM_ERROR error, int elapsed)
{
	if (m_type == e_remote_user || streamName != m_strStreamName)
		return;

	ui.pushButton_cam->setStyleSheet("QPushButton {background-image: url(./resource/image/item_cam_open.png); background-repeat: no-repeat;}");

	switch (state)
	{
	case rz::LOCAL_VIDEO_STREAM_STATE_STOPPED:
	{

		break;
	}

	case rz::LOCAL_VIDEO_STREAM_STATE_CAPTURING:
	{
		break;
	}
	case rz::LOCAL_VIDEO_STREAM_STATE_FAILED:
	{
		//TODO 发送提示信息信息
		break;
	}
	case rz::LOCAL_VIDEO_STREAM_STATE_SENDING:
	{
		//切换当前按钮状态为发送
		ui.pushButton_cam->setText(QString::fromLocal8Bit("推送"));
		break;
	}
	case rz::LOCAL_VIDEO_STREAM_STATE_NO_SEND:
	{
		//切换当前按钮状态为停止
		ui.pushButton_cam->setText(QString::fromLocal8Bit("停止"));
		break;
	}
	default:
	{
		break;
	}
	}


}
void VideoItem::SlotFirstVideoFramePublished(const QString& channelid, const QString& streamName, int elapsed)
{}
void VideoItem::SlotVideoSizeChanged(const QString& channelid, const QString& streamName, int width, int height)
{}

void VideoItem::SlotSwitchCam()
{
	SetVideoCanvas();
}

void VideoItem::showEvent(QShowEvent* event)
{
	this->setAttribute(Qt::WA_Mapped);
	QWidget::showEvent(event);
}

//QPaintEngine* VideoItem::paintEngine() const
//{
//	return 0;
//}

void VideoItem::initStyleSheetFile()
{
	QFile styleFile("./resource/videoitem.qss");
	styleFile.open(QIODevice::ReadOnly);
	this->setStyleSheet(QString(styleFile.readAll()));
}

/*
如果是local uid，不显示中间label
*/
void VideoItem::initControl()
{
	//ui.label_uid->adjustSize();
	QVariant value;
	Settings::instance()->Read("user", value, "Info");
	if (m_strUserId == value.toString()) {
		ui.label_loading_icon->hide();
		ui.label_loading_tip->hide();
		ui.label_state_icon_2->hide();
		ui.label_state_tip_2->hide();
		ui.pushButton_hd->hide();
	}
}

void VideoItem::initConnect()
{
	connect(ui.pushButton_cam, SIGNAL(clicked()), this, SLOT(SlotCam()));
	connect(ui.pushButton_mic, SIGNAL(clicked()), this, SLOT(SlotMic()));
	connect(ui.pushButton_hd, SIGNAL(clicked()), this, SLOT(SlotHd()));

	//TODO 通过流名称和角色（local和remote）进行相应的注册
	
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigLocalVideoStateChanged(LOCAL_VIDEO_STREAM_STATE, LOCAL_VIDEO_STREAM_ERROR)), this, SLOT(SlotLocalVideoStateChanged(LOCAL_VIDEO_STREAM_STATE, LOCAL_VIDEO_STREAM_ERROR)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigLocalAudioStateChanged(LOCAL_AUDIO_STREAM_STATE, LOCAL_AUDIO_STREAM_ERROR)), this, SLOT(SlotLocalAudioStateChanged(LOCAL_AUDIO_STREAM_STATE, LOCAL_AUDIO_STREAM_ERROR)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigAudioVolumeIndication(const AUDIOVOLUMEINFO&, unsigned int, int)), this, SLOT(SlotAudioVolumeIndication(const AUDIOVOLUMEINFO&, unsigned int, int)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigFirstLocalVideoFrame(int, int, int)), this, SLOT(SlotFirstLocalVideoFrame(int, int, int)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigFirstLocalVideoFramePublished(int)), this, SLOT(SlotFirstLocalVideoFramePublished(int)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigMediaDeviceStateChanged(const QString&, MEDIA_DEVICE_TYPE, int)), this, SLOT(SlotMediaDeviceStateChanged(const QString&, MEDIA_DEVICE_TYPE, int)));
	
	
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigFirstLocalAudioFramePublished(int)), this, SLOT(SlotFirstLocalAudioFramePublished(int)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigAudioRouteChanged(AUDIO_ROUTE_TYPE)), this, SLOT(SlotAudioRouteChanged(AUDIO_ROUTE_TYPE)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigNetworkTypeChanged(NETWORK_TYPE)), this, SLOT(SlotNetworkTypeChanged(NETWORK_TYPE)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigLocalAudioStats(const LocalAudioStats&)), this, SLOT(SlotLocalAudioStats(const LocalAudioStats&)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigLocalVideoStats(const LocalVideoStats&)), this, SLOT(SlotLocalVideoStats(const LocalVideoStats&)));
	
	
	
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigError(const QString&, int, const QString&)), this, SLOT(SlotError(const QString&, int, const QString&)));
	
	
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigAudioPublishStateChanged(const QString&, PUBLISH_STREAM_STATE state, int)), this, SLOT(SlotAudioPublishStateChanged(const QString&, PUBLISH_STREAM_STATE, int)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigVideoPublishStateChanged(const QString&, PUBLISH_STREAM_STATE, int)), this, SLOT(SlotVideoPublishStateChanged(const QString&, PUBLISH_STREAM_STATE, int)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigAudioSubscribeStateChanged(const QString&, const QString&, SUBSCRIBE_STREAM_STATE, int)), this, SLOT(SlotAudioSubscribeStateChanged(const QString&, const QString&, SUBSCRIBE_STREAM_STATE, int)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigVideoSubscribeStateChanged(const QString&, const QString&, const QString&, SUBSCRIBE_STREAM_STATE, int)), this, SLOT(SlotVideoSubscribeStateChanged(const QString&, const QString&, const QString&, SUBSCRIBE_STREAM_STATE, int)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigFirstRemoteVideoFrame(const QString&, const QString&, const QString&, int, int, int)), this, SLOT(SlotFirstRemoteVideoFrame(const QString&, const QString&, const QString&, int, int, int)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigVideoSizeChanged(const QString&, const QString&, const QString&, int, int, int)), this, SLOT(SlotVideoSizeChanged(const QString&, const QString&, const QString&, int, int, int)));
	
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigVideoPublishStateChanged(const QString&, const QString&, PUBLISH_STREAM_STATE, int)), this, SLOT(SlotVideoPublishStateChanged(const QString&, const QString&, PUBLISH_STREAM_STATE, int)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigLocalVideoStateChanged(const QString&, const QString&, LOCAL_VIDEO_STREAM_STATE, LOCAL_VIDEO_STREAM_ERROR, int)), this, SLOT(SlotLocalVideoStateChanged(const QString&, const QString&, LOCAL_VIDEO_STREAM_STATE, LOCAL_VIDEO_STREAM_ERROR, int)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigFirstVideoFramePublished(const QString&, const QString&, int)), this, SLOT(SlotFirstVideoFramePublished(const QString&, const QString&, int)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigVideoSizeChanged(const QString&, const QString&, int, int)), this, SLOT(SlotVideoSizeChanged(const QString&, const QString&, int, int)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigSwitchCam()), this, SLOT(SlotSwitchCam()));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigVideoItemUpdate(bool)), this, SLOT(SlotUpdate(bool)));
	connect(this, SIGNAL(SigEnabled()), this, SLOT(SlotEnable()), Qt::QueuedConnection);
}

void VideoItem::disConnect()
{
	disconnect(ui.pushButton_cam, SIGNAL(clicked()), this, SLOT(SlotCam()));
	disconnect(ui.pushButton_mic, SIGNAL(clicked()), this, SLOT(SlotMic()));
	disconnect(ui.pushButton_hd, SIGNAL(clicked()), this, SLOT(SlotHd()));
	disconnect(UserStateNotifySingleton::instance(), SIGNAL(SigLocalVideoStateChanged(LOCAL_VIDEO_STREAM_STATE, LOCAL_VIDEO_STREAM_ERROR)), this, SLOT(SlotLocalVideoStateChanged(LOCAL_VIDEO_STREAM_STATE, LOCAL_VIDEO_STREAM_ERROR)));
	disconnect(UserStateNotifySingleton::instance(), SIGNAL(SigLocalAudioStateChanged(LOCAL_AUDIO_STREAM_STATE, LOCAL_AUDIO_STREAM_ERROR)), this, SLOT(SlotLocalAudioStateChanged(LOCAL_AUDIO_STREAM_STATE, LOCAL_AUDIO_STREAM_ERROR)));
	disconnect(UserStateNotifySingleton::instance(), SIGNAL(SigAudioVolumeIndication(const AUDIOVOLUMEINFO&, unsigned int, int)), this, SLOT(SlotAudioVolumeIndication(const AUDIOVOLUMEINFO&, unsigned int, int)));
	disconnect(UserStateNotifySingleton::instance(), SIGNAL(SigFirstLocalVideoFrame(int, int, int)), this, SLOT(SlotFirstLocalVideoFrame(int, int, int)));
	disconnect(UserStateNotifySingleton::instance(), SIGNAL(SigFirstLocalVideoFramePublished(int)), this, SLOT(SlotFirstLocalVideoFramePublished(int)));
	disconnect(UserStateNotifySingleton::instance(), SIGNAL(SigMediaDeviceStateChanged(const QString&, MEDIA_DEVICE_TYPE, int)), this, SLOT(SlotMediaDeviceStateChanged(const QString&, MEDIA_DEVICE_TYPE, int)));
	disconnect(UserStateNotifySingleton::instance(), SIGNAL(SigFirstLocalAudioFramePublished(int)), this, SLOT(SlotFirstLocalAudioFramePublished(int)));
	disconnect(UserStateNotifySingleton::instance(), SIGNAL(SigAudioRouteChanged(AUDIO_ROUTE_TYPE)), this, SLOT(SlotAudioRouteChanged(AUDIO_ROUTE_TYPE)));
	disconnect(UserStateNotifySingleton::instance(), SIGNAL(SigNetworkTypeChanged(NETWORK_TYPE)), this, SLOT(SlotNetworkTypeChanged(NETWORK_TYPE)));
	disconnect(UserStateNotifySingleton::instance(), SIGNAL(SigLocalAudioStats(const LocalAudioStats&)), this, SLOT(SlotLocalAudioStats(const LocalAudioStats&)));
	disconnect(UserStateNotifySingleton::instance(), SIGNAL(SigLocalVideoStats(const LocalVideoStats&)), this, SLOT(SlotLocalVideoStats(const LocalVideoStats&)));
	disconnect(UserStateNotifySingleton::instance(), SIGNAL(SigError(const QString&, int, const QString&)), this, SLOT(SlotError(const QString&, int, const QString&)));
	disconnect(UserStateNotifySingleton::instance(), SIGNAL(SigAudioPublishStateChanged(const QString&, PUBLISH_STREAM_STATE state, int)), this, SLOT(SlotAudioPublishStateChanged(const QString&, PUBLISH_STREAM_STATE, int)));
	disconnect(UserStateNotifySingleton::instance(), SIGNAL(SigVideoPublishStateChanged(const QString&, PUBLISH_STREAM_STATE, int)), this, SLOT(SlotVideoPublishStateChanged(const QString&, PUBLISH_STREAM_STATE, int)));
	disconnect(UserStateNotifySingleton::instance(), SIGNAL(SigAudioSubscribeStateChanged(const QString&, const QString&, SUBSCRIBE_STREAM_STATE, int)), this, SLOT(SlotAudioSubscribeStateChanged(const QString&, const QString&, SUBSCRIBE_STREAM_STATE, int)));
	disconnect(UserStateNotifySingleton::instance(), SIGNAL(SigVideoSubscribeStateChanged(const QString&, const QString&, const QString&, SUBSCRIBE_STREAM_STATE, int)), this, SLOT(SlotVideoSubscribeStateChanged(const QString&, const QString&, const QString&, SUBSCRIBE_STREAM_STATE, int)));
	disconnect(UserStateNotifySingleton::instance(), SIGNAL(SigFirstRemoteVideoFrame(const QString&, const QString&, const QString&, int, int, int)), this, SLOT(SlotFirstRemoteVideoFrame(const QString&, const QString&, const QString&, int, int, int)));
	disconnect(UserStateNotifySingleton::instance(), SIGNAL(SigVideoSizeChanged(const QString&, const QString&, const QString&, int, int, int)), this, SLOT(SlotVideoSizeChanged(const QString&, const QString&, const QString&, int, int, int)));
	disconnect(UserStateNotifySingleton::instance(), SIGNAL(SigVideoPublishStateChanged(const QString&, const QString&, PUBLISH_STREAM_STATE, int)), this, SLOT(SlotVideoPublishStateChanged(const QString&, const QString&, PUBLISH_STREAM_STATE, int)));
	disconnect(UserStateNotifySingleton::instance(), SIGNAL(SigLocalVideoStateChanged(const QString&, const QString&, LOCAL_VIDEO_STREAM_STATE, LOCAL_VIDEO_STREAM_ERROR, int)), this, SLOT(SlotLocalVideoStateChanged(const QString&, const QString&, LOCAL_VIDEO_STREAM_STATE, LOCAL_VIDEO_STREAM_ERROR, int)));
	disconnect(UserStateNotifySingleton::instance(), SIGNAL(SigFirstVideoFramePublished(const QString&, const QString&, int)), this, SLOT(SlotFirstVideoFramePublished(const QString&, const QString&, int)));
	disconnect(UserStateNotifySingleton::instance(), SIGNAL(SigVideoSizeChanged(const QString&, const QString&, int, int)), this, SLOT(SlotVideoSizeChanged(const QString&, const QString&, int, int)));
	disconnect(UserStateNotifySingleton::instance(), SIGNAL(SigSwitchCam()), this, SLOT(SlotSwitchCam()));
	disconnect(UserStateNotifySingleton::instance(), SIGNAL(SigVideoItemUpdate(bool)), this, SLOT(SlotUpdate(bool)));
	disconnect(this, SIGNAL(SigEnabled()), this, SLOT(SlotEnable()));
}

void VideoItem::initUserStates()
{
	m_type == e_local_user ? m_isSelfSream ? initMultiStreamsLocalUserStates() : initDefLocalUserStates() : initRemoteUserStates();
	//初始化第一帧相关
	//VIDEOFRAMESIZE videoSize;
	//UserStateSingleton::instance()->GetFirstRemoteVideoFrame(m_strChannelId, m_strUserId, m_strStreamName, videoSize);
	//if (videoSize.w > 0 && videoSize.h > 0)
	//{
	//	ui.widget_video->show();
	//	ui.label_state_tip->hide();	//远端视频停止发送
	//	ui.label_state_icon->hide();
	//	ui.label_state_icon_2->hide();
	//	ui.label_state_tip_2->hide(); //已停止拉取远端视频
	//	ui.label_loading_icon->hide();
	//	ui.label_loading_tip->hide();
	//	ui.pushButton_cam->setStyleSheet("QPushButton {background-image: url(./resource/image/item_cam_open.png); background-repeat: no-repeat;}");
	//	ui.pushButton_cam->setText(QString::fromLocal8Bit("拉取"));
	//}
}

void VideoItem::initDefLocalUserStates()
{
	//ui.widget_video->hide();
	ui.label_state_tip->hide();	//远端视频停止发送
	ui.label_state_icon->hide();
	ui.label_state_icon_2->hide();
	ui.label_state_tip_2->hide(); //已停止拉取远端视频
	ui.label_loading_icon->hide();
	ui.label_loading_tip->hide();


	LOCAL_VIDEO_STREAM_STATE videoState = UserStateSingleton::instance()->GetLocalVideoState();
	if (videoState == LOCAL_VIDEO_STREAM_STATE_NO_SEND)
	{
		ui.pushButton_cam->setText(QString::fromLocal8Bit("停止"));
	}
		
	else
	{
		ui.pushButton_cam->setText(QString::fromLocal8Bit("推送"));
	}
	LOCAL_VIDEO_STREAM_STATE videoDeviceState = UserStateSingleton::instance()->GetLocalVideoDeviceState();
	if (videoDeviceState == LOCAL_VIDEO_STREAM_STATE_STOPPED)
	{
		ui.label_state_tip->setText(QString::fromLocal8Bit("摄像头未开启"));
		ui.label_state_tip->show();	//远端视频停止发送
		ui.label_state_icon->show();
		ui.widget_video->hide();
		ui.pushButton_cam->setStyleSheet("QPushButton {background-image: url(./resource/image/item_cam_red.png); background-repeat: no-repeat;}");
	}
	else
	{
		ui.widget_video->show();
	}

	LOCAL_AUDIO_STREAM_STATE audioState = UserStateSingleton::instance()->GetLocalAudioState();
	if (audioState == LOCAL_VIDEO_STREAM_STATE_NO_SEND)
	{
		ui.pushButton_mic->setText(QString::fromLocal8Bit("停止"));

	}
	else
	{
		ui.pushButton_mic->setText(QString::fromLocal8Bit("推送"));
	}
		

	LOCAL_AUDIO_STREAM_STATE audioDeviceState = UserStateSingleton::instance()->GetLocalAudioDeviceState();
	if(audioDeviceState == LOCAL_AUDIO_STREAM_STATE_STOPPED)
	{
		ui.pushButton_mic->setStyleSheet("QPushButton {background-image: url(./resource/image/item_mic_red.png); background-repeat: no-repeat;}");
	}
	else
	{
		ui.pushButton_mic->setStyleSheet("QPushButton {background-image: url(./resource/image/item_mic_open.png); background-repeat: no-repeat;}");
	}
	//本端音量暂不处理
}

void VideoItem::initMultiStreamsLocalUserStates()
{
	ui.label_state_tip->hide();	//远端视频停止发送
	ui.label_state_icon->hide();
	ui.label_state_icon_2->hide();
	ui.label_state_tip_2->hide(); //已停止拉取远端视频
	ui.label_loading_icon->hide();
	ui.label_loading_tip->hide();

	bool isRunning  = false;
	if (!isRunning)
	{
		ui.label_state_tip->setText(QString::fromLocal8Bit("摄像头未开启"));
		ui.label_state_tip->show();	//远端视频停止发送
		ui.label_state_icon->show();
		ui.widget_video->hide();
		ui.pushButton_cam->setStyleSheet("QPushButton {background-image: url(./resource/image/item_cam_red.png); background-repeat: no-repeat;}");
	}
	else
	{
		ui.widget_video->show();
	}

	LOCAL_VIDEO_STREAM_STATE videoState = UserStateSingleton::instance()->GetLocalVideoState(m_strStreamName.toStdString());
	if (videoState == LOCAL_VIDEO_STREAM_STATE_NO_SEND)
	{
		ui.pushButton_cam->setText(QString::fromLocal8Bit("停止"));
	}
	else
	{
		ui.pushButton_cam->setText(QString::fromLocal8Bit("推送"));
	}

	LOCAL_AUDIO_STREAM_STATE audioState = UserStateSingleton::instance()->GetLocalAudioState();
	if (audioState == LOCAL_VIDEO_STREAM_STATE_NO_SEND)
	{
		ui.pushButton_mic->setText(QString::fromLocal8Bit("停止"));

	}
	else
	{
		ui.pushButton_mic->setText(QString::fromLocal8Bit("推送"));
	}


	LOCAL_AUDIO_STREAM_STATE audioDeviceState = UserStateSingleton::instance()->GetLocalAudioDeviceState();
	if (audioDeviceState == LOCAL_AUDIO_STREAM_STATE_STOPPED)
	{
		ui.pushButton_mic->setStyleSheet("QPushButton {background-image: url(./resource/image/item_mic_red.png); background-repeat: no-repeat;}");
	}
	else
	{
		ui.pushButton_mic->setStyleSheet("QPushButton {background-image: url(./resource/image/item_mic_open.png); background-repeat: no-repeat;}");
	}

	//本端音量暂不处理
}

void VideoItem::initRemoteUserStates()
{
	SUBSCRIBE_STREAM_STATE videoState = SUBSCRIBE_STREAM_STATE_NO_SEND;
	UserStateSingleton::instance()->GetVideoSubscribeState(m_strUserId.toStdString(), m_strStreamName.toStdString(), videoState);
	setRemoteUserVideoState(videoState);
	SUBSCRIBE_STREAM_STATE audioState = UserStateSingleton::instance()->GetAudioSubscribeState(m_strUserId.toStdString());
	setRemoteUserAudioState(audioState);
}

void VideoItem::setRemoteUserAudioState(SUBSCRIBE_STREAM_STATE state)
{
	qDebug() << "setRemoteUserAudioState = " << state;
	ui.pushButton_mic->setStyleSheet("QPushButton {background-image: url(./resource/image/item_speaker_open.png); background-repeat: no-repeat;}");
	ui.pushButton_mic->setText(QString::fromLocal8Bit("拉取"));
	switch (state)
	{
	case rz::SUBSCRIBE_STREAM_STATE_FAILED://流订阅失败
		break;
	case rz::SUBSCRIBE_STREAM_STATE_OFFLINE://流下线
		break;
	case rz::SUBSCRIBE_STREAM_STATE_ONLINE: //流上线
		break;
	case rz::SUBSCRIBE_STREAM_STATE_NO_SEND://流上线但没发送
	{
		ui.pushButton_mic->setStyleSheet("QPushButton {background-image: url(./resource/image/item_speaker_red.png); background-repeat: no-repeat;}");
		ui.pushButton_mic->setText(QString::fromLocal8Bit("拉取"));
		break;
	}

	case rz::SUBSCRIBE_STREAM_STATE_NO_RECV://流上线且发送但不接收
	{
		ui.pushButton_mic->setStyleSheet("QPushButton {background-image: url(./resource/image/item_speaker_open.png); background-repeat: no-repeat;}");
		ui.pushButton_mic->setText(QString::fromLocal8Bit("停止"));
		break;
	}

	case rz::SUBSCRIBE_STREAM_STATE_NO_SEND_NO_RECV://流上线但没发送且不接收
	{
		ui.pushButton_mic->setStyleSheet("QPushButton {background-image: url(./resource/image/item_speaker_red.png); background-repeat: no-repeat;}");
		ui.pushButton_mic->setText(QString::fromLocal8Bit("停止"));
		break;
	}
	case rz::SUBSCRIBE_STREAM_STATE_SUBSCRIBING:	//正在订阅
		break;
	case rz::SUBSCRIBE_STREAM_STATE_SUBSCRIBED:	//订阅成功
	{
		
		break;
	}
	case rz::SUBSCRIBE_STREAM_STATE_FROZEN://卡顿中
		break;
	default:
		break;
	}
}

void VideoItem::setRemoteUserVideoState(SUBSCRIBE_STREAM_STATE state)
{
	ui.label_state_tip->hide();	//远端视频停止发送
	ui.label_state_icon->hide();
	ui.label_state_icon_2->hide();
	ui.label_state_tip_2->hide(); //已停止拉取远端视频
	ui.label_loading_icon->hide();
	ui.label_loading_tip->hide();
	ui.pushButton_cam->setStyleSheet("QPushButton {background-image: url(./resource/image/item_cam_open.png); background-repeat: no-repeat;}");
	ui.pushButton_cam->setText(QString::fromLocal8Bit("拉取"));
	ui.label_state_tip->setText(QString::fromLocal8Bit("远端视频流停止发送"));
	switch (state)
	{
	case rz::SUBSCRIBE_STREAM_STATE_FAILED://流订阅失败
		break;
	case rz::SUBSCRIBE_STREAM_STATE_OFFLINE://流下线
		break;
	case rz::SUBSCRIBE_STREAM_STATE_ONLINE: //流上线
		break;
	case rz::SUBSCRIBE_STREAM_STATE_NO_SEND://流上线但没发送
	{
		ui.widget_video->hide();
		//ui.widget_video->hide();
		ui.label_state_tip->show();	//远端视频停止发送
		ui.label_state_icon->show();
		//ui.label_state_icon_2->hide();
		//ui.label_state_tip_2->hide(); //已停止拉取远端视频
		//ui.label_loading_icon->hide();
		//ui.label_loading_tip->hide();

		ui.pushButton_cam->setStyleSheet("QPushButton {background-image: url(./resource/image/item_cam_red.png); background-repeat: no-repeat;}");
		ui.pushButton_cam->setText(QString::fromLocal8Bit("拉取"));
		break;
	}

	case rz::SUBSCRIBE_STREAM_STATE_NO_RECV://流上线且发送但不接收
	{
		ui.widget_video->hide();
		//ui.widget_video->hide();	
		//ui.label_state_tip->hide();	//远端视频停止发送
		//ui.label_state_icon->hide();
		ui.label_state_icon_2->show();
		ui.label_state_tip_2->show(); //已停止拉取远端视频
		//ui.label_loading_icon->hide();
		//ui.label_loading_tip->hide();

		ui.pushButton_cam->setStyleSheet("QPushButton {background-image: url(./resource/image/item_cam_open.png); background-repeat: no-repeat;}");
		ui.pushButton_cam->setText(QString::fromLocal8Bit("停止"));
		break;
	}

	case rz::SUBSCRIBE_STREAM_STATE_NO_SEND_NO_RECV://流上线但没发送且不接收
	{
		ui.widget_video->hide();
		//ui.widget_video->hide();

		ui.label_state_tip->show();	//远端视频停止发送
		ui.label_state_icon->show();
		ui.label_state_icon_2->show();
		ui.label_state_tip_2->show(); //已停止拉取远端视频
		//ui.label_loading_icon->hide();
		//ui.label_loading_tip->hide();

		ui.pushButton_cam->setStyleSheet("QPushButton {background-image: url(./resource/image/item_cam_red.png); background-repeat: no-repeat;}");
		ui.pushButton_cam->setText(QString::fromLocal8Bit("停止"));
		break;
	}
	case rz::SUBSCRIBE_STREAM_STATE_SUBSCRIBING:	//正在订阅
	{
		ui.widget_video->hide();

		//ui.label_state_tip->hide();	//远端视频停止发送
		//ui.label_state_icon->hide();
		//ui.label_state_icon_2->hide();
		//ui.label_state_tip_2->hide(); //已停止拉取远端视频
		ui.label_loading_icon->show();
		ui.label_loading_tip->show();

		ui.pushButton_cam->setStyleSheet("QPushButton {background-image: url(./resource/image/item_cam_open.png); background-repeat: no-repeat;}");
		ui.pushButton_cam->setText(QString::fromLocal8Bit("拉取"));
		break;
	}
	case rz::SUBSCRIBE_STREAM_STATE_SUBSCRIBED:	//订阅成功
	{

		ui.widget_video->show();//TODO: 和第一帧消息来得先后顺序
		/*ui.label_loading_icon->show();
		ui.label_loading_tip->show();*/
		break;
	}
	case rz::SUBSCRIBE_STREAM_STATE_FROZEN://卡顿中
	{
		//ui.widget_video->show();
		break;
	}
		
	default:
		break;
	}
	update();
}

