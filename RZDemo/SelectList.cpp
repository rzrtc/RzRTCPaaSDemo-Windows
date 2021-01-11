#include "SelectList.h"
#include <QFileInfo>
#include <QPainter>
#include <QPainterPath>
#include <qmath.h>
#include "ListWidgetSet.h"
#include "Settings.h"
#include "RzObject.h"
#include <iostream>
SelectList::SelectList(E_SELECT_LIST_TYPE type, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::Tool);
	m_type = type;
	init();
}

SelectList::~SelectList()
{
}

void SelectList::SlotClose()
{
	this->close();
}

void SelectList::SlotList(QListWidgetItem* item)
{
	QWidget* pw = ui.listWidget->itemWidget(item);
	QLabel* label = pw->findChild<QLabel*>("titleTextLabel");
	if (!label) return;
	QString labelText = label->text();
	switch (m_type)
	{
	case e_select_device_cam:
	{
		Settings::instance()->Write("cam", labelText, "Device");
		RzObject::instance()->SetCamDevice(labelText);
		break;
	}
	case e_select_device_mic:
	{
		Settings::instance()->Write("mic", labelText, "Device");
		RzObject::instance()->SetMicDevice(labelText);
		break;
	}
	case e_select_device_speaker:
	{
		/*Settings::instance()->Write("speaker", labelText, "Device");
		if (labelText != QString::fromLocal8Bit("关闭扬声器")) {*/
		RzObject::instance()->SetSpeakerDevice(labelText);
		/*}
		else
			RzObject::instance()->EnableLocalAudio(false);*/
		break;
	}
	case e_select_device_render_mode:
	{
		Settings::instance()->Write("RenderMode", label->text(), "ChannelSetUp");
		break;
	}
	case e_select_stream1_resolution:
	{
		Settings::instance()->Write("Resolution", label->text(), "stream1");
		break;
	}
	case e_select_stream1_fps:
	{
		Settings::instance()->Write("Fps", label->text(), "stream1");
		break;
	}
	case e_select_stream1_pushstreammode:
	{
		Settings::instance()->Write("PushStreamMode", label->text(), "stream1");
		break;
	}
	case e_select_stream1_encodedir:
	{
		Settings::instance()->Write("EncodeDir", label->text(), "stream1");
		break;
	}
	case e_select_stream1_mirror:
	{
		//Settings::instance()->Write("Mirror", label->text(), "stream1");
		break;
	}
	case e_select_stream2_resolution:
	{
		Settings::instance()->Write("Resolution", label->text(), "stream2");
		break;
	}
	case e_select_stream2_fps:
	{
		Settings::instance()->Write("Fps", label->text(), "stream2");
		break;
	}
	case e_select_stream2_pushstreammode:
	{
		Settings::instance()->Write("PushStreamMode", label->text(), "stream2");
		break;
	}
	case e_select_stream2_encodedir:
	{
		Settings::instance()->Write("EncodeDir", label->text(), "stream2");
		break;
	}
	case e_select_stream2_mirror:
	{
		//Settings::instance()->Write("Mirror", label->text(), "stream2");
		break;
	}
	default:
		break;
	}
	switch (m_type)
	{
	
	}
	
	initCtrl();
}

void SelectList::mousePressEvent(QMouseEvent* event)
{
	(void)event;
}

void SelectList::mouseReleaseEvent(QMouseEvent* event)
{
	QRect rect = ui.listWidget->geometry();
	QPoint mousePos = event->pos();
	if (rect.contains(mousePos) == false)
		this->close();
}

void SelectList::paintEvent(QPaintEvent* event)
{
    int w, h;
    w = 4;
    h = 4;
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(w, h, this->width() - w * 2, this->height() - h * 2);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(path, QBrush(Qt::white));
    QColor color(151, 151, 151, 0.5);
    for (int i = 0; i < w; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(w - i, w - i, this->width() - (w - i) * 2, this->height() - (h - i) * 2);
        color.setAlpha(150 - qSqrt(i) * 50);
        painter.setPen(color);
        painter.drawPath(path);
    }
}

void SelectList::init()
{
	initParams();
	initStyleSheetFile();
    initCtrl();
	initConnect();
}

