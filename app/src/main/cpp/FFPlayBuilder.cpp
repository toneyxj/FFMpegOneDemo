//
// Created by xj on 2018/7/9.
//

#include "FFPlayBuilder.h"
#include "FFDemux.h"
#include "FFDecode.h"
#include "FFResample.h"
#include "GLVideoView.h"
#include "SLAudioPlay.h"

void FFPlayBuilder::InitHard(void *vm){
    FFDecode::InitHard(vm);
}

IDemux *FFPlayBuilder::createDemux(){
     IDemux *ff=new FFDemux();
    return ff;
 }
 IDecode *FFPlayBuilder::createDecode(){
     IDecode *de=new FFDecode();
     return de;
 }
 IResample *FFPlayBuilder::createIResample(){
     IResample *re=new FFResample();
     return re;
 }
 IVideoView *FFPlayBuilder::createIVideoView(){
     IVideoView *vv=new GLVideoView();
     return vv;
 }
 IAudioPlay *FFPlayBuilder::createIAudioPlay(){
     IAudioPlay *ap=new SLAudioPlay();
     return ap;
 }
 IPlayer *FFPlayBuilder::createIPlayer(unsigned char index){
     return IPlayer::get(index);
 }
