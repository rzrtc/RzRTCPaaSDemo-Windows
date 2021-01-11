#pragma once

#include <QObject>
#include <QSettings>
#include <QMutex>
#include <QMutexLocker>

class Settings : public QObject
{
	Q_OBJECT

	Settings(QObject* parent) {}
	~Settings() {}
public:
	static Settings* instance();
	void Write(const QString& key, const QVariant& value, const QString& org = QString());
	bool Read(const QString& key, QVariant& value, const QString& org = QString());
private:
	QMutex m_mutex;
	static Settings* m_pSettings;
};
