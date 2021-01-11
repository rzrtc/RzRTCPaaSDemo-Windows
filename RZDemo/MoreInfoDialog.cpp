#include "MoreInfoDialog.h"
#include <QComboBox>
#include <QDesktopServices>
#include <QDir>
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QFileInfo>
#include <QVBoxLayout>
#include "SelectList.h"
#include "ListWidgetSet.h"
#include "SwitchButton.h"
#include "Settings.h"
#include "RzObject.h"
#include "RemoteUserInfoWidgets.h"

MoreInfoDialog::MoreInfoDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::Tool);
	setAttribute(Qt::WA_TranslucentBackground, true);
	init();

	QVariant value;
	Settings::instance()->Read("scene", value, "SceneMode");
	if (value.toString() == QString::fromLocal8Bit("直播观众"))
		ui.tabWidget->setTabEnabled(1, false);
	//TODO
	ui.textEdit->document()->setMaximumBlockCount(200);
}

MoreInfoDialog::~MoreInfoDialog()
{
}

void MoreInfoDialog::SlotClose()
{
	this->close();
}


void MoreInfoDialog::SlotSelectCam()
{
	SelectList dlg(e_select_device_cam, this);
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigCloseSigtonDlg()), &dlg, SLOT(SlotClose()));
	QPoint lpot;
	QPoint gpot = ui.listWidget_stream_set_device->mapToGlobal(ui.listWidget_stream_set_device->pos() + QPoint(50, -38));
	dlg.setGeometry(gpot.x(), gpot.y(), dlg.width(), dlg.height());
	dlg.exec();

	//TODO sdk
	QPushButton* p = qobject_cast<QPushButton*>(sender());
	if (!p)
		return;

	QVariant value;
	Settings::instance()->Read("cam", value, "Device");
	if (value.toString() != QString::fromLocal8Bit("关闭摄像头")) {
		RzObject::instance()->SetCamDevice(value.toString());
		RzObject::instance()->EnableLocalVideo(true);
		QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigSwitchCam", Qt::DirectConnection);
	}
	else
		RzObject::instance()->EnableLocalVideo(false);
	p->setText(value.toString());
}

void MoreInfoDialog::SlotSelectMic()
{
	SelectList dlg(e_select_device_mic);
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigCloseSigtonDlg()), &dlg, SLOT(SlotClose()));
	QPoint lpot;
	QPoint gpot = ui.listWidget_stream_set_device->mapToGlobal(ui.listWidget_stream_set_device->pos() + QPoint(50, 0));
	dlg.setGeometry(gpot.x(), gpot.y(), dlg.width(), dlg.height());
	dlg.exec();

	//TODO sdk
	QPushButton* p = qobject_cast<QPushButton*>(sender());
	if (!p)
		return;

	QVariant value;
	Settings::instance()->Read("mic", value, "Device");
	if (value.toString() != QString::fromLocal8Bit("关闭麦克风")) {
		RzObject::instance()->SetMicDevice(value.toString());
		RzObject::instance()->EnableLocalAudio(true);
	}
	else
		RzObject::instance()->EnableLocalAudio(false);
	p->setText(value.toString());
}

void MoreInfoDialog::SlotSelectSpeaker()
{
	SelectList dlg(e_select_device_speaker);
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigCloseSigtonDlg()), &dlg, SLOT(SlotClose()));
	QPoint lpot;
	QPoint gpot = ui.listWidget_stream_set_device->mapToGlobal(ui.listWidget_stream_set_device->pos() + QPoint(50, 34));
	dlg.setGeometry(gpot.x(), gpot.y(), dlg.width(), dlg.height());
	dlg.exec();

	//TODO sdk
	QPushButton* p = qobject_cast<QPushButton*>(sender());
	if (!p)
		return;

	QString str;
	RzObject::instance()->GetSpeakerDevice(str);
	p->setText(str);
}

void MoreInfoDialog::mousePressEvent(QMouseEvent* event)
{
	(void)event;
}

void MoreInfoDialog::mouseReleaseEvent(QMouseEvent* event)
{
	this->close();
}

void MoreInfoDialog::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);

	QPainter painter(this);

	// QColor最后一个参数80代表alpha通道，一般用作透明度
	painter.fillRect(rect(), QColor(56, 69, 93, 51));
}

void MoreInfoDialog::init()
{
	SetParams();
	initStyleSheetFile();
	initTabWidget();
	initListStreamSet();
	initConnect();
	ui.textEdit->setReadOnly(true);

}

void MoreInfoDialog::initStyleSheetFile()
{
	QFile styleFile("./resource/moreinfo.qss");
	QFileInfo fileInfo("./resource/moreinfo.qss");
	//QString str = fileInfo.absolutePath();

	styleFile.open(QIODevice::ReadOnly);
	this->setStyleSheet(QString(styleFile.readAll()));
}

void MoreInfoDialog::initTabWidget()
{
	QTabBar* bar = ui.tabWidget->tabBar();
	bar->setFixedWidth(ui.tabWidget->width());

	m_strStyle1 = "QLabel{ font-family: Microsoft YaHei; font - size: 12px; font - weight: 500; text - align: left; color:#1e6ceb; }";
	m_strStyle2 = "QLabel{font-family: Microsoft YaHei; font-size: 10px; text-align: left; color:#929baa;}";
	m_strStyle3 = "QLabel{ font-family: Microsoft YaHei; font - size: 12px; font - weight: 500; text - align: left; color:#929baa; }";
	m_strStyle4 = "QLabel{font-family: Microsoft YaHei; font-size: 10px; text-align: left; color:#cfd3de;}";
	setTab(bar, 0, QString::fromLocal8Bit("频道设置"), m_strStyle1, m_strStyle2);
	setTab(bar, 1, QString::fromLocal8Bit("推流设置"), m_strStyle3, m_strStyle4);
	setTab(bar, 2, QString::fromLocal8Bit("用户信息"), m_strStyle3, m_strStyle4);
	setTab(bar, 3, QString::fromLocal8Bit("流程数据"), m_strStyle3, m_strStyle4);
	ui.tabWidget->setCurrentIndex(1);
	bar->hide();
}

