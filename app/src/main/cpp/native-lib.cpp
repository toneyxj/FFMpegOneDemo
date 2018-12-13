#include <jni.h>
#include <string>
#include "IPlayPorxy.h"
#include "XLog.h"
//Android原生的窗口头文件
#include <android/native_window_jni.h>

// 注意 在C++中  new的函数代表的是指针函数

/**
 * 配置硬解码器，java虚拟了会自动调用这个函数
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_xj_ffmpegonedemo_MainActivity_openVideo(JNIEnv *env, jobject instance, jstring path_) {
    const char *path = env->GetStringUTFChars(path_, 0);

    IPlayPorxy::Get()->open(path);
    IPlayPorxy::Get()->Start();

    env->ReleaseStringUTFChars(path_, path);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_xj_ffmpegonedemo_XPlay_InitView(JNIEnv *env, jobject instance, jobject surface) {
    //获取显示窗口的初始化对象
    XLOGE("进入这个窗口里面");
    ANativeWindow *win=ANativeWindow_fromSurface(env,surface);
    IPlayPorxy::Get()->initView(win);

}

extern "C"
JNIEXPORT
jint JNI_OnLoad(JavaVM *vm, void *res){
    IPlayPorxy::Get()->init(vm);

    return JNI_VERSION_1_4;
}
