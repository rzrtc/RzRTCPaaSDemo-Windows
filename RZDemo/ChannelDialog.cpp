#include "ChannelDialog.h"
#include <QDebug>
#include <QFileInfo>
#include "NoFocusDelegate.h"
#include "RzObject.h"
#include "MoreInfoDialog.h"
#include "Settings.h"

#include <QTimer>
#include "LostDialog.h"
#include <QElapsedTimer>

ChannelDialog::ChannelDialog(QWidget *parent)
	: QDialog(parent)
{
	
	this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
	ui.setupUi(this);
	UserStateSingleton::instance()->SetRecvAllAudioState(true);
	UserStateSingleton::instance()->SetRecvAllVideoState(true);
	UserStateSingleton::instance()->SetIsMirror(false);
	UserStateSingleton::instance()->SetIsMirror2(false);
	initStyleSheetFile();
	//QElapsedTimer tm;
	//tm.start();
	initTableWidget();
	initInfo();
	//int xx = tm.elapsed();
	initVideoItem();
	initConnect();
	initUserCount();
	//TODO test
	initMoreInfoDialog();
	
	UserStateSingleton::instance()->SetmChannelLoadedFlag(true);
	//qDebug() << "xxxtm = " << tm.elapsed();
}

ChannelDialog::~ChannelDialog()
{
	qDebug() << "ChannelDialog::~ChannelDialog release-----------";
	UserStateSingleton::instance()->SetmChannelLoadedFlag(false);
}

void ChannelDialog::SlotBack()
{
	if(RzObject::instance()->GetIRtcChannel())
		int ret = RzObject::instance()->GetIRtcChannel()->leaveChannel();
}

void ChannelDialog::SlotMin()
{
	this->showMinimized();
}

void ChannelDialog::SlotMax()
{
	static bool isMax = false;
	isMax = !isMax;
	if (isMax)
		this->showMaximized();
	else
		this->showNormal();
}

void ChannelDialog::SlotClose()
{
	/*RzObject::instance()->EnableLocalAudio(false);
	RzObject::instance()->EnableLocalVideo(false);
	CutomizedRenderMngr::instance()->erase(m_strUid.toStdString(), "first");
	CutomizedRenderMngr::instance()->erase(m_strUid.toStdString(), "second");*/
	if (RzObject::instance()->GetIRtcChannel())
		int ret = RzObject::instance()->GetIRtcChannel()->leaveChannel();
}


void ChannelDialog::SlotMoreInfo()
{
	m_pMoreInfoDialog->setFixedSize(this->width(), this->height());
	m_pMoreInfoDialog->setGeometry(this->x(), this->y(), m_pMoreInfoDialog->width(), m_pMoreInfoDialog->height());
	m_pMoreInfoDialog->show();
}


void ChannelDialog::SlotUserJoined(const QString& channelid, const QString& uid, int elapsed)
{
	/*
	* 是否多路流
	* 
	*/

	if (isVideoItemExist(uid) == false)
		addVideoItem(channelid, uid, "");

	if(ui.stackedWidget->currentIndex() == 0)
		ui.stackedWidget->setCurrentWidget(ui.page_videotable);
}

void ChannelDialog::SlotUserOffline(const QString& channelid, const QString& uid, USER_OFFLINE_REASON_TYPE reason)
{
	//CutomizedRenderMngr::instance()->erase(uid.toStdString(), "");
	//CutomizedRenderMngr::instance()->erase(uid.toStdString(), "first");
	//CutomizedRenderMngr::instance()->erase(uid.toStdString(), "second");
	delVideoItem(channelid, uid, "", true);
	delVideoItem(channelid, uid, "", true);
	int idx = -1;
	if (m_nVideoCount == 0)
	{
		if (RzObject::instance()->isAudience())
		{
			if (ui.stackedWidget->currentIndex() == 0)
				return;
			ui.stackedWidget->setCurrentWidget(ui.page_audience_waiting);
		}
	}
}

void ChannelDialog::SlotRejoinChannelSuccess(const QString& channelid, const QString& uid, int elapsed)
{
	if (channelid != m_strChannelId || uid != m_strUid)
		return;
	//UserStateSingleton::instance()->SetProcessData(QString::fromLocal8Bit("重新加入频道成功！"));
}

void ChannelDialog::SlotLeaveChannel(const QString& channelid, const RtcStats& stats)
{
	
	RzObject::instance()->EnableLocalAudio(false);
	RzObject::instance()->EnableLocalVideo(false);
	RzObject::instance()->Reset();
	//CutomizedRenderMngr::instance()->clear();
	UserStateSingleton::instance()->ClearRemoteUserList();
	UserStateSingleton::instance()->ClearRemoteUserVideoStreamMap();
	if (channelid != m_strChannelId)
		return;
	m_isIgnoreCloseEvent = false;
	QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigCloseSigtonDlg", Qt::DirectConnection);
	this->close();
	emit SigBack(this->x(), this->y());
}


