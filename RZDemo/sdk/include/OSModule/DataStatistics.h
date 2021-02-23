//
// Created by 86157 on 2020/9/28.
//

#ifndef PAASSDK_DATASTATISTICS_H
#define PAASSDK_DATASTATISTICS_H

#include <string>

namespace rz {
    /**
     * 创建DataStatistics需要提供的配置信息
     */
    struct DataStatisticsConfig{
        std::string url;            //上传统计数据的服务器地址, 必须提供, 用户提供, c++实现默认值
        std::string loganFilePath;  //统计数据缓存文件的路径, 用户可以不指定, 平台层实现默认值
        int         sendiInterval;  //统计数据缓存文件发送间隔, 单位秒, 默认60秒
    };

    class DataStatistics {
    public:
        /**
         * 统计数据发送,通常按照配置的internal定时发送, 频道退出的时候为了保险
         * 需要手动调用一次, 该调用需要异步, 起到通知作用
         * @param jsonData
         */
        virtual void StatisticsDataSend() = 0;
        /**
         * 统计数据写文件
         */
        virtual void StatisticsDataWrite(const char* jsonData) = 0;

        /**
         * 释放函数
         */
        virtual void Release() = 0;

        virtual ~DataStatistics() = default;
    };
}

#endif //PAASSDK_DATASTATISTICS_H