void MoreInfoDialog::initListStreamSet()
{
	bool isClose = false;
	QVariant value;
	QString strCur;
	RzObject::instance()->GetCamDevice(strCur);
	Settings::instance()->Read("cam", value, "Device");
	isClose = value.toString() == QString::fromLocal8Bit("关闭摄像头") ? true : false;
	strCur = isClose ? value.toString() : strCur;
	SetListComboBoxSelect(ui.listWidget_stream_set_device, QString::fromLocal8Bit("摄像头"), strCur, e_bind_select_cam, m_bgColorLst[0]);

	RzObject::instance()->GetMicDevice(strCur);
	Settings::instance()->Read("mic", value, "Device");
	isClose = value.toString() == QString::fromLocal8Bit("关闭麦克风") ? true : false;
	strCur = isClose ? value.toString() : strCur;
	SetListComboBoxSelect(ui.listWidget_stream_set_device, QString::fromLocal8Bit("麦克风"), strCur, e_bind_select_mic, m_bgColorLst[1]);

	RzObject::instance()->GetSpeakerDevice(strCur);
	Settings::instance()->Read("speaker", value, "Device");
	//isClose = value.toString() == QString::fromLocal8Bit("关闭扬声器") ? true : false;
	//strCur = isClose ? value.toString() : strCur;
	SetListComboBoxSelect(ui.listWidget_stream_set_device, QString::fromLocal8Bit("扬声器"), strCur, e_bind_select_speaker, m_bgColorLst[0]);

	ui.label_stream_set1->hide();
	ui.label_stream_set_stream1->hide();
	ui.label_stream_set2->hide();
	ui.label_stream_set_stream2->hide();
	ui.listWidget_stream_set_stream1->hide();
	ui.listWidget_stream_set_stream2->hide();
	return;
	//

	bool isEnabled = false;
	Settings::instance()->Read("Resolution", value, "stream1");
	SetListComboBoxSelect(ui.listWidget_stream_set_stream1, QString::fromLocal8Bit("编码分辨率"), value.toString(), e_bind_stream1_resolution, m_bgColorLst[0]);

	
	Settings::instance()->Read("Fps", value, "stream1");
	SetListComboBoxSelect(ui.listWidget_stream_set_stream1, QString::fromLocal8Bit("编码帧率"), value.toString(), e_bind_stream1_framerate, m_bgColorLst[1]);

	
	Settings::instance()->Read("EncodeDir", value, "stream1");
	SetListComboBoxSelect(ui.listWidget_stream_set_stream1, QString::fromLocal8Bit("编码方向"), value.toString(), e_bind_stream1_direction, m_bgColorLst[0]);

	
	Settings::instance()->Read("PushStreamMode", value, "stream1");
	isEnabled = value.toString() == QString::fromLocal8Bit("双流") ? true : false;
	SetListSwitchButton(ui.listWidget_stream_set_stream1, QString::fromLocal8Bit("双流模式"), e_bind_stream1_pushstreammode, m_bgColorLst[1], isEnabled);

	
	/*Settings::instance()->Read("PushStreamMode", value, "stream1");
	isEnabled = value.toString() == QString::fromLocal8Bit("双流") ? true : false;*/
	isEnabled = UserStateSingleton::instance()->GetIsMirror();
	SetListSwitchButton(ui.listWidget_stream_set_stream1, QString::fromLocal8Bit("镜像模式"), e_bind_stream1_mirror_mode, m_bgColorLst[0], isEnabled);

	//自定义推流
	
	Settings::instance()->Read("enablestream2", value, "SelfStreamSwitch");
	if (value.toBool() == false) {
		ui.label_stream_set2->hide();
		ui.label_stream_set_stream2->hide();
		return;
	}
		
	Settings::instance()->Read("Resolution", value, "stream2");
	SetListComboBoxSelect(ui.listWidget_stream_set_stream2, QString::fromLocal8Bit("编码分辨率"), value.toString(), e_bind_stream2_resolution, m_bgColorLst[0]);

	
	Settings::instance()->Read("Fps", value, "stream2");
	SetListComboBoxSelect(ui.listWidget_stream_set_stream2, QString::fromLocal8Bit("编码帧率"), value.toString(), e_bind_stream2_framerate, m_bgColorLst[1]);

	
	Settings::instance()->Read("EncodeDir", value, "stream2");
	SetListComboBoxSelect(ui.listWidget_stream_set_stream2, QString::fromLocal8Bit("编码方向"), value.toString(), e_bind_stream2_direction, m_bgColorLst[0]);

	
	Settings::instance()->Read("PushStreamMode", value, "stream2");
	isEnabled = value.toString() == QString::fromLocal8Bit("双流") ? true : false;
	SetListSwitchButton(ui.listWidget_stream_set_stream2, QString::fromLocal8Bit("双流模式"), e_bind_stream2_pushstreammode, m_bgColorLst[1], isEnabled);

	
	/*Settings::instance()->Read("PushStreamMode", value, "stream2");
	isEnabled = value.toString() == QString::fromLocal8Bit("双流") ? true : false;*/
	isEnabled = UserStateSingleton::instance()->GetIsMirror2();
	SetListSwitchButton(ui.listWidget_stream_set_stream2, QString::fromLocal8Bit("镜像模式"), e_bind_stream2_mirror_mode, m_bgColorLst[0], isEnabled);
}


