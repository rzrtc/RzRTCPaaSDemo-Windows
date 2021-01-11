#include "RzObject.h"
#include "Settings.h"
#include "UserStateSingleton.h"
RzObject* RzObject::m_pRzObject = nullptr;

RzObject* RzObject::instance()
{
	if (!m_pRzObject)
	{
		m_pRzObject = new RzObject;
	}
	return m_pRzObject;
}

IRtcEngine* RzObject::GetIRtcEngine()
{
	if (m_pIRtcEngine)
	{
		if (m_pIRtcChannel)
		{
			m_pIRtcChannel->release();
			m_pIRtcChannel = nullptr;
		}
			

		IRtcEngine::release(true);
	}

	if(!m_pPrivateIRtcEngineEventHandler)
		m_pPrivateIRtcEngineEventHandler = new PrivateIRtcEngineEventHandler;
	if (m_ctx.appID)
		delete[] m_ctx.appID;
	m_ctx.appID = new char[256];
	strcpy((char*)m_ctx.appID, m_strAppId.toStdString().c_str());
	
	m_ctx.codecPriority = (CODEC_PRIORITY)m_nCodecPriority;
	if (m_ctx.envConfig) {
		delete[] m_ctx.envConfig;
		m_ctx.envConfig = nullptr;
	}
		
	
	m_ctx.envConfig = new char[512];
	strcpy((char*)m_ctx.envConfig, ENV_CONFIG);
	m_ctx.eventHandler = m_pPrivateIRtcEngineEventHandler;
	m_pIRtcEngine = createRZRtcEngine(m_ctx);

	return m_pIRtcEngine;
}

IRtcStream* RzObject::GetIRtcStream1()
{
	return m_pIRtcStream;
}

IRtcStream* RzObject::GetIRtcStream2()
{
	return m_pIRtcStream2;
}

IRtcChannel* RzObject::GetIRtcChannel()
{
	return m_pIRtcChannel;
}

bool RzObject::init()
{
	QVariant value;
	QString strEnv;
	int nCodecPriority = 0;
	Settings::instance()->Read("TestEnv", value, "ChannelSetUp");
	strEnv = value.toString();
	Settings::instance()->Read("Codec", value, "ChannelSetUp");
	nCodecPriority = value.toString() == QString::fromLocal8Bit("硬件编解码模式") ? 0 : 2;
	if (m_strEnv == strEnv && nCodecPriority == m_nCodecPriority)
		return false;

	m_strAppId =  APPID_PRD;
	m_strEnv = strEnv;
	m_nCodecPriority = nCodecPriority;
	return true;
}

int RzObject::JoinChannel()
{
	init();
	RzObject::instance()->GetIRtcEngine();
	int ret = -1;
	if (!m_pIRtcEngine) return -1;
	if (m_pIRtcChannel)
		m_pIRtcChannel->release();
	if (m_pPrivateIRtcChannelEventHandler)
		delete m_pPrivateIRtcChannelEventHandler;
	m_pPrivateIRtcChannelEventHandler = new PrivateIRtcChannelEventHandler;
	getSdkParams();

	m_pIRtcChannel = m_pIRtcEngine->createChannel(m_profile, m_strChannelId.c_str(), m_pPrivateIRtcChannelEventHandler);

	if (!m_pIRtcChannel)
		return ret;

	setSdkParams();
	if (m_pIRtcChannel)
		ret = m_pIRtcChannel->joinChannel(m_strUid.c_str());

	UserStateSingleton::instance()->SetProcessData(QString("JoinChannel uid = %1 ret = %2").arg(m_strUid.c_str()).arg(ret));
	return ret;
}

int RzObject::EnableLocalAudio(bool enabled)
{
	//Settings::instance()->Write("mic", enabled ? QString::fromLocal8Bit("开启麦克风") : QString::fromLocal8Bit("关闭麦克风"), "Device");
	int ret = 0;
	bool bret = false;
	//QVariant value;
	//Settings::instance()->Read("mic", value, "Device");
	//m_isEnableMic = value.toString() == QString::fromLocal8Bit("关闭麦克风") ? false : true;
	ret = m_pIRtcEngine->enableLocalAudio(enabled);
	UserStateSingleton::instance()->SetProcessData(QString("EnableLocalAudio enabled = %1 ret = %2").arg(enabled).arg(ret));
	return ret;
}

