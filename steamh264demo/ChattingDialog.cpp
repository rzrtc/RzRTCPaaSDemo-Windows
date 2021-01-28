#include <QFile>
#include <Windows.h>
#include "ChattingDialog.h"
#include "RTC.h"
#include <RZEngineEventHandler.h>
#include <RZRtcChannelEventHandler.cpp>
#include <VideoCapInput.h>
#include <OpenGLSignalMngr.h>
#include "CutomizedVideoSource.h"
#include "UserStateNotifySingleton.h"


static IRtcChannel* rtcChannel = nullptr;
static IRtcStream* rtcStream = nullptr;
ChattingDialog::ChattingDialog(QDialog* parent)
    : QDialog(parent)
{
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    QFile qssFile("./resource/chatting.qss");
    if (!qssFile.open(QFile::ReadOnly))
        return; //TODO:

    this->setStyleSheet(QString(qssFile.readAll()));
    this->setFixedSize(1400, 768);
    qssFile.close();
    drawWindow();

    connect(OpenGLSignalMngr::instance(), SIGNAL(SigShowYuv(const QByteArray&, uint, uint)), videoSelf, SLOT(SlotShowYuv(const QByteArray&, uint, uint)));
    connect(OpenGLSignalMngr::instance(), SIGNAL(SigShowYuv2(const QByteArray&, uint, uint)), videoRemote, SLOT(SlotShowYuv(const QByteArray&, uint, uint)));

    connect(UserStateNotifySingleton::instance(), SIGNAL(SigJoinChannelSuccess(const QString&, const QString&, int)), this, SLOT(SlotJoinChannelSuccess(const QString&, const QString&, int)));
    connect(UserStateNotifySingleton::instance(), SIGNAL(SigUserJoined(const QString&, const QString&, int)), this, SLOT(SlotUserJoined(const QString&, const QString&, int)));
    connect(UserStateNotifySingleton::instance(), SIGNAL(SigUserOffline()), this, SLOT(SlotUserOffline()));
    connect(UserStateNotifySingleton::instance(), SIGNAL(SigChannelError(const QString&, int, const QString&)), this, SLOT(SlotChannelError(const QString&, int, const QString&)));
    
}



ChattingDialog::~ChattingDialog()
{
    if (videoSelf)
        delete videoSelf;
    if (videoRemote)
        delete videoRemote;
}

void ChattingDialog::setLabelChanel(QString channel)
{
    labelHeader->setText(QString::fromLocal8Bit("频道ID：") + channel);
}

void ChattingDialog::setLabelSelf(QString uid)
{
    videoSelf->setTipText(uid + QString::fromLocal8Bit("（我）"));
}

void ChattingDialog::setLabelRemote(QString uid)
{
    videoRemote->setTipText(uid);
}

void ChattingDialog::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        m_leftButtonPressed = true;
        m_start = e->globalPos();
        m_clickPos = e->pos();
    }
}

void ChattingDialog::mouseMoveEvent(QMouseEvent* e)
{
    if (m_leftButtonPressed)
    {
        move(e->globalPos() - m_clickPos);
        m_start = e->globalPos();
    }
}

void ChattingDialog::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        m_leftButtonPressed = false;
    }
}

void ChattingDialog::closeEvent(QCloseEvent* event)
{
    if (this->isHidden())
        return;
    this->close();
}

void ChattingDialog::drawWindow()
{
    videoSelf = new GLYuvWidget(this);
    videoSelf->setObjectName("videoSelf");
    videoSelf->setGeometry(100, 60, 600, 448);

    videoRemote = new GLYuvWidget(this);
    videoRemote->setObjectName("videoRemote");
    videoRemote->setGeometry(700, 60, 600, 448);

    labelHeader = new QLabel(QString::fromLocal8Bit("频道ID：0"), this);
    labelHeader->setObjectName("labelHeader");
    labelHeader->setGeometry(0, 0, 1400, 60);


    buttonMinimize = new QPushButton(this);
    buttonMinimize->setObjectName("buttonMinimize");
    buttonMinimize->move(1296, 20);

    buttonClose = new QPushButton(this);
    buttonClose->setObjectName("buttonClose");
    buttonClose->move(1360, 20);

    connect(buttonClose, SIGNAL(clicked()), this, SLOT(slotClose()));
    connect(buttonMinimize, SIGNAL(clicked()), this, SLOT(slotMinimize()));
}

void ChattingDialog::SlotJoinChannelSuccess(const QString& channelid, const QString& uid, int elapsed)
{
    RTC::instance()->publish();
    setLabelSelf(uid);
}

void ChattingDialog::SlotUserJoined(const QString& channelid, const QString& uid, int elapsed)
{
    if (isRemoteUserExist)
        return;
    RTC::instance()->setRemoteVideoSink(uid.toStdString().c_str());
    setLabelRemote(uid);
    isRemoteUserExist = true;
}

void ChattingDialog::SlotUserOffline()
{
    setLabelRemote("");
    isRemoteUserExist = false;
}

void ChattingDialog::SlotLeaveChannel()
{
    RTC::instance()->leaveChannel();
    setLabelSelf("");
}

void ChattingDialog::SlotChannelError(const QString& channel, int err, const QString& errmsg)
{
    RTC::showTipDialog("频道错误", errmsg.toStdString().c_str());
}

void ChattingDialog::slotClose()
{
    RTC::instance()->leaveChannel();
    this->close();
}

void ChattingDialog::slotMinimize()
{
    this->showMinimized();
}

void ChattingDialog::threadFunc()
{
    const int pixel_w = 320;
    const int pixel_h = 180;
    unsigned char buf[pixel_w * pixel_h * 3 / 2];
    FILE* infile = fopen("test_yuv420p_320x180.yuv", "rb");
    if (infile == nullptr)
        return;
    while (1)
    {
        if (fread(buf, 1, pixel_w * pixel_h * 3 / 2, infile) != pixel_w * pixel_h * 3 / 2)
        {
            // Loop
            fseek(infile, 0, SEEK_SET);
            fread(buf, 1, pixel_w * pixel_h * 3 / 2, infile);
        }

        QByteArray arr((char*)buf, pixel_w * pixel_h * 3 / 2);
        emit SigShowYuv(arr, (uint)pixel_w, (uint)pixel_h);
        Sleep(30);
    }
}
