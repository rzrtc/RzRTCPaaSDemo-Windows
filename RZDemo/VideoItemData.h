#pragma once
/*
* VideoItem数据处理类
*/
#include <QObject>
#include <QLabel>
#include <QPushbutton>
class VideoItemData : public QObject
{
	Q_OBJECT

public:
	VideoItemData(QObject *parent);
	~VideoItemData();
public:
	void SetLocalVideoButtonStyle(QPushButton* bt, bool isSend);
	void SetLocalAudioButtonStyle(QPushButton* bt, bool isSend);
	void SetLocalAudioVolumeLabelStyleActive(QLabel* label, bool isSend);

	void SetRemoteVideoButtonStyleActive(QPushButton* bt, bool isPull);
	void SetRemoteAudioButtonStyleActive(QPushButton* bt, bool isPull);
	void SetRemoteAudioVolumeLabelStyleActive(QLabel* label, bool isPull);

	void SetRemoteVideoButtonStyleUnactive(QPushButton* bt, bool isSend);
	void SetRemoteAudioButtonStyleUnactive(QPushButton* bt, bool isSend);
	void SetRemoteAudioVolumeLabelStyleUnactive(QLabel* label, bool isSend);

	void SetHdButtonStyle(QPushButton* bt, bool isHd);
};
