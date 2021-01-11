#include "VideoItemData.h"

VideoItemData::VideoItemData(QObject *parent)
	: QObject(parent)
{
}

VideoItemData::~VideoItemData()
{
}

void VideoItemData::SetLocalVideoButtonStyle(QPushButton* bt, bool isSend)
{
	if (!bt)
		return;

	QString strStyle = isSend ? "QPushButton {background-image: url(./resource/image/item_cam_open.png); background-repeat: no-repeat;}" : "QPushButton {background-image: url(./resource/image/item_cam_red.png); background-repeat: no-repeat;}";
	bt->setStyleSheet(strStyle);
}

void VideoItemData::SetLocalAudioButtonStyle(QPushButton* bt, bool isSend)
{
	if (!bt)
		return;

	QString strStyle = isSend ? "QPushButton {background-image: url(./resource/image/item_cam_open.png); background-repeat: no-repeat;}" : "QPushButton {background-image: url(./resource/image/item_cam_red.png); background-repeat: no-repeat;}";
	bt->setStyleSheet(strStyle);
}

void VideoItemData::SetLocalAudioVolumeLabelStyleActive(QLabel* label, bool isSend)
{
	if (!label)
		return;

	QString strStyle = isSend ? "" : "";
	label->setStyleSheet(strStyle);
}

void VideoItemData::SetRemoteVideoButtonStyleActive(QPushButton* bt, bool isPull)
{
	if (!bt)
		return;

	QString strStyle = isPull ? "" : "";
	bt->setStyleSheet(strStyle);
}

void VideoItemData::SetRemoteAudioButtonStyleActive(QPushButton* bt, bool isPull)
{
	if (!bt)
		return;

	QString strStyle = isPull ? "" : "";
	bt->setStyleSheet(strStyle);
}

void VideoItemData::SetRemoteAudioVolumeLabelStyleActive(QLabel* label, bool isPull)
{
	if (!label)
		return;

	QString strStyle = isPull ? "" : "";
	label->setStyleSheet(strStyle);
}

void VideoItemData::SetRemoteVideoButtonStyleUnactive(QPushButton* bt, bool isSend)
{
	if (!bt)
		return;

	QString strStyle = isSend ? "" : "";
	bt->setStyleSheet(strStyle);
}

void VideoItemData::SetRemoteAudioButtonStyleUnactive(QPushButton* bt, bool isSend)
{
	if (!bt)
		return;

	QString strStyle = isSend ? "" : "";
	bt->setStyleSheet(strStyle);
}

void VideoItemData::SetRemoteAudioVolumeLabelStyleUnactive(QLabel* label, bool isSend)
{
	if (!label)
		return;

	QString strStyle = isSend ? "" : "";
	label->setStyleSheet(strStyle);
}

void VideoItemData::SetHdButtonStyle(QPushButton* bt, bool isHd)
{
	if (!bt)
		return;

	QString strStyle = isHd ? "" : "";
	bt->setStyleSheet(strStyle);
}

