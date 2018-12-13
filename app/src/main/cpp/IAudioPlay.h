//
// Created by xj on 2018/6/29.
//

#ifndef FFMPEGONEDEMO_IAUDIOPLAY_H
#define FFMPEGONEDEMO_IAUDIOPLAY_H


#include <list>
#include "Iobserver.h"
#include "XParameter.h"

class IAudioPlay : public Iobserver{
public:
    //缓冲满之后阻塞函数
    virtual void Update(XData data);
    //获取缓冲数据如没有则阻塞
    virtual XData getData();
    virtual bool StartPlay(XParameter out)=0;
    virtual void Close()=0;
    virtual void Clear();
    //最大缓冲
    int maxFrame=100;
    long pts=0;
protected:
    std::list<XData> frames;
    std::mutex frameMutex;

};


#endif //FFMPEGONEDEMO_IAUDIOPLAY_H
