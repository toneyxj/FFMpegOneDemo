//
// Created by xj on 2018/6/27.
//

#ifndef FFMPEGONEDEMO_IVIDEOVIEW_H
#define FFMPEGONEDEMO_IVIDEOVIEW_H


#include "XData.h"
#include "Iobserver.h"

class IVideoView :public Iobserver{
public:
    virtual void SetRender(void *win)=0;
    virtual void Render(XData data)=0;
    virtual void Close()=0;
    virtual void Update(XData data);

};


#endif //FFMPEGONEDEMO_IVIDEOVIEW_H
