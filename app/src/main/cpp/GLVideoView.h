//
// Created by xj on 2018/6/27.
//

#ifndef FFMPEGONEDEMO_GLVIDEOVIEW_H
#define FFMPEGONEDEMO_GLVIDEOVIEW_H


#include "IVideoView.h"
class XTextTure;

class GLVideoView : public IVideoView{
public:
    virtual void SetRender(void *win);
    virtual void Render(XData data);
    virtual void Close();

protected:
    void *view=0;
    XTextTure *txt=0;
    std::mutex mux;
};


#endif //FFMPEGONEDEMO_GLVIDEOVIEW_H
