#pragma once

#include <QDialog>
#include "ui_LoadingDialog.h"

class LoadingDialog : public QDialog
{
	Q_OBJECT

public:
	LoadingDialog(QWidget *parent = Q_NULLPTR);
	~LoadingDialog();
public slots:
	void SlotClose();
	void SlotStartProgressBar();
private:
	void initStyleSheetFile();
	void initConnect();
	void initControl();
private:
	Ui::LoadingDialog ui;
};
