#include "PaasTest.h"
#include <QDebug>
#include <QFileInfo>
#include <QFile>
#include <QTimer>
#include "ChannelDialog.h"
#include "Settings.h"
#include "UserStateSingleton.h"
#include "RzObject.h"
#include "LoadingDialog.h"
#include <QMessageBox>
#include "LostDialog.h"
PaasTest::PaasTest(QWidget *parent)
    : QDialog(parent)
{
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    ui.setupUi(this);
	init();
}

void PaasTest::SlotMin()
{
	this->showMinimized();
}

void PaasTest::SlotClose()
{
	RzObject::instance()->Release();
	this->close();
}

void PaasTest::SlotReEnter(int x, int y)
{
	//TODO::重新初始化sdk 
	initSdk();
	initStyleSheetFile();
	initUserInfo();
	this->show();
}

void PaasTest::SlotLoading()
{
	if (judgeInfo() == false)
		return;

	this->hide();

	int ret = RzObject::instance()->JoinChannel();
	if (ret != 0)
	{
		this->show();
		if(ret == -2)
			showTipDialog(ret, "");
		return;
	}
		
	QVariant value;
	Settings::instance()->Read("scene", value, "SceneMode");
	if (value.toString() != QString::fromLocal8Bit("直播观众"))
	{
		Settings::instance()->Read("mic", value, "Device");
		bool isMicOpen = value.toString() == QString::fromLocal8Bit("关闭麦克风") ? false : true;
		RzObject::instance()->EnableLocalAudio(isMicOpen);
		Settings::instance()->Read("cam", value, "Device");
		bool isCamOpen = value.toString() == QString::fromLocal8Bit("关闭摄像头") ? false : true;
		RzObject::instance()->EnableLocalVideo(isCamOpen);
	}

	//LoadingDialog dlg;
	//connect(this, SIGNAL(SigCloseLoading()), &dlg, SLOT(SlotClose()));
	//connect(UserStateNotifySingleton::instance(), SIGNAL(SigCloseSigtonDlg()), &dlg, SLOT(SlotClose()));
	//dlg.exec();
}

void PaasTest::SlotJoinChannel()
{
	this->hide();
	ChannelDialog dlg;
	connect(&dlg, SIGNAL(SigBack(int, int)), this, SLOT(SlotReEnter(int, int)));
	dlg.exec();
}

void PaasTest::SlotLossLeave()
{
	if (RzObject::instance()->GetIRtcChannel() == nullptr)
		return;
	int ret = RzObject::instance()->GetIRtcChannel()->leaveChannel();
	RzObject::instance()->EnableLocalAudio(false);
	RzObject::instance()->EnableLocalVideo(false);
	UserStateSingleton::instance()->Clear();
}

void PaasTest::SlotWarning(int warn, const QString& msg)
{

}

void PaasTest::SlotError(int err, const QString& msg)
{
	LostDialog dlg;
	connect(&dlg, SIGNAL(SigBack()), this, SLOT(SlotLossLeave()));
	dlg.setFixedSize(this->width(), this->height());
	dlg.setGeometry(this->x(), this->y(), dlg.width(), dlg.height());
	dlg.SetButton(QString::fromLocal8Bit("确定"));
	switch (err)
	{
	case 101:
	{
		//dlg.SetText(QString::fromLocal8Bit("进入频道失败"), QString::fromLocal8Bit("当前频道 ID 不合法"));
		//dlg.exec();
		break;
	}
	case 102:
	{
		dlg.SetText(QString::fromLocal8Bit("进入频道失败"), QString::fromLocal8Bit("当前 Appid 不合法"));
		dlg.exec();
		break;
	}
	case 103:
	{
		dlg.SetText(QString::fromLocal8Bit("进入频道失败"), QString::fromLocal8Bit("当前频道 ID 不合法"));
		dlg.exec();
		break;
	}
	case 104:
	{
		dlg.SetText(QString::fromLocal8Bit("进入频道失败"), QString::fromLocal8Bit("获取服务器资源失败\n 错误码：104"));
		dlg.exec();
		break;
	}
	case 105:
	{
		dlg.SetText(QString::fromLocal8Bit("错误码：%1").arg(err), msg);
		dlg.exec();
		break;
	}
	case 106:
	{
		dlg.SetText(QString::fromLocal8Bit("错误码：%1").arg(err), msg);
		dlg.exec();
		break;
	}
	case 107:
	{
		dlg.SetText(QString::fromLocal8Bit("错误码：%1").arg(err), msg);
		dlg.exec();
		break;
	}
	default:
		break;
	}
	
	qDebug() << "PaasTest::SlotError " << msg;
}

void PaasTest::SlotError(const QString& channelid, int err, const QString& msg)
{
	this->show();
	LostDialog dlg;
	connect(&dlg, SIGNAL(SigBack()), this, SLOT(SlotLossLeave()));
	dlg.setFixedSize(this->width(), this->height());
	dlg.setGeometry(this->x(), this->y(), dlg.width(), dlg.height());
	dlg.SetButton(QString::fromLocal8Bit("确定"));
	switch (err)
	{
	case 104:
	case 106:
	case 107:
	{
		dlg.SetText(QString::fromLocal8Bit("进入频道失败"), QString::fromLocal8Bit("获取服务器资源失败\n 错误码：104"));
		dlg.exec();
		break;
	}
	
	default:
		break;
	}
}

void PaasTest::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_leftButtonPressed = true;
		m_start = event->globalPos();
		m_clickPos = event->pos();
	}
	//qDebug() << "this->width = " << this->width() << "  this->height = " << this->height();
}