int RzObject::EnableLocalVideo(bool enabled)
{
	//Settings::instance()->Write("cam", enabled ? QString::fromLocal8Bit("开启摄像头") : QString::fromLocal8Bit("关闭摄像头"), "Device");

	int ret = 0;
	bool bret = false;
	//QVariant value;
	//Settings::instance()->Read("cam", value, "Device");
	//m_isEnableCam = value.toString() == QString::fromLocal8Bit("关闭摄像头") ? false : true;
	if (isSelfPubStream() == false)
	{
		ret = m_pIRtcEngine->enableLocalVideo(enabled);
	}
	else
	{
		//TODO 自定义推流开关
		if (enabled)
		{
			//bret = VideoCapInputMngr::instance()->GetVideoCapInput()->start();
			QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigLocalVideoStateChanged", Q_ARG(LOCAL_VIDEO_STREAM_STATE, LOCAL_VIDEO_STREAM_STATE_CAPTURING), Q_ARG(LOCAL_VIDEO_STREAM_ERROR, LOCAL_VIDEO_STREAM_ERROR_OK));
			/*m_pIRtcStream->publish();
			m_pIRtcStream2->publish();
			m_pIRtcChannel->publish(PUBLISH_AUDIO);*/
			ret = bret ? 0 : -1;
			MuteLocalVideoStream(false, "first");
			MuteLocalVideoStream(false, "second");
		}
		else
		{	
			//bret = VideoCapInputMngr::instance()->GetVideoCapInput()->stop();
			/*m_pIRtcStream->unPublish();
			m_pIRtcStream2->unPublish();
			ret = m_pIRtcChannel->unPublish(PUBLISH_AUDIO);*/
			QMetaObject::invokeMethod(UserStateNotifySingleton::instance(), "SigLocalVideoStateChanged", Q_ARG(LOCAL_VIDEO_STREAM_STATE, LOCAL_VIDEO_STREAM_STATE_STOPPED), Q_ARG(LOCAL_VIDEO_STREAM_ERROR, LOCAL_VIDEO_STREAM_ERROR_OK));
			ret = bret ? 0 : -1;
			MuteLocalVideoStream(true, "first");
			MuteLocalVideoStream(true, "second");
		}
	}

	UserStateSingleton::instance()->SetProcessData(QString("EnableLocalVideo enabled = %1 ret = %2").arg(enabled).arg(ret));
	return ret;
}

int RzObject::MuteLocalVideoStream(bool mute, QString streamName)
{
	int ret = -1;
	if (isSelfPubStream() == false)
		ret = m_pIRtcEngine->muteLocalVideoStream(mute);
	else
		ret = streamName == "first" ? m_pIRtcStream->muteVideoStream(mute) : m_pIRtcStream2->muteVideoStream(mute);

	UserStateSingleton::instance()->SetProcessData(QString("MuteLocalVideoStream streamName = %1 mute = %2 ret = %3").arg(streamName).arg(mute).arg(ret));
	return ret;
}

int RzObject::MuteLocalAudioStream(bool mute)
{
	int ret = -1;
	ret = m_pIRtcEngine->muteLocalAudioStream(mute);
	UserStateSingleton::instance()->SetProcessData(QString("MuteLocalAudioStream mute = %1 ret = %2").arg(mute).arg(ret));
	return ret;
}

int RzObject::SetupLocalVideo(VideoCanvas& canvas, QString streamName)
{
	canvas.renderMode = m_renderMode;
	int ret = 0;
	ret = m_pIRtcEngine->setupLocalVideo(canvas);
	return ret;
}

int RzObject::SetupRemoteVideo(QString uid, QString streamName, VideoCanvas& canvas)
{
	canvas.renderMode = m_renderMode;
	int ret = 0;
	ret = m_pIRtcChannel->setupRemoteVideo(uid.toStdString().c_str(), streamName.toStdString().c_str(), canvas);

	return ret;
}

int RzObject::Publish()
{
	int ret = 0;

	if(m_pIRtcChannel)
		ret = m_pIRtcChannel->publish();
	
	return ret;
}

