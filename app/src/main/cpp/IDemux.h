//
// Created by xj on 2018/5/31.
//

#ifndef FFMPEGONEDEMO_IDEMUX_H
#define FFMPEGONEDEMO_IDEMUX_H

#include "XData.h"
#include "Iobserver.h"
#include "XParameter.h"

//解封装接口
class IDemux :public Iobserver{
    //-----virtual 创建纯虚函数，也就是接口
public:
    //打开文件或者流媒体 rmtp http rtsp
    virtual bool Open(const char *url)=0;
    virtual void Close()=0;
    //获取视频参数-存虚函数意思是可以
    virtual XParameter getVPara()=0;
    //初始化获取音视频或者其它数据通道的位置
    virtual void initDecodeIndex()=0;
    //获取音频参数-存虚函数意思是可以
    virtual XParameter getAPara()=0;
    //读取一针数据，数据由调用者清理。
    virtual XData Read()=0;
    //总时长，单位毫秒
    int totalMs=0;


protected:
    virtual void Main();
};


#endif //FFMPEGONEDEMO_IDEMUX_H
