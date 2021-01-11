#pragma once
/*
* 本端用户和远端用户audio，video状态处理类
* 从回调中保存状态，同时发射状态改变信号
* 状态存储；状态通知
*/
#include <mutex>
#include <set>
#include <QObject>
#include <QMetaObject>
#include <QMetaType>
#include <IRtcEngineState.h>
#include <QString>
#include <QMap>
#include "common.h"
using namespace rz;

struct mutex_wrapper : std::mutex
{
	mutex_wrapper() = default;
	mutex_wrapper(mutex_wrapper const&) noexcept : std::mutex() {}
	bool operator==(mutex_wrapper const& other) noexcept { return this == &other; }
};

class UserStateSingleton
{
	enum STATE_INFO_ENUM {
		state_info_local_audio, state_info_local_audio_device, state_info_local_video, state_info_local_video_device,  state_info_rtc_stats, \
		state_info_audio_sub, state_info_video_sub, state_info_remote_user_list,\
		state_info_local_multi_video, state_info_remote_video_stats, state_info_remote_volume,\
		state_info_remote_net_qulity, state_info_process_data, state_info_channel_loaded, state_info_remote_fisrt
	};
public:
	static UserStateSingleton* instance();
	//local default
	void SetLocalAudioState(LOCAL_AUDIO_STREAM_STATE state, LOCAL_AUDIO_STREAM_ERROR error);
	void SetLocalAudioDeviceState(LOCAL_AUDIO_STREAM_STATE state, LOCAL_AUDIO_STREAM_ERROR error);
	void SetLocalVideoState(LOCAL_VIDEO_STREAM_STATE state, LOCAL_VIDEO_STREAM_ERROR error, const std::string streamName = "");
	void SetLocalVideoDeviceState(LOCAL_VIDEO_STREAM_STATE state, LOCAL_VIDEO_STREAM_ERROR error, const std::string streamName = "");
	const LOCAL_AUDIO_STREAM_STATE& GetLocalAudioState();
	const LOCAL_AUDIO_STREAM_STATE& GetLocalAudioDeviceState();
	const LOCAL_VIDEO_STREAM_STATE& GetLocalVideoState(const std::string streamName = "");
	const LOCAL_VIDEO_STREAM_STATE& GetLocalVideoDeviceState(const std::string streamName = "");

	//暂不使用
	void SetAudioPublishState(const std::string& channelid, PUBLISH_STREAM_STATE state);
	void SetVideoPublishState(const std::string& channelid, PUBLISH_STREAM_STATE state);
	const PUBLISH_STREAM_STATE& GetAudioPublishState();
	const PUBLISH_STREAM_STATE& GetVideoPublishState();
	//获取统计信息
	void SetRtcStats(const std::string& channelid, const RtcStats& stats);
	int GetRtcStats(const std::string& channelid, RtcStats& stats);

	//local 自定义推流
	/*void SetLocalVideoState(const std::string& channelid, const std::string& streamName, LOCAL_VIDEO_STREAM_STATE state);
	const LOCAL_VIDEO_STREAM_STATE& GetLocalVideoState(const std::string& channelid, const std::string& streamName);*/

	//remote
	void SetAudioSubscribeState(const std::string& channelid, const std::string& uid, SUBSCRIBE_STREAM_STATE state);
	//弃用远端视频流状态，统一使用UpdateRemoteUserVideoStreamMap
	//void SetVideoSubscribeState(const std::string& channelid, const std::string& uid, const std::string& streamName, SUBSCRIBE_STREAM_STATE state);
	const SUBSCRIBE_STREAM_STATE& GetAudioSubscribeState(const std::string& uid);
	int GetVideoSubscribeState(const std::string& uid, const std::string& streamName, SUBSCRIBE_STREAM_STATE& state);
	//远端用户列表
	void AddRemoteUser(const std::string& uid);
	void DelRemoteUser(const std::string& uid);
	void ClearRemoteUserList();
	const std::set<std::string>& GetRemoteUserList();

