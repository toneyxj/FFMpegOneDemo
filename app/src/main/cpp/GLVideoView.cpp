//
// Created by xj on 2018/6/27.
//

#include "GLVideoView.h"
#include "XTextTure.h"

void GLVideoView::SetRender(void *win){
     this->view=win;
 }
 void GLVideoView::Render(XData data){
     if(!view)return;
     if(!txt){
         txt=XTextTure::Create();
         txt->Init(view,(XTextTureType)data.format);
     }
     txt->Draw(data.datas,data.width,data.height);
 }
 void GLVideoView::Close(){
     mux.lock();
     if(txt){
        txt->Drop();
        txt=NULL;
     }
     mux.unlock();
 }