//
// Created by xj on 2018/5/31.
//

#ifndef FFMPEGONEDEMO_FFDEMUX_H
#define FFMPEGONEDEMO_FFDEMUX_H



#include "IDemux.h"
#include <mutex>
//结构体声明 ，这样可以不用引用头文件。
struct AVFormatContext;

class FFDemux: public IDemux {
public:
    //打开文件或者流媒体 rmtp http rtsp
    virtual bool Open(const char *url);
    virtual void Close();
    //获取视频参数
    virtual XParameter getVPara();
    //初始化获取音视频或者其它数据通道的位置
    virtual void initDecodeIndex();
    //获取音频参数
    virtual XParameter getAPara();
    //读取一针数据，数据由调用者清理。
    virtual XData Read();
    FFDemux();

private:
    AVFormatContext *ic=0;//值只有在无参数的构造函数中才能生效
    //音视频索引
    int audioStream=1;
    int videoStream=0;
    std::mutex mut;
};


#endif //FFMPEGONEDEMO_FFDEMUX_H
