//
// Created by 86157 on 2020/9/23.
//

#ifndef PAASSDK_RZSYSTEMINFO_H
#define PAASSDK_RZSYSTEMINFO_H

#include <cstdint>
#include <string>

#include "IRtcEngineState.h"

namespace rz {

struct systemInfo {
    //获取操作系统名称
    std::string OSName;
    //获取平台信息
    std::string Platform;
    //获取产品名加平台信息的名称
    std::string ClientName;
    //获取sdk版本号
    std::string ClientVersion;
    //获取app包名
    std::string AppPack;
    //获取app版本
    std::string AppVersion;
    //获取终端ID
    std::string DeviceID;
    //获取设备名称
    std::string DeviceName;
    //获取设备型号
    std::string Model;
    //获取操作系统版本
    std::string OSRelease;
    //获取当前系统的网络类型
    NETWORK_TYPE systemNetType = NETTYPE_UNAVAILABLE;
};

struct systemLoadInfo {
    //当前系统CPU使用率
    float systemCpuUsag = 0.0;
    //当前APP的CPU使用率
    float appCpuUsage = 0.0;
    //当前系统内存使用量 MB
    uint32_t systemMemUsag = 0;
    //当前APP的内存使用量 MB
    uint32_t appMemUsage = 0;
};

class systemInfoCollectionEventHandler {
public:
    //网络类型变化通知
    virtual void onNetTypeChanged(NETWORK_TYPE oldNetType, NETWORK_TYPE newNetType) = 0;

    virtual void onSystemLoadInfo(const systemLoadInfo& loadInfo) = 0;

    virtual ~systemInfoCollectionEventHandler() = default;
};

class systemInfoCollection {
public:
    virtual systemInfo getSystemInfo() = 0;

    virtual ~systemInfoCollection() = default;
};
}  // namespace rz

#endif  //PAASSDK_RZSYSTEMINFO_H
