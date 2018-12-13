//
// Created by xj on 2018/7/6.
//

#ifndef FFMPEGONEDEMO_IPLAYER_H
#define FFMPEGONEDEMO_IPLAYER_H


#include <sys/types.h>
#include "XThread.h"
#include "XParameter.h"
#include <mutex>


class IDemux;
class IDecode;
class IResample;
class IVideoView;
class IAudioPlay;

class IPlayer : public XThread {
public:
    static IPlayer *get(unsigned char index=0);
    virtual bool open(const char *path);
    virtual void Close();
    virtual bool Start();
    virtual void initView(void *win);
    //是否开启硬解码
    bool  isHardDecode=true;
    //音频输出参数配置
    XParameter outpara;

    IDemux *demux=0;
    IDecode *vdecode=0;
    IDecode *adecode=0;
    IResample *resample=0;
    IVideoView *videoView=0;
    IAudioPlay *audioPlay=0;
protected:
    //用作音视频同步
    void Main();
    std::mutex mux;
    IPlayer(){};
};


#endif //FFMPEGONEDEMO_IPLAYER_H
