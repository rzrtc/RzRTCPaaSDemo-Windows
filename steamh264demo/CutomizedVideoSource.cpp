#include "CutomizedVideoSource.h"
#include <Windows.h>
#include <QDebug>
#include <QElapsedTimer>
bool CutomizedVideoSource::onInitialize(IVideoFrameConsumer* consumer)
{
	m_pConsumer = consumer;
	return true;
}

void CutomizedVideoSource::onDispose()
{
	m_pConsumer = nullptr;
}

int CutomizedVideoSource::onStart()
{
    m_isStart = true;
    return 0;
}

void CutomizedVideoSource::onStop()
{
    m_isStart = false;
}

void CutomizedVideoSource::SetParams(const std::string& channelId, const std::string& streamName)
{
}

void CutomizedVideoSource::onVideoData(uint8_t* y, int w, int h)
{
	std::lock_guard<std::mutex> lck(m_mutex);
	if(m_isStart)
        m_pConsumer->consumeVideoPacket(y, w, VIDEO_STREAM_H264, h, GetTickCount());
	/*	if(m_isStart)
		m_pConsumer->consumeVideoFrame((const unsigned char*)y, VIDEO_PIXEL_I420, w, h, GetTickCount());*/
}