void ChannelDialog::SlotConnectionLost(const QString& channelid)
{
	//收到该消息，弹出“已断开连接窗口”
	LostDialog dlg;
	dlg.SetText(QString::fromLocal8Bit("已断开连接"), QString::fromLocal8Bit("网络链接丢失"));
	connect(&dlg, SIGNAL(SigBack()), this, SLOT(SlotLossLeave()));
	dlg.setFixedSize(this->width(), this->height());
	dlg.setGeometry(this->x(), this->y(), dlg.width(), dlg.height());
	dlg.exec();
	//UserStateSingleton::instance()->SetProcessData(str);
}

void ChannelDialog::SlotConnectionStateChanged(const QString& channelid, CONNECTION_STATE_TYPE state, CONNECTION_CHANGED_REASON_TYPE reason)
{
	if (m_strChannelId != channelid)
		return;
	/*
	* TODO
	* CONNECTION_CHANGED_REJECTED_BY_SERVER 被踢
	*/
	switch (state)
	{
	case rz::CONNECTION_STATE_DISCONNECTED:
	{
		//UserStateSingleton::instance()->SetProcessData(QString::fromLocal8Bit("网络连接断开"));
		break;
	}
	case rz::CONNECTION_STATE_CONNECTING:
	{
		//UserStateSingleton::instance()->SetProcessData(QString::fromLocal8Bit("建立网络连接中"));
		break;
	}
	case rz::CONNECTION_STATE_CONNECTED:
	{
		//UserStateSingleton::instance()->SetProcessData(QString::fromLocal8Bit("网络已连接."));
		break;
	}
	case rz::CONNECTION_STATE_RECONNECTING:
	{
		//UserStateSingleton::instance()->SetProcessData(QString::fromLocal8Bit("重新建立网络连接中..."));
		break;
	}
	case rz::CONNECTION_STATE_FAILED:
	{
		QString str = QString::fromLocal8Bit("网络连接失败");
		switch (reason)
		{
		case rz::CONNECTION_CHANGED_CONNECTING:
		{
			str = QString::fromLocal8Bit("网络连接失败, 原因：建立网络连接中");
			break;
		}
			
		case rz::CONNECTION_CHANGED_JOIN_SUCCESS:
		{
			str = QString::fromLocal8Bit("网络连接失败, 原因：成功加入频道");
			break;
		}
		case rz::CONNECTION_CHANGED_INTERRUPTED:
		{
			str = QString::fromLocal8Bit("网络连接失败, 原因：网络连接中断");
			break;
		}
		case rz::CONNECTION_CHANGED_BANNED_BY_SERVER:
		{
			str = QString::fromLocal8Bit("网络连接失败, 原因：网络连接被服务器禁止");
			break;
		}
		case rz::CONNECTION_CHANGED_JOIN_FAILED:
		{
			str = QString::fromLocal8Bit("网络连接失败");
			LostDialog dlg;
			dlg.SetText(QString::fromLocal8Bit("已断开连接"), QString::fromLocal8Bit("网络连接丢失"));
			connect(&dlg, SIGNAL(SigBack()), this, SLOT(SlotLossLeave()));
			dlg.setFixedSize(this->width(), this->height());
			dlg.setGeometry(this->x(), this->y(), dlg.width(), dlg.height());
			dlg.exec();
			break;
		}
		case rz::CONNECTION_CHANGED_LEAVE_CHANNEL:
		{
			str = QString::fromLocal8Bit("网络连接失败, 原因：离开频道");
			break;
		}
		case rz::CONNECTION_CHANGED_INVALID_CHANNEL_ID:
		{
			str = QString::fromLocal8Bit("网络连接失败, 原因：不是有效的频道ID");
			break;
		}
		case rz::CONNECTION_CHANGED_REJECTED_BY_SERVER:
		{
			LostDialog dlg;
			QString str1 = QString::fromLocal8Bit("检测到您在其他设备登录，请返回登录后");
			QString str2 = QString::fromLocal8Bit("重试。");
			dlg.SetText(QString::fromLocal8Bit("已断开连接"), str1, str2);
			connect(&dlg, SIGNAL(SigBack()), this, SLOT(SlotLossLeave()));
			dlg.setFixedSize(this->width(), this->height());
			dlg.setGeometry(this->x(), this->y(), dlg.width(), dlg.height());
			dlg.exec();
			break;
		}
		case rz::CONNECTION_CHANGED_CLIENT_IP_ADDRESS_CHANGED:
		{
			str = QString::fromLocal8Bit("网络连接失败, 原因：客户端 IP 地址变更");
			break;
		}
		case rz::CONNECTION_CHANGED_KEEP_ALIVE_TIMEOUT:
		{
			str = QString::fromLocal8Bit("网络连接失败, 原因：SDK 和服务器连接保活超时");
			break;
		}
		case rz::CONNECTION_CHANGED_INVALID_APP_ID:
		{
			str = QString::fromLocal8Bit("网络连接失败, 原因：不是有效的 APP ID");
			LostDialog dlg;
			dlg.SetText(QString::fromLocal8Bit("已断开连接"), QString::fromLocal8Bit("APPID错误"));
			connect(&dlg, SIGNAL(SigBack()), this, SLOT(SlotLossLeave()));
			dlg.setFixedSize(this->width(), this->height());
			dlg.setGeometry(this->x(), this->y(), dlg.width(), dlg.height());
			dlg.exec();
			break;
		}
		default:
			break;
		}
		//UserStateSingleton::instance()->SetProcessData(str);
		break;
	}
	default:
		break;
	}
}

