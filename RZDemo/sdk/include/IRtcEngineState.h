//
// Created by 86157 on 2020/9/21.
//

#ifndef PAASSDK_IRTCENGINESTATE_H
#define PAASSDK_IRTCENGINESTATE_H

#include "IMediaObserver.h"

#if defined(_WIN32)
#define RZ_CALL __cdecl
#define RZ_API extern "C" __declspec(dllexport)
#define RZ_CPP_API __declspec(dllexport)
#elif defined(__APPLE__)
#include <TargetConditionals.h>
#define RZ_API __attribute__((visibility("default"))) extern "C"
#define RZ_CPP_API __attribute__((visibility("default")))
#define RZ_CALL
#elif defined(__ANDROID__) || defined(__linux__)
#define RZ_API extern "C" __attribute__((visibility("default")))
#define RZ_CPP_API __attribute__((visibility("default")))
#define RZ_CALL
#else
#define RZ_API extern "C"
#define RZ_CPP_API
#define RZ_CALL
#endif

namespace rz {
//频道场景
enum CHANNEL_PROFILE_TYPE
{
  CHANNEL_PROFILE_COMMUNICATION = 0,  // 0：通信场景。该场景下，频道内所有用户都可以发布和接收音、视频流。适用于语音通话、视频群聊等应用场景。
  CHANNEL_PROFILE_LIVE_BROADCASTING = 1,  // 1：直播场景。该场景有主播和观众两种用户角色，可以通过 setClientRole
                                          // 设置。主播可以发布和接收音视频流，观众直接接收流。适用于语聊房、视频直播、互动大班课等应用场景。
};

//用户角色
enum CLIENT_ROLE_TYPE
{
  CLIENT_ROLE_BROADCASTER = 1,  // 1：主播。主播可以发流也可以收流。
  CLIENT_ROLE_AUDIENCE = 2,     // 2：（默认）观众。观众只能收流不能发流。
};

//音频采样
enum AUDIO_PROFILE_TYPE
{
  AUDIO_PROFILE_DEFAULT =
      0,  // 0: 默认值。直播场景下：16 KHz 采样率，单声道，编码码率最大值为 64 Kbps。通信场景下：16 KHz 采样率，单声道，编码码率最大值为 32 Kbps。
  AUDIO_PROFILE_SPEECH_STANDARD = 1,            // 1: 指定 32 KHz 采样率，语音编码，单声道，编码码率最大值为 18 Kbps。
  AUDIO_PROFILE_MUSIC_STANDARD = 2,             // 2: 指定 48 KHz 采样率，音乐编码，单声道，编码码率最大值为 64 Kbps。
  AUDIO_PROFILE_MUSIC_STANDARD_STEREO = 3,      // 3: 指定 48 KHz 采样率，音乐编码，双声道，编码码率最大值为 80 Kbps。
  AUDIO_PROFILE_MUSIC_HIGH_QUALITY = 4,         // 4: 指定 48 KHz 采样率，音乐编码，单声道，编码码率最大值为 96 Kbps。
  AUDIO_PROFILE_MUSIC_HIGH_QUALITY_STEREO = 5,  // 5: 指定 48 KHz 采样率，音乐编码，双声道，编码码率最大值为 128 Kbps。
  AUDIO_PROFILE_IOT = 6,                        // 6: 指定 16 KHz 采样率，语音编码，单声道，应用回声消除算法 AEC。
  AUDIO_PROFILE_NUM = 7,
};

//音频使用场景
enum AUDIO_SCENARIO_TYPE
{
  AUDIO_SCENARIO_DEFAULT = 0,                 // 0: 默认设置。
  AUDIO_SCENARIO_CHATROOM_ENTERTAINMENT = 1,  // 1: 娱乐应用，需要频繁上下麦的场景。
  AUDIO_SCENARIO_EDUCATION = 2,               // 2: 教育应用，流畅度和稳定性优先。
  AUDIO_SCENARIO_GAME_STREAMING = 3,          // 3: 高音质语聊房应用。
  AUDIO_SCENARIO_SHOWROOM = 4,                // 4: 秀场应用，音质优先和更好的专业外设支持。
  AUDIO_SCENARIO_CHATROOM_GAMING = 5,         // 5: 游戏开黑。
  AUDIO_SCENARIO_IOT = 6,                     // 6: 物联网应用。
  AUDIO_SCENARIO_NUM = 7
};

//视频大小流
enum REMOTE_VIDEO_STREAM_TYPE
{
  REMOTE_VIDEO_STREAM_HIGH = 0,  // 0：视频大流
  REMOTE_VIDEO_STREAM_LOW = 1,   // 1：视频小流
};

//视频镜像配置
enum VIDEO_MIRROR_MODE_TYPE
{
  VIDEO_MIRROR_MODE_AUTO = 0,      // 0：自动
  VIDEO_MIRROR_MODE_ENABLED = 1,   // 1：镜像显示
  VIDEO_MIRROR_MODE_DISABLED = 2,  // 2：非镜像显示
};

//视频在屏幕中显示模式
enum ORIENTATION_MODE
{
  ORIENTATION_MODE_ADAPTIVE = 0,  // 0：如果采集的视频是横屏模式，则输出的视频也是横屏模式；如果采集的视频是竖屏模式，则输出的视频也是竖屏模式。
  ORIENTATION_MODE_FIXED_LANDSCAPE = 1,  // 1：输出的视频也是横屏模式
  ORIENTATION_MODE_FIXED_PORTRAIT = 2,   // 2：输出的视频也是竖屏模式
};

//设置视频显示模式。
enum RENDER_MODE_TYPE
{
  RENDER_MODE_HIDDEN = 1,  // 1：视频尺寸等比缩放。优先保证视窗被填满。因视频尺寸与显示视窗尺寸不一致而多出的视频将被截掉。
  RENDER_MODE_FIT = 2,  // 2：视频尺寸等比缩放。优先保证视频内容全部显示。因视频尺寸与显示视窗尺寸不一致造成的视窗未被填满的区域填充黑色。
  RENDER_MODE_FILL = 3,  // 3：视频尺寸进行缩放和拉伸以充满显示视窗。
};

//码率控制方式
enum BIT_RATE_CTRL_TYPE
{
  BIT_RATE_CTRL_CBR,  //固定码率
  BIT_RATE_CTRL_VBR,  //可变码率
  BIT_RATE_CTRL_ABR   //平均码率
};

//视频编码配置
struct VideoEncoderConfiguration {
  int width = 0;                                                 //大流视频宽度
  int height = 0;                                                //大流视频高度
  int bitrate = 0;                                               //大流码率 kbps
  int minBitrate = 0;                                            //大流最小码率 kbps
  int frameRate = 0;                                             //大流帧率
  int minFrameRate = 0;                                          //大流最小帧率 
  int lowWidth = 0;                                              //小流视频宽度, 最多不能超过大流的百之五十, 为0则使用推荐值
  int lowHeight = 0;                                             //小流视频高度, 最多不能超过大流的百分之五十, 为0则使用推荐值
  int lowBitrate = 0;                                            //小流码率,单位kbps,  宽高同为0时, 使用推荐值, 否则根据宽高进行计算
  int lowFrameRate = 0;                                          //小流帧率, 不超过大流帧率, 不低于5. (可选值: 5 10 15 25 30 60), 移动端没有60帧
  ORIENTATION_MODE orientationMode = ORIENTATION_MODE_ADAPTIVE;  //视频输出模式(横屏、竖屏), 大小流共用
  VIDEO_MIRROR_MODE_TYPE mirrorMode = VIDEO_MIRROR_MODE_AUTO;    //镜像配置, 大小流共用
  BIT_RATE_CTRL_TYPE bitrateControlType = BIT_RATE_CTRL_CBR;     //码率控制方式, 默认固定码率
};

//视频视图结构
struct VideoCanvas {
  void *view = nullptr;                                                                //视频绘制视图
  RENDER_MODE_TYPE renderMode = RENDER_MODE_TYPE::RENDER_MODE_FIT;                     //渲染模式
  void *priv = nullptr;                                                                //私有数据
  VIDEO_MIRROR_MODE_TYPE mirrorMode = VIDEO_MIRROR_MODE_TYPE::VIDEO_MIRROR_MODE_AUTO;  //镜像配置
};

//音频PCM格式
enum AUDIO_PCM_FORMAT
{
  AUDIO_PCM_UNAVILABLE = -1,  //-1：无效
  AUDIO_PCM_U8 = 0,           // 0：U8格式
  AUDIO_PCM_S16 = 1,          // 1：S16格式
  AUDIO_PCM_S32 = 2,          // 2：S32格式
  AUDIO_PCM_S64 = 3,          // 3：S64格式
  AUDIO_PCM_FLT = 4,          // 4：float格式
  AUDIO_PCM_DBL = 5,          // 5：double格式