void MoreInfoDialog::initConnect()
{
	connect(ui.tabWidget, SIGNAL(tabBarClicked(int)), this, SLOT(SlotClickedTabBar(int)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigRemoteAudioStats(const QString&, const RemoteAudioStats&)), this, SLOT(SlotRemoteAudioStats(const QString&, const RemoteAudioStats&)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigRemoteVideoStats(const QString&, const REMOTEVIDEOSTATS&)), this, SLOT(SlotRemoteVideoStats(const QString&, const REMOTEVIDEOSTATS&)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigAudioSubscribeStateChanged(const QString&, const QString&, SUBSCRIBE_STREAM_STATE, int)), this, SLOT(SlotAudioSubscribeStateChanged(const QString&, const QString&, SUBSCRIBE_STREAM_STATE, int)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigVideoSubscribeStateChanged(const QString&, const QString&, const QString&, SUBSCRIBE_STREAM_STATE, int)), this, SLOT(SlotVideoSubscribeStateChanged(const QString&, const QString&, const QString&, SUBSCRIBE_STREAM_STATE, int)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigUserJoined(const QString&, const QString&, int)), this, SLOT(SlotUserJoined(const QString&, const QString&, int)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigUserOffline(const QString&, const QString&, USER_OFFLINE_REASON_TYPE)), this, SLOT(SlotUserOffline(const QString&, const QString&, USER_OFFLINE_REASON_TYPE)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigAudioVolumeIndication(const AUDIOVOLUMEINFO&, unsigned int, int)), this, SLOT(SlotAudioVolumeIndication(const AUDIOVOLUMEINFO&, unsigned int, int)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigClientRoleChanged(const QString&, CLIENT_ROLE_TYPE, CLIENT_ROLE_TYPE, int)), this, SLOT(SlotClientRoleChanged(const QString&, CLIENT_ROLE_TYPE, CLIENT_ROLE_TYPE, int)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigVideoSizeChanged(int, int)), this, SLOT(SlotVideoSizeChanged(int, int)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigNetworkQuality(const QString&, const QString&, QUALITY_TYPE, QUALITY_TYPE)), this, SLOT(SlotNetworkQuality(const QString&, const QString&, QUALITY_TYPE, QUALITY_TYPE)));
	connect(UserStateNotifySingleton::instance(), SIGNAL(SigProcessData(QString)), this, SLOT(SlotProcessData(QString)));
}

void MoreInfoDialog::initSdkParams()
{

}

void MoreInfoDialog::initRemoteStats(QString uid, QString streamName)
{
	REMOTEVIDEOSTATS stats;
	UserStateSingleton::instance()->GetRemoteVideoStats(uid, streamName, stats);
	m_remoteUserAttrValueMap[e_re_resolution_value] = QString("%1 * %2").arg(stats.width).arg(stats.height);
	m_remoteUserAttrValueMap[e_re_framerate_value] = QString("%1 FPS").arg(stats.decoderOutputFrameRate);
	m_remoteUserAttrValueMap[e_re_bit_value] = QString("%1 Kbps").arg(stats.receivedBitrate);
	m_remoteUserAttrValueMap[e_re_loss_value] = QString("%1%").arg(stats.packetLossRate);
	int volume = UserStateSingleton::instance()->GetRemoteVolume(uid);
	m_remoteUserAttrValueMap[e_re_volume_value] = QString("%1").arg(volume);
	int quality = UserStateSingleton::instance()->GetRemoteNetQulity(uid);
	switch ((QUALITY_TYPE)quality)
	{
	case rz::QUALITY_UNKNOWN:
	{
		m_remoteUserAttrValueMap[e_re_netquality_value] = QString::fromLocal8Bit("差");
		break;
	}
	case rz::QUALITY_EXCELLENT:
	{
		m_remoteUserAttrValueMap[e_re_netquality_value] = QString::fromLocal8Bit("优");
		break;
	}
	case rz::QUALITY_GOOD:
	{
		m_remoteUserAttrValueMap[e_re_netquality_value] = QString::fromLocal8Bit("良");
		break;
	}
	case rz::QUALITY_POOR:
	{
		m_remoteUserAttrValueMap[e_re_netquality_value] = QString::fromLocal8Bit("中");
		break;
	}
	/*case rz::QUALITY_BAD:
	{
		m_remoteUserAttrValueMap[e_re_netquality_value] = QString::fromLocal8Bit("差");
		break;
	}
	case rz::QUALITY_VBAD:
		break;
	case rz::QUALITY_DOWN:
		break;
	case rz::QUALITY_UNSUPPORTED:
		break;
	case rz::QUALITY_DETECTING:
		break;*/
	default:
	{
		m_remoteUserAttrValueMap[e_re_netquality_value] = QString::fromLocal8Bit("差");
		break;
	}
	}
}

void MoreInfoDialog::setTab(QTabBar* bar, int index,  QString text, QString label1style, QString label2style)
{
	QWidget* pTab = new QWidget(bar);
	pTab->setStyleSheet("QWidget{background-color: white;}");

	QLabel* label = new QLabel(pTab);
	label->setObjectName("label");
	label->setFixedSize(70, 20);
	label->setText(text);
	label->setScaledContents(true);
	label->setStyleSheet(label1style);

	QLabel* label1 = new QLabel(pTab);
	label1->setObjectName("label1");
	label1->setText(text);
	label1->setFixedSize(70, 16);
	label1->setScaledContents(true);
	label1->setStyleSheet(label2style);

	QVBoxLayout* verLayout = new QVBoxLayout(pTab);
	verLayout->setMargin(0);
	verLayout->setSpacing(0);
	verLayout->setContentsMargins(8, 6, 0, 2);
	verLayout->addWidget(label);
	verLayout->addWidget(label1);
	pTab->setLayout(verLayout);

	bar->setTabButton(index, QTabBar::LeftSide, pTab);
}


void MoreInfoDialog::SetListChannelSetUid(QListWidget* lst, QString iconText, QString valueText)
{
	lst->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	lst->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	QWidget* widget = new QWidget(lst);

	//创建自定义的item
	widget->setStyleSheet("background-color:#f7f8fa;");

	QLabel* titleTextLabel = new QLabel(widget);
	titleTextLabel->setObjectName("titleTextLabel");
	titleTextLabel->setFixedSize(151, 20);
	titleTextLabel->setText(iconText);
	titleTextLabel->setScaledContents(true);
	titleTextLabel->setStyleSheet("QLabel{font-family: Microsoft YaHei; font-size: 12px; text-align: left; color:#929baa;}");


	QLabel* valueLabel = new QLabel(widget);
	valueLabel->setObjectName("valueLabel");
	valueLabel->setFixedSize(103, 20);
	valueLabel->setText(valueText);
	valueLabel->setScaledContents(true);
	valueLabel->setStyleSheet("QLabel{font-family: Microsoft YaHei; font-size: 12px; text-align: right; color:#929baa;}");
	valueLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	valueLabel->setLayoutDirection(Qt::LayoutDirection::RightToLeft);


	QHBoxLayout* horLayout = new QHBoxLayout(widget);
	horLayout->setMargin(0);
	horLayout->setSpacing(0);
	horLayout->setContentsMargins(8, 0, 8, 0);
	horLayout->addWidget(titleTextLabel);
	horLayout->addSpacing(300);
	horLayout->addWidget(valueLabel);

	widget->setLayout(horLayout);

	//将widget作为列表的item
	QListWidgetItem* item = new QListWidgetItem(lst);

	QSize size = item->sizeHint();
	item->setSizeHint(QSize(351, 36));
	lst->addItem(item);
	widget->setSizeIncrement(size.width(), 36);
	lst->setItemWidget(item, widget);
}