int RzObject::UnPublish()
{
	int ret = 0;
	if(m_pIRtcChannel)
		ret = m_pIRtcChannel->unPublish();
	
	return ret;
}

int RzObject::SetClientRole(CLIENT_ROLE_TYPE role)
{
	if (!m_pIRtcChannel) return -1;
	int ret = -1;

	ret = m_pIRtcChannel->setClientRole(role);
	return ret;
}

int RzObject::MuteAllRemoteVideoStreams(bool mute)
{
	int ret = -1;
	ret = m_pIRtcChannel->muteAllRemoteVideoStreams(mute);
	return ret;
}
int RzObject::MuteAllRemoteAudioStreams(bool mute)
{
	int ret = -1;
	ret = m_pIRtcChannel->muteAllRemoteAudioStreams(mute);
	return ret;
}

int RzObject::SetLocalRenderMode()
{
	int ret = -1;
	QVariant value;
	Settings::instance()->Read("RenderMode", value, "ChannelSetUp");
	m_renderMode = value.toString() == QString::fromLocal8Bit("填充") ? RENDER_MODE_HIDDEN : value.toString() == QString::fromLocal8Bit("等比缩放") ? RENDER_MODE_FIT : RENDER_MODE_FILL;
	ret = m_pIRtcEngine->setLocalRenderMode(m_renderMode, m_mirrorMode); 

	return ret;
}

int RzObject::SetRemoteRenderMode(QString uid, QString streamName)
{
	int ret = -1;
	QVariant value;
	Settings::instance()->Read("RenderMode", value, "ChannelSetUp");
	m_renderMode = value.toString() == QString::fromLocal8Bit("填充") ? RENDER_MODE_HIDDEN : value.toString() == QString::fromLocal8Bit("等比缩放") ? RENDER_MODE_FIT : RENDER_MODE_FILL;
	ret = m_pIRtcChannel->setRemoteRenderMode(uid.toStdString().c_str(), streamName.toStdString().c_str(), m_renderMode, VIDEO_MIRROR_MODE_AUTO);
	return ret;
}

int RzObject::SetRemoteVideoStreamType(QString uid, QString streamName, bool type)
{
	int ret = m_pIRtcChannel->setRemoteVideoStreamType(uid.toStdString().c_str(), streamName.toStdString().c_str(), type ? REMOTE_VIDEO_STREAM_HIGH : REMOTE_VIDEO_STREAM_LOW);
	return ret;
}

int RzObject::SetVideoResolution(QString resolution)
{
	int ret = -1;
	m_strStreamResolution = resolution;
	VideoEncoderConfiguration cfg;
	cfg.width = m_strStreamResolution.left(m_strStreamResolution.indexOf("*")).simplified().toInt();
	cfg.height = m_strStreamResolution.right(m_strStreamResolution.indexOf("*")).simplified().remove("*").toInt();
	cfg.frameRate = m_nFrameRate;
	cfg.orientationMode = m_orientationMode;
	if (isSelfPubStream() == false)
		ret = m_pIRtcEngine->setVideoEncoderConfiguration(cfg);
	else
	{
		ret = m_pIRtcStream->setVideoEncoderConfiguration(cfg);
	}
		
	return ret;
}
int RzObject::SetVideoFrameRate(QString frameRate)
{
	int ret = -1;
	m_nFrameRate = frameRate.left(frameRate.indexOf("F")).simplified().toInt();
	VideoEncoderConfiguration cfg;
	cfg.width = m_strStreamResolution.left(m_strStreamResolution.indexOf("*")).simplified().toInt();
	cfg.height = m_strStreamResolution.right(m_strStreamResolution.indexOf("*")).simplified().remove("*").toInt();
	cfg.frameRate = m_nFrameRate;
	cfg.orientationMode = m_orientationMode;
	if (isSelfPubStream() == false)
		ret = m_pIRtcEngine->setVideoEncoderConfiguration(cfg);
	else
		ret = m_pIRtcStream->setVideoEncoderConfiguration(cfg);
	//UserStateSingleton::instance()->SetProcessData(QString("SetVideoFrameRate frameRate = %1 ret = %2").arg(frameRate).arg(ret));
	return ret;
}

