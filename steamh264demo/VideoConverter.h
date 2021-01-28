#pragma once
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
}


class VideoConverter
{
public:
	VideoConverter(int width = 1280, int height = 720);
	~VideoConverter();
	bool encode(const uint8_t* buffer, uint8_t** out, int& size, bool& isKey);

private:
	AVCodec* codec = nullptr;
	AVCodecContext* c = nullptr;
	AVFrame* frame = nullptr;
	AVPacket* pkt = nullptr;
	uint32_t pts = 0;
};