void MoreInfoDialog::SetListChannelSetSwitchRole(QListWidget* lst, QString iconText, QString valueText, QString bgcolor)
{
	lst->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	lst->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	QWidget* widget = new QWidget(lst);
	widget->setStyleSheet(bgcolor);
	
	QLabel* titleTextLabel = new QLabel(widget);
	titleTextLabel->setObjectName("titleTextLabel");
	titleTextLabel->setFixedSize(175, 20);
	titleTextLabel->setText(iconText);
	titleTextLabel->setScaledContents(true);
	titleTextLabel->setStyleSheet("QLabel{font-family: Microsoft YaHei; font-size: 12px; text-align: left; color:#929baa;}");


	QPushButton* valueButton = new QPushButton(widget);
	BindSlot(valueButton, e_bind_role);
	valueButton->setObjectName("valueLabel");
	valueButton->setFixedSize(78, 24);
	valueButton->setText(valueText);
	valueButton->setStyleSheet("QPushButton{border-radius: 4px;	border: 1px solid #1e6ceb;\
		background-color: white; font-family: Microsoft YaHei; font-size: 12px; text-align: center; color:#1e6ceb;}");
	valueButton->setLayoutDirection(Qt::LayoutDirection::RightToLeft);


	QHBoxLayout* horLayout = new QHBoxLayout(widget);
	horLayout->setMargin(0);
	horLayout->setSpacing(0);
	horLayout->setContentsMargins(8, 0, 8, 0);
	horLayout->addWidget(titleTextLabel);
	horLayout->addSpacing(300);
	horLayout->addWidget(valueButton);
	if (valueText.isEmpty())
		valueButton->hide();
	widget->setLayout(horLayout);

	//将widget作为列表的item
	QListWidgetItem* item = new QListWidgetItem(lst);

	QSize size = item->sizeHint();
	item->setSizeHint(QSize(351, 36));
	lst->addItem(item);
	widget->setSizeIncrement(size.width(), 36);
	lst->setItemWidget(item, widget);
}

void MoreInfoDialog::SetListSwitchButton(QListWidget* lst, QString iconText, E_BING_TYPE type, QString bgcolor, bool isEnabled)
{
	lst->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	lst->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	QWidget* widget = new QWidget(lst);
	widget->setStyleSheet(bgcolor);
	//创建自定义的item
	/*if (isvideo)
		widget->setStyleSheet("background-color:#f7f8fa;");
	else
		widget->setStyleSheet("background-color:white;");*/
	

	QLabel* titleTextLabel = new QLabel(widget);
	titleTextLabel->setObjectName("titleTextLabel");
	titleTextLabel->setFixedSize(175, 20);
	titleTextLabel->setText(iconText);
	titleTextLabel->setScaledContents(true);
	titleTextLabel->setStyleSheet("QLabel{font-family: Microsoft YaHei; font-size: 12px; text-align: left; color:#929baa;}");

	SwitchButton* valueButton = new SwitchButton(widget);
	valueButton->SetState(isEnabled);
	valueButton->setObjectName("valueLabel");
	valueButton->setFixedSize(44, 24);
	valueButton->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
	BindSlot((QPushButton*)valueButton, type);

	QHBoxLayout* horLayout = new QHBoxLayout(widget);
	horLayout->setMargin(0);
	horLayout->setSpacing(0);
	horLayout->setContentsMargins(8, 0, 8, 0);
	horLayout->addWidget(titleTextLabel);
	horLayout->addSpacing(300);
	horLayout->addWidget(valueButton);

	widget->setLayout(horLayout);

	//将widget作为列表的item
	QListWidgetItem* item = new QListWidgetItem(lst);

	QSize size = item->sizeHint();
	item->setSizeHint(QSize(351, 36));
	lst->addItem(item);
	widget->setSizeIncrement(size.width(), 36);
	lst->setItemWidget(item, widget);
}

void MoreInfoDialog::SetListChannelSetLog(QListWidget* lst, QString iconText, QString labelText)
{
	lst->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	lst->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	QWidget* widget = new QWidget(lst);

	//创建自定义的item
	widget->setStyleSheet("background-color:white;");

	QLabel* titleTextLabel = new QLabel(widget);
	titleTextLabel->setObjectName("titleTextLabel");
	titleTextLabel->setFixedSize(175, 20);
	titleTextLabel->setText(iconText);
	titleTextLabel->setScaledContents(true);
	titleTextLabel->setStyleSheet("QLabel{font-family: Microsoft YaHei; font-size: 12px; text-align: left; color:#929baa;}");


	QLabel* textButton = new QLabel(widget);
	textButton->setObjectName("textButton");
	textButton->setText(labelText);
	textButton->setFixedSize(118, 24);
	textButton->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	textButton->setStyleSheet("QLabel{border-radius: 2px;\
		background-color: white; font-family: Microsoft YaHei; font-size: 12px; text-align: right; color:#1e6ceb;}");

	QPushButton* iconButton = new QPushButton(widget);
	BindSlot(iconButton, e_bind_open_log);
	iconButton->setObjectName("iconButton");
	iconButton->setFixedSize(24, 24);
	iconButton->setStyleSheet("QPushButton{background-color: white; border: none; background-image: url(./resource/image/testenvback.png);\
		background-color: white;background-repeat: no-repeat;}");
	QHBoxLayout* horLayout = new QHBoxLayout(widget);
	horLayout->setMargin(0);
	horLayout->setSpacing(0);
	horLayout->setContentsMargins(8, 0, 8, 0);
	horLayout->addWidget(titleTextLabel);
	horLayout->addSpacing(20);
	horLayout->addWidget(textButton);
	horLayout->addWidget(iconButton);
	widget->setLayout(horLayout);

	//将widget作为列表的item
	QListWidgetItem* item = new QListWidgetItem(lst);

	QSize size = item->sizeHint();
	item->setSizeHint(QSize(351, 36));
	lst->addItem(item);
	widget->setSizeIncrement(size.width(), 36);
	lst->setItemWidget(item, widget);
}

