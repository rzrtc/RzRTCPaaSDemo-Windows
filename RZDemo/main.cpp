#ifdef _WIN32

int main(int argc, char *argv[]) {
    return 0;
}
#else
#include <iostream>
#include <memory>

#include "Base/DataFlow.h"
#include "Src/ProcessImpl/RZAudioFile.h"
#include "include/DataFlow/DataConsumer.h"
#include "include/DataFlow/DataProducer.h"
#include "include/ProcessInterface/VideoScaleCrop.h"
using namespace rz;

class sink : public rz::IVideoSink {
    uint32_t lastts = 0;

public:
    bool onInitialize() override {
        return true;
    };

    bool onStart() override {
        return true;
    }

    void onStop() override {
    }

    void onDispose() override {
    }

    rz::VideoSinkConfig getSinkConfig() override {
        rz::VideoSinkConfig sinkConfig;
        sinkConfig.streamType = VIDEO_STREAM_FRAME;
        sinkConfig.pixelFormat = VIDEO_PIXEL_I420;
        return sinkConfig;
    }

    void consumeVideoData(rz::VideoData *data) {
        uint8_t *content = nullptr;
        uint32_t len = 0;

        std::cout << "sink consumeVideoData " << data->getTimeStamp() << std::endl;

        if (lastts != data->getTimeStamp())
            std::cout << data->getTimeStamp() << std::endl;

        if (data->getTimeStamp() == 99999)
            std::cout << "recv Complate " << std::endl;

        lastts = data->getTimeStamp() + 1;
    };

    /**
 * 进行数据的抛出
 * sdk调用该接口将视频数据给到平台层进行渲染
 */
    void consumeVideoFrame(const unsigned char *buffer, VIDEO_PIXEL_FORMAT frameType,
                           int width, int height, long timestamp) override {};

    /**
     * streamType = VIDEO_STREAM_H264 || streamType = VIDEO_STREAM_CUSTOM
     * 进行数据的抛出
     * sdk调用该接口将视频数据给到平台层进行渲染
     */
    void consumeVideoPacket(const unsigned char *buffer, long length, VIDEO_STREAM_TYPE streamType,bool isKey, long timestamp) override {};
};

class VideoProcess : public rz::VideoScaleCrop {
public:
    void WorkFun(std::shared_ptr<rz::VideoData> &data) override {
        SendData(data);
    }

    int ReSet(rz::VideoScaleCropConfig &dst) override {
        return 0;
    }

    int Release() override {
        return 0;
    }
};

int processTest() {
    //启动线程池
    rz::ThreadPool::Start();

    sink *sinkPtr = new sink;

    auto consumerPtr = std::make_shared<rz::VideoSinkDataConsumer>(sinkPtr, rz::VIDEO_MIRROR_MODE_TYPE::VIDEO_MIRROR_MODE_AUTO,nullptr);

    auto dataProducerPtr = std::make_shared<rz::VideoSourceDataProducer>(nullptr, nullptr);

    auto process = std::make_shared<VideoProcess>();

    auto videoTestFlow = new rz::DataFlow<rz::VideoData>("testFlow", dataProducerPtr, consumerPtr);

    videoTestFlow->registDataProcess(process);

    uint32_t ts = 0;

    uint8_t *plane[4] = {nullptr};
    uint32_t pitch[4] = {0};

    while (ts < 100000) {
        auto data = new rz::VideoData(rz::VIDEO_PIXEL_I420, ts, 1080, 720,
                                      rz::VIDEO_MIRROR_MODE_DISABLED, plane, pitch);

        dataProducerPtr->consumeVideoData(data);

        ts++;
    }

    std::cout << "send Complate " << std::endl;

    //主线程等待数据帧处理完成才能退出 避免线程池的停止导致的DataFlow失去驱动器 导致数据帧处理不完整
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    //停止线程池
    rz::ThreadPool::Stop();

    return 0;
}

int main() {
    //streamTest
    /*StreamSample streamSample;
    streamSample.init();
    streamSample.videoPubStreamTest();
    streamSample.audioPubStreamTest();
    std::this_thread::sleep_for(std::chrono::seconds(100));
    streamSample.stop();*/

    //threadPoolTest
    /*ThreadPoolSample threadPoolSample;
    threadPoolSample.init();
    //threadPoolSample.audioProcessTaskTest("audio", "../data/16000_16_noise.pcm");
    threadPoolSample.threadPoolTets(10);
    threadPoolSample.stop();*/
    return 0;
}
#endif