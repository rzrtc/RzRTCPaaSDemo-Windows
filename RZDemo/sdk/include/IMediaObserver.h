#pragma once

#include <cstdint>

namespace rz {

enum VIDEO_FRAME_TYPE
{ VIDEO_FRAME_I420 };

struct VideoFrame {
  VIDEO_FRAME_TYPE type = VIDEO_FRAME_I420;
  int width = 0;
  int height = 0;
  int yStride = 0;
  int uStride = 0;
  int vStride = 0;
  void* yBuffer = nullptr;
  void* uBuffer = nullptr;
  void* vBuffer = nullptr;
  int32_t timeStamp = 0;
  ~VideoFrame() {
    delete[] yBuffer;
    delete[] uBuffer;
    delete[] vBuffer;
  }
};

class VideoObserver {
 public:
  //获取各频道远端视频播放前的数据
  virtual bool onRenderVideoFrame(const char* channelId, const char* uid, const char* streamName, VideoFrame& videoFrame) = 0;

  //获得采集的视频
  virtual bool onCaptureVideoFrame(const char* streamName, VideoFrame& videoFrame) = 0;

  //获得视频编码前的数据
  virtual bool onPreEncodeVideoFrame(const char* streamName, VideoFrame& videoFrame) = 0;
};

enum VIDEO_OBSERVER_POSITION
{
  POSITION_POST_CAPTURER = 1 << 0,  //视频采集后的位置
  POSITION_PRE_ENCODER = 1 << 1,    //视频编码前的位置
  POSITION_PRE_RENDERER = 1 << 2    //远端视频播放前的位置
};

struct VideoObserverInfo {
  VideoObserver* observer = nullptr;
  uint8_t observerPosition = 0;  //该值的由来为OBSERVER_POSITION各个枚举值通过 & 操作同时配置多个观察点
};

enum AUDIO_FRAME_TYPE
{ AUDIO_FRAME_PCM16 };

struct AudioFrame {
  AUDIO_FRAME_TYPE type = AUDIO_FRAME_PCM16;  //音频数据类型
  int samples = 320;                          //该帧音频数据采样数
  int bytesPerSample = 2;                     //每个采样占有的byte数量
  int channels = 1;                           //声道数
  int samplesRate = 16000;                    //采样率
  void* buffer = nullptr;                     //数据
  uint32_t timeStamp = 0;                     //音频帧时间戳
  ~AudioFrame() { delete[] buffer; }
};

class AudioObserver {
 public:
  //获取各频道混音前的音频播放数据。
  virtual bool onPlaybackAudioFrameBeforeMixing(const char* channelId, const char* uid, AudioFrame& audioFrame) = 0;

  //获得采集的音频。
  virtual bool onRecordAudioFrame(AudioFrame& audioFrame) = 0;

  //获得播放的声音。
  virtual bool onPlaybackAudioFrame(AudioFrame& audioFrame) = 0;
};

enum AUDIO_OBSERVER_POSITION
{
  AUDIO_OBSERVER_RECORD = 1 << 0,    //音频采集后的位置
  AUDIO_OBSERVER_PLAYBACK = 1 << 1,  //音频播放前的位置
  AUDIO_OBSERVER_MIXBEFORE = 1 << 2  //音频混音前的位置
};

struct AudioObserverInfo {
  AudioObserver* observer = nullptr;
  uint8_t observerPosition = 0;  //该值的由来为OBSERVER_POSITION各个枚举值通过 & 操作同时配置多个观察点
};
}  // namespace rz