void SelectList::initStyleSheetFile()
{
	QFile styleFile("./resource/cam.qss");
	QFileInfo fileInfo("./resource/cam.qss");
	//QString str = fileInfo.absolutePath();

	styleFile.open(QIODevice::ReadOnly);
	this->setStyleSheet(QString(styleFile.readAll()));
}

void SelectList::initCtrl()
{
	ui.listWidget->clear();
	QVariant value;
	QStringList clst;
	switch (m_type)
	{
	case e_select_device_cam:
	{
		ui.label->setText(QString::fromLocal8Bit("选择摄像头"));
		//TODO sdk read
		addCamList();
		break;
	}
	case e_select_device_mic:
	{
		ui.label->setText(QString::fromLocal8Bit("选择麦克风"));
		addMicList();
		break;
	}
	case e_select_device_speaker:
	{
		ui.label->setText(QString::fromLocal8Bit("选择扬声器"));
		addSpeakerList();
		break;
	}
	case e_select_device_render_mode:
	{
		ui.label->setText(QString::fromLocal8Bit("渲染方式"));
		clst << QString::fromLocal8Bit("等比缩放") << QString::fromLocal8Bit("填充") << QString::fromLocal8Bit("拉伸");
		Settings::instance()->Read("RenderMode", value, "ChannelSetUp");
		for (int i = 0; i < clst.count(); i++)
			SetList(ui.listWidget, clst[i], value.toString() == clst[i] ? "ok.png" : "");
		break;
	}
	case e_select_stream1_resolution:
	{
		ui.label->setText(QString::fromLocal8Bit("选择分辨率"));
		addStream1Resolution();
		break;
	}
	case e_select_stream1_fps:
	{
		ui.label->setText(QString::fromLocal8Bit("选择编码帧率"));
		addStream1Fps();
		break;
	}
	case e_select_stream1_pushstreammode:
	{
		/*ui.label->setText(QString::fromLocal8Bit("选择推流模式"));
		addStream1PushStreamMode();*/
		break;
	}
	case e_select_stream1_encodedir:
	{
		ui.label->setText(QString::fromLocal8Bit("选择编码方向"));
		addStream1Dir();
		break;
	}
	case e_select_stream1_mirror:
		break;
	case e_select_stream2_resolution:
	{
		ui.label->setText(QString::fromLocal8Bit("选择分辨率"));
		addStream2Resolution();
		break;
	}
	case e_select_stream2_fps:
	{
		ui.label->setText(QString::fromLocal8Bit("选择编码帧率"));
		addStream2Fps();
		break;
	}
	case e_select_stream2_pushstreammode:
		break;
	case e_select_stream2_encodedir:
	{
		ui.label->setText(QString::fromLocal8Bit("选择编码方向"));
		addStream2Dir();
		break;
	}
	case e_select_stream2_mirror:
		break;
	default:
		break;
	}
}

void SelectList::initConnect()
{
	connect(ui.listWidget, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(SlotList(QListWidgetItem*)));
}

void SelectList::initParams()
{
	m_resolutionLst << "*120"
		<< "120 * 120"
		<< "160 * 120"
		<< "180 * 180"
		<< "*180"
		<< "240 * 180"
		<< "320 * 180"
		<< "*240"
		<< "240 * 240"
		<< "320 * 240"
		<< "424 * 240"
		<< "*360"
		<< "360 * 360"
		<< "480 * 360"
		<< "640 * 360"
		<< "*480"
		<< "480 * 480"
		<< "640 * 480"
		<< "840 * 360"
		<< QString::fromLocal8Bit("*其他")
		<< "960 * 720"
		<< "1280 * 720"
		<< "1920 * 1080";
}

