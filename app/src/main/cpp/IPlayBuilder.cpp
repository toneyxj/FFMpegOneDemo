//
// Created by xj on 2018/7/9.
//

#include "IPlayBuilder.h"

#include "IDemux.h"
#include "IDecode.h"
#include "GLVideoView.h"
#include "FFResample.h"
#include "SLAudioPlay.h"

IPlayer *IPlayBuilder::builderIPlayer(unsigned char index) {
    IPlayer *paly = createIPlayer(index);
    //解封装
    IDemux *de = createDemux();
    //视频解码
    IDecode *vdecode = createDecode();
    //音频解码
    IDecode *adecode = createDecode();
    //解码观察器
    de->AddObs(vdecode);
    de->AddObs(adecode);
    //显示视频解码器
    IVideoView *view = createIVideoView();
    vdecode->AddObs(view);
    //重采样音频解码器
    IResample *resample = createIResample();
    adecode->AddObs(resample);
    //音频播放观察重采样
    IAudioPlay *audioPlay = createIAudioPlay();
    resample->AddObs(audioPlay);

    paly->demux = de;
    paly->vdecode = vdecode;
    paly->adecode = adecode;
    paly->videoView = view;
    paly->resample = resample;
    paly->audioPlay = audioPlay;

    return paly;
}