void MoreInfoDialog::SetListComboBoxSelect(QListWidget* lst, QString iconText, QString buttonValue, E_BING_TYPE type, QString bgcolor)
{
	lst->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	lst->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	QWidget* widget = new QWidget(lst);
	widget->setStyleSheet(bgcolor);

	QLabel* titleTextLabel = new QLabel(widget);
	titleTextLabel->setObjectName("titleTextLabel");
	titleTextLabel->setFixedSize(175, 20);
	titleTextLabel->setText(iconText);
	titleTextLabel->setScaledContents(true);
	titleTextLabel->setStyleSheet("QLabel{font-family: Microsoft YaHei; font-size: 12px; text-align: left; color:#929baa;}");

	QPushButton* button = new QPushButton(widget);
	button->setObjectName("button");
	button->setText(buttonValue);
	button->setToolTip(buttonValue);
	button->setFixedSize(100, 24);
	button->setStyleSheet("QPushButton{border-radius: 4px;	border: 1px solid #929baa;\
		background-color: white; font-family: Microsoft YaHei; font-size: 12px; text-align: left;\
		color:#929baa;background-image: url(./resource/image/arrow.png);background-origin: content;\
		background-position: right; padding-left: 12px; padding-right: 10px; background-repeat: no-repeat;}");
	BindSlot(button, type);

	QHBoxLayout* horLayout = new QHBoxLayout(widget);
	horLayout->setMargin(0);
	horLayout->setSpacing(0);
	horLayout->setContentsMargins(8, 0, 8, 0);
	horLayout->addWidget(titleTextLabel);
	horLayout->addSpacing(300);
	horLayout->addWidget(button);

	widget->setLayout(horLayout);

	QListWidgetItem* item = new QListWidgetItem(lst);
	QSize size = item->sizeHint();
	item->setSizeHint(QSize(351, 36));
	lst->addItem(item);
	widget->setSizeIncrement(size.width(), 36);
	lst->setItemWidget(item, widget);
}

//widget objectName规则：属性名+uid+streamid 例如：user_654345654_video_99988
void MoreInfoDialog::SetListRemoteUserInfo(QListWidget* lst, E_REMOTE_INFO_TYPE type)
{
	lst->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	lst->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	RemoteUserInfoWidgets* widget = new RemoteUserInfoWidgets(lst);

	//创建自定义的item
	widget->setStyleSheet(m_remoteUserBgColorMap[type]);
	widget->SetUid(m_remoteUserAttrValueMap[e_re_user_id]);
	//widget->SetChannelId()
	widget->SetType(type);
	widget->SetStreamName(m_remoteUserAttrValueMap[e_re_user_stream]);
	//widget->setObjectName(m_remoteUserAttrNameMap[type] + "_" + uid + "_" + streamid);
	/*widget->setObjectName(m_remoteUserAttrValueMap[e_re_user_id] + "_" + m_remoteUserAttrValueMap[e_re_user_stream]);
	QString objName = widget->objectName();*/
	QLabel* titleTextLabel = new QLabel(widget);
	titleTextLabel->setObjectName("titleTextLabel");
	titleTextLabel->setFixedSize(m_remoteUserSizeMap[type]);
	titleTextLabel->setText(m_remoteUserAttrValueMap[type]);
	titleTextLabel->setScaledContents(true);
	titleTextLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	titleTextLabel->setStyleSheet(m_remoteUserStyleMap[type]);

	QLabel* valueLabel = new QLabel(widget);
	valueLabel->setObjectName("valueTextLabel");
	valueLabel->setFixedSize(m_remoteUserSizeMap[(E_REMOTE_INFO_TYPE)(type + 1)]);
	valueLabel->setText(m_remoteUserAttrValueMap[(E_REMOTE_INFO_TYPE)(type + 1)]);
	valueLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	//valueLabel->setScaledContents(true);
	valueLabel->setStyleSheet(m_remoteUserStyleMap[(E_REMOTE_INFO_TYPE)(type + 1)]);
	QHBoxLayout* horLayout = new QHBoxLayout(widget);
	horLayout->setMargin(0);
	horLayout->setSpacing(0);
	
	horLayout->setContentsMargins(m_remoteUserLeftPaddingMap[type], 0, 8, 0);
	horLayout->addWidget(titleTextLabel);
	if (type != e_re_user_id)
		horLayout->addSpacing(300);
	horLayout->addWidget(valueLabel);
	if (type == e_re_user_id)
		horLayout->addSpacing(251);
	widget->setLayout(horLayout);

	//将widget作为列表的item
	QListWidgetItem* item = new QListWidgetItem(lst);
	
	int itemHeight = type == e_re_user_id ? 38 : 30;
	QSize size = item->sizeHint();
	item->setSizeHint(QSize(351, itemHeight));
	lst->addItem(item);
	widget->setSizeIncrement(size.width(), itemHeight);
	lst->setItemWidget(item, widget);
}

