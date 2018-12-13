//
// Created by xj on 2018/7/9.
//

#include "IPlayPorxy.h"
#include "FFPlayBuilder.h"
#include "XLog.h"

void IPlayPorxy::init(void *vm){
    mux.lock();
    if (vm){
        FFPlayBuilder::InitHard(vm);
    }
    if(!player)player=FFPlayBuilder::Get()->builderIPlayer();
    mux.unlock();
}

 bool IPlayPorxy::open(const char *path){
     bool re= false;
     mux.lock();
     if(player)re=player->open(path);
     mux.unlock();
     return re;
}

 bool IPlayPorxy::Start(){
     bool re= false;
     mux.lock();
     if(player)re=player->Start();
     mux.unlock();
     return re;
}

void IPlayPorxy::initView(void *win){
     bool re= false;
     mux.lock();
     if(player)player->initView(win);
     mux.unlock();
}
 void IPlayPorxy::Close(){
     mux.lock();
     if(player)
     player->Close();
     mux.unlock();
 }