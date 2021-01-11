#pragma once
#include <QMap>

//static QMap<QString, QString> s_ConfigMap{ 
//		{QString::fromLocal8Bit("测试环境"), "0"},
//		{QString::fromLocal8Bit("正式环境"), "34"},
//		{QString::fromLocal8Bit("自定义环境"), "34"},
//		{QString::fromLocal8Bit("软件编解码模式"), "34"},
//		{QString::fromLocal8Bit("硬件编解码模式"), "34"},
//		{QString::fromLocal8Bit("内置渲染器"), "34"},
//		{QString::fromLocal8Bit("自定义渲染器"), "34"},
//		{QString::fromLocal8Bit("等比缩放"), "34"},
//		{QString::fromLocal8Bit("填充"), "34"},
//		{QString::fromLocal8Bit("拉伸"), "34"},
//		{QString::fromLocal8Bit("单流"), "34"},
//		{QString::fromLocal8Bit("双流"), "34"},
//		{QString::fromLocal8Bit("120 * 120"), "34"},
//		{QString::fromLocal8Bit("160 * 120"), "34"},
//		{QString::fromLocal8Bit("180 * 120"), "34"},
//		{QString::fromLocal8Bit("240 * 180"), "34"},
//		{QString::fromLocal8Bit("320 * 180"), "34"},
//		{QString::fromLocal8Bit("240 * 240"), "34"},
//		{QString::fromLocal8Bit("320 * 240"), "34"},
//		{QString::fromLocal8Bit("424 * 240"), "34"},
//		{QString::fromLocal8Bit("360 * 360"), "34"},
//		{QString::fromLocal8Bit("480 * 360"), "34"},
//		{QString::fromLocal8Bit("640 * 360"), "34"},
//		{QString::fromLocal8Bit("480 * 480"), "34"},
//		{QString::fromLocal8Bit("640 * 480"), "34"},
//		{QString::fromLocal8Bit("840 * 360"), "34"},
//		{QString::fromLocal8Bit("960 * 720"), "34"},
//		{QString::fromLocal8Bit("1280 * 720"), "34"},
//		{QString::fromLocal8Bit("1920 * 1080"), "34"},
//		{QString::fromLocal8Bit("7 FPS"), "34"},
//		{QString::fromLocal8Bit("15 FPS"), "34"},
//		{QString::fromLocal8Bit("25 FPS"), "34"},
//		{QString::fromLocal8Bit("30 FPS"), "34"}
//};

//QStringList itemTextLst;
//itemTextLst << " *120" << "120 * 120" << "160 * 120" << ""


static QMap<QString, QString> s_ConfigMap{
		{"TestEnv", QString::fromLocal8Bit("测试环境")},
		{"Codec", QString::fromLocal8Bit("编解码")},
		{"RenderDevice", QString::fromLocal8Bit("渲染器")},
		{"RenderMode", QString::fromLocal8Bit("渲染方式")}
};

enum E_BING_TYPE {
	e_bind_role,
	e_bind_receive_a_stream,
	e_bind_receive_v_stream,
	e_bind_render_mode, 
	e_bind_open_log,
	e_bind_select_cam,
	e_bind_select_mic,
	e_bind_select_speaker, 
	e_bind_stream1_resolution, 
	e_bind_stream1_framerate, 
	e_bind_stream1_direction,
	e_bind_stream1_pushstreammode, 
	e_bind_stream1_mirror_mode,
	e_bind_stream2_resolution,
	e_bind_stream2_framerate,
	e_bind_stream2_direction,
	e_bind_stream2_pushstreammode,
	e_bind_stream2_mirror_mode
};
enum E_REMOTE_INFO_TYPE { 
	e_re_user_id,
	e_re_user_stream,
	e_re_video,
	e_re_video_state,
	e_re_resolution,
	e_re_resolution_value,
	e_re_framerate,
	e_re_framerate_value,
	e_re_audio,
	e_re_audio_state,
	e_re_volume,
	e_re_volume_value,
	e_re_bit,
	e_re_bit_value,
	e_re_loss,
	e_re_loss_value,
	e_re_netquality,
	e_re_netquality_value
};

enum E_SELECT_LIST_TYPE {
	e_select_device_cam,
	e_select_device_mic,
	e_select_device_speaker,
	e_select_device_render_mode,
	e_select_stream1_resolution,
	e_select_stream1_fps,
	e_select_stream1_pushstreammode,
	e_select_stream1_encodedir,
	e_select_stream1_mirror,
	e_select_stream2_resolution,
	e_select_stream2_fps,
	e_select_stream2_pushstreammode,
	e_select_stream2_encodedir,
	e_select_stream2_mirror
};

enum E_VIDEO_ITEM_TYPE {
	e_local_user,
	e_remote_user,
	e_init
};

typedef struct STREAMINFO_ {
	std::string uid;
	std::string streamName;
	STREAMINFO_() {}
	STREAMINFO_(std::string uid, std::string streamName)
	{
		this->uid = uid;
		this->streamName = streamName;
	}
	bool operator < (const STREAMINFO_& a) const
	{
		if (this->uid < a.uid || (this->uid == a.uid && this->streamName < a.streamName)) 
		{
			return true;
		}
		return false;
	}
} STREAMINFO;


typedef struct VIDEOFRAMESIZE_
{
	int w = 0;
	int h = 0;
	VIDEOFRAMESIZE_() {}
	VIDEOFRAMESIZE_(int w, int h)
	{
		this->w = w;
		this->h = h;
	}
}VIDEOFRAMESIZE;


typedef struct AUDIOVOLUMEINFO_
{
	QString uid;
	QString channelid;
	int volume;
	int vad;

}AUDIOVOLUMEINFO;

typedef struct REMOTEVIDEOSTATS_
{
	QString uid;                                         //用户ID
	QString streamName;                                  //流名字
	int width = 0;                                                     //视频流宽（像素）。
	int height = 0;                                                    //视频流高（像素）。
	int receivedBitrate = 0;                                           //（上次统计后）的接收码率(Kbps)。
	int decoderOutputFrameRate = 0;                                    //远端视频解码器的输出帧率，单位为 fps。
	int packetLossRate = 0;                                            //远端视频在使用抗丢包技术之后的丢包率(%)。
	int totalFrozenTime = 0;                                           //加入频道后该远端视频流发生视频卡顿的累计时长（ms）。通话过程中，视频帧率设置不低于 5 fps 时，连续渲染的两帧视频之间间隔超过 500 ms，则记为一次视频卡顿。
	int frozenRate = 0;                                                //加入频道后该远端视频流发生视频卡顿的累计时长占视频总有效时长的百分比 (%)。视频有效时长是指远端用户加入频道后视频未被停止发送或接收的时长。
	int totalActiveTime = 0;                                           //视频有效时长（毫秒），即远端用户/主播加入频道后，没有停止发送视频流的通话时长。
	int publishDuration = 0;
}REMOTEVIDEOSTATS;