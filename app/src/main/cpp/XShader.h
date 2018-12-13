//
// Created by xj on 2018/6/27.
//

#ifndef FFMPEGONEDEMO_XSHADER_H
#define FFMPEGONEDEMO_XSHADER_H


#include <GLES2/gl2.h>
#include <mutex>

//与XTextTure值一样：为了不产生关联
enum XShaderType {
    XSHADER_YUV420 = 0, //Y4　U1 V1
            XSHADER_NV12 = 25, //YYYY　UV
            XSHADER_NV21 = 26
};

class XShader {
public:
    virtual bool Init(XShaderType type=XSHADER_YUV420);
    virtual void Close();

    //获取材质并映射到内存isa:是否有透明通道
    virtual void getTexture(unsigned int index, int width, int height, unsigned char *buf, bool  isa= false);

    virtual void Draw();

protected:
    //顶点着色器
    unsigned int vsh = 0;
    //片元着色器
    unsigned int fsh = 0;
    //
    unsigned int program = 0;

    unsigned int texts[100] = {0};
    std::mutex mux;

};


#endif //FFMPEGONEDEMO_XSHADER_H
