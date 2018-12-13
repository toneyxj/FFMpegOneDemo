//
// Created by xj on 2018/7/9.
//

#ifndef FFMPEGONEDEMO_IPLAYBUILDER_H
#define FFMPEGONEDEMO_IPLAYBUILDER_H

#include "IPlayer.h"

class IPlayBuilder {

public:
    virtual IPlayer *builderIPlayer(unsigned char index=0);
protected:
    virtual IDemux *createDemux()=0;
    virtual IDecode *createDecode()=0;
    virtual IResample *createIResample()=0;
    virtual IVideoView *createIVideoView()=0;
    virtual IAudioPlay *createIAudioPlay()=0;
    virtual IPlayer *createIPlayer(unsigned char index)=0;
};


#endif //FFMPEGONEDEMO_IPLAYBUILDER_H