int RzObject::SetVideoDir(QString dir)
{
	int ret = -1;
	m_orientationMode = dir == QString::fromLocal8Bit("自适应") ? ORIENTATION_MODE_ADAPTIVE : dir == QString::fromLocal8Bit("横向") ? ORIENTATION_MODE_FIXED_LANDSCAPE : ORIENTATION_MODE_FIXED_PORTRAIT;
	VideoEncoderConfiguration cfg;
	cfg.width = m_strStreamResolution.left(m_strStreamResolution.indexOf("*")).simplified().toInt();
	cfg.height = m_strStreamResolution.right(m_strStreamResolution.indexOf("*")).simplified().remove("*").toInt();
	cfg.frameRate = m_nFrameRate;
	cfg.orientationMode = m_orientationMode;
	if (isSelfPubStream() == false)
		ret = m_pIRtcEngine->setVideoEncoderConfiguration(cfg);
	else
		ret = m_pIRtcStream->setVideoEncoderConfiguration(cfg);
	//UserStateSingleton::instance()->SetProcessData(QString("SetVideoDir dir = %1 ret = %2").arg(dir).arg(ret));
	return ret;
}

int RzObject::SetVideoPushStreamMode(QString mode)
{
	int ret = -1;
	m_isStreamDual = mode == QString::fromLocal8Bit("双流") ? true : false;
	if (isSelfPubStream() == false)
		ret = m_pIRtcEngine->enableDualStreamMode(m_isStreamDual);
	else
		ret = m_pIRtcStream->enableDualStreamMode(m_isStreamDual);
	//UserStateSingleton::instance()->SetProcessData(QString("SetVideoPushStreamMode mode = %1 ret = %2").arg(mode).arg(ret));
	return ret;
}
int RzObject::SetVideoMirror(bool enable)
{
	int ret = -1;
	VideoEncoderConfiguration cfg;
	cfg.width = m_strStreamResolution.left(m_strStreamResolution.indexOf("*")).simplified().toInt();
	cfg.height = m_strStreamResolution.right(m_strStreamResolution.indexOf("*")).simplified().remove("*").toInt();
	cfg.frameRate = m_nFrameRate;
	cfg.orientationMode = m_orientationMode;
	cfg.mirrorMode = enable ? VIDEO_MIRROR_MODE_ENABLED : VIDEO_MIRROR_MODE_DISABLED;
	m_mirrorMode = cfg.mirrorMode;

	ret = m_pIRtcEngine->setVideoEncoderConfiguration(cfg);
	ret = m_pIRtcEngine->setLocalRenderMode(m_renderMode, cfg.mirrorMode);
		
	return ret;
}

int RzObject::GetCamDeviceCount()
{
	int ret = 0;
	IVideoDeviceManager* pIVideoDeviceManager = m_pIRtcEngine->getVideoDeviceManager();
	IVideoDeviceCollection* pIVideoDeviceCollection = pIVideoDeviceManager->enumerateVideoDevices();
	ret = pIVideoDeviceCollection->getCount();
	//UserStateSingleton::instance()->SetProcessData(QString("GetCamDeviceCount ret = %1").arg(ret));
	return ret;
}

int RzObject::GetCamDevice(int index, QString& strDeviceName, QString& strDeviceId)
{
	char deviceName[MAX_DEVICE_ID_LENGTH];
	char deviceId[MAX_DEVICE_ID_LENGTH];
	IVideoDeviceManager* pIVideoDeviceManager = m_pIRtcEngine->getVideoDeviceManager();
	IVideoDeviceCollection* pIVideoDeviceCollection = pIVideoDeviceManager->enumerateVideoDevices();
	int ret = pIVideoDeviceCollection->getDevice(index, deviceName, deviceId);
	strDeviceName = QString::fromLocal8Bit(deviceName);
	strDeviceId = QString::fromLocal8Bit(deviceId);
	//UserStateSingleton::instance()->SetProcessData(QString("GetCamDevice strDeviceName = %1 strDeviceId = %2 index = %3 ret = %4").arg(strDeviceName).arg(strDeviceId).arg(index).arg(ret));
	return ret;
}

