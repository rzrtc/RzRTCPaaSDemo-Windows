#pragma once

#include <QDialog>
#include "ui_TipDialog.h"
#include <QMouseEvent>
#include <QPaintEvent>
class TipDialog : public QDialog
{
	Q_OBJECT

public:
	TipDialog(QWidget *parent = Q_NULLPTR);
	~TipDialog();
	void SetText(QString str);
public slots:
	void SlotClose();
protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* ev) override;
	void paintEvent(QPaintEvent* event) override;
private:
	Ui::TipDialog ui;
};
