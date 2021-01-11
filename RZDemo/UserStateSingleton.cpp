#include "UserStateSingleton.h"
UserStateSingleton* UserStateSingleton::m_pUserStateSingleton = nullptr;

UserStateSingleton* UserStateSingleton::instance()
{
	if (!m_pUserStateSingleton)
	{
		m_pUserStateSingleton = new UserStateSingleton;
		m_pUserStateSingleton->init();
	}
	return m_pUserStateSingleton;
}

void UserStateSingleton::SetLocalAudioState(LOCAL_AUDIO_STREAM_STATE state, LOCAL_AUDIO_STREAM_ERROR error)
{
	std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_local_audio].get());
	m_LocalAudioStreamState = state;
}

void UserStateSingleton::SetLocalAudioDeviceState(LOCAL_AUDIO_STREAM_STATE state, LOCAL_AUDIO_STREAM_ERROR error)
{
	std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_local_audio_device].get());
	m_LocalAudioDeviceState = state;
}

void UserStateSingleton::SetLocalVideoState(LOCAL_VIDEO_STREAM_STATE state, LOCAL_VIDEO_STREAM_ERROR error, const std::string streamName)
{
	std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_local_video].get());
	if (streamName.empty())
		m_LocalVideoStreamState = state;
	else
		m_SelfLocalVideoStreamStateMap[streamName] = state;
}

void UserStateSingleton::SetLocalVideoDeviceState(LOCAL_VIDEO_STREAM_STATE state, LOCAL_VIDEO_STREAM_ERROR error, const std::string streamName)
{
	std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_local_video_device].get());
	m_LocalVideoDeviceState = state;
}

void UserStateSingleton::SetAudioPublishState(const std::string& channelid, PUBLISH_STREAM_STATE state)
{

}

void UserStateSingleton::SetVideoPublishState(const std::string& channelid, PUBLISH_STREAM_STATE state)
{

}

const LOCAL_AUDIO_STREAM_STATE& UserStateSingleton::GetLocalAudioState()
{
	std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_local_audio].get());
	return m_LocalAudioStreamState;
}

const LOCAL_AUDIO_STREAM_STATE& UserStateSingleton::GetLocalAudioDeviceState()
{
	std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_local_audio_device].get());
	return m_LocalAudioDeviceState;
}

const LOCAL_VIDEO_STREAM_STATE& UserStateSingleton::GetLocalVideoState(const std::string streamName)
{
	std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_local_video].get());
	if (streamName.empty())
		return m_LocalVideoStreamState;
	else
		return m_SelfLocalVideoStreamStateMap[streamName];
}

const LOCAL_VIDEO_STREAM_STATE& UserStateSingleton::GetLocalVideoDeviceState(const std::string streamName)
{
	std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_local_video_device].get());
	return m_LocalVideoDeviceState;
}

const PUBLISH_STREAM_STATE& UserStateSingleton::GetAudioPublishState()
{
	return PUBLISH_STREAM_STREAM_STATE_ERROR;
}

const PUBLISH_STREAM_STATE& UserStateSingleton::GetVideoPublishState()
{
	return PUBLISH_STREAM_STREAM_STATE_ERROR;
}

void UserStateSingleton::SetRtcStats(const std::string& channelid, const RtcStats& stats)
{
	std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_rtc_stats].get());
	m_RtcStatsMap[channelid] = stats;
}

int UserStateSingleton::GetRtcStats(const std::string& channelid, RtcStats& stats)
{
	std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_rtc_stats].get());
	auto iter = m_RtcStatsMap.find(channelid);
	if (iter != m_RtcStatsMap.end()) {
		stats = iter->second;
		return 0;
	}
	return -1;
}

//void UserStateSingleton::SetLocalVideoState(const std::string& channelid, const std::string& streamName, LOCAL_VIDEO_STREAM_STATE state)
//{
//
//}
//
//const LOCAL_VIDEO_STREAM_STATE& UserStateSingleton::GetLocalVideoState(const std::string& channelid, const std::string& streamName)
//{
//	return LOCAL_VIDEO_STREAM_STATE_NO_SEND;
//}

