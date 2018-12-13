//
// Created by xj on 2018/6/29.
//

#ifndef FFMPEGONEDEMO_FFRESAMPLE_H
#define FFMPEGONEDEMO_FFRESAMPLE_H


#include "IResample.h"
struct SwrContext;

class FFResample : public IResample{
    virtual bool Open(XParameter in,XParameter out=XParameter());
    virtual void Close();
    virtual XData Resample(XData inData);
protected:
    SwrContext *actx=0;
    std::mutex mux;
};


#endif //FFMPEGONEDEMO_FFRESAMPLE_H
