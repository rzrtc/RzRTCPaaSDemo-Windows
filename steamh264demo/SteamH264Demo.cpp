#include <QFile>

#include "SteamH264Demo.h"
#include "ChattingDialog.h"
#include "RTC.h"
#include <QApplication>

SteamH264Demo::SteamH264Demo(QWidget* parent)
    : QWidget(parent)
{
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    //ui.setupUi(this);


    QFile qssFile("./resource/welcome.qss");
    if (!qssFile.open(QFile::ReadOnly))
        ; //TODO:

    this->setStyleSheet(QString(qssFile.readAll()));
    this->setFixedSize(1200, 720);
    qssFile.close();
    drawWindow();
    RTC::instance()->init("appid");
    // 错误APPID测试
    //RTC::instance()->init("Erv4Q0KB14Y85qma");
}



SteamH264Demo::~SteamH264Demo()
{
    if (labelTitle)
        delete labelTitle;
    if (labelUID)
        delete labelUID;
    if (editChanel)
        delete editChanel;
    if (editUID)
        delete editUID;
    if (button)
        delete button;
    /* if (chattingDialog)
        delete chattingDialog;*/
}


void SteamH264Demo::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        m_leftButtonPressed = true;
        m_start = e->globalPos();
        m_clickPos = e->pos();
    }
}

void SteamH264Demo::mouseMoveEvent(QMouseEvent* e)
{
    if (m_leftButtonPressed)
    {
        move(e->globalPos() - m_clickPos);
        m_start = e->globalPos();
    }
}

void SteamH264Demo::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        m_leftButtonPressed = false;
    }
}

void SteamH264Demo::closeEvent(QCloseEvent* event)
{
    if (this->isHidden())
        return;
    else
    {
        //RzObject::instance()->Release();
        this->close();
    }
}

void SteamH264Demo::drawWindow()
{
    // labelTitle
    labelTitle = new QLabel("H264.Demo", this);
    labelTitle->setObjectName("labelTitle");
    labelTitle->move(182, 252);

    // labelChanel
    labelChanel = new QLabel(QString::fromLocal8Bit("频道ID"), this);
    labelChanel->setObjectName("labelChanel");
    labelChanel->move(647, 214);

    // labelUID
    labelUID = new QLabel(QString::fromLocal8Bit("用户ID"), this);
    labelUID->setObjectName("labelUID");
    labelUID->move(647, 279);

    // editChanel
    editChanel = new QLineEdit(this);
    editChanel->setObjectName("editChanel");
    editChanel->move(647, 242);


    // editUID
    editUID = new QLineEdit(this);
    editUID->setObjectName("editUID");
    editUID->move(647, 307);

    // button
    button = new QPushButton(QString::fromLocal8Bit("进入频道"), this);
    button->setObjectName("button");
    button->move(647, 352);

    buttonMinimize = new QPushButton(this);
    buttonMinimize->setObjectName("buttonMinimize");
    buttonMinimize->move(1100, 20);

    buttonClose = new QPushButton(this);
    buttonClose->setObjectName("buttonClose");
    buttonClose->move(1144, 20);

    buttonline = new QPushButton(this);
    buttonline->setObjectName("buttonline");
    buttonline->setFocusPolicy(Qt::NoFocus);
    buttonline->setGeometry(567, 223, 1, 160);

    connect(button, SIGNAL(clicked()), this, SLOT(slotEntry()));
    connect(buttonClose, SIGNAL(clicked()), this, SLOT(slotClose()));
    connect(buttonMinimize, SIGNAL(clicked()), this, SLOT(slotMinimize()));
}

void SteamH264Demo::slotClose()
{
    // TODO: release
    this->showMinimized();
    RTC::instance()->release();
    this->close();
    QApplication::quit();
}

void SteamH264Demo::slotMinimize()
{
    this->showMinimized();
}

void SteamH264Demo::slotEntry()
{
    if (editChanel->text().isEmpty() || editUID->text().isEmpty())
    {
        return;
    }

    
    if (!RTC::instance()->createChannel(editChanel->text().toStdString().c_str()))
        return;
    if (!RTC::instance()->joinChannel(editUID->text().toStdString().c_str()))
        return;
    this->hide();
    ChattingDialog c;
    c.setLabelChanel(editChanel->text().toStdString().c_str());
    c.show();
    RTC::instance()->show();
    c.exec();
    this->show();
}