void PaasTest::SlotJoinChannelSuccess(const QString& channelid, const QString& uid, int elapsed)
{
	QVariant value;
	Settings::instance()->Read("channel", value, "Info");
	if (channelid != value.toString()) return;
	Settings::instance()->Read("user", value, "Info");
	if (uid != value.toString()) return;

	//this->hide();
	emit SigCloseLoading();

	//UserStateSingleton::instance()->SetProcessData(QString::fromLocal8Bit("加入频道成功！"));

	RzObject::instance()->Publish();
	ChannelDialog dlg;
	connect(&dlg, SIGNAL(SigBack(int, int)), this, SLOT(SlotReEnter(int, int)));
	dlg.exec();
}

void PaasTest::SlotConnectionStateChanged(const QString& channelid, CONNECTION_STATE_TYPE state, CONNECTION_CHANGED_REASON_TYPE reason)
{
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
			str = QString::fromLocal8Bit("网络连接失败, 原因：网络连接已超时");
			LostDialog dlg;
			dlg.SetText(QString::fromLocal8Bit("已断开连接"), QString::fromLocal8Bit("网络连接已超时，请返回登录后重试。"));
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

void PaasTest::mouseMoveEvent(QMouseEvent* event)
{
	if (m_leftButtonPressed)
	{
		move(event->globalPos() - m_clickPos);
		m_start = event->globalPos();
	}
	//qDebug() << "this->width = " << this->width() << "  this->height = " << this->height();
}

void PaasTest::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_leftButtonPressed = false;
	}
	
	//qDebug() << "this->width = " << this->width() << "  this->height = " << this->height();
}

void PaasTest::closeEvent(QCloseEvent* event)
{
	if (this->isHidden())
		return;
	else
	{
		RzObject::instance()->Release();
		this->close();
	}
}

void PaasTest::init()
{
	UserStateNotifySingleton::instance();
	UserStateSingleton::instance();
	initStyleSheetFile();
	initUserInfo();
	initConnect();
	initSdk();
	RzObject::instance()->StartLastmileProbeTest();
	QTimer::singleShot(10*1000, this, SLOT(SlotTimer()));
}


void PaasTest::initStyleSheetFile()
{
	QFile styleFile("./resource/login.qss");
	QFileInfo fileInfo("./resource/login.qss");
	//QString str = fileInfo.absolutePath();

	styleFile.open(QIODevice::ReadOnly);
	this->setStyleSheet(QString(styleFile.readAll()));
}

void PaasTest::initUserInfo()
{
	ui.label_channelid_tip->hide();
	ui.label_uid_tip->hide();
	QVariant value;
	Settings::instance()->Read("channel", value, "Info");
	if (value.toString().isEmpty() == false)
		ui.lineEdit_channelid->setText(value.toString());
	value.clear();
	Settings::instance()->Read("user", value, "Info");
	if (value.toString().isEmpty() == false)
		ui.lineEdit_uid->setText(value.toString());
}

void PaasTest::initConnect()
{
	connect(ui.pushButton_join, SIGNAL(clicked()), this, SLOT(SlotLoading()));
	connect(ui.pushButton_min, SIGNAL(clicked()), this, SLOT(SlotMin()));
	connect(ui.pushButton_close, SIGNAL(clicked()), this, SLOT(SlotClose()));
	
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigJoinChannelSuccess(const QString&, const QString&, int)), this, SLOT(SlotJoinChannelSuccess(const QString&, const QString&, int)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigWarning(int, const QString&)), this, SLOT(SlotWarning(int, const QString&)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigError(int, const QString&)), this, SLOT(SlotError(int, const QString&)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigError(const QString&, int, const QString&)), this, SLOT(SlotError(const QString&, int, const QString&)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigConnectionStateChanged(const QString&, CONNECTION_STATE_TYPE, CONNECTION_CHANGED_REASON_TYPE)), this, SLOT(SlotConnectionStateChanged(const QString&, CONNECTION_STATE_TYPE, CONNECTION_CHANGED_REASON_TYPE)));
}

void PaasTest::initSdk()
{
	RzObject::instance()->init();
	RzObject::instance()->GetIRtcEngine();
}

bool PaasTest::judgeInfo()
{
	//判断频道id， 用户id是否为空
	if (ui.lineEdit_channelid->text().isEmpty())
		ui.label_channelid_tip->show();

	if (ui.lineEdit_uid->text().isEmpty())
		ui.label_uid_tip->show();

	if (ui.lineEdit_channelid->text().isEmpty() || ui.label_uid_tip->text().isEmpty())
		return false;

	Settings::instance()->Write("channel", ui.lineEdit_channelid->text(), "Info");
	Settings::instance()->Write("user", ui.lineEdit_uid->text(), "Info");
	return true;
}

void PaasTest::showTipDialog(int error, QString msg)
{
	if (error = -2)
	{
		LostDialog dlg;
		dlg.SetText(QString::fromLocal8Bit("进入频道失败"), QString::fromLocal8Bit("当前用户 ID 不合法"));
		dlg.SetButton(QString::fromLocal8Bit("确定"));
		connect(&dlg, SIGNAL(SigBack()), this, SLOT(SlotLossLeave()));
		dlg.setFixedSize(this->width(), this->height());
		dlg.setGeometry(this->x(), this->y(), dlg.width(), dlg.height());
		dlg.exec();
	}
	else
	{
		LostDialog dlg;
		dlg.SetText(msg, QString::fromLocal8Bit("错误码：%1").arg(error));
		dlg.SetButton(QString::fromLocal8Bit("确定"));
		connect(&dlg, SIGNAL(SigBack()), this, SLOT(SlotLossLeave()));
		dlg.setFixedSize(this->width(), this->height());
		dlg.setGeometry(this->x(), this->y(), dlg.width(), dlg.height());
		dlg.exec();
	}
}