//remote
void UserStateSingleton::SetAudioSubscribeState(const std::string& channelid, const std::string& uid, SUBSCRIBE_STREAM_STATE state)
{
	std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_audio_sub].get());
	m_RemoteAudioSubStateMap[uid] = state;
}

//void UserStateSingleton::SetVideoSubscribeState(const std::string& channelid, const std::string& uid, const std::string& streamName, SUBSCRIBE_STREAM_STATE state)
//{
//
//}

const SUBSCRIBE_STREAM_STATE& UserStateSingleton::GetAudioSubscribeState(const std::string& uid)
{
	SUBSCRIBE_STREAM_STATE state = SUBSCRIBE_STREAM_STATE_FAILED;
	auto iter = m_RemoteAudioSubStateMap.find(uid);
	if (iter != m_RemoteAudioSubStateMap.end())
	{
		state = m_RemoteAudioSubStateMap[uid];
		return state;
	}
	return SUBSCRIBE_STREAM_STATE_NO_SEND;
}

int UserStateSingleton::GetVideoSubscribeState(const std::string& uid, const std::string& streamName, SUBSCRIBE_STREAM_STATE& state)
{
	std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_local_multi_video].get());
	STREAMINFO info(uid, streamName);
	auto iter = m_RemoteVideoStreamMap.find(info);
	if (iter != m_RemoteVideoStreamMap.end())
	{
		state = m_RemoteVideoStreamMap[info];
		return 0;
	}
	return -1;
		
}

void UserStateSingleton::AddRemoteUser(const std::string& uid)
{
	std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_remote_user_list].get());
	m_RemoteUserList.insert(uid);
}

void UserStateSingleton::DelRemoteUser(const std::string& uid)
{
	{
		std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_remote_user_list].get());
		auto iter = m_RemoteUserList.find(uid);
		if (iter != m_RemoteUserList.end())
		{
			m_RemoteUserList.erase(iter);
		}
	}
	
	{
		//清空该uid对应视频流列表
		std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_local_multi_video].get());
		for (int i = 0; i < m_RemoteVideoStreamMap.size(); i++)
		{
			for (auto iter = m_RemoteVideoStreamMap.begin(); iter != m_RemoteVideoStreamMap.end(); iter++)
			{
				if (iter->first.uid == uid)
				{
					m_RemoteVideoStreamMap.erase(iter);
					break;
				}
			}
		}
	}
}

void UserStateSingleton::ClearRemoteUserList()
{
	std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_remote_user_list].get());
	std::set<std::string> lst;
	m_RemoteUserList.swap(lst);
}

const std::set<std::string>& UserStateSingleton::GetRemoteUserList()
{
	std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_remote_user_list].get());
	return m_RemoteUserList;
}

void UserStateSingleton::UpdateRemoteUserVideoStreamMap(const std::string& uid, const std::string& streamName, SUBSCRIBE_STREAM_STATE state)
{
	//只存放上线和下线两种状态
	std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_local_multi_video].get());
	STREAMINFO info;
	info.uid = uid;
	info.streamName = streamName;
	m_RemoteVideoStreamMap[info] = state;
}

const std::map<STREAMINFO, SUBSCRIBE_STREAM_STATE>& UserStateSingleton::GetRemoteUserVideoStreamMap()
{
	std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_local_multi_video].get());
	return m_RemoteVideoStreamMap;
}

void UserStateSingleton::ClearRemoteUserVideoStreamMap()
{
	std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_local_multi_video].get());
	std::map<STREAMINFO, SUBSCRIBE_STREAM_STATE> mp;
	m_RemoteVideoStreamMap.swap(mp);
}

void UserStateSingleton::SetFirstRemoteVideoFrame(const QString& channelid, const QString& uid, const QString& streamName, int width, int height)
{
	std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_remote_fisrt].get());
	STREAMINFO info(uid.toStdString(), streamName.toStdString());
	VIDEOFRAMESIZE videoSize(width, height);
	m_RemoteFirstVideoFrameMap[info] = videoSize;
}

