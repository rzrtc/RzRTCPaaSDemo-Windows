#include "VideoWidget.h"
#include <QTimer>
VideoWidget::VideoWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//this->setAttribute(Qt::WA_StaticContents);
	//this->setAttribute(Qt::WA_OpaquePaintEvent, true);
	//this->setAttribute(Qt::WA_PaintOnScreen, true);
	//this->setAttribute(Qt::WA_DontCreateNativeAncestors, true);
	//this->setAttribute(Qt::WA_NativeWindow, true);
	//this->setAttribute(Qt::WA_NoSystemBackground, true);
	//this->setAttribute(Qt::WA_MSWindowsUseDirect3D, true);
	//this->setAutoFillBackground(false);
	/*QTimer* timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(20);*/
	//setUpdatesEnabled(false);
}

VideoWidget::~VideoWidget()
{
}

//QPaintEngine* VideoWidget::paintEngine() const
//{
//     return NULL;
// }