void MoreInfoDialog::SetParams()
{
	m_bgColorLst << "background-color:white;" << "background-color:#f7f8fa;";
	m_remoteUserStyleMap[e_re_user_id] = "QLabel{ font-family: Microsoft YaHei; font-size: 12px; font-weight: 500; text-align: left; color:#38455d; }";
	m_remoteUserStyleMap[e_re_user_stream] = "QLabel{ font-family: Microsoft YaHei; font-size: 12px; font-weight: 300; text-align: left; color:#64728c; }";
	m_remoteUserStyleMap[e_re_video] = "QLabel{ font-family: Microsoft YaHei; font-size: 12px; text-align: left; color:#64728c; }";
	m_remoteUserStyleMap[e_re_video_state] = "QLabel{ font-family: Microsoft YaHei; font-size: 12px; text-align: left; color:#1e6ceb; }";
	m_remoteUserStyleMap[e_re_resolution] = "QLabel{ font-family: Microsoft YaHei; font-size: 12px; text-align: left; color:#929baa; }";
	m_remoteUserStyleMap[e_re_resolution_value] = "QLabel{ font-family: Microsoft YaHei; font-size: 12px; text-align: left; color:#929baa; }";
	m_remoteUserStyleMap[e_re_framerate] = "QLabel{ font-family: Microsoft YaHei; font-size: 12px; text-align: left; color:#929baa; }";
	m_remoteUserStyleMap[e_re_framerate_value] = "QLabel{ font-family: Microsoft YaHei; font-size: 12px; text-align: left; color:#929baa; }";
	m_remoteUserStyleMap[e_re_audio] = "QLabel{ font-family: Microsoft YaHei; font-size: 12px; text-align: left; color:#64728c; }";
	m_remoteUserStyleMap[e_re_audio_state] = "QLabel{ font-family: Microsoft YaHei; font-size: 12px; text-align: left; color:#1e6ceb; }";
	m_remoteUserStyleMap[e_re_volume] = "QLabel{ font-family: Microsoft YaHei; font-size: 12px; text-align: left; color:#929baa; }";
	m_remoteUserStyleMap[e_re_volume_value] = "QLabel{ font-family: Microsoft YaHei; font-size: 12px; text-align: left; color:#929baa; }";
	m_remoteUserStyleMap[e_re_bit] = "QLabel{ font-family: Microsoft YaHei; font-size: 12px; text-align: left; color:#64728c; }";
	m_remoteUserStyleMap[e_re_bit_value] = "QLabel{ font-family: Microsoft YaHei; font-size: 12px; text-align: left; color:#64728c; }";
	m_remoteUserStyleMap[e_re_loss] = "QLabel{ font-family: Microsoft YaHei; font-size: 12px; text-align: left; color:#64728c; }";
	m_remoteUserStyleMap[e_re_loss_value] = "QLabel{ font-family: Microsoft YaHei; font-size: 12px; text-align: left; color:#64728c; }";
	m_remoteUserStyleMap[e_re_netquality] = "QLabel{ font-family: Microsoft YaHei; font-size: 12px; text-align: left; color:#64728c; }";
	m_remoteUserStyleMap[e_re_netquality_value] = "QLabel{ font-family: Microsoft YaHei; font-size: 12px; text-align: left; color:#64728c; }";

	m_remoteUserBgColorMap[e_re_user_id] = m_bgColorLst[0];
	m_remoteUserBgColorMap[e_re_video] = m_bgColorLst[1];
	m_remoteUserBgColorMap[e_re_resolution] = m_bgColorLst[0];
	m_remoteUserBgColorMap[e_re_framerate] = m_bgColorLst[0];
	m_remoteUserBgColorMap[e_re_audio] = m_bgColorLst[1];
	m_remoteUserBgColorMap[e_re_volume] = m_bgColorLst[0];
	m_remoteUserBgColorMap[e_re_bit] = m_bgColorLst[1];
	m_remoteUserBgColorMap[e_re_loss] = m_bgColorLst[0];
	m_remoteUserBgColorMap[e_re_netquality] = m_bgColorLst[1];

	m_remoteUserSizeMap[e_re_user_id] = QSize(52, 22);
	m_remoteUserSizeMap[e_re_user_stream] = QSize(52, 22);
	m_remoteUserSizeMap[e_re_video] = QSize(24, 17);
	m_remoteUserSizeMap[e_re_video_state] = QSize(24, 17);
	m_remoteUserSizeMap[e_re_resolution] = QSize(36, 17);
	m_remoteUserSizeMap[e_re_resolution_value] = QSize(70, 17);
	m_remoteUserSizeMap[e_re_framerate] = QSize(24, 17);
	m_remoteUserSizeMap[e_re_framerate_value] = QSize(41, 17);
	m_remoteUserSizeMap[e_re_audio] = QSize(24, 17);
	m_remoteUserSizeMap[e_re_audio_state] = QSize(24, 17);
	m_remoteUserSizeMap[e_re_volume] = QSize(48, 17);
	m_remoteUserSizeMap[e_re_volume_value] = QSize(15, 17);
	m_remoteUserSizeMap[e_re_bit] = QSize(48, 17);
	m_remoteUserSizeMap[e_re_bit_value] = QSize(55, 17);
	m_remoteUserSizeMap[e_re_loss] = QSize(60, 17);
	m_remoteUserSizeMap[e_re_loss_value] = QSize(37, 17);
	m_remoteUserSizeMap[e_re_netquality] = QSize(48, 17);
	m_remoteUserSizeMap[e_re_netquality_value] = QSize(12, 17);

	m_remoteUserAttrNameMap[e_re_user_id] = "user";
	m_remoteUserAttrNameMap[e_re_video] = "video";
	m_remoteUserAttrNameMap[e_re_resolution] = "resolution";
	m_remoteUserAttrNameMap[e_re_framerate] = "framerate";
	m_remoteUserAttrNameMap[e_re_audio] = "audio";
	m_remoteUserAttrNameMap[e_re_volume] = "volume";
	m_remoteUserAttrNameMap[e_re_bit] = "bit";
	m_remoteUserAttrNameMap[e_re_loss] = "loss";
	m_remoteUserAttrNameMap[e_re_netquality] = "netquality";

	m_remoteUserAttrValueMap[e_re_user_id] = "uid";
	m_remoteUserAttrValueMap[e_re_user_stream] = "default";
	m_remoteUserAttrValueMap[e_re_video] = QString::fromLocal8Bit("视频");
	m_remoteUserAttrValueMap[e_re_video_state] = QString::fromLocal8Bit("开启");
	m_remoteUserAttrValueMap[e_re_resolution] = QString::fromLocal8Bit("分辨率");
	m_remoteUserAttrValueMap[e_re_resolution_value] = QString::fromLocal8Bit("640 * 360");
	m_remoteUserAttrValueMap[e_re_framerate] = QString::fromLocal8Bit("帧率");
	m_remoteUserAttrValueMap[e_re_framerate_value] = QString::fromLocal8Bit("24 FPS");
	m_remoteUserAttrValueMap[e_re_audio] = QString::fromLocal8Bit("音频");
	m_remoteUserAttrValueMap[e_re_audio_state] = QString::fromLocal8Bit("开启");
	m_remoteUserAttrValueMap[e_re_volume] = QString::fromLocal8Bit("音量大小");
	m_remoteUserAttrValueMap[e_re_volume_value] = QString::fromLocal8Bit("40");
	m_remoteUserAttrValueMap[e_re_bit] = QString::fromLocal8Bit("接收码流");
	m_remoteUserAttrValueMap[e_re_bit_value] = QString::fromLocal8Bit("1.00 Kbps");
	m_remoteUserAttrValueMap[e_re_loss] = QString::fromLocal8Bit("接收丢包率");
	m_remoteUserAttrValueMap[e_re_loss_value] = QString::fromLocal8Bit("0.00%");
	m_remoteUserAttrValueMap[e_re_netquality] = QString::fromLocal8Bit("网络质量");
	m_remoteUserAttrValueMap[e_re_netquality_value] = QString::fromLocal8Bit("优");

	m_remoteUserLeftPaddingMap[e_re_user_id] = 4;
	m_remoteUserLeftPaddingMap[e_re_video] = 8;
	m_remoteUserLeftPaddingMap[e_re_resolution] = 18;
	m_remoteUserLeftPaddingMap[e_re_framerate] = 18;
	m_remoteUserLeftPaddingMap[e_re_audio] = 8;
	m_remoteUserLeftPaddingMap[e_re_volume] = 18;
	m_remoteUserLeftPaddingMap[e_re_bit] = 8;
	m_remoteUserLeftPaddingMap[e_re_loss] = 8;
	m_remoteUserLeftPaddingMap[e_re_netquality] = 8;
}

