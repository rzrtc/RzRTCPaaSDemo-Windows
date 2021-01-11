#include "TipDialog.h"
#include <QPainter>
TipDialog::TipDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::Tool);
	setAttribute(Qt::WA_TranslucentBackground, true);
}

TipDialog::~TipDialog()
{
}

void TipDialog::SetText(QString str)
{
	ui.label->setText(str);
}

void TipDialog::SlotClose()
{
	this->close();
}

void TipDialog::mousePressEvent(QMouseEvent* event)
{
	(void)event;
	//this->close();
}

void TipDialog::mouseReleaseEvent(QMouseEvent* event)
{
		this->close();
}

void TipDialog::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);

	QPainter painter(this);

	// QColor最后一个参数80代表alpha通道，一般用作透明度
	painter.fillRect(rect(), QColor(56, 69, 93, 51));
}