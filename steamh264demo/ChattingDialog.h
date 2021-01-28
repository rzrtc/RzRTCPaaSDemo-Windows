#pragma once

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMouseEvent>
#include <GLYuvWidget.h>
#include <thread>
class ChattingDialog : public QDialog
{
    Q_OBJECT

public:
    ChattingDialog(QDialog* parent = Q_NULLPTR);
    ~ChattingDialog();
    void setLabelChanel(QString channel);
    void setLabelSelf(QString uid);
    void setLabelRemote(QString uid);

protected:
    void mousePressEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
    void closeEvent(QCloseEvent* event) override;

private:
    GLYuvWidget *videoSelf, *videoRemote;
    QLabel* labelHeader;
    QPushButton *buttonClose, *buttonMinimize;

    QPoint m_start;
    QPoint m_clickPos;
    bool m_leftButtonPressed = false;
    bool isRemoteUserExist = false;

    void drawWindow();

signals:
    void SigShowYuv(const QByteArray&, uint, uint);

private slots:
    void SlotJoinChannelSuccess(const QString& channelid, const QString& uid, int elapsed);
    void SlotUserJoined(const QString& channelid, const QString& uid, int elapsed);
    void SlotUserOffline();
    void SlotLeaveChannel();
    void SlotChannelError(const QString& channel, int err, const QString& errmsg);

    void slotClose();
    void slotMinimize();

private:
    void threadFunc();

private:
    std::thread m_t;
};