void ChannelDialog::SlotVideoSubscribeStateChanged(const QString& channelid, const QString& uid, const QString& streamName, SUBSCRIBE_STREAM_STATE state, int elapsed)
{
	/*
	* 第二路流下线，直接干掉对应的videoitem
	* 单流，不存在，消息通知出问题
	* 双流，第一路，流名称设置到默认item
	*/
	if (state == SUBSCRIBE_STREAM_STATE_OFFLINE && streamName == "second")
		delVideoItem(channelid, uid, streamName);
	else
	{
		VideoItem* wgt = getVideoItem(uid, streamName);
		if (wgt->GetType() != e_init)
			return;

		if (streamName == "second")
		{
			wgt = addVideoItem(m_strChannelId, uid, streamName);
			if (wgt->GetType() != e_init) {
				wgt->SetVideoCanvas();
				//流上线，设置渲染方式
				RzObject::instance()->SetRemoteRenderMode(uid, streamName);
			}
		}
		else
		{
			wgt = getVideoItem(uid, "");
			if (!wgt) return;
			if (wgt->GetType() == e_init) return;
			wgt->UpdateStreamName(streamName);
			wgt->SetVideoCanvas();
			//流上线，设置渲染方式
			RzObject::instance()->SetRemoteRenderMode(uid, streamName);

		}
		/*VideoCanvas canvas;
		canvas.view = (void*)wgt->GetWid();
		RzObject::instance()->SetupRemoteVideo(uid, streamName, canvas);*/
	}

	if (m_nVideoCount == 0)
	{
		if (RzObject::instance()->isAudience())
		{
			ui.stackedWidget->setCurrentWidget(ui.page_audience_waiting);
		}
	}
}

void ChannelDialog::SlotWarning(int warn, const QString& msg)
{

}
void ChannelDialog::SlotError(int err, const QString& msg)
{
	if (err >= 101 && err <= 107)
	{
		LostDialog dlg;
		dlg.SetText(QString::fromLocal8Bit("错误码：%1").arg(err), msg);
		connect(&dlg, SIGNAL(SigBack()), this, SLOT(SlotLossLeave()));
		dlg.setFixedSize(this->width(), this->height());
		dlg.setGeometry(this->x(), this->y(), dlg.width(), dlg.height());
		dlg.exec();
	}

	if (err >= 1501 && err <= 1512)
	{
		LostDialog dlg;
		dlg.SetButton(QString::fromLocal8Bit("确定"));
		dlg.SetText(QString::fromLocal8Bit("错误码：%1").arg(err), msg);
		connect(&dlg, SIGNAL(SigBack()), this, SLOT(SlotLossLeave()));
		dlg.SetIsClose(true);
		dlg.setFixedSize(this->width(), this->height());
		dlg.setGeometry(this->x(), this->y(), dlg.width(), dlg.height());
		dlg.exec();
	}
	
	qDebug() << "ChannelDialog::SlotError " << err << msg;
}

void ChannelDialog::SlotWarning(const QString& channelid, int warn, const QString& msg)
{
	if (warn >= 3041 && warn <= 3043)
	{
		LostDialog dlg;
		dlg.SetText(QString::fromLocal8Bit("错误码：%1").arg(warn), msg);
		dlg.SetButton(QString::fromLocal8Bit("确定"));
		dlg.SetIsClose(true);
		connect(&dlg, SIGNAL(SigBack()), this, SLOT(SlotLossLeave()));
		dlg.setFixedSize(this->width(), this->height());
		dlg.setGeometry(this->x(), this->y(), dlg.width(), dlg.height());
		dlg.exec();
	}
}

