#ifndef SWITCHBUTTON_H
#define SWITCHBUTTON_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

class SwitchButton : public QWidget
{
	Q_OBJECT

public:
	SwitchButton(QWidget* parent);
	~SwitchButton();
public:
	void SetState(bool state);
private:
	bool bSwitch;                     //开关状态:开true，关false

	QColor bgColorOn;                 //开状态时的背景颜色
	QColor bgColorOff;                //关状态时的背景颜色

	QColor sliderColor;
	QColor sliderColorOn;             //开状态时滑块的背景颜色
	QColor sliderColorOff;            //关状态时滑块的背景颜色

	QColor textColorOn;               //开状态时文字颜色
	QColor textColorOff;              //关状态时文字颜色

	QString strText;                  //文字内容

	QPoint startPoint;                //滑块移动的起始点
	QPoint endPoint;                  //滑块移动的终点   
	QPoint centerPoint;               //滑块移动的中间某点

	int mouseX;

	bool bPress;                      //左键是否按下

	bool bSelfState = false;		//手动设置

	bool bMove = false;
private:
	void paintEvent(QPaintEvent* e);
	void resizeEvent(QResizeEvent* e);
	void mousePressEvent(QMouseEvent* e);
	void mouseMoveEvent(QMouseEvent* e);
	void mouseReleaseEvent(QMouseEvent* e);
	void drawBg(QPainter& painter);        //绘制背景
	void drawSlidBlock(QPainter& painter); //绘制滑块
	void drawText(QPainter& painter);      //绘制文字

signals:
	void btnState(bool bswitch);
};

#endif // SWITCHBUTTON_H