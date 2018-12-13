//
// Created by xj on 2018/6/27.
//

#include "XShader.h"
#include "XLog.h"
#include <GLES2/gl2.h>
//顶点作色器
//#可以把传入的x内容变为字符串直接拼接
#define GET_SIR(x) #x

static const char *vertexShader = GET_SIR(
        attribute
        vec4 aPosition;//输入的顶点坐标
        attribute
        vec2 aTexCoord;//输入的顶点材质坐标
        varying
        vec2 vTexCoord;//输出的材质坐标
        void main() {
            vTexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);
            gl_Position = aPosition;
        }
);
//注意：FFmpeg软解码和部分硬解码是420p
//片元着色器
static const char *fragYUV420P = GET_SIR(
//注意vec代表的是向量，后面的数字代表有几个元素的向量

        precision
        mediump float;//精度
        varying
        vec2 vTexCoord;//顶点着色器传递坐标
        uniform
        sampler2D yTexture;//输入的材质y：（不透明灰度，单像素）
        uniform
        sampler2D uTexture;//输入的材质u：
        uniform
        sampler2D vTexture;//输入的材质v

        void main() {
            vec3 yuv;
            vec3 rgb;
            //第一个参数：输入的材质
            //第二个参数：顶点着色器转换后的坐标值
            yuv.r = texture2D(yTexture, vTexCoord).r;
            yuv.g = texture2D(uTexture, vTexCoord).r - 0.5;
            yuv.b = texture2D(vTexture, vTexCoord).r - 0.5;
            rgb = mat3(1.0, 1.0, 1.0,
                       0.0, -0.39465, 2.03211,
                       1.13983, -0.58060, 0.0) * yuv;//转换为RGB的矩阵
            //输出像素颜色 参数一：输出rgb数据源，参数二：透明通道
            gl_FragColor = vec4(rgb, 1.0);
        }
);
//片元着色器,软解码和部分x86硬解码
static const char *fragNV12 = GET_SIR(
        precision
        mediump float;    //精度
        varying
        vec2 vTexCoord;     //顶点着色器传递的坐标
        uniform
        sampler2D yTexture; //输入的材质（不透明灰度，单像素）
        uniform
        sampler2D uvTexture;
        void main() {
            vec3 yuv;
            vec3 rgb;
            yuv.r = texture2D(yTexture, vTexCoord).r;
            yuv.g = texture2D(uvTexture, vTexCoord).r - 0.5;
            yuv.b = texture2D(uvTexture, vTexCoord).a - 0.5;
            rgb = mat3(1.0, 1.0, 1.0,
                       0.0, -0.39465, 2.03211,
                       1.13983, -0.58060, 0.0) * yuv;
            //输出像素颜色
            gl_FragColor = vec4(rgb, 1.0);
        }
);

//片元着色器,软解码和部分x86硬解码
static const char *fragNV21 = GET_SIR(
        precision
        mediump float;    //精度
        varying
        vec2 vTexCoord;     //顶点着色器传递的坐标
        uniform
        sampler2D yTexture; //输入的材质（不透明灰度，单像素）
        uniform
        sampler2D uvTexture;
        void main() {
            vec3 yuv;
            vec3 rgb;
            yuv.r = texture2D(yTexture, vTexCoord).r;
            yuv.g = texture2D(uvTexture, vTexCoord).a - 0.5;
            yuv.b = texture2D(uvTexture, vTexCoord).r - 0.5;
            rgb = mat3(1.0, 1.0, 1.0,
                       0.0, -0.39465, 2.03211,
                       1.13983, -0.58060, 0.0) * yuv;
            //输出像素颜色
            gl_FragColor = vec4(rgb, 1.0);
        }
);