void ChannelDialog::SlotError(const QString& chnnelid, int err, const QString& msg)
{
	if (err >= 101 && err <= 107)
	{
		LostDialog dlg;
		dlg.SetText(QString::fromLocal8Bit("错误码：%1").arg(err), msg);
		connect(&dlg, SIGNAL(SigBack()), this, SLOT(SlotLossLeave()));
		dlg.setFixedSize(this->width(), this->height());
		dlg.setGeometry(this->x(), this->y(), dlg.width(), dlg.height());
		dlg.exec();
	}
}

void ChannelDialog::SlotRenderMode()
{
	std::map<STREAMINFO, SUBSCRIBE_STREAM_STATE> remoteUserVideoStreamMap = UserStateSingleton::instance()->GetRemoteUserVideoStreamMap();
	RzObject::instance()->SetLocalRenderMode();
	for (auto iter = remoteUserVideoStreamMap.begin(); iter != remoteUserVideoStreamMap.end(); iter++)
	{
		if (iter->second == SUBSCRIBE_STREAM_STATE_OFFLINE)
			continue;
		
		
		RzObject::instance()->SetRemoteRenderMode(QString::fromStdString(iter->first.uid), QString::fromStdString(iter->first.streamName));
	}
}

void ChannelDialog::SlotTimer()
{
	LostDialog dlg;
	dlg.setFixedSize(500, 400);
	dlg.setGeometry(this->x(), this->y(), dlg.width(), dlg.height());
	dlg.exec();
}

void ChannelDialog::SlotLossLeave()
{
	if (RzObject::instance()->GetIRtcChannel() == nullptr)
		return;
	int ret = RzObject::instance()->GetIRtcChannel()->leaveChannel();
	RzObject::instance()->EnableLocalAudio(false);
	RzObject::instance()->EnableLocalVideo(false);
	//CutomizedRenderMngr::instance()->clear();
	UserStateSingleton::instance()->Clear();
	m_isIgnoreCloseEvent = false;
	this->close();
	emit SigBack(this->x(), this->y());
}

void ChannelDialog::SlotRtcStats(QString channelid, const RtcStats& stats)
{
	if (m_strChannelId != channelid) {
		qDebug() << "UserInfoDialog::SlotRtcStats";
		return;
	}

	QString str = QString::fromLocal8Bit("当前在线：%1人").arg(stats.userCount);
	//ui.label_onlineusers->setText(str);
	qDebug() << "ChannelDialog::SlotRtcStats count = " << stats.userCount;
}
void ChannelDialog::SlotDelItem()
{
	/*qDebug() << m_nVideoCount;
	QString str = "0_0";
	delVideoItem(str, str, str);
	str = "0_1";
	delVideoItem(str, str, str);
	str = "0_2";
	delVideoItem(str, str, str);
	str = "0_3";
	delVideoItem(str, str, str);

	str = "1_0";
	delVideoItem(str, str, str);
	str = "1_1";
	delVideoItem(str, str, str);
	str = "1_2";
	delVideoItem(str, str, str);
	str = "1_3";
	delVideoItem(str, str, str);*/

	
	VideoItem* wgt = addVideoItem(m_strChannelId, m_strUid, "");
	if (wgt && wgt->GetType() != e_init)
	{
		VideoCanvas canvas;

		canvas.view =(void*) wgt->GetWid();
		RzObject::instance()->SetupLocalVideo(canvas);
	}
}

void ChannelDialog::changeEvent(QEvent* event)
{
	/*if (event->type() != QEvent::WindowStateChange) return;
	
	if (this->windowState() == Qt::WindowMinimized)
	{
		bool ret = QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigVideoItemUpdate", Q_ARG(bool, false));
	}
	else
	{
		bool ret = QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigVideoItemUpdate", Q_ARG(bool, true));
	}*/
	
}

void ChannelDialog::showEvent(QShowEvent *event)
{
	this->setAttribute(Qt::WA_Mapped);
	QWidget::showEvent(event);
}

void ChannelDialog::closeEvent(QCloseEvent* event)
{
	if (m_isIgnoreCloseEvent)
		event->ignore();
}

void ChannelDialog::initStyleSheetFile()
{
	QFile styleFile("./resource/channel.qss");
	QFileInfo fileInfo("./resource/channel.qss");
	//QString str = fileInfo.absolutePath();

	styleFile.open(QIODevice::ReadOnly);
	this->setStyleSheet(QString(styleFile.readAll()));
}