void UserStateSingleton::GetFirstRemoteVideoFrame(const QString& channelid, const QString& uid, const QString& streamName, VIDEOFRAMESIZE& size)
{
	std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_remote_fisrt].get());
	STREAMINFO info(uid.toStdString(), streamName.toStdString());
	size = m_RemoteFirstVideoFrameMap[info];
}

void UserStateSingleton::SetRemoteVideoStats(REMOTEVIDEOSTATS info)
{
	std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_remote_video_stats].get());
	STREAMINFO keyInfo;
	keyInfo.uid = info.uid.toStdString();
	keyInfo.streamName = info.streamName.toStdString();
	m_RemoteVideoStats[keyInfo] = info;
}

void UserStateSingleton::GetRemoteVideoStats(const QString& uid, const QString& streamName, REMOTEVIDEOSTATS& info)
{
	std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_remote_video_stats].get());
	STREAMINFO keyInfo(uid.toStdString(), streamName.toStdString());
	info = m_RemoteVideoStats[keyInfo];
}

void UserStateSingleton::SetRemoteVolume(const QString& uid, int volume)
{
	std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_remote_volume].get());
	m_RemoteVolumeMap[uid.toStdString()] = volume;
}

int  UserStateSingleton::GetRemoteVolume(const QString& uid)
{
	std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_remote_volume].get());
	return m_RemoteVolumeMap[uid.toStdString()];
}

void UserStateSingleton::SetRemoteNetQulity(const QString& uid, int quality)
{
	std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_remote_net_qulity].get());
	m_RemoteNetQulityMap[uid.toStdString()] = quality;
}

int UserStateSingleton::GetRemoteNetQulity(const QString& uid)
{
	std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_remote_net_qulity].get());
	return m_RemoteNetQulityMap[uid.toStdString()];
}

void UserStateSingleton::SetRecvAllVideoState(bool state)
{
	m_isRecvAllVideo = state;
}

bool UserStateSingleton::GetRecvAllVideoState()
{
	return m_isRecvAllVideo;
}

void UserStateSingleton::SetRecvAllAudioState(bool state)
{
	m_isRecvAllAudio = state;
}

bool UserStateSingleton::GetRecvAllAudioState()
{
	return m_isRecvAllAudio;
}

void UserStateSingleton::SetIsMirror(bool state)
{
	m_isMirror = state;
}

bool UserStateSingleton::GetIsMirror()
{
	return m_isMirror;
}

void UserStateSingleton::SetIsMirror2(bool state)
{
	m_isMirror2 = state;
}

bool UserStateSingleton::GetIsMirror2()
{
	return m_isMirror2;
}

void UserStateSingleton::SetProcessData(QString data)
{
	//std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_process_data].get());
	//m_processDataList << data;
	QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigProcessData", Q_ARG(QString, data));
}

const QStringList& UserStateSingleton::GetProcessData()
{
	std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_process_data].get());
	return m_processDataList;
}

void UserStateSingleton::Clear()
{
	{
		std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_remote_user_list].get());
		std::set<std::string> tempset;
		m_RemoteUserList.swap(tempset);
	}

	{
		std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_rtc_stats].get());
		std::map<std::string, RtcStats> tempmap;
		m_RtcStatsMap.swap(tempmap);
	}

	{
		std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_local_multi_video].get());
		std::map<STREAMINFO, SUBSCRIBE_STREAM_STATE> tempmap;
		m_RemoteVideoStreamMap.swap(tempmap);
	}

	{
		std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_remote_fisrt].get());
		std::map<STREAMINFO, VIDEOFRAMESIZE> mp;
		m_RemoteFirstVideoFrameMap.swap(mp);
	}

	{
		std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_local_video].get());
		std::map<std::string, LOCAL_VIDEO_STREAM_STATE> mp;
		m_SelfLocalVideoStreamStateMap.swap(mp);
	}
	
	{
		std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_remote_video_stats].get());
		std::map<STREAMINFO, REMOTEVIDEOSTATS> mp;
		m_RemoteVideoStats.swap(mp);
	}

	{
		std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_remote_volume].get());
		std::map<std::string, int> mp;
		m_RemoteVolumeMap.swap(mp);
	}
	{
		std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_remote_net_qulity].get());
		std::map<std::string, int> mp;
		m_RemoteNetQulityMap.swap(mp);
	}

	{
		std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_process_data].get());
		m_processDataList.swap(QStringList());
	}
}

