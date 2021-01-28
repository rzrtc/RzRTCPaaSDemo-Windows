#pragma once

#include <QObject>
#include <IRtcEngineState.h>
using namespace rz;
class OpenGLSignalMngr : public QObject
{
	Q_OBJECT

public:
	static OpenGLSignalMngr* instance();
signals:
	void SigShowYuv(const QByteArray& arr, uint w, uint h);
    void SigShowYuv2(const QByteArray& arr, uint w, uint h);
	
private:
	static OpenGLSignalMngr* m_pOpenGLSignalMngr;
};