void ChannelDialog::initTableWidget()
{
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	ui.tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

	ui.tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui.tableWidget->setFrameShape(QTableWidget::NoFrame);
	ui.tableWidget->verticalHeader()->setHidden(true);
	ui.tableWidget->horizontalHeader()->setHidden(true);
	ui.tableWidget->setColumnCount(4);
	ui.tableWidget->setRowCount(5);
	ui.tableWidget->setItemDelegate(new NoFocusDelegate());
	ui.tableWidget->setStyleSheet("selection-background-color:#929baa;background-color:#929baa;");
	for (int i = 0; i < ui.tableWidget->columnCount(); i++)
	{
		ui.tableWidget->setColumnWidth(i, 300);
	}

	for (int i = 0; i < ui.tableWidget->rowCount(); i++)
	{
		ui.tableWidget->setRowHeight(i, 224);
	}

	for (int i = 0; i < ui.tableWidget->rowCount(); i++)
	{
		for (int j = 0; j < ui.tableWidget->columnCount(); j++)
		{
			VideoItem* w = new VideoItem(ui.tableWidget);
			w->setFixedSize(300, 224);
			
			ui.tableWidget->setCellWidget(i, j, w);
		}
	}

	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void ChannelDialog::initConnect()
{
	connect(ui.pushButton_min, SIGNAL(clicked()), this, SLOT(SlotMin()));
	connect(ui.pushButton_max, SIGNAL(clicked()), this, SLOT(SlotMax()));
	connect(ui.pushButton_quit, SIGNAL(clicked()), this, SLOT(SlotClose()));
	connect(ui.pushButton_more, SIGNAL(clicked()), this, SLOT(SlotMoreInfo()));

	connect(UserStateNotifySingleton::instance(), SIGNAL(SigUserJoined(const QString&, const QString&, int)), this, SLOT(SlotUserJoined(const QString&, const QString&, int)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigUserOffline(const QString&, const QString&, USER_OFFLINE_REASON_TYPE)), this, SLOT(SlotUserOffline(const QString&, const QString&, USER_OFFLINE_REASON_TYPE)));

	connect(UserStateNotifySingleton::instance(), SIGNAL(SigWarning(int, const QString&)), this, SLOT(SlotWarning(int, const QString&)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigError(int, const QString&)), this, SLOT(SlotError(int, const QString&)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigWarning(const QString&, int, const QString&)), this, SLOT(SlotWarning(const QString&, int, const QString&)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigError(const QString&, int, const QString&)), this, SLOT(SlotError(const QString&, int, const QString&)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigRejoinChannelSuccess(const QString&, const QString&, int)), this, SLOT(SlotRejoinChannelSuccess(const QString&, const QString&, int)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigLeaveChannel(const QString&, const RtcStats&)), this, SLOT(SlotLeaveChannel(const QString&, const RtcStats&)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigClientRoleChanged(const QString&, CLIENT_ROLE_TYPE, CLIENT_ROLE_TYPE, int)), this, SLOT(SlotClientRoleChanged(const QString&, CLIENT_ROLE_TYPE, CLIENT_ROLE_TYPE, int)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigConnectionLost(const QString&)), this, SLOT(SlotConnectionLost(const QString&)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigConnectionStateChanged(const QString&, CONNECTION_STATE_TYPE, CONNECTION_CHANGED_REASON_TYPE)), this, SLOT(SlotConnectionStateChanged(const QString&, CONNECTION_STATE_TYPE, CONNECTION_CHANGED_REASON_TYPE)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigVideoSubscribeStateChanged(const QString&, const QString&, const QString&, SUBSCRIBE_STREAM_STATE, int)), this, SLOT(SlotVideoSubscribeStateChanged(const QString&, const QString&, const QString&, SUBSCRIBE_STREAM_STATE, int)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigRtcStats(QString, const RtcStats&)), this, SLOT(SlotRtcStats(QString, const RtcStats&)));
}

void ChannelDialog::initInfo()
{
	QVariant value;
	Settings::instance()->Read("channel", value, "Info");
	ui.label_channelid->setText(QString::fromLocal8Bit("频道ID：") + value.toString());
	m_strChannelId = value.toString();
	Settings::instance()->Read("user", value, "Info");
	m_strUid = value.toString();
}

void ChannelDialog::initVideoItem()
{
	/*
	* 判断是否自定义推流
	* 是，创建两个框
	* 不是，创建一个框
		读取用户列表和视频流列表
		如何排列添加
		用户上线，分配框
		流上线，streamName不是second设置上，是second重新分配框


	*/
	QVariant value;
	Settings::instance()->Read("enablestream2", value, "SelfStreamSwitch");
	bool isSelfPub = value.toBool();
	Settings::instance()->Read("scene", value, "SceneMode");
	if (value.toString() != QString::fromLocal8Bit("直播观众"))
	{
		if (isSelfPub)
		{
			//TODO
			addVideoItem(m_strChannelId, m_strUid, "first");
			addVideoItem(m_strChannelId, m_strUid, "second");
		}
		else
		{
			/*VideoItem* wgt = */addVideoItem(m_strChannelId, m_strUid, "");
			/*VideoCanvas canvas;
			canvas.view = (void*)wgt->GetWid();
			RzObject::instance()->SetupLocalVideo(canvas);*/
		}
	}
	
	Settings::instance()->Read("channel", value, "Info");
	std::set<std::string> userlist = UserStateSingleton::instance()->GetRemoteUserList();
	std::map<STREAMINFO, SUBSCRIBE_STREAM_STATE> remoteUserVideoStreamMap = UserStateSingleton::instance()->GetRemoteUserVideoStreamMap();
	
	for (auto iter = userlist.begin(); iter != userlist.end(); iter++)
	{
		QString str = QString::fromStdString(*iter);
		addVideoItem(value.toString(), str, "");
	}

	for (auto iter = remoteUserVideoStreamMap.begin(); iter != remoteUserVideoStreamMap.end(); iter++)
	{
		if (iter->second == SUBSCRIBE_STREAM_STATE_OFFLINE)
			continue;
		QString uid = QString::fromStdString(iter->first.uid);
		QString streamName = QString::fromStdString(iter->first.streamName);
		if (streamName == "second")
		{
			VideoItem* wgt = addVideoItem(value.toString(), uid, streamName);
			if (wgt->GetType() == e_remote_user)
			{
				wgt->UpdateStreamName(streamName);
				wgt->SetVideoCanvas();
			}
		}
		else if(streamName == "first")
		{
			VideoItem* wgt = getVideoItem(uid, "");
			if (wgt && streamName.isEmpty() == false && wgt->GetType() == e_remote_user)
			{
				wgt->UpdateStreamName(streamName);
				wgt->SetVideoCanvas();
			}
		}
	}

	if (userlist.size() == 0 && remoteUserVideoStreamMap.size() == 0)
	{
		if (RzObject::instance()->isAudience())
		{
			ui.stackedWidget->setCurrentWidget(ui.page_audience_waiting);
		}
	}
	//TEST getVideoItem
	/*for (int i = 0; i < ui.tableWidget->rowCount(); i++)
	{
		for (int j = 0; j < ui.tableWidget->columnCount(); j++)
		{
			QString str = QString("%1_%2").arg(i).arg(j);
			addVideoItem(str, str, str);
		}
	}*/
}

void ChannelDialog::initUserCount()
{
	RtcStats stats;
	UserStateSingleton::instance()->GetRtcStats(m_strChannelId.toStdString(), stats);
	QString str = QString::fromLocal8Bit("当前在线：%1人").arg(stats.userCount == 0 ? 1 : stats.userCount);
	//ui.label_onlineusers->setText(str);
}


void ChannelDialog::initMoreInfoDialog()
{
	m_pMoreInfoDialog = new MoreInfoDialog(this);
	connect(m_pMoreInfoDialog, SIGNAL(SigRenderMode()), this, SLOT(SlotRenderMode()));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigCloseSigtonDlg()), m_pMoreInfoDialog, SLOT(SlotClose()), Qt::DirectConnection);
}

