//
// Created by xj on 2018/7/9.
//

#ifndef FFMPEGONEDEMO_FFPLAYBUILDER_H
#define FFMPEGONEDEMO_FFPLAYBUILDER_H

#include "IPlayBuilder.h"

class FFPlayBuilder: public IPlayBuilder {
public:
     static void InitHard(void *vm);
    static FFPlayBuilder *Get(){
        static FFPlayBuilder ff;
        return &ff;
    }
protected:
    FFPlayBuilder(){};
    virtual IDemux *createDemux();
    virtual IDecode *createDecode();
    virtual IResample *createIResample();
    virtual IVideoView *createIVideoView();
    virtual IAudioPlay *createIAudioPlay();
    virtual IPlayer *createIPlayer(unsigned char index);
};


#endif //FFMPEGONEDEMO_FFPLAYBUILDER_H