	//远端用户视频流状态，主要兼容多路流情形
	void UpdateRemoteUserVideoStreamMap(const std::string& uid, const std::string& streamName, SUBSCRIBE_STREAM_STATE state);
	const std::map<STREAMINFO, SUBSCRIBE_STREAM_STATE>& GetRemoteUserVideoStreamMap();
	void ClearRemoteUserVideoStreamMap();
	//远端用户视频第一帧是否来到
	void SetFirstRemoteVideoFrame(const QString& channelid, const QString& uid, const QString& streamName, int width, int height);
	void GetFirstRemoteVideoFrame(const QString& channelid, const QString& uid, const QString& streamName, VIDEOFRAMESIZE &size);
	//远端视频统计信息
	void SetRemoteVideoStats(REMOTEVIDEOSTATS info);
	void GetRemoteVideoStats(const QString& uid, const QString& streamName, REMOTEVIDEOSTATS& info);

	//远端用户音量
	void SetRemoteVolume(const QString& uid, int volume);
	int  GetRemoteVolume(const QString& uid);

	//网络质量
	void SetRemoteNetQulity(const QString& uid, int quality);
	int GetRemoteNetQulity(const QString& uid);

	//频道内接受所有音频视频按钮
	void SetRecvAllVideoState(bool state);
	bool GetRecvAllVideoState();
	void SetRecvAllAudioState(bool state);
	bool GetRecvAllAudioState();

	void SetIsMirror(bool state);
	bool GetIsMirror();
	void SetIsMirror2(bool state);
	bool GetIsMirror2();
	//流程数据
	void SetProcessData(QString data);
	const QStringList& GetProcessData();

	void Clear();

	//频道页面是否加载完成
	void SetmChannelLoadedFlag(bool flag);
	bool GetmChannelLoadedFlag();
private:
	void init();
private:
	static UserStateSingleton* m_pUserStateSingleton;
	/*state*/
	//local
	LOCAL_AUDIO_STREAM_STATE m_LocalAudioStreamState = LOCAL_AUDIO_STREAM_STATE_SENDING;
	LOCAL_AUDIO_STREAM_STATE m_LocalAudioDeviceState = LOCAL_AUDIO_STREAM_STATE_STOPPED;
	LOCAL_VIDEO_STREAM_STATE m_LocalVideoStreamState = LOCAL_VIDEO_STREAM_STATE_SENDING;
	LOCAL_VIDEO_STREAM_STATE m_LocalVideoDeviceState = LOCAL_VIDEO_STREAM_STATE_STOPPED;


	std::set<std::string> m_RemoteUserList;	//远端用户列表
	std::map<STATE_INFO_ENUM, std::unique_ptr<mutex_wrapper>> m_mutexMap;
	std::map<std::string, RtcStats>	m_RtcStatsMap;	//channelid 
	std::map<STREAMINFO, SUBSCRIBE_STREAM_STATE>	m_RemoteVideoStreamMap;	//远端用户视频流状态
	std::map<STREAMINFO, VIDEOFRAMESIZE>	m_RemoteFirstVideoFrameMap;
	std::map<std::string, LOCAL_VIDEO_STREAM_STATE> m_SelfLocalVideoStreamStateMap;	//自定义推流本端状态
	std::map<STREAMINFO, REMOTEVIDEOSTATS> m_RemoteVideoStats;	//远端视频流统计数据
	std::map<std::string, int>	m_RemoteVolumeMap;
	std::map<std::string, int>	m_RemoteNetQulityMap;
	std::map<std::string, SUBSCRIBE_STREAM_STATE> m_RemoteAudioSubStateMap;
	QStringList m_processDataList;
	volatile bool m_isRecvAllVideo = true;
	volatile bool m_isRecvAllAudio = true;
	volatile bool m_isMirror = false;
	volatile bool m_isMirror2 = false;
	volatile bool m_isChannelLoaded = false;
};