int RzObject::GetCamDevice(QString& strDeviceId)
{
	int ret = -1;
	//if (isSelfPubStream() == false)
	{
		char deviceId[MAX_DEVICE_ID_LENGTH];
		IVideoDeviceManager* pIVideoDeviceManager = m_pIRtcEngine->getVideoDeviceManager();
		IVideoDeviceCollection* pIVideoDeviceCollection = pIVideoDeviceManager->enumerateVideoDevices();
		int ret = pIVideoDeviceCollection->getDevice(deviceId);
		strDeviceId = QString::fromLocal8Bit(deviceId);
	}
	/*else
	{
		strDeviceId = QString::fromStdString(VideoCapInputMngr::instance()->GetVideoCapInput()->GetCurDeviceName());
	}*/
	//UserStateSingleton::instance()->SetProcessData(QString("GetCamDevice strDeviceId = %1 ret = %2").arg(strDeviceId).arg(ret));
	return ret;
}

int RzObject::SetCamDevice(QString strDeviceId)
{
	int ret = -1;
	//if (isSelfPubStream() == false)
	{
		IVideoDeviceManager* pIVideoDeviceManager = m_pIRtcEngine->getVideoDeviceManager();
		IVideoDeviceCollection* pIVideoDeviceCollection = pIVideoDeviceManager->enumerateVideoDevices();
		ret = pIVideoDeviceCollection->setDevice(strDeviceId.toLocal8Bit());
	}
	//else
	{
		//VideoCapInputMngr::instance()->GetVideoCapInput()->setDeviceName(strDeviceId.toStdString());
	}
	
	//UserStateSingleton::instance()->SetProcessData(QString("SetCamDevice strDeviceId = %1 ret = %2").arg(strDeviceId).arg(ret));
	return ret;
}

int RzObject::GetMicDeviceCount()
{
	int ret = -1;
	IAudioDeviceManager* pIAudioDeviceManager = m_pIRtcEngine->getAudioDeviceManager();
	IAudioDeviceCollection* pIAudioDeviceCollection = pIAudioDeviceManager->enumerateRecordingDevices();
	ret = pIAudioDeviceCollection->getCount();
	//UserStateSingleton::instance()->SetProcessData(QString("GetMicDeviceCount ret = %2").arg(ret));
	return ret;
}

int RzObject::GetMicDevice(int index, QString& strDeviceName, QString& strDeviceId)
{
	char deviceName[MAX_DEVICE_ID_LENGTH];
	char deviceId[MAX_DEVICE_ID_LENGTH];
	IAudioDeviceManager* pIAudioDeviceManager = m_pIRtcEngine->getAudioDeviceManager();
	IAudioDeviceCollection* pIAudioDeviceCollection = pIAudioDeviceManager->enumerateRecordingDevices();
	int ret = pIAudioDeviceCollection->getDevice(index, deviceName, deviceId);
	strDeviceName = QString::fromLocal8Bit(deviceName);
	strDeviceId = QString::fromLocal8Bit(deviceId);
	//UserStateSingleton::instance()->SetProcessData(QString("GetMicDevice strDeviceName = %1 strDeviceId = %2 index = %3 ret = %4").arg(strDeviceName).arg(strDeviceId).arg(index).arg(ret));
	return ret;
}

int RzObject::GetMicDevice(QString& strDeviceId)
{
	char deviceId[MAX_DEVICE_ID_LENGTH];
	IAudioDeviceManager* pIAudioDeviceManager = m_pIRtcEngine->getAudioDeviceManager();
	IAudioDeviceCollection* pIAudioDeviceCollection = pIAudioDeviceManager->enumerateRecordingDevices();
	int ret = pIAudioDeviceCollection->getDevice(deviceId);

	strDeviceId = QString::fromLocal8Bit(deviceId);
	//UserStateSingleton::instance()->SetProcessData(QString("GetMicDevice strDeviceId = %1 ret = %2").arg(strDeviceId).arg(ret));
	return ret;
}

int RzObject::SetMicDevice(QString deviceId)
{
	IAudioDeviceManager* pIAudioDeviceManager = m_pIRtcEngine->getAudioDeviceManager();
	IAudioDeviceCollection* pIAudioDeviceCollection = pIAudioDeviceManager->enumerateRecordingDevices();
	int ret = pIAudioDeviceCollection->setDevice(deviceId.toLocal8Bit());
	//UserStateSingleton::instance()->SetProcessData(QString("SetMicDevice deviceId = %1 ret = %2").arg(deviceId).arg(ret));
	return ret;
}

