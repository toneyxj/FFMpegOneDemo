//
// Created by xj on 2018/6/25.
//

#ifndef FFMPEGONEDEMO_XPARAMETER_H
#define FFMPEGONEDEMO_XPARAMETER_H

//直接声名结构体，不需要管实现
struct AVCodecParameters;
//解码参数类
class XParameter {
public:
    AVCodecParameters *para=0;
    int channnels=2;
    int sample_rate=44100;

};


#endif //FFMPEGONEDEMO_XPARAMETER_H