class UserStateNotifySingleton : public QObject
{
	Q_OBJECT
public:
	static UserStateNotifySingleton* instance();
signals:
	void SigWarning(int warn, const QString& msg);
	void SigError(int err, const QString& msg);
	void SigLocalVideoStateChanged(LOCAL_VIDEO_STREAM_STATE state, LOCAL_VIDEO_STREAM_ERROR error);
	void SigLocalAudioStateChanged(LOCAL_AUDIO_STREAM_STATE state, LOCAL_AUDIO_STREAM_ERROR error);
	void SigAudioVolumeIndication(const AUDIOVOLUMEINFO& speakers, unsigned int speakerNumber, int totalVolume);
	void SigFirstLocalVideoFrame(int width, int height, int elapsed);
	void SigFirstLocalVideoFramePublished(int elapsed);
	void SigMediaDeviceStateChanged(const QString& deviceId, MEDIA_DEVICE_TYPE deviceType, int deviceState);
	void SigAudioDeviceVolumeChanged(MEDIA_DEVICE_TYPE deviceType, int volume, bool muted);
	void SigVideoSizeChanged(int width, int height);
	void SigFirstLocalAudioFramePublished(int elapsed);
	void SigAudioRouteChanged(AUDIO_ROUTE_TYPE routing);
	void SigNetworkTypeChanged(NETWORK_TYPE type);
	void SigLocalAudioStats(const LocalAudioStats& stats);
	void SigLocalVideoStats(const LocalVideoStats& stats);
	void SigLocalNetworkQuality(QUALITY_TYPE txQuality, QUALITY_TYPE rxQuality);
	void SigLastmileProbeResult(const LastmileProbeResult& result);
//remote
	void SigWarning(const QString& channelid, int warn, const QString& msg);
	void SigError(const QString& channelid, int err, const QString& msg);
	void SigJoinChannelSuccess(const QString& channelid, const QString& uid, int elapsed);
	void SigRejoinChannelSuccess(const QString& channelid, const QString& uid, int elapsed);
	void SigLeaveChannel(const QString& channelid, const RtcStats& stats);
	void SigClientRoleChanged(const QString& channelid, CLIENT_ROLE_TYPE oldRole, CLIENT_ROLE_TYPE newRole, int elapsed);
	void SigUserJoined(const QString& channelid, const QString& uid, int elapsed);
	void SigUserOffline(const QString& channelid, const QString& uid, USER_OFFLINE_REASON_TYPE reason);
	void SigAudioPublishStateChanged(const QString& channelid, PUBLISH_STREAM_STATE state, int elapsed);
	void SigVideoPublishStateChanged(const QString& channelid, PUBLISH_STREAM_STATE state, int elapsed);
	void SigAudioSubscribeStateChanged(const QString& channelid, const QString& uid, SUBSCRIBE_STREAM_STATE state, int elapsed);
	void SigVideoSubscribeStateChanged(const QString& channelid, const QString& uid, const QString& streamName, SUBSCRIBE_STREAM_STATE state, int elapsed);
	void SigFirstRemoteVideoFrame(const QString& channelid, const QString& uid, const QString& streamName, int width, int height, int elapsed);
	void SigVideoSizeChanged(const QString& channelid, const QString& uid, const QString& streamName, int width, int height, int elapsed);
	void SigNetworkQuality(const QString& channelid, const QString& uid, QUALITY_TYPE txQuality, QUALITY_TYPE rxQuality);
	void SigConnectionLost(const QString& channelid);
	void SigConnectionStateChanged(const QString& channelid, CONNECTION_STATE_TYPE state, CONNECTION_CHANGED_REASON_TYPE reason);
	void SigRtcStats(QString channelid, const RtcStats& stats);
	void SigRemoteAudioStats(const QString& channelid, const RemoteAudioStats& stats);
	void SigRemoteVideoStats(const QString& channelid, const REMOTEVIDEOSTATS& stats);
//多路流stream
	void SigVideoPublishStateChanged(const QString& channelid, const QString& streamName, PUBLISH_STREAM_STATE state, int elapsed) ;
	void SigLocalVideoStateChanged(const QString& channelid, const QString& streamName, LOCAL_VIDEO_STREAM_STATE state, LOCAL_VIDEO_STREAM_ERROR error, int elapsed) ;
	void SigFirstVideoFramePublished(const QString& channelid, const QString& streamName, int elapsed) ;
	void SigVideoSizeChanged(const QString& channelid, const QString& streamName, int width, int height) ;

