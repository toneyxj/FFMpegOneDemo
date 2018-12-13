//
// Created by xj on 2018/6/29.
//

#ifndef FFMPEGONEDEMO_IRESAMPLE_H
#define FFMPEGONEDEMO_IRESAMPLE_H


#include "Iobserver.h"
#include "XParameter.h"

class IResample : public Iobserver{
public:
    //输入参数是解码器获取，输出参数可以自己配置，目前采用默认
    virtual bool Open(XParameter in,XParameter out=XParameter())=0;
    //解码音频数据
    virtual XData Resample(XData inData)=0;
    virtual void Close()=0;
    virtual void Update(XData data);
    int outChannels=2;//通道数
    int outFormat=1;
};


#endif //FFMPEGONEDEMO_IRESAMPLE_H
