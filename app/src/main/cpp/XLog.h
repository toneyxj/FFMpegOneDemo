//
// Created by xj on 2018/5/31.
//

#ifndef FFMPEGONEDEMO_XLOG_H
#define FFMPEGONEDEMO_XLOG_H


class XLog {

};
//判断平台不同平台打印数据方式不同
#ifdef ANDROID
#include "android/log.h"
#define XLOGD(...) __android_log_print(ANDROID_LOG_WARN,"Xplay",__VA_ARGS__)
#define XLOGI(...) __android_log_print(ANDROID_LOG_INFO,"Xplay",__VA_ARGS__)
#define XLOGE(...) __android_log_print(ANDROID_LOG_ERROR,"Xplay",__VA_ARGS__)
#else
#define XLOGD(...) printf( "Xplay",__VA_ARGS__)
#define XLOGI(...) printf("Xplay",__VA_ARGS__)
#define XLOGE(...) printf("Xplay",__VA_ARGS__)
#endif

#endif //FFMPEGONEDEMO_XLOG_H
