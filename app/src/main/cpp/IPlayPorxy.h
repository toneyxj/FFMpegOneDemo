//
// Created by xj on 2018/7/9.
//

#ifndef FFMPEGONEDEMO_IPLAYPORXY_H
#define FFMPEGONEDEMO_IPLAYPORXY_H

#include "IPlayer.h"
#include <mutex>

//代理类
class IPlayPorxy :public IPlayer{
public:
    static IPlayPorxy *Get(){
        static IPlayPorxy px;
        return &px;
    }
    void init(void *vm=0);

    virtual bool open(const char *path);
    virtual void Close();
    virtual bool Start();
    virtual void initView(void *win);

protected:
    IPlayPorxy(){};
    IPlayer *player=0;
    std::mutex mux;
};


#endif //FFMPEGONEDEMO_IPLAYPORXY_H
