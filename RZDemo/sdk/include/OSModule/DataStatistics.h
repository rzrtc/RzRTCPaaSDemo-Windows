//
// Created by 86157 on 2020/9/28.
//

#ifndef PAASSDK_DATASTATISTICS_H
#define PAASSDK_DATASTATISTICS_H

namespace rz {

    class DataStatistics {
    public:
        /**
         * 统计数据发送
         * @param jsonData
         */
        virtual void StatisticsDataSend(const char *jsonData) = 0;

        /**
         * 释放函数
         */
        virtual void Release() = 0;

        virtual ~DataStatistics() = default;
    };
}

#endif //PAASSDK_DATASTATISTICS_H
