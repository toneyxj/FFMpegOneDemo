//
// Created by xj on 2018/6/25.
//

#ifndef FFMPEGONEDEMO_FFDECODE_H
#define FFMPEGONEDEMO_FFDECODE_H

#include "XParameter.h"
#include "IDecode.h"
struct  AVCodecContext;
struct  AVFrame;

class FFDecode : public IDecode {
public:
    //硬解码注册
    static void InitHard(void *vm);
    //打开解码器
    virtual bool Open(XParameter para, bool isHard= false);
    virtual void Close();
    //future模型,发送数据到线程解码
    virtual bool SendPacket(XData pkt);
    //从线程中获取解码结果，重复使用frame，线程不安全：只能在当前线程中使用，不能在其它线程中使用
    virtual XData RecvFrame();
protected:
    AVCodecContext *codec=0;//创建解码器上下文
    AVFrame *frame=0;//接收解码数据
    std::mutex mux;
};


#endif //FFMPEGONEDEMO_FFDECODE_H
