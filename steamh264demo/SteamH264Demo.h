#pragma once

#include <QtWidgets/QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
//#include "ui_SteamH264Demo.h"

class ChattingDialog;
class SteamH264Demo : public QWidget
{
    Q_OBJECT

public:
    SteamH264Demo(QWidget* parent = Q_NULLPTR);
    ~SteamH264Demo();

protected:
    void mousePressEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
    void closeEvent(QCloseEvent* event) override;

private:
    void drawWindow();

    QLabel *labelTitle, *labelChanel, *labelUID;
    QLineEdit *editChanel, *editUID;
    QPushButton *button, *buttonClose, *buttonMinimize, *buttonline;
    ChattingDialog* chattingDialog;

    QPoint m_start;
    QPoint m_clickPos;
    bool m_leftButtonPressed = false;
private slots:
    void slotEntry();
    void slotClose();
    void slotMinimize();
};
