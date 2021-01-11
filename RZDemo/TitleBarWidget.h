#pragma once

#include <QWidget>
#include "ui_TitleBarWidget.h"
#include <QMouseEvent>
class TitleBarWidget : public QWidget
{
	Q_OBJECT

public:
	TitleBarWidget(QWidget *parent = Q_NULLPTR);
	~TitleBarWidget();
protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
private:
	Ui::TitleBarWidget ui;

	QPoint m_start;
	QPoint m_clickPos;
	bool m_leftButtonPressed = false;
};
