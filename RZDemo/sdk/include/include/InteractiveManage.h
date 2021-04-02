//
// Created by yhy on 11/1/20.
//

#ifndef PAASSDK_INTERACTIVEMANAGE_H
#define PAASSDK_INTERACTIVEMANAGE_H

#include <map>
#include <list>
#include <mutex>
#include <atomic>
#include <thread>
#include <functional>
#include <condition_variable>

#include "RZTime.h"
#include "RZLog.h"

namespace rz{

    struct InteractiveTask{
        std::string taskName;
        uint32_t startTime;
        std::function<void ()> taskFun = nullptr;
        std::function<void()> taskOverFun = nullptr;
        explicit InteractiveTask(std::string name,std::function<void ()> fun){
            taskName = name;
            taskFun = fun;
            startTime = RZTime::opts();
        }
    };

    class AsyncQuit;

    class InteractiveManage{
    private:
        struct TimedTask {
            std::function<void(void)> taskFun = nullptr;              //任务函数
            int timeInterval = 0;                                     //任务调用间隔   ms     最小间隔 10ms
            int taskImplemenCount = 0;                                //任务已经执行的次数
            uint32_t taskStartTime = 0;                                    //任务启动时间
        };

        static std::mutex timedTaskPoolMX;
        static std::map<std::string, std::shared_ptr<TimedTask>> timedTaskPool;    //定时任务池

        static std::mutex taskPoolMX;
        static std::list<InteractiveTask> taskPool;
        static std::thread workTh;

        static std::condition_variable conditionVariable;

        static std::atomic<bool> running;

        //各静态变量遵循先声明后释放准则 异步退出依赖于静态变量asyncQuit的析构函数清理退出线程
        //TODO 因此将静态变量asyncQuit放在清理线程InteractiveManage::workTh的后方 不可更改
        static AsyncQuit asyncQuit;

    protected:
        static void workFun();
    public:

        static void addTask(InteractiveTask task);

        /**
         * 注册定时任务
         * @param taskId 任务ID
         * @param taskFun 任务执行函数
         * @param timeInterval 任务执行间隔 /ms 不得小于10ms 系统依据任务启动时间、当前时间和任务执行间隔计算执行次数,可以保障长时间内的任务执行次数与任务间隔的一致性
         * @param Immediate 是否立即执行
         */
        static void registTimedTask(const std::string& taskId, std::function<void(void)> taskFun, int timeInterval, bool Immediate);

        /**
         * 移除定时任务
         * @param taskId 任务ID
         */
        static void removeTimedTask(const std::string& taskId);

        static std::thread::id getThreadId();

        static void taskClear();

        static void start();

        static void stop();
    };

    class AsyncQuit {
    public:
        
        ~AsyncQuit(){
            InteractiveManage::stop();
        }
    };
}


#endif //PAASSDK_INTERACTIVEMANAGE_H
