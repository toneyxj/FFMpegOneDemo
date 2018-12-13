//
// Created by xj on 2018/6/27.
//

#ifndef FFMPEGONEDEMO_XTEXTTURE_H
#define FFMPEGONEDEMO_XTEXTTURE_H

enum XTextTureType{
    XTEXTTRES_YUV420=0,//Y4　U1 V1
    STEXTTURE_NV12=25,//Y4　UV1  ;与上一个只是排版不一样
    STEXTTURE_NV21=26//Y4 vu1
};
//材质初始化
class XTextTure {
public:

    virtual bool Init(void *win,XTextTureType type=XTEXTTRES_YUV420)=0;
    static XTextTure *Create();
    virtual void Draw(unsigned char *data[],int width,int height)=0;
    virtual void Drop()=0;
    virtual ~XTextTure(){};
protected:
    XTextTure(){};
};


#endif //FFMPEGONEDEMO_XTEXTTURE_H