VideoItem* ChannelDialog::addVideoItem(QString channelid, QString uid, QString streamName)
{
	
	E_VIDEO_ITEM_TYPE type = e_remote_user;
	if (uid == m_strUid)
		type = e_local_user;
	bool flag = false;
	VideoItem* wgt = nullptr;
	for (int i = 0; i < ui.tableWidget->rowCount(); i++)
	{
		for (int j = 0; j < ui.tableWidget->columnCount(); j++)
		{
			
			wgt = (VideoItem*)ui.tableWidget->cellWidget(i, j);
			if (wgt && wgt->GetType() == e_init) {
				wgt->SetUid(uid);
				wgt->SetStreamName(streamName);
				wgt->Init(type, i, j);
				flag = true;
				break;
			}
		}
		if (flag)
			break;
	}

	m_nVideoCount++;

	
	return wgt;
}

void ChannelDialog::delVideoItem(QString channelid, QString uid, QString streamName, bool isAll)
{
	/*
		问题：
		1、local不能被删除
		2、删除最后一个
	*/
	/*
		1、del 指定widget
		2、后面的widget前移
		3、处理性能问题，不重新创建widget，只移动数据，重新设置canvas
		获取最后一个内容
	*/

	/*if (uid == m_strUid)
		return;*/

	VideoItem* pVideoItem = nullptr;
	bool flag = false;

	for (int i = 0; i < ui.tableWidget->rowCount(); i++)
	{
		for (int j = 0; j < ui.tableWidget->columnCount(); j++)
		{
			pVideoItem = qobject_cast<VideoItem*>(ui.tableWidget->cellWidget(i, j));
			if (!pVideoItem) continue;
			QString name = pVideoItem->metaObject()->className();
			if (name != "VideoItem")	continue;

			bool bCondition = pVideoItem->GetUid() == uid && pVideoItem->GetStreamName() == streamName;
			if (isAll)
				bCondition = pVideoItem->GetUid() == uid;
			if (bCondition)
			{
				//pVideoItem->SetChannelId(strChannelId);
				//pVideoItem->SetUid(strUid);
				//pVideoItem->SetStreamName(strStreamName);
				//pVideoItem->SetType(type);
				//pVideoItem->ReInit();
				//pVideoItem->show();
				E_VIDEO_ITEM_TYPE type;
				QString strChannelId, strUid, strStreamName;
				bool isMain = getLastItemInfo(type, strChannelId, strUid, strStreamName);
				if (isAll ? (strUid != uid) : (strUid != uid || streamName != strStreamName))
				{
					pVideoItem = (VideoItem*)ui.tableWidget->cellWidget(i, j);
					pVideoItem->SetShowMainState(isMain);
					pVideoItem->SetUid(strUid);
					pVideoItem->SetStreamName(strStreamName);
					pVideoItem->Init(type, i, j);
				}
				flag = true;
				break;
			}
		}
		if (flag)
			break;
	}

	
}

