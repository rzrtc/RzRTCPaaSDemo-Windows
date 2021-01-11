#include "Settings.h"
#include <QTextCodec>
Settings* Settings::m_pSettings = nullptr;

Settings* Settings::instance()
{
	if (!m_pSettings)
	{
		m_pSettings = new Settings(nullptr);
	}
	return m_pSettings;
}

void Settings::Write(const QString& key, const QVariant& value, const QString& org)
{
	QMutexLocker lck(&m_mutex);
	QSettings settings("./config/settings.ini", QSettings::IniFormat);
	settings.setIniCodec("GBK");
	settings.beginGroup(org);
	settings.setValue(key, value);
	settings.endGroup();	
}

bool Settings::Read(const QString& key, QVariant& value, const QString& org)
{
	QMutexLocker lck(&m_mutex);
	value.clear();
	QSettings settings("./config/settings.ini", QSettings::IniFormat);
	settings.setIniCodec("GBK");
	//QTextCodec *codec = QTextCodec::codecForName("system");
	//configIniRead->setIniCodec(codec);

	QString strKey = org;
	strKey += "/";
	strKey += key;

	value = settings.value(strKey).toString();
	
	if (value.isNull())
		return false;
	return true;
}