	//关闭所有漂浮dialog
	void SigCloseSigtonDlg();

	//流程数据
	void SigProcessData(QString data);

	//切换摄像头
	void SigSwitchCam();

	void SigVideoItemUpdate(bool);
private:
	void init();
private:
	static UserStateNotifySingleton* m_pUserStateNotifySingleton;
};
Q_DECLARE_METATYPE(LOCAL_VIDEO_STREAM_STATE)
Q_DECLARE_METATYPE(LOCAL_VIDEO_STREAM_ERROR)
Q_DECLARE_METATYPE(LOCAL_AUDIO_STREAM_STATE)
Q_DECLARE_METATYPE(LOCAL_AUDIO_STREAM_ERROR)
Q_DECLARE_METATYPE(AudioVolumeInfo)
Q_DECLARE_METATYPE(MEDIA_DEVICE_TYPE)
Q_DECLARE_METATYPE(AUDIO_ROUTE_TYPE)
Q_DECLARE_METATYPE(NETWORK_TYPE)
Q_DECLARE_METATYPE(LocalAudioStats)
Q_DECLARE_METATYPE(LocalVideoStats)
Q_DECLARE_METATYPE(QUALITY_TYPE)
Q_DECLARE_METATYPE(LastmileProbeResult)
Q_DECLARE_METATYPE(RtcStats)
Q_DECLARE_METATYPE(CLIENT_ROLE_TYPE)
Q_DECLARE_METATYPE(USER_OFFLINE_REASON_TYPE)
Q_DECLARE_METATYPE(PUBLISH_STREAM_STATE)
Q_DECLARE_METATYPE(SUBSCRIBE_STREAM_STATE)
Q_DECLARE_METATYPE(CONNECTION_STATE_TYPE)
Q_DECLARE_METATYPE(CONNECTION_CHANGED_REASON_TYPE)
Q_DECLARE_METATYPE(RemoteAudioStats)
Q_DECLARE_METATYPE(REMOTEVIDEOSTATS)
Q_DECLARE_METATYPE(E_VIDEO_ITEM_TYPE)
Q_DECLARE_METATYPE(AUDIOVOLUMEINFO)
Q_DECLARE_METATYPE(VIDEO_MIRROR_MODE_TYPE)
Q_DECLARE_METATYPE(RENDER_MODE_TYPE)

/*
LOCAL_VIDEO_STREAM_STATE
LOCAL_VIDEO_STREAM_ERROR
LOCAL_AUDIO_STREAM_STATE
LOCAL_AUDIO_STREAM_ERROR
AudioVolumeInfo
MEDIA_DEVICE_TYPE
AUDIO_ROUTE_TYPE
NETWORK_TYPE
LocalAudioStats
LocalVideoStats
QUALITY_TYPE
LastmileProbeResult
RtcStats
CLIENT_ROLE_TYPE
USER_OFFLINE_REASON_TYPE
PUBLISH_STREAM_STATE
SUBSCRIBE_STREAM_STATE
CONNECTION_STATE_TYPE
CONNECTION_CHANGED_REASON_TYPE
RemoteAudioStats
RemoteVideoStats
*/
