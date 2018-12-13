//
// Created by xj on 2018/6/29.
//

#ifndef FFMPEGONEDEMO_SLAUDIOPLAY_H
#define FFMPEGONEDEMO_SLAUDIOPLAY_H


#include "IAudioPlay.h"

class SLAudioPlay : public IAudioPlay{
public:
    virtual bool StartPlay(XParameter out);
    void PlayCall(void  *);
    virtual void Close();
    //构造函数析构函数
    SLAudioPlay();
    virtual ~SLAudioPlay();

protected:
    unsigned char *buf=0;
    std::mutex mux;
};


#endif //FFMPEGONEDEMO_SLAUDIOPLAY_H