void MoreInfoDialog::BindSlot(QWidget* bt, E_BING_TYPE type)
{
	switch (type)
	{
	case e_bind_role:
	{
		connect(bt, SIGNAL(clicked()), this, SLOT(SlotSwitchRole()));
		break;
	}
	case e_bind_receive_a_stream:
	{	
		SwitchButton* psw = dynamic_cast<SwitchButton*>(bt);
		if(psw)
			connect(bt, SIGNAL(btnState(bool)), this, SLOT(SlotIsRecvAudio(bool)));
		break;
	}
	case e_bind_receive_v_stream:
	{
		SwitchButton* psw = dynamic_cast<SwitchButton*>(bt);
		if (psw)
			connect(bt, SIGNAL(btnState(bool)), this, SLOT(SlotIsRecvVideo(bool)));
		break;
	}
	case e_bind_render_mode:
	{
		connect(bt, SIGNAL(clicked()), this, SLOT(SlotRenderMode()));
		break;
	}
	case e_bind_open_log:
	{
		connect(bt, SIGNAL(clicked()), this, SLOT(SlotOpenLog()));
		break;
	}
	case e_bind_select_cam:
	{
		connect(bt, SIGNAL(clicked()), this, SLOT(SlotSelectCam()));
		break;
	}
	case e_bind_select_mic:
	{
		connect(bt, SIGNAL(clicked()), this, SLOT(SlotSelectMic()));
		break;
	}
	case e_bind_select_speaker:
	{
		connect(bt, SIGNAL(clicked()), this, SLOT(SlotSelectSpeaker()));
		break;
	}
	case e_bind_stream1_resolution:
	{
		connect(bt, SIGNAL(clicked()), this, SLOT(SlotSelectResolution()));
		break;
	}
	case e_bind_stream1_framerate:
	{
		connect(bt, SIGNAL(clicked()), this, SLOT(SlotSelectFrameRate()));
		break;
	}
	case e_bind_stream1_direction:
	{
		connect(bt, SIGNAL(clicked()), this, SLOT(SlotSelectDir()));
		break;
	}
	case e_bind_stream1_pushstreammode:
	{
		SwitchButton* psw = dynamic_cast<SwitchButton*>(bt);
		if (psw)
			connect(bt, SIGNAL(btnState(bool)), this, SLOT(SlotSelectPushStreamMode(bool)));
		break;
	}
	case e_bind_stream1_mirror_mode:
	{
		SwitchButton* psw = dynamic_cast<SwitchButton*>(bt);
		if (psw)
			connect(bt, SIGNAL(btnState(bool)), this, SLOT(SlotSelectMirrorMode(bool)));
		break;
	}
	case e_bind_stream2_resolution:
	{
		connect(bt, SIGNAL(clicked()), this, SLOT(SlotSelectResolution2()));
		break;
	}
	case e_bind_stream2_framerate:
	{
		connect(bt, SIGNAL(clicked()), this, SLOT(SlotSelectFrameRate2()));
		break;
	}
	case e_bind_stream2_direction:
	{
		connect(bt, SIGNAL(clicked()), this, SLOT(SlotSelectDir2()));
		break;
	}
	case e_bind_stream2_pushstreammode:
	{
		SwitchButton* psw = dynamic_cast<SwitchButton*>(bt);
		if (psw)
			connect(bt, SIGNAL(btnState(bool)), this, SLOT(SlotSelectPushStreamMode2(bool)));
		break;
	}
	case e_bind_stream2_mirror_mode:
	{
		SwitchButton* psw = dynamic_cast<SwitchButton*>(bt);
		if (psw)
			connect(bt, SIGNAL(btnState(bool)), this, SLOT(SlotSelectMirrorMode2(bool)));
		break;
	}
	}
}