void UserStateSingleton::SetmChannelLoadedFlag(bool flag)
{
	std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_channel_loaded].get());
	m_isChannelLoaded = flag;
}

bool UserStateSingleton::GetmChannelLoadedFlag()
{
	std::lock_guard<std::mutex> lck(*m_mutexMap[state_info_channel_loaded].get());
	return m_isChannelLoaded;
}

void UserStateSingleton::init()
{
	for (int i = state_info_local_audio; i <= state_info_remote_fisrt; i++)
	{
		m_mutexMap[(STATE_INFO_ENUM)i] = std::unique_ptr<mutex_wrapper>(new mutex_wrapper);
	}
}

UserStateNotifySingleton* UserStateNotifySingleton::m_pUserStateNotifySingleton = nullptr;
UserStateNotifySingleton* UserStateNotifySingleton::instance()
{
	if (!m_pUserStateNotifySingleton)
	{
		m_pUserStateNotifySingleton = new UserStateNotifySingleton;
		m_pUserStateNotifySingleton->init();
	}
	return m_pUserStateNotifySingleton;
}

void UserStateNotifySingleton::init()
{
	qRegisterMetaType<LOCAL_VIDEO_STREAM_STATE>("LOCAL_VIDEO_STREAM_STATE");
	qRegisterMetaType<LOCAL_VIDEO_STREAM_ERROR>("LOCAL_VIDEO_STREAM_ERROR");
	qRegisterMetaType<LOCAL_AUDIO_STREAM_STATE>("LOCAL_AUDIO_STREAM_STATE");
	qRegisterMetaType<LOCAL_AUDIO_STREAM_ERROR>("LOCAL_AUDIO_STREAM_ERROR");
	qRegisterMetaType<AudioVolumeInfo>("AudioVolumeInfo");
	qRegisterMetaType<MEDIA_DEVICE_TYPE>("MEDIA_DEVICE_TYPE");
	qRegisterMetaType<AUDIO_ROUTE_TYPE>("AUDIO_ROUTE_TYPE");
	qRegisterMetaType<NETWORK_TYPE>("NETWORK_TYPE");
	qRegisterMetaType<LocalAudioStats>("LocalAudioStats");
	qRegisterMetaType<LocalVideoStats>("LocalVideoStats");
	qRegisterMetaType<QUALITY_TYPE>("QUALITY_TYPE");
	qRegisterMetaType<LastmileProbeResult>("LastmileProbeResult");
	qRegisterMetaType<RtcStats>("RtcStats");
	qRegisterMetaType<CLIENT_ROLE_TYPE>("CLIENT_ROLE_TYPE");
	qRegisterMetaType<USER_OFFLINE_REASON_TYPE>("USER_OFFLINE_REASON_TYPE");
	qRegisterMetaType<PUBLISH_STREAM_STATE>("PUBLISH_STREAM_STATE");
	qRegisterMetaType<SUBSCRIBE_STREAM_STATE>("SUBSCRIBE_STREAM_STATE");
	qRegisterMetaType<CONNECTION_STATE_TYPE>("CONNECTION_STATE_TYPE");
	qRegisterMetaType<CONNECTION_CHANGED_REASON_TYPE>("CONNECTION_CHANGED_REASON_TYPE");
	qRegisterMetaType<RemoteAudioStats>("RemoteAudioStats");
	qRegisterMetaType<REMOTEVIDEOSTATS>("REMOTEVIDEOSTATS");
	qRegisterMetaType<E_VIDEO_ITEM_TYPE>("E_VIDEO_ITEM_TYPE");
	qRegisterMetaType<AUDIOVOLUMEINFO>("AUDIOVOLUMEINFO");
	qRegisterMetaType<VIDEO_MIRROR_MODE_TYPE>("VIDEO_MIRROR_MODE_TYPE");
	qRegisterMetaType<RENDER_MODE_TYPE>("RENDER_MODE_TYPE");
}