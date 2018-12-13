//
// Created by xj on 2018/6/29.
//
extern "C" {
#include <libswresample/swresample.h>

}

#include <libavcodec/avcodec.h>
#include "FFResample.h"
#include "XLog.h"
void FFResample::Close(){
    mux.lock();
    if(actx){
        swr_free(&actx);
    }
    mux.unlock();
}
bool FFResample::Open(XParameter in, XParameter out) {
    Close();
    mux.lock();
    if (in.para->format)
        //音频重采样上下文初始化    输入参数 2  8  48000   输出参数 2  1  0

        actx = swr_alloc();
    //设置音频采样参数
    actx = swr_alloc_set_opts(actx,
            //前半部分是输出参数
//    av_get_default_channel_layout(ac->channels),//获得默认的通道格式-av_get_default_channel_layout根据通道数直接返回，也可以直接设置成
                              av_get_default_channel_layout(out.channnels),//也可以直接设置成2 代表两声道
                              AV_SAMPLE_FMT_S16,//输出格式
                              out.sample_rate,//样本率
            //下面是输入参数
                              av_get_default_channel_layout(in.para->channels),
                              (AVSampleFormat) in.para->format,
                              in.para->sample_rate,
                              0, 0

    );
    //初始化音频重采样上下文
    int re = swr_init(actx);
    if (re != 0) {
        XLOGE("swr_init failed !");
        mux.unlock();
        return false;
    } else {
        XLOGI("swr_init sucessed !");
    }
    outChannels = in.para->channels;
    outFormat = AV_SAMPLE_FMT_S16;
    mux.unlock();
    return true;
}

XData FFResample::Resample(XData inData) {
    mux.lock();
    if (!actx || inData.size <= 0 || !inData.data) {
        mux.unlock();
    return XData();
}
    AVFrame *frame = (AVFrame *) inData.data;
    //输出空间分配
    XData out;

    //通道数*单通道样本数*样本大小
    int size =
            outChannels * frame->nb_samples * av_get_bytes_per_sample((AVSampleFormat) outFormat);
    if (size <= 0)return XData();
    out.Alloc(size);
    uint8_t *outarr[2] = {0};
    outarr[0]=out.data;
    int len = swr_convert(actx, outarr, frame->nb_samples, (const uint8_t **)frame->data,frame->nb_samples);
    if (len<=0){
        out.Drop();
        mux.unlock();
        return  XData();
    }
    out.pts=inData.pts;
    mux.unlock();
    return out;
}