GLuint InitShader(const char *code, GLint type) {
    //创建shader
    GLuint sh = glCreateShader(type);
    if (sh == 0) {
        XLOGE("glCreateShader %d failed!!", type);
        return 0;
    }
    //加载shader
    /**
     * 参数一：创建的shader
     * 参数二：shader的数量
     * 参数三：shader的代码
     * 参数四：标识代码的长度，不传入直接找字符串结尾
     */
    glShaderSource(sh, 1, &code, 0);
    //编译shader:程序编译过程中是在显卡里面运算
    glCompileShader(sh);

    GLint status;//编译的状态
    //获取编译情况
    glGetShaderiv(sh, GL_COMPILE_STATUS, &status);
    if (status == 0) {//失败
        XLOGE("glGetShaderiv %d failed!!", type);
        return 0;
    }
    XLOGI("glCompileShader success!");
    return sh;
}
void XShader::Close(){
    mux.lock();
    //释放shader
    if(program)glDeleteProgram(program);
    if(fsh) glDeleteShader(fsh);
    if(vsh)glDeleteShader(vsh);

    //释放材质
    for(int i=0;i< sizeof(texts)/ sizeof(unsigned int);i++){
        if(texts[i]){
            glDeleteTextures(1,&texts[i]);
        }
        texts[i]=0;
    }
    mux.unlock();
}
bool XShader::Init(XShaderType type) {
    Close();
    mux.lock();
    //顶点和片元shader初始化
    //初始化顶点坐标,参数二获得顶点着色器固定值
    vsh = InitShader(vertexShader, GL_VERTEX_SHADER);
    if (vsh == 0) {
        mux.unlock();
        XLOGE("InitShader vertexShader failed!");
        return false;
    }
    XLOGI("InitShader vertexShader success!");
    //片元yuv420 shader初始化
    switch (type) {
        case XSHADER_YUV420:
            fsh = InitShader(fragYUV420P, GL_FRAGMENT_SHADER);
            break;
        case XSHADER_NV12:
            fsh = InitShader(fragNV12, GL_FRAGMENT_SHADER);
            break;
        case XSHADER_NV21:
            fsh = InitShader(fragNV21, GL_FRAGMENT_SHADER);

            break;
        default:
            XLOGE("XSHADER format is error");
            mux.unlock();
            return false;
    }
    if (fsh == 0) {
        XLOGE("InitShader fragYUV420P failed!");
        mux.unlock();
        return false;
    }
    XLOGI("InitShader fragYUV420P success!");

    //创建渲染程序
    program = glCreateProgram();
    if (program == 0) {
        mux.unlock();
        XLOGE("glCreateProgram %s !", "failed");
        return false;
    }
    //渲染程序加入着色器代码
    glAttachShader(program, vsh);
    glAttachShader(program, fsh);

    //连接程序
    glLinkProgram(program);
    GLint status = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        mux.unlock();
        XLOGE("glGetProgramiv %s !", "failed");
        return false;
    }
    glUseProgram(program);//使用渲染
    XLOGI("glGetProgramiv %s !", "success");
    //----------------------------------渲染程序结束

    //加入三维顶点数据
    //顶点数据两个三角形组成一个正方形
    static float ver[] = {
            1.0f, -1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f
    };
    //获取数据属性
    GLuint apos = (GLuint) glGetAttribLocation(program, "aPosition");
    //使的数据有效
    glEnableVertexAttribArray(apos);
    //传递顶点
    glVertexAttribPointer(apos, 3, GL_FLOAT, GL_FALSE, 12,
                          ver);//参数二：一个点是几个坐标表示三维的就是3个,参数五：一个数据的偏移量，一个点由3个float表示，一个float占4个字节，所以填12

    //加入材质坐标数据
    static float txt[] = {
            1.0f, 0.0f,//右下
            0.0f, 0.0f,//左下
            1.0f, 1.0f,//右上
            0.0f, 1.0f//左上
    };
    GLuint atx = (GLuint) glGetAttribLocation(program, "aTexCoord");
    glEnableVertexAttribArray(atx);
    glVertexAttribPointer(atx, 2, GL_FLOAT, GL_FALSE, 8, txt);

    //-----------------------------------材质纹理初始化
    //第一步：设置纹理层,材质分成几层，第一次是y，第二次是u，第三次是v。
    glUniform1i(glGetUniformLocation(program, "yTexture"), 0);//0代表的是第一层
    switch (type) {
        case XSHADER_YUV420:
            glUniform1i(glGetUniformLocation(program, "uTexture"), 1); //对于纹理第2层
            glUniform1i(glGetUniformLocation(program, "vTexture"), 2); //对于纹理第3层
            break;
        case XSHADER_NV21:
        case XSHADER_NV12:
            glUniform1i(glGetUniformLocation(program, "uvTexture"), 1); //对于纹理第2层
            break;
    }

    XLOGI("初始化sharder成功");
    mux.unlock();
    return true;
}

void XShader::Draw() {
    mux.lock();
    if (!program) {
        mux.unlock();
        return;
    }
    //三维绘制-绘制存放的四个顶点信息
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    mux.unlock();
}

void XShader::getTexture(unsigned int index, int width, int height, unsigned char *buf, bool isa) {
    unsigned int format = GL_LUMINANCE;//不同模式需要修改灰度图
    if (isa) {
        format = GL_LUMINANCE_ALPHA;//带透明通道
    }
    mux.lock();
    if (texts[index] == 0) {
        //材质初始化
        //创建一个纹理
        glGenTextures(1, &texts[index]);
        //设置纹理属性
        glBindTexture(GL_TEXTURE_2D, texts[index]);
        //缩小的过滤器i表示int类型的。参数一：2D材质；参数二：设置为缩小的过滤；参数三：线性插值
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //缩小的过滤器
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //设置纹理的格式和大小
        glTexImage2D(GL_TEXTURE_2D,
                     0,//显示细节级别，在3D材质的时候需要设置
                     format,//内部GPU的格式,亮度，灰度图
                     width, height,//尺寸要的是2的次方
                     0,//边框
                     format,//数据的像素格式，必须与上面传入参数一致
                     GL_UNSIGNED_BYTE,//保存一个像素点的数据类型
                     NULL//纹理的数据，这里不需要传递
        );
    }
    //激活纹理-也就是是下面使用纹理
    glActiveTexture(GL_TEXTURE0 + index);//激活一层纹理
    glBindTexture(GL_TEXTURE_2D, texts[index]);//绑定纹理到创建的材质也就是y当中
    //替换纹理内容,参数3,4：是便宜量
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height,
                    format,//灰度图
                    GL_UNSIGNED_BYTE,//存储的格式
                    buf//获得材质保存于buf中，修改buf中间缓存数据，就可以改变里面的纹理内容
    );
    mux.unlock();
}