void MoreInfoDialog::SetScene(QString& scene, QString& role)
{
	QVariant value;
	Settings::instance()->Read("scene", value, "SceneMode");
	QString str = value.toString();
	if (str == QString::fromLocal8Bit("通信"))
	{
		scene = QString::fromLocal8Bit("频道模式：通信");
	}
	else if (str == QString::fromLocal8Bit("直播主播"))
	{
		scene = QString::fromLocal8Bit("频道模式：直播");
		role = QString::fromLocal8Bit("切换为观众");
	}
	else if (str == QString::fromLocal8Bit("直播观众"))
	{
		scene = QString::fromLocal8Bit("频道模式：直播");
		role = QString::fromLocal8Bit("切换为主播");
	}
}

void MoreInfoDialog::UpdateRemoteUserInfoInList(E_REMOTE_INFO_TYPE type, QString uid, QString streamid, QString value)
{
	QString strStreamId = streamid.isEmpty() ? QString("default") : streamid;
	for (int i = 0; i < ui.listWidget_remote_user_info->count(); i++)
	{
		QListWidgetItem* item = ui.listWidget_remote_user_info->item(i);
		RemoteUserInfoWidgets* pw = static_cast<RemoteUserInfoWidgets*>(ui.listWidget_remote_user_info->itemWidget(item));
		if (pw->GetType() != type || pw->GetUid() != uid || pw->GetStreamName() != strStreamId)
			continue;
		QLabel* label = pw->findChild<QLabel*>("valueTextLabel");
		if (label) {
			label->setText(value);
			//TODO 是否需要设置样式
			break;
		}
	}
}

void MoreInfoDialog::UpdateRemoteUserInfoInList(E_REMOTE_INFO_TYPE type, QString uid, QString value)
{
	for (int i = 0; i < ui.listWidget_remote_user_info->count(); i++)
	{
		QListWidgetItem* item = ui.listWidget_remote_user_info->item(i);
		RemoteUserInfoWidgets* pw = static_cast<RemoteUserInfoWidgets*>(ui.listWidget_remote_user_info->itemWidget(item));
		if (pw->GetType() != type || pw->GetUid() != uid)
			continue;
		QLabel* label = pw->findChild<QLabel*>("valueTextLabel");
		if (label) {
			label->setText(value);
			//TODO 是否需要设置样式
		}
	}
}

void MoreInfoDialog::UpdateRemoteUserDefaultObjectName(QString uid, QString streamid)
{
	if (streamid != "first")
		return;

	/*QString strStreamName = QString("default");
	QString objNameDefault = uid + "_" + strStreamName;
	QString objNameNew = uid + "_" + streamid;*/
	QString strUid, strStreamName;
	//QString
	for (int i = 0; i < ui.listWidget_remote_user_info->count(); i++)
	{
		QListWidgetItem* item = ui.listWidget_remote_user_info->item(i);
		RemoteUserInfoWidgets* pw = dynamic_cast<RemoteUserInfoWidgets*>(ui.listWidget_remote_user_info->itemWidget(item));
		if (!pw) continue;
		strUid = pw->GetUid();
		strStreamName = pw->GetStreamName();
		if ("default" == strStreamName && strUid == uid)
		{
			pw->SetStreamName(streamid);
			QLabel* label = pw->findChild<QLabel*>("valueTextLabel");
			if (label) {
				QString labelText = label->text();
				if (label->text() == "default")
					label->setText(streamid);
				//TODO 是否需要设置样式
			}
		}
		
	}
}

void MoreInfoDialog::AddRemoteUserInfoList(QString uid)
{
	m_remoteUserAttrValueMap[e_re_user_id] = uid;
	for (auto i = 0; i <= e_re_netquality_value; i += 2)
	{
		SetListRemoteUserInfo(ui.listWidget_remote_user_info, (E_REMOTE_INFO_TYPE)i);
	}
}

void MoreInfoDialog::DelRemoteUserAllInfoList(QString uid)
{
	QString strUid;
	for (int i = ui.listWidget_remote_user_info->count() - 1; i >= 0; i--)
	{
		QListWidgetItem* var = ui.listWidget_remote_user_info->item(i);
		if (!var) continue;
		RemoteUserInfoWidgets* pw = static_cast<RemoteUserInfoWidgets*>(ui.listWidget_remote_user_info->itemWidget(var));
		if(pw->GetUid() == uid)
			ui.listWidget_remote_user_info->takeItem(i);
	}
}

void MoreInfoDialog::DelRemoteUserInfoInList(QString uid, QString streamid)
{
	for (int i = ui.listWidget_remote_user_info->count() - 1; i >= 0; i--)
	{
		QListWidgetItem* var = ui.listWidget_remote_user_info->item(i);
		if (!var)continue;
		RemoteUserInfoWidgets* pw = static_cast<RemoteUserInfoWidgets*>(ui.listWidget_remote_user_info->itemWidget(var));
		if (pw->GetUid() != uid || pw->GetStreamName() != streamid)
			continue;
		ui.listWidget_remote_user_info->takeItem(i);
	}
}

bool MoreInfoDialog::isRemoteUserExist(QString uid)
{
	bool flag = false;
	for (int i = ui.listWidget_remote_user_info->count() - 1; i >= 0; i--)
	{
		QListWidgetItem* var = ui.listWidget_remote_user_info->item(i);
		if (!var)continue;
		RemoteUserInfoWidgets* pw = static_cast<RemoteUserInfoWidgets*>(ui.listWidget_remote_user_info->itemWidget(var));
		if (pw->GetUid() == uid) {
			flag = true;
			break;
		}
	}
	return flag;
}

bool MoreInfoDialog::isRemoteStreamExist(QString uid, QString streamName)
{
	bool flag = false;
	for (int i = ui.listWidget_remote_user_info->count() - 1; i >= 0; i--)
	{
		QListWidgetItem* var = ui.listWidget_remote_user_info->item(i);
		if (!var)continue;
		RemoteUserInfoWidgets* pw = static_cast<RemoteUserInfoWidgets*>(ui.listWidget_remote_user_info->itemWidget(var));
		if (pw->GetUid() == uid && pw->GetStreamName() == streamName) {
			flag = true;
			break;
		}
	}
	return flag;
}