int RzObject::GetMicVolume()
{
	int volume = 0;
	IAudioDeviceManager* pIAudioDeviceManager = m_pIRtcEngine->getAudioDeviceManager();
	IAudioDeviceCollection* pIAudioDeviceCollection = pIAudioDeviceManager->enumerateRecordingDevices();
	int ret = pIAudioDeviceCollection->getApplicationVolume(volume);
	//UserStateSingleton::instance()->SetProcessData(QString("GetMicVolume volume = %1 ret = %2").arg(volume).arg(ret));
	return volume;
}

int RzObject::GetSpeakerDeviceCount()
{
	IAudioDeviceManager* pIAudioDeviceManager = m_pIRtcEngine->getAudioDeviceManager();
	IAudioDeviceCollection* pIAudioDeviceCollection = pIAudioDeviceManager->enumeratePlaybackDevices();
	int ret = pIAudioDeviceCollection->getCount();
	//UserStateSingleton::instance()->SetProcessData(QString("GetSpeakerDeviceCount ret = %1").arg(ret));
	return ret;
}

int RzObject::GetSpeakerDevice(int index, QString& strDeviceName, QString& strDeviceId)
{
	char deviceName[MAX_DEVICE_ID_LENGTH];
	char deviceId[MAX_DEVICE_ID_LENGTH];
	IAudioDeviceManager* pIAudioDeviceManager = m_pIRtcEngine->getAudioDeviceManager();
	IAudioDeviceCollection* pIAudioDeviceCollection = pIAudioDeviceManager->enumeratePlaybackDevices();
	int ret = pIAudioDeviceCollection->getDevice(index, deviceName, deviceId);

	strDeviceName = QString::fromLocal8Bit(deviceName);
	strDeviceId = QString::fromLocal8Bit(deviceId);
	//UserStateSingleton::instance()->SetProcessData(QString("GetSpeakerDevice index = %1 strDeviceName = %2 strDeviceId = %3 ret = %4").arg(index).arg(strDeviceName).arg(strDeviceId).arg(ret));
	return ret;
}

int RzObject::GetSpeakerDevice(QString& strDeviceId)
{
	char deviceId[MAX_DEVICE_ID_LENGTH];
	IAudioDeviceManager* pIAudioDeviceManager = m_pIRtcEngine->getAudioDeviceManager();
	IAudioDeviceCollection* pIAudioDeviceCollection = pIAudioDeviceManager->enumeratePlaybackDevices();
	int ret = pIAudioDeviceCollection->getDevice(deviceId);

	strDeviceId = QString::fromLocal8Bit(deviceId);
	//UserStateSingleton::instance()->SetProcessData(QString("GetSpeakerDevice strDeviceId = %1 ret = %2").arg(strDeviceId).arg(ret));
	return ret;
}

int RzObject::SetSpeakerDevice(QString deviceId)
{
	IAudioDeviceManager* pIAudioDeviceManager = m_pIRtcEngine->getAudioDeviceManager();
	IAudioDeviceCollection* pIAudioDeviceCollection = pIAudioDeviceManager->enumeratePlaybackDevices();
	int ret = pIAudioDeviceCollection->setDevice(deviceId.toLocal8Bit());
	//UserStateSingleton::instance()->SetProcessData(QString("SetSpeakerDevice deviceId = %1 ret = %2").arg(deviceId).arg(ret));
	return ret;
}

RENDER_MODE_TYPE RzObject::GetRenderMode()
{
	QVariant value;
	Settings::instance()->Read("RenderMode", value, "ChannelSetUp");
	m_renderMode = value.toString() == QString::fromLocal8Bit("填充") ? RENDER_MODE_HIDDEN : value.toString() == QString::fromLocal8Bit("等比缩放") ? RENDER_MODE_FIT : RENDER_MODE_FILL;
	UserStateSingleton::instance()->SetProcessData(QString("GetRenderMode ret = %1").arg(m_renderMode));
	return m_renderMode;
}

