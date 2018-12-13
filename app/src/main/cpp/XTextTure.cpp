//
// Created by xj on 2018/6/27.
//

#include "XTextTure.h"
#include "XLog.h"
#include "XEGL.h"
#include "XShader.h"

class CXTextTure:public XTextTure{
public:
    XShader sh;
    XTextTureType type;
    std::mutex mux;
    virtual void Drop(){
        mux.lock();
        XEGL::Get()->Close();
        sh.Close();
        mux.unlock();
        delete  this;
    }

    virtual bool Init(void *win,XTextTureType type){
        mux.lock();
        XEGL::Get()->Close();
        sh.Close();
        this->type=type;
        if(!win){
            XLOGE("CXTextTure Init failed win is NULL");
            mux.unlock();
            return false;
        }
        //如果EGl初始化失败直接返回
        if(!XEGL::Get()->Init(win)){
            mux.unlock();
            return false;}
        sh.Init((XShaderType)type);
        mux.unlock();
        return true;
    }
    //参数一是传递的解码后的数据
    virtual void Draw(unsigned char *data[],int width,int height){
        mux.lock();
        sh.getTexture(0,width,height,data[0]);
        if (type==XTEXTTRES_YUV420){//u v
            sh.getTexture(1,width/2,height/2,data[1]);
            sh.getTexture(2,width/2,height/2,data[2]);
        } else if(type==STEXTTURE_NV12){//uv
            sh.getTexture(1,width/2,height/2,data[1], true);//一个像素点两个字节，包含一个透明通道
        } else if(type==STEXTTURE_NV21){
            sh.getTexture(1,width/2,height/2,data[1], true);//一个像素点两个字节，包含一个透明通道
        }

        sh.Draw();
        XEGL::Get()->Draw();
        mux.unlock();
    }
};

XTextTure * XTextTure::Create(){
    return new CXTextTure();
}