  AUDIO_PCM_U8P = 20,   // 20：U8P格式
  AUDIO_PCM_S16P = 21,  // 21：S16P格式
  AUDIO_PCM_S32P = 22,  // 22：S32P格式
  AUDIO_PCM_S64P = 23,  // 23：S64格式
  AUDIO_PCM_FLTP = 24,  // 24：FLOATP格式
  AUDIO_PCM_DBLP = 25,  // 25：DOUBLEP格式
};

//音频流类型标识
enum AUDIO_STREAM_TYPE
{
  AUDIO_STREAM_PCM = 0,     // 0：pcm格式的原始音频流
  AUDIO_STREAM_OPUS = 1,    // 1：opus编码格式的音频流
  AUDIO_STREAM_CUSTOM = 2,  // 2：未知的用户自定义类型
};

// sink配置
struct AudioSinkConfig {
  int channelCount = 1;                             //声道数
  unsigned int sampleRate = 16000;                  //采样率
  AUDIO_PCM_FORMAT pcmFormat = AUDIO_PCM_S16;       //音频采集PCM格式
  AUDIO_STREAM_TYPE streamType = AUDIO_STREAM_PCM;  //音频流类型
};

struct AudioSourceConfig {
  AUDIO_STREAM_TYPE streamType = AUDIO_STREAM_PCM;  //音频流类型 非PCM类型下面几项不需要配置
  bool enableAec = false;
};

//媒体流类型
enum VIDEO_STREAM_TYPE
{
  VIDEO_STREAM_FRAME = 1,   // 1：原始流
  VIDEO_STREAM_H264 = 2,    // 2：h264裸流
  VIDEO_STREAM_CUSTOM = 3,  // 3：未知的用户自定义类型
};

//视频像素格式
enum VIDEO_PIXEL_FORMAT
{
  VIDEO_PIXEL_UNAVILABLE = -1,  //-1：无效接口
  VIDEO_PIXEL_I420 = 0,         // 0：I420 格式
  VIDEO_PIXEL_BGRA = 1,         // 1：BGRA 格式
  VIDEO_PIXEL_NV21 = 2,         // 2：NV21 格式
  VIDEO_PIXEL_RGBA = 3,         // 3：RGBA 格式
  VIDEO_PIXEL_IMC2 = 4,         // 4：IMC2 格式
  VIDEO_PIXEL_ARGB = 5,         // 5：ARGB 格式
  VIDEO_PIXEL_NV12 = 6,         // 6：NV12 格式
  VIDEO_PIXEL_I422 = 7,         // 7：I422 格式
  VIDEO_PIXEL_COUNT = 8,
};

//音频路由配置
enum AUDIO_ROUTE_TYPE
{
  AUDIO_ROUTE_DEFAULT = 0,         // 0：使用默认的语音路由
  AUDIO_ROUTE_HEADSET = 1,         // 1：使用耳机为语音路由
  AUDIO_ROUTE_EARPIECE = 2,        // 2：使用听筒为语音路由
  AUDIO_ROUTE_HEADSET_NO_MIC = 3,  // 3：使用不带麦的耳机为语音路由
  AUDIO_ROUTE_SPEAKERPHONE = 4,    // 4：使用手机的扬声器为语音路由
  AUDIO_ROUTE_LOUDSPEAKER = 5,     // 5：使用外接的扬声器为语音路由
  AUDIO_ROUTE_BLUETOOTH = 6,       // 6：使用蓝牙耳机为语音路由
  AUDIO_ROUTE_USB = 7,             // 7：使用 USB 外围设备为语音路由（仅适用于 macOS）
  AUDIO_ROUTE_HDMI = 8,            // 8：使用 HDMI 外围设备为语音路由（仅适用于 macOS）
  AUDIO_ROUTE_DISPLAYPORT = 9,     // 9：使用 DisplayPort 外围设备为语音路由（仅适用于 macOS）
  AUDIO_ROUTE_AIRPLAY = 10,        // 10：使用 Apple AirPlay 为语音路由（仅适用于 macOS）
};

//媒体设备配置
enum MEDIA_DEVICE_TYPE
{
  AUDIO_PLAYOUT_DEVICE = 0,    // 0：音频播放设备
  AUDIO_RECORDING_DEVICE = 1,  // 1：音频录制设备
  VIDEO_CAPTURE_DEVICE = 2,    // 2：视频采集设备
};

//设备状态配置
enum MEDIA_DEVICE_STATE_TYPE
{
  MEDIA_DEVICE_STATE_ACTIVE = 0,       // 0： 设备正在使用
  MEDIA_DEVICE_STATE_DISABLED = 1,     // 1：设备被禁用
  MEDIA_DEVICE_STATE_NOT_PRESENT = 2,  // 2： 没有此设备
  MEDIA_DEVICE_STATE_UNPLUGGED = 3,    // 3：设备被拔出
};

// video sink配置
struct VideoSinkConfig {
  VIDEO_STREAM_TYPE streamType = VIDEO_STREAM_FRAME;                      //视频流类型
  VIDEO_PIXEL_FORMAT pixelFormat = VIDEO_PIXEL_FORMAT::VIDEO_PIXEL_I420;  //视频像素格式
  int bitDepth = 8;                                                       //位深
};

//通信状态信息
struct RtcStats {
  unsigned int duration = 0;            //通话时长（毫秒）。
  unsigned long long txBytes = 0;       //发送字节数（bytes）, 音视频累计值。
  unsigned long long rxBytes = 0;       //接收字节数（bytes）, 音视频累计值。
  unsigned long long txAudioBytes = 0;  //发送音频字节数（bytes），累计值。
  unsigned long long txVideoBytes = 0;  //发送视频字节数（bytes），累计值。
  unsigned long long rxAudioBytes = 0;  //接收音频字节数（bytes），累计值。
  unsigned long long rxVideoBytes = 0;  //接收视频字节数（bytes），累计值。
  unsigned short txKBitRate = 0;        //发送码率（Kbps）。
  unsigned short rxKBitRate = 0;        //接收码率（Kbps）。
  unsigned short rxAudioKBitRate = 0;   //音频接收码率 (Kbps）。
  unsigned short txAudioKBitRate = 0;   //音频包的发送码率 (Kbps）。
  unsigned short rxVideoKBitRate = 0;   //视频接收码率 (Kbps）。
  unsigned short txVideoKBitRate = 0;   //视频发送码率 (Kbps）。
  unsigned short lastmileDelay = 0;     //客户端-接入服务器延时 (毫秒)。
  unsigned short txPacketLossRate = 0;  //使用抗丢包技术前，客户端上行发送到服务器丢包率 (%)
  unsigned short rxPacketLossRate = 0;  //使用抗丢包技术前，客户端上行发送到服务器丢包率 (%)
  unsigned int userCount = 0;           //当前频道内的用户人数。
  float cpuAppUsage = 0.0;              //当前 App 的 CPU 使用率 (%)。
  float cpuTotalUsage = 0.0;            //当前系统的 CPU 使用率 (%)。
  float memoryAppUsageRatio = 0.0;      //当前 App 的内存占比 (%)
  float memoryTotalUsageRatio = 0.0;    //当前系统的内存占比 (%)
  int memoryAppUsageInKbytes = 0;
};

enum REGION_CODE
{
  REGION_GLOBAL = 0,  //全球
  REGION_CN = 1,      //中国大陆
  REGION_NA = 2,      //北美
  REGION_EU = 3,      //欧洲
  REGION_AS = 4,      //除中国大陆以外的亚洲区域。
};

enum CODEC_PRIORITY
{
  PRIORITY_AUTO = 0,  // SDK 自动选择
  PRIORITY_HW = 1,    //优先使用硬件编解码
  PRIORITY_SW = 2,    //使用软件编解码
};

//网络质量类型
enum QUALITY_TYPE
{
  QUALITY_UNKNOWN = 0,      // 0: 网络质量未知。
  QUALITY_EXCELLENT = 1,    // 1: 网络质量极好。
  QUALITY_GOOD = 2,         // 2: 用户主观感觉和 excellent 差不多，但码率可能略低于 excellent。
  QUALITY_POOR = 3,         // 3: 用户主观感受有瑕疵但不影响沟通。
  QUALITY_BAD = 4,          // 4: 勉强能沟通但不顺畅。
  QUALITY_VBAD = 5,         // 5: 网络质量非常差，基本不能沟通。
  QUALITY_DOWN = 6,         // 6: 完全无法沟通。
  QUALITY_UNSUPPORTED = 7,  // 7: 暂时无法检测网络质量（未使用）。
  QUALITY_DETECTING = 8,    // 8: 网络质量检测已开始还没完成。
};

// channel发布默认流方式  todo 各枚举值不可更改
enum PUBLISH_MEDIA_TYPE
{
  PUBLISH_INIT = 0,        //不可用
  PUBLISH_AUDIO = 1,       //将默认音频发布到该频道中
  PUBLISH_VIDEO = 2,       //将默认视频发布到该频道中
  PUBLISH_AUDIO_VIDEO = 3  //将默认音视频流都发布到该频道中
};

//最后一公里测速状态
enum LASTMILE_PROBE_RESULT_STATE
{
  LASTMILE_PROBE_RESULT_COMPLETE = 1,  // 1: 表示本次 last mile 质量探测的结果是完整的。
  LASTMILE_PROBE_RESULT_INCOMPLETE_NO_BWE = 2,  // 2: 表示本次 last mile 质量探测未进行带宽预测，因此结果不完整。一个可能的原因是测试资源暂时受限。
  LASTMILE_PROBE_RESULT_UNAVAILABLE = 3,  // 3: 未进行 last mile 质量探测。一个可能的原因是网络连接中断。
};

//最后一公里测速配置
struct LastmileProbeConfig {
  bool probeUplink = false;                       //是否探测上行网络
  bool probeDownlink = false;                     //是否探测下行网络。
  unsigned int expectedUplinkBitrate = 500000;    //用户期望的最高发送码率，单位为 bps，范围为 [100000, 5000000]。
  unsigned int expectedDownlinkBitrate = 500000;  //用户期望的最高接收码率，单位为 bps，范围为 [100000, 5000000]。
};

//最后一公里测速网络质量信息
struct LastmileProbeOneWayResult {
  unsigned int packetLossRate = 0;      //丢包率。
  unsigned int jitter = 0;              //网络抖动 (ms)。
  unsigned int availableBandwidth = 0;  //可用网络带宽预估 (bps)。
  QUALITY_TYPE quality = QUALITY_UNKNOWN;
};

//最后一公里测速结果信息
struct LastmileProbeResult {
  LASTMILE_PROBE_RESULT_STATE state = LASTMILE_PROBE_RESULT_COMPLETE;  // Last mile 质量探测结果的状态
  LastmileProbeOneWayResult uplinkReport;                              //上行网络质量报告
  LastmileProbeOneWayResult downlinkReport;                            //下行网络质量报告
  unsigned int rtt = 0;                                                //往返时延 (ms)。
};

//音频音量信息
struct AudioVolumeInfo {
  const char *uid = nullptr;        //用户ID
  unsigned int volume = 0;          //音量
  unsigned int vad = 0;             //人声检测
  const char *channelId = nullptr;  //频道ID
};

//本地音频流信息
struct LocalAudioStats {
  int numChannels = 0;                  //声道数。
  int sentSampleRate = 0;               //发送的采样率，单位为 Hz。
  int sentBitrate = 0;                  //发送码率的平均值，单位为 Kbps。
  unsigned short txPacketLossRate = 0;  //弱网对抗前本端到流媒体边缘服务器的音频丢包率 (%)。
};

//远端音频状态信息
struct RemoteAudioStats {
  const char *uid = nullptr;   //用户 ID
  int audioLossRate = 0;       //统计周期内的远端音频流的丢帧率 (%)。
  int numChannels = 0;         //声道数。
  int receivedSampleRate = 0;  //统计周期内接收到的远端音频采样率。
  int receivedBitrate = 0;     //接收流在统计周期内的平均码率（Kbps）。
  int totalFrozenTime = 0;  //加入频道后该远端用户音频卡顿的累计时长（ms）。通话过程中，音频丢帧率达到 4% 即记为一次音频卡顿。
  int frozenRate =
      0;  //加入频道后该远端用户音频卡顿的累计时长占音频总有效时长的百分比 (%)。音频有效时长是指远端用户加入频道后音频未被停止发送的时长。
  int totalActiveTime = 0;  //音频有效时长（毫秒），即远端用户/主播加入频道后，没有停止发送音频流的通话时长。
  int publishDuration = 0;  //远端音频流的累计发布时长（毫秒）。
};

//本地视频流信息
struct LocalVideoStats {
  const char *streamName = nullptr;     //流名字
  int sentBitrate = 0;                  //实际发送码率 (Kbps)。
  int sentFrameRate = 0;                //实际发送帧率 (fps)。
  int encoderOutputFrameRate = 0;       //本地视频编码器的输出帧率，单位为 fps。
  int rendererOutputFrameRate = 0;      //本地视频渲染器的输出帧率，单位为 fps
  int encodedFrameWidth = 0;            //视频编码宽度（px）。
  int encodedFrameHeight = 0;           //视频编码高度（px）。
  int encodedFrameCount = 0;            //视频发送的帧数，累计值。
  unsigned short txPacketLossRate = 0;  //弱网对抗前本端到 RZ 边缘服务器的视频丢包率 (%)。
  int captureFrameRate = 0;             //本地视频采集帧率 (fps)。
};

//远端视频流信息
struct RemoteVideoStats {
  const char *uid = nullptr;                                         //用户ID
  const char *streamName = nullptr;                                  //流名字
  int width = 0;                                                     //视频流宽（像素）。
  int height = 0;                                                    //视频流高（像素）。
  int receivedBitrate = 0;                                           //（上次统计后）的接收码率(Kbps)。
  int decoderOutputFrameRate = 0;                                    //远端视频解码器的输出帧率，单位为 fps。
  int packetLossRate = 0;                                            //远端视频在使用抗丢包技术之后的丢包率(%)。
  REMOTE_VIDEO_STREAM_TYPE rxStreamType = REMOTE_VIDEO_STREAM_HIGH;  //视频流类型，大流或小流。
  int totalFrozenTime = 0;  //加入频道后该远端视频流发生视频卡顿的累计时长（ms）。通话过程中，视频帧率设置不低于 5 fps
                            //时，连续渲染的两帧视频之间间隔超过 500 ms，则记为一次视频卡顿。
  int frozenRate =
      0;  //加入频道后该远端视频流发生视频卡顿的累计时长占视频总有效时长的百分比 (%)。视频有效时长是指远端用户加入频道后视频未被停止发送或接收的时长。
  int totalActiveTime = 0;  //视频有效时长（毫秒），即远端用户/主播加入频道后，没有停止发送视频流的通话时长。
  int publishDuration = 0;  //远端视频流的累计时长（毫秒）。
};

//网络状态
enum CONNECTION_STATE_TYPE
{
  CONNECTION_STATE_DISCONNECTED = 1,  // 1: 网络连接断开。
  CONNECTION_STATE_CONNECTING = 2,    // 2: 建立网络连接中。
  CONNECTION_STATE_CONNECTED = 3,     // 3: 网络已连接。
  CONNECTION_STATE_RECONNECTING = 4,  // 4: 重新建立网络连接中。
  CONNECTION_STATE_FAILED = 5,        // 5: 网络连接失败。
};

//引起网络连接状态发生改变的原因。
enum CONNECTION_CHANGED_REASON_TYPE
{
  CONNECTION_CHANGED_CONNECTING = 0,        // 0: 建立网络连接中。
  CONNECTION_CHANGED_JOIN_SUCCESS = 1,      // 1: 成功加入频道。
  CONNECTION_CHANGED_INTERRUPTED = 2,       // 2: 网络连接中断。
  CONNECTION_CHANGED_BANNED_BY_SERVER = 3,  // 3: 网络连接被服务器禁止。
  CONNECTION_CHANGED_JOIN_FAILED = 4,  // 4: 加入频道失败。SDK 在尝试加入频道 20 分钟后还是没能加入频道，会返回该状态，并停止尝试重连。
  CONNECTION_CHANGED_LEAVE_CHANNEL = 5,       // 5: 离开频道。
  CONNECTION_CHANGED_INVALID_CHANNEL_ID = 6,  // 6: 不是有效的频道ID。请更换有效的频道ID重新加入频道。
  CONNECTION_CHANGED_REJECTED_BY_SERVER = 7,  // 7: 此用户被服务器拒绝, 一般是因为使用相同uid重复进入频,此时先进入频道的用户会收到
                                              // CONNECTION_STATE_FAILED, reason是CONNECTION_CHANGED_REJECTED_BY_SERVER
  CONNECTION_CHANGED_CLIENT_IP_ADDRESS_CHANGED = 8,  // 8: 客户端 IP 地址变更，可能是由于网络类型，或网络运营商的 IP 或端口发生改变引起。
  CONNECTION_CHANGED_KEEP_ALIVE_TIMEOUT = 9,  // 9: SDK 和服务器连接保活超时，进入自动重连状态 CONNECTION_STATE_RECONNECTING(4)。
  CONNECTION_CHANGED_INVALID_APP_ID = 10,     // 10: 不是有效的 APP ID。请更换有效的 APP ID 重新加入频道。
};

//网络类型
enum NETWORK_TYPE
{
  NETTYPE_UNAVAILABLE,  //  0: 不可用
  NETTYPE_WIFI,         //  1: wifi
  NETTYPE_WIRED,        //  2: 有线
  NETTYPE_2G,           //  3: 网络类型为 2G 移动网络。
  NETTYPE_3G,           //  4: 网络类型为 3G 移动网络。
  NETTYPE_4G,           //  5: 网络类型为 4G 移动网络。
  NETTYPE_5G,           //  6: 网络类型为 5G 移动网络。
};

//本地视频流发布状态
enum LOCAL_VIDEO_STREAM_STATE
{
  LOCAL_VIDEO_STREAM_STATE_STOPPED = 0,    // 0: 本地视频默认初始状态。
  LOCAL_VIDEO_STREAM_STATE_CAPTURING = 1,  // 1: 本地视频启动成功。
  LOCAL_VIDEO_STREAM_STATE_FAILED = 2,     // 2: 本地视频启动失败。
  LOCAL_VIDEO_STREAM_STATE_SENDING = 3,    // 3: 正在发送媒体数据
  LOCAL_VIDEO_STREAM_STATE_NO_SEND = 4,    // 4: 不发送媒体数据
};

//本地视频流错误码
enum LOCAL_VIDEO_STREAM_ERROR
{
  LOCAL_VIDEO_STREAM_ERROR_OK = 0,                    // 0: 本地视频状态正常。
  LOCAL_VIDEO_STREAM_ERROR_FAILURE = 1,               // 1: 出错原因不明确。
  LOCAL_VIDEO_STREAM_ERROR_DEVICE_NO_PERMISSION = 2,  // 2: 没有权限启动本地视频采集设备。
  LOCAL_VIDEO_STREAM_ERROR_DEVICE_BUSY = 3,           // 3: 本地视频采集设备正在使用中。
  LOCAL_VIDEO_STREAM_ERROR_CAPTURE_FAILURE = 4,       // 4: 本地视频采集失败，建议检查采集设备是否正常工作。
  LOCAL_VIDEO_STREAM_ERROR_ENCODE_FAILURE = 5,        // 5: 地视频编码失败。
};

//本地音频流发布状态
enum LOCAL_AUDIO_STREAM_STATE
{
  LOCAL_AUDIO_STREAM_STATE_STOPPED = 0,    // 0: 本地音频默认初始状态。
  LOCAL_AUDIO_STREAM_STATE_RECORDING = 1,  // 1: 本地音频录制设备启动成功。
  LOCAL_AUDIO_STREAM_STATE_FAILED = 2,     // 2: 本地音频启动失败。
  LOCAL_AUDIO_STREAM_STATE_SENDING = 3,    // 3: 正在发送媒体数据
  LOCAL_AUDIO_STREAM_STATE_NO_SEND = 4,    // 4: 不发送媒体数据
};

//本地音频流错误码
enum LOCAL_AUDIO_STREAM_ERROR
{
  LOCAL_AUDIO_STREAM_ERROR_OK = 0,                    // 0: 本地音频状态正常。
  LOCAL_AUDIO_STREAM_ERROR_FAILURE = 1,               // 1: 本地音频出错原因不明确。
  LOCAL_AUDIO_STREAM_ERROR_DEVICE_NO_PERMISSION = 2,  // 2: 没有权限启动本地音频录制设备。
  LOCAL_AUDIO_STREAM_ERROR_DEVICE_BUSY = 3,           // 3: 本地音频录制设备已经在使用中。
  LOCAL_AUDIO_STREAM_ERROR_RECORD_FAILURE = 4,        // 4: 本地音频录制失败，建议你检查录制设备是否正常工作。
  LOCAL_AUDIO_STREAM_ERROR_ENCODE_FAILURE = 5,        // 5: 本地音频编码失败。
};

//本地流发布状态
enum PUBLISH_STREAM_STATE
{
  PUBLISH_STREAM_STREAM_STATE_ERROR = 0,       // 0: 发布失败
  PUBLISH_STREAM_STREAM_STATE_NO_PUBLISH = 1,  // 1: 未发布
  PUBLISH_STREAM_STREAM_STATE_PUBLISHED = 2,   // 2: 正在发布
};

//远端流订阅状态
enum SUBSCRIBE_STREAM_STATE
{
  SUBSCRIBE_STREAM_STATE_FAILED = 0,           // 0: 流订阅失败
  SUBSCRIBE_STREAM_STATE_OFFLINE = 1,          // 1: 流下线
  SUBSCRIBE_STREAM_STATE_ONLINE = 2,           // 2: 流上线
  SUBSCRIBE_STREAM_STATE_NO_SEND = 3,          // 3: 流上线但没发送
  SUBSCRIBE_STREAM_STATE_NO_RECV = 4,          // 4: 流上线且发送但不接收
  SUBSCRIBE_STREAM_STATE_NO_SEND_NO_RECV = 5,  // 5: 流上线但没发送且不接收
  SUBSCRIBE_STREAM_STATE_SUBSCRIBING = 6,      // 6: 正在订阅
  SUBSCRIBE_STREAM_STATE_SUBSCRIBED = 7,       // 7: 订阅成功
  SUBSCRIBE_STREAM_STATE_FROZEN = 8,           // 8: 卡顿中  ，这个会取消掉，1.5.3 以后不会用。卡顿状态日志有单独的日志
};

//用户状态
enum USER_OFFLINE_REASON_TYPE
{
  USER_OFFLINE_QUIT = 0,  // 0: 用户主动离开。
  USER_OFFLINE_DROPPED =
      1,  // 1: 因过长时间收不到对方数据包，超时掉线。注意：由于 SDK 使用的是不可靠通道，也有可能对方主动离开本方没收到对方离开消息而误判为超时掉线。
  USER_OFFLINE_BECOME_AUDIENCE = 2,  // 2: 用户身份从主播切换为观众时触发。
};

enum LOG_FILTER_TYPE
{
  LOG_FILTER_OFF = 0,    // 不输出文件
  LOG_FILTER_DEBUG = 1,  // 输出所有 API 日志信息。 如果你想获取最完整的日志，可以将日志级别设为该等级。
  LOG_FILTER_INFO = 2,   // 输出 FATAL、ERROR、WARNING 和 INFO 级别的日志信息。 我们推荐你将日志级别设为该等级。
  LOG_FILTER_WARN = 3,   // 输出 FATAL、ERROR 和 WARNING 级别的日志信息。
  LOG_FILTER_ERROR = 4,  // 输出 FATAL 和 ERROR 级别的日志信息。
  LOG_FILTER_FATAL = 5,  // 输出 FATAL 级别的日志信息。
};

}  // namespace rz

#endif  // PAASSDK_IRTCENGINESTATE_H
