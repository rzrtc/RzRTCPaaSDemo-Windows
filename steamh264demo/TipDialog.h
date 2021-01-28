#pragma once

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>

class TipDialog : public QDialog
{
    Q_OBJECT

public:
    TipDialog(QDialog* parent = Q_NULLPTR);
    ~TipDialog();
    void setContent(const char* title, const char* content);
private slots:
    void slotClose();

private:
    QLabel* labelHeader = nullptr;
    QPushButton* buttonClose = nullptr;
    QTextEdit* info = nullptr;

    void drawWindow();
};