void SelectList::SetListTitle(QListWidget* lst, QString iconText)
{
	lst->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	lst->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	QWidget* widget = new QWidget(lst);

	//创建自定义的item
	widget->setStyleSheet("background-color: #f7f8fa;");

	QLabel* textLabel = new QLabel(widget);
	textLabel->setObjectName("textLabel");
	textLabel->setFixedSize(169, 22);
	textLabel->setText(iconText);
	textLabel->setScaledContents(true);
	textLabel->setStyleSheet("QLabel{font-family: Microsoft YaHei; font-size: 12px; text-align: left; color:#cfd3de;}");

	QHBoxLayout* horLayout = new QHBoxLayout;
	horLayout->setMargin(0);
	horLayout->setSpacing(0);
	horLayout->setContentsMargins(8, 0, 0, 0);
	horLayout->addWidget(textLabel);
	horLayout->addSpacing(184);
	widget->setLayout(horLayout);

	//backLabel->clear();
	//将widget作为列表的item
	QListWidgetItem* item = new QListWidgetItem();
	item->setFlags(Qt::NoItemFlags);
	QSize size = item->sizeHint();
	item->setSizeHint(QSize(200, 44));
	lst->addItem(item);
	widget->setSizeIncrement(size.width(), 44);
	lst->setItemWidget(item, widget);
}
void SelectList::SetList(QListWidget* lst, QString iconText, QString icon, int width, int spacing)
{
	lst->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	lst->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	QWidget* widget = new QWidget(lst);

	//创建自定义的item
	widget->setStyleSheet("background:transparent;");

	QLabel* titleTextLabel = new QLabel(widget);
	titleTextLabel->setObjectName("titleTextLabel");
	titleTextLabel->setFixedSize(width, 22);
	titleTextLabel->setText(iconText);
	titleTextLabel->setToolTip(iconText);
	titleTextLabel->setScaledContents(true);
	titleTextLabel->setStyleSheet("QLabel{font-family: Microsoft YaHei; font-size: 12px; text-align: left; color:#64728c;}");

	QLabel* backLabel = new QLabel(widget);
	if (icon == "ok.png")
	{
		std::cout << "";
	}
	QPixmap backPix("./resource/image/" + icon);
	backLabel->setObjectName("backLabel");
	backLabel->setFixedSize(24, 24);
	backLabel->setPixmap(backPix);
	backLabel->setScaledContents(true);
	backLabel->setStyleSheet("background:transparent;");
	backLabel->setLayoutDirection(Qt::LayoutDirection::RightToLeft);

	QHBoxLayout* horLayout = new QHBoxLayout;

	horLayout->setMargin(0);
	horLayout->setSpacing(0);

	horLayout->setContentsMargins(16, 0, 16, 0);
	horLayout->addWidget(titleTextLabel);
	horLayout->addSpacing(spacing);
	horLayout->addWidget(backLabel);
	
	widget->setLayout(horLayout);

	//backLabel->clear();
	//将widget作为列表的item
	QListWidgetItem* item = new QListWidgetItem();

	QSize size = item->sizeHint();
	item->setSizeHint(QSize(200, 44));
	lst->addItem(item);
	widget->setSizeIncrement(size.width(), 44);
	lst->setItemWidget(item, widget);
}

void SelectList::addCamList()
{
	QString name, id, cur;

	int count = RzObject::instance()->GetCamDeviceCount();
	RzObject::instance()->GetCamDevice(cur);

	for (int i = 0; i < count; i++)
	{
		RzObject::instance()->GetCamDevice(i, name, id);
		SetList(ui.listWidget, name, name == cur ? "ok.png" : "");
	}
}

void SelectList::addMicList()
{
	QString name, id, cur;

	int count = RzObject::instance()->GetMicDeviceCount();
	RzObject::instance()->GetMicDevice(cur);

	for (int i = 0; i < count; i++)
	{
		RzObject::instance()->GetMicDevice(i, name, id);
		SetList(ui.listWidget, name,  name == cur ? "ok.png" : "");
	}
}

void SelectList::addSpeakerList()
{
	QString name, id, cur;

	int count = RzObject::instance()->GetSpeakerDeviceCount();
	RzObject::instance()->GetSpeakerDevice(cur);

	/*QVariant value;
	Settings::instance()->Read("speaker", value, "Device");
	bool isClose = value.toString() == QString::fromLocal8Bit("关闭扬声器") ? true : false;*/

	for (int i = 0; i < count; i++)
	{
		RzObject::instance()->GetSpeakerDevice(i, name, id);
		SetList(ui.listWidget, name, name == cur ? "ok.png" : "");
	}
	//SetList(ui.listWidget, QString::fromLocal8Bit("关闭扬声器"), isClose ? "ok.png" : "");
}

