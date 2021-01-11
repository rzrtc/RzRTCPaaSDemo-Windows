#pragma once

#include <QDialog>
#include "ui_SelectList.h"
#include <QMouseEvent>
#include <QListWidget>
#include "common.h"
class SelectList : public QDialog
{
	Q_OBJECT
public:
	SelectList(E_SELECT_LIST_TYPE type, QWidget *parent = Q_NULLPTR);
	~SelectList();
public slots:
	void SlotClose();
	void SlotList(QListWidgetItem* item);
protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* ev) override;
	void paintEvent(QPaintEvent* event) override;
private:
	void init();
	void initStyleSheetFile();
	void initCtrl();
	void initConnect();
	void initParams();
private:
	void SetListTitle(QListWidget* lst, QString iconText);
	void SetList(QListWidget* lst, QString iconText, QString icon, int width = 90, int spacing = 200);
	void addCamList();
	void addMicList();
	void addSpeakerList();
	void addStream1Resolution();
	void addStream1Fps();
	void addStream1PushStreamMode();
	void addStream1Dir();
	void addStream1Mirror();
	void addStream2Resolution();
	void addStream2Fps();
	void addStream2PushStreamMode();
	void addStream2Dir();
	void addStream2Mirror();


private:
	Ui::SelectList ui;
	E_SELECT_LIST_TYPE m_type;
	QStringList m_resolutionLst;
};
