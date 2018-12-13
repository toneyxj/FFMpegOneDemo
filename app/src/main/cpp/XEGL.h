//
// Created by xj on 2018/6/27.
//

#ifndef FFMPEGONEDEMO_XEGL_H
#define FFMPEGONEDEMO_XEGL_H


class XEGL {
public:
    virtual bool Init(void *win)=0;
    virtual void Close()=0;
    virtual  void Draw()=0;
    static XEGL *Get();//单键模式-也就是单列模式
protected:
    XEGL(){};
};


#endif //FFMPEGONEDEMO_XEGL_H
