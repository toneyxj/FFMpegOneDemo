//
// Created by xj on 2018/6/27.
//

#include <android/native_window.h>
#include "XEGL.h"
#include <EGL/egl.h>
#include <mutex>
#include "XLog.h"

class CXEGL : public XEGL {
public:
    //需要销毁的dispaly
    EGLDisplay dispaly = EGL_NO_DISPLAY;
    EGLSurface surface = EGL_NO_SURFACE;
    EGLContext context = EGL_NO_CONTEXT;
    std::mutex mux;

    virtual void Draw() {
        mux.lock();
        if (dispaly == EGL_NO_DISPLAY || surface == EGL_NO_SURFACE) {
            mux.unlock();
            return;
        }
        //如果不等于空把dispaly 与surface 交换到显示设备当中
        eglSwapBuffers(dispaly, surface);
        mux.unlock();
    }

    virtual void Close() {
        mux.lock();
        if (dispaly == EGL_NO_DISPLAY) {
            mux.unlock();
            return;
        }
        //去除绑定关系
        eglMakeCurrent(dispaly, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_SURFACE);
        if(surface!=EGL_NO_SURFACE)
        eglDestroySurface(dispaly, surface);
        if(surface!=EGL_NO_CONTEXT)
        eglDestroyContext(dispaly, context);
        //终止dispaly
        eglTerminate(dispaly);

        dispaly = EGL_NO_DISPLAY;
        surface = EGL_NO_SURFACE;
        context = EGL_NO_CONTEXT;
        mux.unlock();
    }

    //初始化EGL相关部分
    virtual bool Init(void *win) {

        ANativeWindow *nwin = (ANativeWindow *) win;
        Close();
        mux.lock();
        //初始化EGL-----------------

        //1.获取EGlDisplay对象，显示设备
        dispaly = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (dispaly == EGL_NO_DISPLAY) {
            XLOGE("eglGetDisplay failed");
        }
        XLOGI("eglGetDisplay sucess");
        //2.初始化dispaly
        if (EGL_TRUE != eglInitialize(dispaly, 0, 0)) {
            XLOGE("eglInitialize failed!");
            mux.unlock();
            return false;
        }
        XLOGI("eglInitialize success!");
        //获取配置并创建surface
        //输出
        EGLConfig config = 0;
        //接收的参数可以用来判断是否成功
        EGLint configNum;
        //输入配置项-固定参数
        EGLint configSpec[] = {
                EGL_RED_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_BLUE_SIZE, 8,
                EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_NONE
        };
        //参数一：窗口呢连接器
        //参数二：输入窗口的配置项
        //参数三：输出的配置项
        //参数四：表示最多存储多少个配置项
        //参数五：返回的配置项
        if (EGL_TRUE != eglChooseConfig(dispaly, configSpec, &config, 1, &configNum)) {
            XLOGE("eglChooseConfig failed!");
            mux.unlock();
            return false;
        }
        XLOGI("eglChooseConfig success!");
        //window（窗口）与EGl关联
        surface = eglCreateWindowSurface(dispaly, config, nwin, NULL);

        //创建并打开EGL上下文-上面是创建好设备和空间--下面是具体的与opengl存储的状态

        const EGLint ctxAttr[] = {
                EGL_CONTEXT_CLIENT_VERSION,
                2,//客户端的(EGL)版本号，对应2
                EGL_NONE//标识数组的结尾
        };
        //参数三是代表是否有共享的上下文 EGL_NO_CONTEXT 代表不共享
        context = eglCreateContext(dispaly, config, EGL_NO_CONTEXT, ctxAttr);
        if (context == EGL_NO_CONTEXT) {
            XLOGE("eglCreateContext failed!");
            mux.unlock();
            return false;
        }
        XLOGI("eglCreateContext success!");

        //上下文切换
        if (EGL_TRUE != eglMakeCurrent(dispaly, surface, surface, context)) {
            XLOGE("eglMakeCurrent failed!");
            mux.unlock();
            return false;
        }
        XLOGI("eglMakeCurrent success!");
        mux.unlock();
        return true;
    }
};

XEGL *XEGL::Get() {
    static CXEGL egl;
    return &egl;
}