void ChannelDialog::getItemIndex(int oldrow, int oldcolum, int& row, int& column)
{
	if (oldcolum == 0)
	{
		column = ui.tableWidget->columnCount() - 1;
		row = oldrow - 1;
	}
	else
	{
		column = oldcolum - 1;
		row = oldrow;
	}
}

VideoItem* ChannelDialog::getVideoItem(int row, int column)
{
	return nullptr;
}

bool ChannelDialog::getLastItemInfo(E_VIDEO_ITEM_TYPE& type, QString& channelid, QString& uid, QString& streamName)
{
	VideoItem* pVideoItem = nullptr;
	int nColumnCount = ui.tableWidget->columnCount();
	int lastRow = (m_nVideoCount - 1) / nColumnCount;
	int lastColum = (m_nVideoCount - 1) % nColumnCount;
	pVideoItem = qobject_cast<VideoItem*>(ui.tableWidget->cellWidget(lastRow, lastColum));
	if (!pVideoItem) return false;
	QString name = pVideoItem->metaObject()->className();
	if (name != "VideoItem") return false;
	type = pVideoItem->GetType();
	channelid = pVideoItem->GetChannelId();
	uid = pVideoItem->GetUid();
	streamName = pVideoItem->GetStreamName();
	bool isMain = pVideoItem->GetShowMainState();
	pVideoItem->Reset();
	m_nVideoCount--;
	return isMain;
}

VideoItem* ChannelDialog::getVideoItem(QString uid, QString streamName)
{
	VideoItem* pVideoItem = nullptr;
	bool flag = false;
	for (int i = 0; i < ui.tableWidget->rowCount(); i++)
	{
		for (int j = 0; j < ui.tableWidget->columnCount(); j++)
		{
			QWidget* wgt = ui.tableWidget->cellWidget(i, j);
			pVideoItem = qobject_cast<VideoItem*>(ui.tableWidget->cellWidget(i, j));
			if (!pVideoItem) continue;
			QString name = pVideoItem->metaObject()->className();
			if (name != "VideoItem")	continue;
			if (pVideoItem->GetUid() == uid && pVideoItem->GetStreamName() == streamName)
			{
				flag = true;
				break;
			}
		}
		if (flag)
			break;
	}

	return pVideoItem;
}

bool ChannelDialog::isVideoItemExist(QString uid)
{
	VideoItem* pVideoItem = nullptr;
	bool flag = false;
	for (int i = 0; i < ui.tableWidget->rowCount(); i++)
	{
		for (int j = 0; j < ui.tableWidget->columnCount(); j++)
		{
			QWidget* wgt = ui.tableWidget->cellWidget(i, j);
			pVideoItem = qobject_cast<VideoItem*>(ui.tableWidget->cellWidget(i, j));
			if (!pVideoItem) continue;
			QString name = pVideoItem->metaObject()->className();
			if (name != "VideoItem")	continue;
			if (pVideoItem->GetUid() == uid)
			{
				flag = true;
				break;
			}
		}
		if (flag)
			break;
	}

	return flag;
}