void SelectList::addStream1Resolution()
{
	QVariant value;
	Settings::instance()->Read("Resolution", value, "stream1");
	for (int i = 0; i < m_resolutionLst.count(); i++)
	{
		if (i == 0 || i == 4 || i == 7 || i == 11 || i == 15 || i == 19)
			SetListTitle(ui.listWidget, m_resolutionLst[i]);
		else
			SetList(ui.listWidget, m_resolutionLst[i], value.toString() == m_resolutionLst[i] ? "ok.png" : ""/*, 50, 20*/);
	}
}
void SelectList::addStream1Fps()
{
	QStringList lst;
	QVariant value;
	lst << "10 FPS" << "15 FPS" << "30 FPS" << "60 FPS";
	Settings::instance()->Read("Fps", value, "stream1");
	for (int i = 0; i < lst.count(); i++)
	{
		SetList(ui.listWidget, lst[i], value.toString() == lst[i] ? "ok.png" : ""/*, 50, 20*/);
	}
}

void SelectList::addStream1PushStreamMode()
{
	//QStringList lst;
	//QVariant value;
	//lst << QString::fromLocal8Bit("单流") << QString::fromLocal8Bit("双流");
	//Settings::instance()->Read("PushStreamMode", value, "stream1");
	//for (int i = 0; i < lst.count(); i++)
	//{
	//	SetList(ui.listWidget, lst[i], value.toString() == lst[i] ? "ok.png" : ""/*, 50, 20*/);
	//}
}

void SelectList::addStream1Dir()
{
	QStringList lst;
	QVariant value;
	lst << QString::fromLocal8Bit("自适应") << QString::fromLocal8Bit("横向") << QString::fromLocal8Bit("竖向");;
	Settings::instance()->Read("EncodeDir", value, "stream1");
	for (int i = 0; i < lst.count(); i++)
	{
		SetList(ui.listWidget, lst[i], value.toString() == lst[i] ? "ok.png" : ""/*, 50, 20*/);
	}
}
void SelectList::addStream1Mirror()
{
	//QStringList lst;
	//QVariant value;
	//lst << QString::fromLocal8Bit("自适应") << QString::fromLocal8Bit("横向") << QString::fromLocal8Bit("竖向");;
	//Settings::instance()->Read("EncodeDir", value, "stream1");
	//for (int i = 0; i < lst.count(); i++)
	//{
	//	SetList(ui.listWidget, lst[i], value.toString() == lst[i] ? "ok.png" : ""/*, 50, 20*/);
	//}
}
void SelectList::addStream2Resolution()
{
	QVariant value;
	Settings::instance()->Read("Resolution", value, "stream2");
	for (int i = 0; i < m_resolutionLst.count(); i++)
	{
		if (i == 0 || i == 4 || i == 7 || i == 11 || i == 15 || i == 19)
			SetListTitle(ui.listWidget, m_resolutionLst[i]);
		else
			SetList(ui.listWidget, m_resolutionLst[i], value.toString() == m_resolutionLst[i] ? "ok.png" : ""/*, 50, 20*/);
	}
}
void SelectList::addStream2Fps()
{
	QStringList lst;
	QVariant value;
	lst << "10 FPS" << "15 FPS" << "30 FPS" << "60 FPS";
	Settings::instance()->Read("Fps", value, "stream2");
	for (int i = 0; i < lst.count(); i++)
	{
		SetList(ui.listWidget, lst[i], value.toString() == lst[i] ? "ok.png" : ""/*, 50, 20*/);
	}
}
void SelectList::addStream2PushStreamMode()
{

}
void SelectList::addStream2Dir()
{
	QStringList lst;
	QVariant value;
	lst << QString::fromLocal8Bit("自适应") << QString::fromLocal8Bit("横向") << QString::fromLocal8Bit("竖向");;
	Settings::instance()->Read("EncodeDir", value, "stream2");
	for (int i = 0; i < lst.count(); i++)
	{
		SetList(ui.listWidget, lst[i], value.toString() == lst[i] ? "ok.png" : ""/*, 50, 20*/);
	}
}
void SelectList::addStream2Mirror()
{

}