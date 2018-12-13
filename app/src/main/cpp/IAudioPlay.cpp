//
// Created by xj on 2018/6/29.
//

#include "IAudioPlay.h"
#include "XLog.h"
 XData IAudioPlay::getData(){
     XData d;
     while (!isExits){
         frameMutex.lock();
         if(!frames.empty()){
             d=frames.front();
             frames.pop_front();
             frameMutex.unlock();
             pts=d.pts;
             return d;
         }
         frameMutex.unlock();
         XSleep(1);
     }
     pts=d.pts;
     //未获取到数据
     return d;
 }
void IAudioPlay::Update(XData data) {
    //压如缓冲队列
    if (data.size <= 0 || !data.data)return;
    while (!isExits) {
        frameMutex.lock();
        if (frames.size() > maxFrame) {
            frameMutex.unlock();
            XSleep(1);
            continue;
        }
        frames.push_back(data);
        frameMutex.unlock();
        break;
    }
}
void IAudioPlay::Clear(){
    frameMutex.lock();
    while (!frames.empty()){
        frames.front().Drop();
        frames.pop_front();
    }
    frameMutex.unlock();
}