void ChannelDialog::moveItem()
{
	//首先判断需要获取几个item,保存参数，移动到尾部
	//
	QVariant value;
	Settings::instance()->Read("enablestream2", value, "SelfStreamSwitch");
	
	QString uid1, uid2;
	QString stream1, stream2;
	E_VIDEO_ITEM_TYPE type1, type2;
	int count = 0;
	int flag = -1;
	VideoItem* wgt = nullptr;
	for (int i = 0; i < ui.tableWidget->rowCount(); i++)
	{
		for (int j = 0; j < ui.tableWidget->columnCount(); j++)
		{
			VideoItem *pVideoItem = qobject_cast<VideoItem*>(ui.tableWidget->cellWidget(i, j));
			if (!pVideoItem) continue;
			QString name = pVideoItem->metaObject()->className();
			if (name != "VideoItem")	continue;
			if (pVideoItem->GetType() == e_local_user)
				return;
			if (count == 0)
			{
				uid1 = pVideoItem->GetUid();
				stream1 = pVideoItem->GetStreamName();
				type1 = pVideoItem->GetType();
				if (value.toBool() == false) {
					flag = 0;
					break;
				}
			}

			if (count == 1)
			{
				uid2 = pVideoItem->GetUid();
				stream2 = pVideoItem->GetStreamName();
				type2 = pVideoItem->GetType();
				if (value.toBool() == false) {
					flag = 0;
					break;
				}
			}

			count++;
		}
		if (flag == 0)
			break;
	}

	if (uid2.isEmpty()==false)	//移动两个
	{
		if (value.toBool())	//自定义
		{
			wgt = (VideoItem*)ui.tableWidget->cellWidget(0, 0);
			wgt->Reset();
			wgt = (VideoItem*)ui.tableWidget->cellWidget(0, 1);
			wgt->Reset();
			//ui.tableWidget->setCellWidget(0, 0, nullptr);
			//ui.tableWidget->setCellWidget(0, 1, nullptr);
			moveVideoItem(2, type1, m_strChannelId, uid1, stream1);
			moveVideoItem(2, type2, m_strChannelId, uid2, stream2);
			addVideoItem(m_strChannelId, m_strUid, "first");
			addVideoItem(m_strChannelId, m_strUid, "second");
		}
		else//默认
		{
			wgt = (VideoItem*)ui.tableWidget->cellWidget(0, 0);
			wgt->Reset();
			moveVideoItem(1, type1, m_strChannelId, uid1, stream1);
			addVideoItem(m_strChannelId, m_strUid, "");
		}
	}
	else if (uid1.isEmpty() == false)
	{
		if (value.toBool())	//自定义
		{
			wgt = (VideoItem*)ui.tableWidget->cellWidget(0, 0);
			wgt->Reset();
			moveVideoItem(2, type1, m_strChannelId, uid1, stream1);
			addVideoItem(m_strChannelId, m_strUid, "first");
			addVideoItem(m_strChannelId, m_strUid, "second");
		}
		else//默认
		{
			wgt = (VideoItem*)ui.tableWidget->cellWidget(0, 0);
			wgt->Reset();
			moveVideoItem(1, type1, m_strChannelId, uid1, stream1);
			addVideoItem(m_strChannelId, m_strUid, "");
		}
	}
	else
	{
		if (value.toBool())	//自定义
		{
			
			addVideoItem(m_strChannelId, m_strUid, "first");
			addVideoItem(m_strChannelId, m_strUid, "second");
		}
		else//默认
		{
		
			addVideoItem(m_strChannelId, m_strUid, "");
		}
	}
}

VideoItem* ChannelDialog::moveVideoItem(int idx, E_VIDEO_ITEM_TYPE type, QString channelid, QString uid, QString streamName)
{
	/*
	* 本端

	*/
	VideoItem* wgt = nullptr;
	bool flag = false;
	int j = idx;
	for (int i = 0; i < ui.tableWidget->rowCount(); i++)
	{
		if (i > 0)
			j = 0;
		for (; j < ui.tableWidget->columnCount(); j++)
		{
			VideoItem* wgt = (VideoItem*)ui.tableWidget->cellWidget(i, j);
			if (wgt->GetType() == e_init) {
			
				wgt->SetUid(uid);
				wgt->SetStreamName(streamName);
				wgt->Init(type, i, j);
				flag = true;
				break;
			}
		}
		if (flag)
			break;
	}

	//m_nVideoCount++;

	return wgt;
}

void ChannelDialog::showTipDialog(int error, QString msg)
{
	LostDialog dlg;
	dlg.SetText(msg, QString::fromLocal8Bit("错误码：%1").arg(error));
	dlg.SetButton(QString::fromLocal8Bit("确定"));
	connect(&dlg, SIGNAL(SigBack()), this, SLOT(SlotLossLeave()));
	dlg.setFixedSize(this->width(), this->height());
	dlg.setGeometry(this->x(), this->y(), dlg.width(), dlg.height());
	dlg.exec();
}