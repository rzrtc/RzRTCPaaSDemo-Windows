#pragma once

#include <QObject>
#include <QListWidget>
#include <QDebug>
#include <QFileInfo>
#include <QFile>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidgetItem>
#include <QString>
class ListWidgetSet : public QObject
{
	Q_OBJECT

public:
	ListWidgetSet(QObject *parent);
	~ListWidgetSet();

	static void LeftSetList(QListWidget* lst, QString imageFile, QString iconText, QString valueText, QString btImage,
		QString valueTextSheetStyle = "QLabel{ font-family: Microsoft YaHei; font-size: 12px; text-align: right; color:#cfd3de; }");
	
	static void SetList(QListWidget* lst, QString iconText, QString iconfile = "ok.png", int labelWidth = 727 - 24 - 16 * 2);
	static void SetListTitle(QListWidget* lst, QString iconText);

	
};