int RzObject::StartLastmileProbeTest()
{
	int ret = -1;
	if (!m_pIRtcEngine)
		return -1;
	LastmileProbeConfig cfg;
	cfg.probeDownlink = true;
	cfg.probeUplink = true;
	cfg.expectedUplinkBitrate = 1024000;
	cfg.expectedDownlinkBitrate = 1024000;
	ret = m_pIRtcEngine->startLastmileProbeTest(cfg);
	//UserStateSingleton::instance()->SetProcessData(QString("StartLastmileProbeTest ret = %1").arg(ret));
	return ret;
}

int RzObject::StopLastmileProbeTest()
{
	int ret = -1;
	if (!m_pIRtcEngine)
		return -1;
	ret = m_pIRtcEngine->stopLastmileProbeTest();
	//UserStateSingleton::instance()->SetProcessData(QString("StopLastmileProbeTest ret = %1").arg(ret));
	return ret;
}

void RzObject::Release()
{
	IRtcEngine::release(true);
	m_pIRtcEngine = nullptr;
	//UserStateSingleton::instance()->SetProcessData(QString("Release "));
}

void RzObject::Reset()
{
	m_mirrorMode = VIDEO_MIRROR_MODE_DISABLED;
}

bool RzObject::isSelfPubStream()
{
	QVariant value;
	Settings::instance()->Read("enablestream2", value, "SelfStreamSwitch");
	return value.toBool();
}

bool RzObject::isSelfRender()
{
	QVariant value;
	Settings::instance()->Read("RenderDevice", value, "ChannelSetUp");
	return value.toString() == QString::fromLocal8Bit("自定义渲染器") ? true : false;
}

bool RzObject::isAudience()
{
	QVariant value;
	Settings::instance()->Read("scene", value, "SceneMode");
	m_profile = value.toString() == QString::fromLocal8Bit("通信") ? CHANNEL_PROFILE_COMMUNICATION : CHANNEL_PROFILE_LIVE_BROADCASTING;
	if (value.toString() == QString::fromLocal8Bit("直播观众"))
		m_role = CLIENT_ROLE_AUDIENCE;
	else if (value.toString() == QString::fromLocal8Bit("直播主播"))
		m_role = CLIENT_ROLE_BROADCASTER;
	return m_role == CLIENT_ROLE_AUDIENCE && CHANNEL_PROFILE_LIVE_BROADCASTING == m_profile;
}

void RzObject::getSdkParams()
{
	/*
	* 场景
	* uid channelid
	* 测试环境
	* 编解码方式
	* 渲染器
	* 渲染方式
	* 视频一：推流模式（单/双）、编码分辨率、编码帧率、编码方向
	* 视频二：
	* 麦克风、摄像头、扬声器
	* 获取所有视频
	* 获取所有音频
	*/
	QVariant value;
	m_profile =  CHANNEL_PROFILE_COMMUNICATION;
	Settings::instance()->Read("channel", value, "Info");
	m_strChannelId = value.toString().toStdString();
	Settings::instance()->Read("user", value, "Info");
	m_strUid = value.toString().toStdString();
	m_renderMode =  RENDER_MODE_FIT ;
	m_isStreamDual =  false;
	Settings::instance()->Read("Resolution", value, "stream1");
	m_strStreamResolution = "640 * 480";
	m_orientationMode =  ORIENTATION_MODE_ADAPTIVE ;
	Settings::instance()->Read("Fps", value, "stream1");
	m_nFrameRate = 15;
	
	Settings::instance()->Read("speaker", value, "Device");
	m_isEnableSpeaker = value.toString() == QString::fromLocal8Bit("关闭扬声器") ? false : true;

	m_isDefMuteAllAudio = false;
	m_isDefMuteAllVideo = false;
}

void RzObject::setSdkParams()
{
	m_pIRtcEngine->enableAudioVolumeIndication(2, 10, true);
	m_pIRtcEngine->enableDualStreamMode(true);
	VideoEncoderConfiguration cfg;
	cfg.width = 640;
	cfg.height = 480;
	cfg.frameRate = 15;
	cfg.orientationMode = ORIENTATION_MODE_FIXED_LANDSCAPE;
	m_pIRtcEngine->setVideoEncoderConfiguration(cfg);
	
	m_pIRtcChannel->setDefaultMuteAllRemoteVideoStreams(false);
	m_pIRtcChannel->setDefaultMuteAllRemoteAudioStreams(false);
}