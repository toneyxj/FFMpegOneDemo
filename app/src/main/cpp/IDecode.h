//
// Created by xj on 2018/6/25.
//

#ifndef FFMPEGONEDEMO_IDECODE_H
#define FFMPEGONEDEMO_IDECODE_H

#include "XParameter.h"
#include "Iobserver.h"
#include <list>
//解码接口类--支持硬解码
class IDecode : public Iobserver {
public:
    //打开解码器
    virtual bool Open(XParameter para, bool isHard= false)=0;
    virtual void Close()=0;
    virtual void Clear();
    //future模型,发送数据到线程解码
    virtual bool SendPacket(XData pkt)=0;
    //从线程中获取解码结果
    virtual XData RecvFrame()=0;
    //由主题notify通知的数据
    virtual void Update(XData data);
    //0代表视频，1代表音频，2代表其他
    int decodeType=-1;
    int maxList=100;//最大缓冲数据量

    //同步时间，再次打开文件要清理
    long synpts=0;
    long pts;//当前播放到的位置

protected:
    virtual void Main();
    //保存读取到的帧数据列表
    std::list<XData> packs;
    //读取数据队列的互斥变量
    std::mutex packsMutex;
};


#endif //FFMPEGONEDEMO_IDECODE_H
