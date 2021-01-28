#include "CustomizedVideoSink.h"
#include <QString>
#include "OpenGLSignalMngr.h"
#include "RTC.h"
#include <libyuv.h>
bool CustomizedVideoSink::onInitialize()
{
    return true;
}

bool CustomizedVideoSink::onStart()
{
    return true;
}

void CustomizedVideoSink::onStop()
{
}

void CustomizedVideoSink::onDispose()
{
}

rz::VideoSinkConfig CustomizedVideoSink::getSinkConfig()
{
    VideoSinkConfig cfg;
    cfg.streamType = VIDEO_STREAM_H264;
    cfg.pixelFormat = VIDEO_PIXEL_UNAVILABLE;
    return cfg;
}

void CustomizedVideoSink::consumeVideoFrame(const unsigned char* buffer, rz::VIDEO_PIXEL_FORMAT frameType, int width, int height, long timestamp)
{
    QByteArray arr((char*)buffer, width * height * 3 / 2);
    bool ret1 = QMetaObject::invokeMethod(OpenGLSignalMngr::instance(), "SigShowYuv2", Qt::DirectConnection, Q_ARG(const QByteArray&, arr), Q_ARG(uint, width), Q_ARG(uint, height));
}

void CustomizedVideoSink::consumeVideoPacket(const unsigned char* buffer, long length, rz::VIDEO_STREAM_TYPE streamType, bool isKey, long timestamp)
{
    //QByteArray arr((char*)buffer, width * height * 3 / 2);
    //bool ret1 = QMetaObject::invokeMethod(OpenGLSignalMngr::instance(), "SigShowYuv2", Qt::DirectConnection, Q_ARG(const QByteArray&, arr), Q_ARG(uint, width), Q_ARG(uint, height));
    memcpy(RTC::instance()->getDecoder()->inbuf, buffer, length);
    RTC::instance()->getDecoder()->decode(length, [](uint8_t* data, int width, int height) {
        QByteArray arr((char*)data, width * height * 3 / 2);
        bool ret1 = QMetaObject::invokeMethod(OpenGLSignalMngr::instance(), "SigShowYuv2", Qt::DirectConnection, Q_ARG(const QByteArray&, arr), Q_ARG(uint, width), Q_ARG(uint, height));
    });
    
}
