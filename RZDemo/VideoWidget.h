#pragma once

#include <QWidget>
#include "ui_VideoWidget.h"

class VideoWidget : public QWidget
{
	Q_OBJECT

public:
	VideoWidget(QWidget *parent = Q_NULLPTR);
	~VideoWidget();
protected:
	//QPaintEngine* paintEngine() const;
private:
	Ui::VideoWidget ui;
};
