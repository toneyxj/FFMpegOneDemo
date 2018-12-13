//
// Created by xj on 2018/5/31.
//

#include "FFDemux.h"
#include "XLog.h"
//因为ffmpgg是用c开发的所以所有头文件需要继承C
extern "C"{
#include <libavformat/avformat.h>
}
//分数转为浮点数
static double r2d(AVRational r){
    return r.num==0||r.den==0?0.:(double)r.num/(double)r.den;
}


//打开文件或者流媒体 rmtp http rtsp
bool FFDemux::Open(const char *url) {
    XLOGI("open file %s begin",url);
    Close();
    mux.lock();
    int re=avformat_open_input(&ic,url,0,0);

    if (re!=0){
        char buf[1024]={0};
        av_strerror(re,buf, sizeof(buf));
        XLOGI("FFmpeg open %s failed! is error %S",url,buf);
        mux.unlock();
        return false;
    }
    XLOGI("FFmpeg open %s sucess!",url);
    //读取文件信息，对于有些文件格式不调用无法获取
    re=avformat_find_stream_info(ic,0);
    if (re!=0){
        char buf[1024]={0};
        av_strerror(re,buf, sizeof(buf));
        XLOGI("avformat_find_stream_info %s failed!",url);
        mux.unlock();
        return false;
    }
    //总时长，这个值不一定有
    this->totalMs=ic->duration/(AV_TIME_BASE/1000);
    XLOGI("totalMs %d ",totalMs);

    //初始化音视频具体索引位置
    initDecodeIndex();
    mux.unlock();
    return true;
}

void  FFDemux::Close(){
    mux.lock();
    if(ic){
        avformat_close_input(&ic);
    }
    mux.unlock();
}

void FFDemux::initDecodeIndex(){
    if (!ic){
        XLOGE("initDecodeIndex ic is null!");
        return ;
    }
    videoStream=av_find_best_stream(ic,AVMEDIA_TYPE_VIDEO,-1,-1,0,0);
    audioStream=av_find_best_stream(ic,AVMEDIA_TYPE_AUDIO,-1,-1,0,0);
    if(videoStream<0||audioStream<0){//小于0代表没有找到索引
        XLOGE("initDecodeIndex av_find_best_stream failed!");
        return ;
    }
}
//一个视频压缩文件里面分多路，通过av_find_best_stream 获取文件在压缩文件里面的信息索引

//获取视频信息
XParameter FFDemux::getVPara(){
    mux.lock();
    if (!ic){
        XLOGE("getVPara ic is null!");
        mux.unlock();
        return XParameter();
    }
    //获取视频流索引信息，
    int re=av_find_best_stream(ic,AVMEDIA_TYPE_VIDEO,-1,-1,0,0);
    if(re<0){//小于0代表没有找到索引
        XLOGE("av_find_best_stream failed!");
        mux.unlock();
        return XParameter();
    }
    XParameter para;
    para.para=ic->streams[re]->codecpar;
//    videoStream=re;
    mux.unlock();
    return para;
}
//获取音频信息
XParameter FFDemux::getAPara(){
    mux.lock();
    if (!ic){
        XLOGE("getVPara ic is null!");
        mux.unlock();
        return XParameter();
    }
    //获取音频流索引信息，
    int re=av_find_best_stream(ic,AVMEDIA_TYPE_AUDIO,-1,-1,0,0);
    if(re<0){//小于0代表没有找到索引
        XLOGE("av_find_best_stream failed!");
        mux.unlock();
        return XParameter();
    }
    XParameter para;
    para.para=ic->streams[re]->codecpar;
    para.channnels=ic->streams[re]->codecpar->channels;
    para.sample_rate=ic->streams[re]->codecpar->sample_rate;
//    audioStream=re;
    mux.unlock();
    return para;
}
//读取一针数据，数据由调用者清理。
XData FFDemux::Read() {
    mux.lock();
    if(!ic) {//当遇见上下文还未初始化直接返回
        mux.unlock();
        return XData();
    }
    XData d;
    AVPacket *pkt=av_packet_alloc();
    int re=av_read_frame(ic,pkt);
    if(re!=0){
        av_packet_free(&pkt);//释放空间
        mux.unlock();
        return XData();
    }
    d.data=(unsigned char *)pkt;
    d.size=pkt->size;
    if(pkt->stream_index==audioStream){
        d.decodeType=1;
    } else if(pkt->stream_index==videoStream){
        d.decodeType=0;
    } else{
        //不是音视频，我们不需要的内容需要销毁掉，因为已经分配了控件给pkt
        av_packet_free(&pkt);//释放空间
        mux.unlock();
        return XData();
    }
    //转换pts未毫秒
    d.pts=(long)pkt->pts*1000*r2d(ic->streams[pkt->stream_index]->time_base);
    mux.unlock();
    return d;
}
//初始化函数只能调用一次
FFDemux::FFDemux() {
    static bool  isFrist=true;
    if(isFrist){
        isFrist=false;
        //注册所有解封装器
        av_register_all();
        //注册所有的解码器
        avcodec_register_all();
        //初始化网络
        avformat_network_init();
        XLOGI("register FFmpeg");

    }
}
