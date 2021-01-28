#include "VideoConverter.h"

VideoConverter::VideoConverter(int width, int height)
{
	codec = avcodec_find_encoder(AV_CODEC_ID_H264);
	if (!codec)
	{
		exit(1);
	}
	c = avcodec_alloc_context3(codec);
	if (!c)
	{
		exit(1);
	}
	pkt = av_packet_alloc();
	if (!pkt)
		exit(1);

	c->bit_rate = 1200 * 1024;
	c->width = width;
	c->height = height;
	c->time_base = AVRational{ 1, 15 };
	c->framerate = AVRational{ 15, 1 };
	c->gop_size = 10;
	// ²»Òªmax_b_frames
	c->max_b_frames = 0;
	c->pix_fmt = AV_PIX_FMT_YUV420P;

	if (codec->id == AV_CODEC_ID_H264)
		av_opt_set(c->priv_data, "preset", "ultrafast", 0);

	av_opt_set(c->priv_data, "tune", "zerolatency", 0);
	av_opt_set(c->priv_data, "profile", "high", 0);

	int ret = avcodec_open2(c, codec, nullptr);
	if (ret < 0)
		exit(1);

	frame = av_frame_alloc();
	if (!frame)
		exit(1);

	frame->format = c->pix_fmt;
	frame->width = c->width;
	frame->height = c->height;

	ret = av_frame_get_buffer(frame, 0);
	if (ret < 0)
		exit(1);
}

VideoConverter::~VideoConverter()
{
	avcodec_free_context(&c);
	av_frame_free(&frame);
	av_packet_free(&pkt);
}

bool VideoConverter::encode(const uint8_t* buffer, uint8_t** out, int& size, bool& isKey)
{
	int ret;
	pkt->pts = pts;

	int y_size = frame->height * frame->width;
	frame->data[0] = const_cast<uint8_t*>(buffer);
	frame->data[1] = const_cast<uint8_t*>(buffer + y_size);
	frame->data[2] = const_cast<uint8_t*>(buffer + y_size * 5 / 4);

	frame->linesize[0] = frame->width;
	frame->linesize[1] = frame->width / 2;
	frame->linesize[2] = frame->width / 2;

	/* send the frame to the encoder */
	ret = avcodec_send_frame(c, frame);
	if (ret < 0)
		return false;

	while (ret >= 0)
	{
		ret = avcodec_receive_packet(c, pkt);
		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
			break;
		else if (ret < 0)
			return false;

		if (*out)
			delete[](*out);
		*out = new uint8_t[pkt->size];
		memcpy(*out, pkt->data, pkt->size);
		isKey = pkt->flags;
		size = pkt->size;
		//fwrite(pkt->data, 1, pkt->size, outfile);
		av_packet_unref(pkt);
		pts++;
	}
	return true;
}




