#pragma once

#include <QDialog>
#include "ui_LostDialog.h"

class LostDialog : public QDialog
{
	Q_OBJECT

public:
	LostDialog(QWidget *parent = Q_NULLPTR);
	~LostDialog();
public:
	void SetText(QString str1,QString str2, QString str3="");
	void SetContent(QString str1, QString str2);
	void SetButton(QString str);
	void SetIsClose(bool is);
signals:
	void SigBack();
public slots:
	void SlotQuit();
	void SlotCancel();
protected:
	void paintEvent(QPaintEvent* event) override;

private:
	void init();
	void initStyleSheetFile();
	void initConnect();
private:
	Ui::LostDialog ui;
	bool m_isClose = false;
};
