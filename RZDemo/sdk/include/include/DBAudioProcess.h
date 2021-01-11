#pragma once
#include <memory>

class DBAudioProcess {
    class Implementation;
    std::unique_ptr<Implementation> pimpl;
public:
    DBAudioProcess(int sample);
    ~DBAudioProcess();

    // mode 0 1 2 3
    int WebRtcVadCreate(int mode);

    // 静音返回true  
    bool WebRtcVadProcess(char* pSrcData, int len);

    int WebRtcNsCreate(int mode);

    int WebRtcNsProcess(char* pSrcData, int len);

    int WebRtcAecCreate(int16_t nlpMode);

    int WebRtcAecBufferFarendSet(size_t nDataLen, char* szFarData);

    int WebRtcAecBufferProcess(size_t nDataLen, char* szNearData, uint16_t delayTime);

    int WebRtcAecProcess(size_t nFarLen, char* pFarData, size_t nNearLen, char* pNearData, uint16_t delayTime);

    int WebRtcAec3Process(size_t nFarLen, char* pFarData, size_t nNearLen, char* pNearData, uint16_t delayTime);

    int WebRtcAgcCreate(int16_t nlpMode);

    int WebRtcAgcProcess(char* pSrcData, int len);

    int WebRtcAec3Create(int16_t nlpMode);

    int getAec3Delay();
};