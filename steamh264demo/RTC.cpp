#include "RTC.h"
#include <QWidget>
#include <IRtcEngine.h>

#include "RZEngineEventHandler.h"
#include "RZRtcChannelEventHandler.h"
#include "CutomizedVideoSource.h"
#include "VideoCapInput.h"



RTC* RTC::m_instance = nullptr;

RTC::RTC()
{
    engineEventHandler = new RZEngineEventHandler();
    channelEventHandler = new RZRtcChannelEventHandler();
    videoSink = new CustomizedVideoSink();
    videoSource = new CutomizedVideoSource;
    encoder = std::make_shared<VideoConverter>();
    decoder = std::make_shared<VideoDecoder>();
    /* RtcEngineContext context{ "", engineEventHandler };
    rtcEngine = (rz::IRtcEngine*)createRZRtcEngine(context);*/
}

RTC::~RTC()
{
    // TODO:
    if (!m_instance)
        return;

    release();

    if (engineEventHandler)
        delete engineEventHandler;
    if (channelEventHandler)
        delete channelEventHandler;
    if (videoSink)
        delete videoSink;
    if (videoSource)
        delete videoSource;

    engineEventHandler = nullptr;
    channelEventHandler = nullptr;
    videoSink = nullptr;
    videoSource = nullptr;

    delete m_instance;
    m_instance = nullptr;
}

bool RTC::createChannel(const char* channelId)
{
    if (rtcChannel)
        return true;

    rtcChannel = rtcEngine->createChannel(CHANNEL_PROFILE_COMMUNICATION, channelId, channelEventHandler);
    if (rtcChannel == nullptr)
    {
        showTipDialog("创建频道错误", "当前频道ID不合法");
        return false;
    }

    rtcStream = rtcChannel->createVideoStream("first", videoSource, VIDEO_STREAM_H264, streamEventHandler, VideoEncoderConfiguration{ 1280, 720, 1200, 1200, 15, 15 });
    VideoCapInputMngr::instance()->GetVideoCapInput()->iyuvoutfun = std::bind(&CutomizedVideoSource::onVideoData, videoSource,
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

    return true;
}

int RTC::joinChannel(const char* uid)
{
    int ret = rtcChannel->joinChannel(uid);

    if (ret < 0)
    {
        if (ret == -2)
            showTipDialog("加入频道失败", "当前用户ID不合法");
        else if (ret == -3)
            showTipDialog("加入频道失败", "用于已加入频道或调用被拒绝");
        else
            showTipDialog("加入频道失败", "未知错误");
        return false;
    }
    return true;
}

void RTC::publish()
{
    rtcChannel->publish(PUBLISH_AUDIO);
    rtcStream->publish();
}

void RTC::show()
{
    VideoCapInputMngr::instance()->GetVideoCapInput()->start();
}

void RTC::setRemoteVideoSink(const char* uid)
{
    // TODO:steam name
    rtcChannel->setRemoteVideoSink(uid, "first", videoSink);
}

void RTC::leaveChannel()
{
    if (!rtcChannel)
        return;
    rtcChannel->leaveChannel();
    rtcChannel->release();
    //rtcStream->release();
    rtcChannel = nullptr;
    //rtcStream = nullptr;
}

void RTC::release()
{
    if (!m_instance)
        return;

    if (rtcEngine)
    {
        rtcEngine->release(true);
    }

    if (rtcChannel)
    {
        //TODO: leaveChannel
        //rtcChannel->leaveChannel();
        rtcChannel->release();
    }
    rtcEngine = nullptr;
    rtcChannel = nullptr;
}

void RTC::showTipDialog(const char* title, const char* content)
{
    TipDialog tip;
    tip.setContent(title, content);
    tip.show();
    tip.exec();
}

RTC* RTC::instance()
{
    if (!m_instance)
        m_instance = new RTC();
    return m_instance;
}

bool RTC::init(const char* appid)
{
    if (rtcEngine)
        return false;

    if (!engineEventHandler)
        engineEventHandler = new RZEngineEventHandler;
    if (!streamEventHandler)
        streamEventHandler = new IRtcStreamEventHandler;

    rtcEngine = (rz::IRtcEngine*)createRZRtcEngine(RtcEngineContext{ appid, engineEventHandler });

    if (!rtcEngine)
    {
        showTipDialog("初始化失败", "创建引擎失败");
        return false;
    }

    if (rtcEngine->enableLocalVideo(true) < 0)
    {
        showTipDialog("初始化失败", "启用本地视频设备失败");
    }

    if (rtcEngine->enableLocalAudio(true) < 0)
    {
        showTipDialog("初始化失败", "启用本地视音频设备失败");
    }


    return true;
}
