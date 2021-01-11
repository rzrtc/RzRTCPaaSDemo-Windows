//
// Created by yhy on 10/13/20.
//

#ifndef PAASSDK_GLOBALOPERATION_H
#define PAASSDK_GLOBALOPERATION_H

#include <map>
#include <mutex>

#include "MediaSink.h"
#include "MediaSource.h"

namespace rz{
    class GlobalOperation{
    private:
        static std::shared_ptr<VideoSourceProducer> videoSourceProducer;
        static std::shared_ptr<AudioSourceProducer> audioSourceProducer;
        static std::shared_ptr<VideoSinkProducer> videoSinkProducer;
        static std::mutex viewPoolMX;
        static std::map<void*, void*> viewPool;
    public:

        static void Init();

        static void Release();

        /**
         * 更新显示模式
         * @param videoSink 要配置的videosink
         * @param renderMode
         */
        static void setVideoRenderMode(const VideoSink *videoSink, RENDER_MODE_TYPE renderMode);


        /**
         * 为videosink 配置画板 videocanvas
         * @param videoSink 要配置的videosink
         * @param videoCanvas 画板参数
         */
        static void setVideoCanvas(const VideoSink *videoSink, const VideoCanvas &videoCanvas);

        /**
         * 更新视频方向模式。
         * @param orientationMode
         */
        static void setVideoOrientationMode(VideoSource *source,ORIENTATION_MODE orientationMode);

        /**
         * 重置视频采集格式
         * @param videoConfig
         */
        static void setVideoConfig(VideoSource *source,const VideoConfig &videoConfig);

        static void HoldView(void* view);

        static void ReleaseView(void* view);
    };
}


#endif //PAASSDK_GLOBALOPERATION_H
