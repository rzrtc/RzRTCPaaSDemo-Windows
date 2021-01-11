#include "TitleBarWidget.h"
#include <QDebug>
TitleBarWidget::TitleBarWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

TitleBarWidget::~TitleBarWidget()
{
}

void TitleBarWidget::mousePressEvent(QMouseEvent* event)
{
    // 鼠标左键按下事件
    if (event->button() == Qt::LeftButton)
    {
        // 记录鼠标左键状态
        m_leftButtonPressed = true;
        //记录鼠标在屏幕中的位置
        m_start = event->globalPos();
    }

}

void TitleBarWidget::mouseMoveEvent(QMouseEvent* event)
{
    // 持续按住才做对应事件
    if (m_leftButtonPressed)
    {
        //将父窗体移动到父窗体原来的位置加上鼠标移动的位置：event->globalPos()-m_start
        parentWidget()->move(parentWidget()->geometry().topLeft() + event->globalPos() - m_start);
        //将鼠标在屏幕中的位置替换为新的位置
        m_start = event->globalPos();
    }
}

void TitleBarWidget::mouseReleaseEvent(QMouseEvent* event)
{
    // 鼠标左键释放
    if (event->button() == Qt::LeftButton)
    {
        // 记录鼠标状态
        m_leftButtonPressed = false;
    }
}
