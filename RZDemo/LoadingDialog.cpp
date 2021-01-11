#include "LoadingDialog.h"
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include "UserStateSingleton.h"
#include "ChannelDialog.h"
#include "Settings.h"
LoadingDialog::LoadingDialog(QWidget *parent)
	: QDialog(parent)
{
	this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
	ui.setupUi(this);
	initStyleSheetFile();
	initConnect();
	initControl();
}

LoadingDialog::~LoadingDialog()
{
	qDebug() << "LoadingDialog xigou";
}

void LoadingDialog::SlotClose()
{
	this->close();
}

void LoadingDialog::SlotStartProgressBar()
{
	ui.progressBar->setRange(0, 1000000);
	for (int i = 0; i < 1000000; i++) {
		ui.progressBar->setValue(i);
	}
}

void LoadingDialog::initStyleSheetFile()
{
	QFile styleFile("./resource/loading.qss");
	QFileInfo fileInfo("./resource/loading.qss");
	//QString str = fileInfo.absolutePath();

	styleFile.open(QIODevice::ReadOnly);
	this->setStyleSheet(QString(styleFile.readAll()));
}

void LoadingDialog::initConnect()
{
}

void LoadingDialog::initControl()
{
	ui.progressBar->setMinimum(0);
	ui.progressBar->setMaximum(0);
}