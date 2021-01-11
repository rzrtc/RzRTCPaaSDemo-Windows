#include "LostDialog.h"
#include <QPainter>
#include <QFileInfo>
#include "UserStateSingleton.h"
LostDialog::LostDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::Tool);
	setAttribute(Qt::WA_TranslucentBackground, true);
	init();
}

LostDialog::~LostDialog()
{
}

void LostDialog::SetText(QString str1, QString str2, QString str3)
{
	ui.label->setText(str1);
	ui.label_2->setText(str2);
	if (str3.isEmpty() == false)
	{
		str2.append("\n");
		str2.append(str3);
		ui.label_2->setText(str2);
	}
	ui.label->adjustSize();
	ui.label_2->adjustSize();
	ui.label->setWordWrap(true);
	ui.label_2->setWordWrap(true);
}

void LostDialog::SetContent(QString str1, QString str2)
{
	//ui.label->setText()
}

void LostDialog::SetButton(QString str)
{
	ui.pushButton->setText(str);
}


void LostDialog::SetIsClose(bool is)
{
	m_isClose = is;
}

void LostDialog::SlotQuit()
{
	if(!m_isClose)
		QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigCloseSigtonDlg", Qt::DirectConnection);
	this->close();
	if (!m_isClose)
		emit SigBack();
}

void LostDialog::SlotCancel()
{
	this->close();
}

void LostDialog::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);

	QPainter painter(this);

	// QColor最后一个参数80代表alpha通道，一般用作透明度
	painter.fillRect(rect(), QColor(56, 69, 93, 51));
}


void LostDialog::init()
{
	initStyleSheetFile();
	initConnect();
	ui.label->adjustSize();
	ui.label_2->adjustSize();
	ui.label->setWordWrap(true);
	ui.label_2->setWordWrap(true);
}

void LostDialog::initStyleSheetFile()
{
	QFile styleFile("./resource/loss.qss");
	styleFile.open(QIODevice::ReadOnly);
	this->setStyleSheet(QString(styleFile.readAll()));
}

void LostDialog::initConnect()
{
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(SlotQuit()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(SlotCancel()));
}