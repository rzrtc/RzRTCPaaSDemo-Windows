#include "GLYuvWidget.h"
#include <QDebug>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QPainter>
#include <thread>

#define VERTEXIN 0
#define TEXTUREIN 1


GLYuvWidget::GLYuvWidget(QWidget* parent, Qt::WindowFlags f) : QOpenGLWidget(parent, f)
{
    pRender = new YUV420P_Render;
    labelTip = new QLabel( this);
    //labelTip->adjustSize();
    setTipText("");
    labelTip->setObjectName("labelTip");
    labelTip->move(12, 7);
}

GLYuvWidget::~GLYuvWidget()
{
    if (pRender)
    {
        delete pRender;
        pRender = nullptr;
    }
}

void GLYuvWidget::initializeGL()
{
    pRender->initialize();
}


void GLYuvWidget::paintGL()
{
    if (m_ba.size() == 0 || m_w == 0 || m_h == 0)
    {
        pRender->clear();
        return;
    }


    int x, y, w, h;
    double fper1, fper2;
    double rcW = width();
    double rcH = height();
    fper1 = rcW / rcH;
    fper2 = (double)m_w / (double)m_h;
    if ((fper1 - fper2) > 1e-6)
    {
        y = 0;
        h = rcH;
        int tempw = fper2 * rcH;
        x = fabs(rcW - tempw) / 2;
        w = tempw;
    }
    else
    {
        x = 0;
        w = rcW;
        double tempH = (double)rcW / fper2;
        y = fabs(rcH - tempH) / 2;
        h = tempH;
    }
    pRender->setGLView(x, y, w, h);

    pRender->render(m_ba, m_w, m_h, 0);
    m_ba.clear();
    m_w = 0;
    m_h = 0;
}

void GLYuvWidget::SlotShowYuv(const QByteArray& ptr, uint width, uint height)
{
    if (ptr.size() == 0 || width == 0 || height == 0)
        return;


    m_ba = ptr;

    m_w = width;
    m_h = height;
    update();
}

void GLYuvWidget::resizeGL(int w, int h)
{
    update();
}

void GLYuvWidget::setTipText(QString uid)
{
    if (uid.isEmpty())
        labelTip->setText(QString::fromLocal8Bit("ÎÞÐÅºÅ"));
    else
        labelTip->setText(uid);

}
