//
// Created by xj on 2018/7/6.
//

#include "IPlayer.h"
#include "IDemux.h"
#include "IDecode.h"
#include "IResample.h"
#include "IVideoView.h"
#include "IAudioPlay.h"
#include "XLog.h"

IPlayer *IPlayer::get(unsigned char index) {
    static IPlayer p[256];
    return &p[index];
}

void IPlayer::Close() {
    mux.lock();
    //1.先关闭主体线程在清理观察者
    //同步线程
    XThread::Stop();
    //解封装
    if (demux)
        demux->Stop();
    //解码
    if (vdecode)
        vdecode->Stop();
    if (adecode)
        adecode->Stop();
    if(audioPlay)
        audioPlay->Stop();

    //2.清理缓冲队列
    if (vdecode)
        vdecode->Clear();
    if (adecode)
        adecode->Clear();
    if (audioPlay)
        audioPlay->Clear();

    //3.清理资源
    if (audioPlay)
        audioPlay->Close();
    if (videoView)
        videoView->Close();
    if (vdecode)
        vdecode->Close();
    if (adecode)
        adecode->Close();
    if (demux)
        demux->Close();

    mux.unlock();
}

bool IPlayer::open(const char *path) {
    Close();
    mux.lock();
    //解封装模块初始化
    if (!demux || !demux->Open(path)) {
        XLOGE("demux open failed!!");
        mux.unlock();
        return false;
    }
    //视频解码初始化--有可能存在解封装之后就是原始数据
    if (!vdecode || !vdecode->Open(demux->getVPara(), isHardDecode)) {
        XLOGE("vdecode open failed!!");
//        return false;
    }
    //音频解码初始化
    if (!adecode || !adecode->Open(demux->getAPara())) {
        XLOGE("adecode open failed!!");
//        return false;
    }
    //重采样--有可能不需要-解码后或者解封装后直接能够播放的数据
//    if (!outpara.para)
        outpara = demux->getAPara();
    if (!resample || !resample->Open(demux->getAPara(), outpara)) {
        XLOGE("resample open failed!!");
    }
    XLOGE("IPlayer open sucess!!");
    mux.unlock();
    return true;
}

bool IPlayer::Start() {
    mux.lock();
    //一般先启动音频
    if (audioPlay)
        audioPlay->StartPlay(outpara);

    if (vdecode)vdecode->Start();
    if (adecode)adecode->Start();

    if (!demux || !demux->Start()) {
        XLOGE("demux start failed!!");
        mux.unlock();
        return false;
    }

    XThread::Start();
    mux.unlock();
    return true;
}

void IPlayer::initView(void *win) {
    if(videoView) {
        videoView->Close();
        if (videoView)videoView->SetRender(win);
    }
}

void IPlayer::Main() {

    while (!isExits) {
        mux.lock();
        if (!audioPlay || !vdecode) {
            mux.unlock();
            XSleep(2);
            continue;
        }
        //同步
        //获取音频的pts
        long apts = audioPlay->pts;
        //获取音频的pts，告诉视频
        vdecode->synpts = apts;

        mux.unlock();
        XSleep(2);
    }


}
