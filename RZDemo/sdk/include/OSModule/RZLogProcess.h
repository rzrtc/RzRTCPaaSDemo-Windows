//
// Created by 86157 on 2020/9/23.
//

#include <map>
#include <string>

#ifndef PAASSDK_RZLOGPROCESS_H
#define PAASSDK_RZLOGPROCESS_H

namespace rz {

    enum RZ_LOG_LEVEL {
        RZ_LOG_LEVEL_DEBUG,      //运行时的调试信息 发版后不需要打印
        RZ_LOG_LEVEL_INFO,       //状态性信息 用于问题排查
        RZ_LOG_LEVEL_WORN,       //警告性信息 拥有一定的风险 但程序可以继续运行
        RZ_LOG_LEVEL_ERROR,      //模块性的错误信息 需要对单模块进行处理
        RZ_LOG_LEVEL_FATAL       //严重性错误信息 需要整个SDK进行退出
    };

    // 用于 http  Get() 的返回值
    enum HttpErrorCode{
    No_Error = 0,

    /** 链接阶段的错误100开始 */
    Connect_Error_Start_From_100 = 100,     //该错误码为了阶段划分, 不要使用
    DNS_Resolve_Fail,                       //DNS解析失败, 待研究确认
    Connect_Timeout,                        //链接服务器超时, 待研究确认, 可能不能这么细致
  
      /** 发送阶段的错误200开始 */
     Send_Error_Start_From_200 = 200,       //该错误码为了阶段划分, 不要使用
     Send_Timeout,                          //发送数据超时
   
     /** 接收阶段的错误300开始 */
    Receive_Error_Start_From_300 = 300,     //该错误码为了阶段划分, 不要使用
    Receive_Timeout,                        //接收数据超时

     Other_Error_Start_From_400 = 400
};


    struct RZLogConfig {
        /**是否到控制台*/
        bool            enableConsole       = false;
        //是否输出到文件
        bool            enableLogFile       = false;
        /**
         完整的文件路径
         1. null, 空字符串, 用默认的
         2. 非法，不写
         */
        const char* filePath                = nullptr;        //本地日志路径
        uint32_t        fileSizeInKBytes    = 0;              //日志文件大小,默认10MB
        uint32_t        logCacheSize        = 0;              //KB, 默认80KB
    };

    class RZLog {
    public:
        virtual void WriteLog(RZ_LOG_LEVEL level, const char *msg) = 0;

        virtual void Flush() = 0;

        virtual void Init(const RZLogConfig &logCfg) = 0;

        virtual void Release() = 0;

        virtual ~RZLog() = default;
    };


    class RZHttpClient {
    public:
        virtual int Get(const std::string &url, std::string &response) = 0;

        virtual int Post(const std::string &url, const std::string &forms, std::string &response) = 0;

        virtual void Release() = 0;

        virtual ~RZHttpClient() = default;
    };
}
#endif //PAASSDK_RZLOGPROCESS_H
