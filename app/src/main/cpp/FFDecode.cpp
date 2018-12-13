//
// Created by xj on 2018/6/25.
//
extern "C"{
#include <libavcodec/avcodec.h>
#include <libavcodec/jni.h>  //硬解码使用
}
#include "FFDecode.h"
#include "XLog.h"
/**
 * 初始化硬解码
 */
void FFDecode::InitHard(void *vm){
        av_jni_set_java_vm(vm,0);
}

void FFDecode::Close(){
    IDecode::Clear();
    mux.lock();
    pts=0;
    if(frame)av_frame_free(&frame);
    if(codec){
        avcodec_close(codec);
        avcodec_free_context(&codec);
    }

    mux.unlock();
}

bool FFDecode::Open(XParameter para, bool isHard){
    Close();
    if(!para.para)
        return false;
    AVCodecParameters *p=para.para;
    //1.查找解码器
    AVCodec *cd=avcodec_find_decoder(p->codec_id);
    if(isHard){
        cd=avcodec_find_decoder_by_name("h264_mediacodec");
    }
    if(!cd){
        XLOGE("avcodec_find_decoder %d failed! %d",p->codec_id,isHard);
        return false;
    }
    XLOGI("avcodec_find_decoder success! %d",isHard);
    mux.lock();
    //创建解码器上下文，并复制参数
    codec=avcodec_alloc_context3(cd);
    //复制参数
    avcodec_parameters_to_context(codec,p);
    //开启8个线程解码
    codec->thread_count=8;
    //打开解码器
    int re=avcodec_open2(codec,0,0);
    if(re!=0){
        char buf[1024]={0};
        av_strerror(re,buf, sizeof(buf)-1);//最后一位不能用
        XLOGE("%s",buf);
        mux.unlock();
        return false;
    }
    if (codec->codec_type==AVMEDIA_TYPE_VIDEO){
        decodeType=0;
    } else if(codec->codec_type==AVMEDIA_TYPE_AUDIO){
        decodeType=1;
    } else{
        decodeType=-1;
    }
    XLOGI("avcodec_open2 sucess");
    mux.unlock();
    return true;
}

//future模型,发送数据到线程解码
 bool FFDecode::SendPacket(XData pkt){
    if(pkt.size<=0||!pkt.data)return false;
    mux.lock();
    if (!codec ){
        mux.unlock();
        return false;
    }
    int re=avcodec_send_packet(codec,(AVPacket *)pkt.data);
    if (re!=0){//发送失败
        mux.unlock();
        return false;
    }
    mux.unlock();
    return true;
}
//从线程中接收解码结果
 XData FFDecode::RecvFrame(){
    mux.lock();
    if(!codec) {
        mux.unlock();
        return XData();
    }
    if(!frame)frame=av_frame_alloc();//如果AVFrame是空的时候需要创建初始化
    int re=avcodec_receive_frame(codec,frame);
    if(re!=0) {//解码失败
        mux.unlock();
        return XData();
    }
    XData d;
    d.data=(unsigned char *)frame;
    //判断是否是视屏进入
    if(codec->codec_type==AVMEDIA_TYPE_VIDEO) {
        //YUV分三路解码*高度获取总大小。
        d.size = (frame->linesize[0] + frame->linesize[1] + frame->linesize[2]) * frame->height;
        d.width=frame->width;
        d.height=frame->height;
    } else if(codec->codec_type==AVMEDIA_TYPE_AUDIO){//音频大小
        //av_get_bytes_per_sample((AVSampleFormat) frame->format)可以确定单个样本大小 是16或者24位32位等
        //nb_samples 单通道样本数 *2代表是双通道
        d.size = av_get_bytes_per_sample((AVSampleFormat) frame->format)*frame->nb_samples*2;
    }
    d.format=frame->format;
    //frame->data 解码之后最终的数据
    memcpy(d.datas,frame->data, sizeof(d.datas));
    d.pts=frame->pts;
    pts=d.pts;
    mux.unlock();
    return d;
}

//在这里FFMpeg的解码模式类似于压栈  先进去解码器开启线程进行解码 解码成功后自动保存在一个地方再由自己调用获取解码结果