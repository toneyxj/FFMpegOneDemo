//
// Created by xj on 2018/6/25.
//

#include "IDecode.h"
#include "XLog.h"


void IDecode::Update(XData pkt) {
    //判断是否是自己解码的数据
    if(pkt.decodeType!=decodeType){//一样的视频类型进行处理
        return;
    }
    while (!isExits) {
        packsMutex.lock();
        if (packs.size() <= maxList) {
            packs.push_back(pkt);
            packsMutex.unlock();
            break;
        }
            packsMutex.unlock();
            XSleep(2);
    }
}
//启动线程的进入
void IDecode::Main() {
    while (!isExits) {
        packsMutex.lock();
        //判断音视频同步
        if(decodeType==0&&synpts>0){//音频
        if(synpts<pts){
            packsMutex.unlock();
            XSleep(1);
            continue;
        }
        }
        if(packs.empty()){
            packsMutex.unlock();
            XSleep(2);
            continue;
        }
        //取出第一个数据
        XData pack=packs.front();
        //删除链表的第一个数据
        packs.pop_front();
        //发送数据到解码线程进行解码--FFMpeg会复制一份数据所以是线程安全的
        if(this->SendPacket(pack)){
            //获取解码结果
            while (!isExits){
                //获取解码数据
                XData frame=RecvFrame();
                //获取解码数据可能同时有多个解码数据所以循环获取解码数据
                if(!frame.data)break;
                pts=frame.pts;
//                XLOGE("RecvFrame index=%d frame %d",decodeType,frame.size);
                //获得解码数据发送解码数据给显示模块使用
                this->Notify(frame);
            }
        }
        //删除无用的音视频解码原数据
        pack.Drop();
        packsMutex.unlock();
    }
}
void IDecode::Clear(){
    packsMutex.lock();
    while (!packs.empty()){
        packs.front().Drop();
        packs.pop_front();
    }
    pts=0;
    synpts=0;

    packsMutex.unlock();
}
