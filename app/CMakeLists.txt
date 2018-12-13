# Sets the minimum version of CMake required to build the native
# library. You should either keep the default value or only pass a
# value of 3.4.0 or lower.

cmake_minimum_required(VERSION 3.4.1)

#添加头文件路径
include_directories(libs/include)

add_library( native-lib
             SHARED
             src/main/cpp/native-lib.cpp
             src/main/cpp/IDemux.cpp
             src/main/cpp/FFDemux.cpp
             src/main/cpp/XData.cpp
             src/main/cpp/XLog.cpp
             src/main/cpp/XThread.cpp
             src/main/cpp/Iobserver.cpp
             src/main/cpp/FFDecode.cpp
             src/main/cpp/IDecode.cpp
             src/main/cpp/XParameter.cpp
             src/main/cpp/IVideoView.cpp
             src/main/cpp/GLVideoView.cpp
             src/main/cpp/XTextTure.cpp
             src/main/cpp/XShader.cpp
             src/main/cpp/XEGL.cpp
             src/main/cpp/FFResample.cpp
             src/main/cpp/IResample.cpp
             src/main/cpp/IAudioPlay.cpp
             src/main/cpp/SLAudioPlay.cpp
             src/main/cpp/IPlayer.cpp
             src/main/cpp/IPlayBuilder.cpp
             src/main/cpp/FFPlayBuilder.cpp
             src/main/cpp/IPlayPorxy.cpp
#设置导入库的位置
              )

find_library(log-lib
              log )

#设置so库所在路径变量,变量就是FF
set(FF ${CMAKE_CURRENT_SOURCE_DIR}/libs/${ANDROID_ABI})

#set(distribution_DIR ${CMAKE_SOURCE_DIR}/../../../../libs)

add_library( avcodec
             SHARED
             IMPORTED #需要导入库的意思
              )
set_target_properties( avcodec
                       PROPERTIES IMPORTED_LOCATION #导入本地文件的属性
                       ${FF}/libavcodec.so #直接指向编译的文件
                       )

add_library( avformat
             SHARED
             IMPORTED #需要导入库的意思
              )
#设置导入库的位置
set_target_properties( avformat
                       PROPERTIES IMPORTED_LOCATION #导入本地文件的属性
                       ${FF}/libavformat.so #直接指向编译的文件
                       )

add_library( avfilter
             SHARED
             IMPORTED #需要导入库的意思
              )
#设置导入库的位置
set_target_properties( avfilter
                       PROPERTIES IMPORTED_LOCATION #导入本地文件的属性
                       ${FF}/libavfilter.so #直接指向编译的文件
                       )
add_library( avutil
             SHARED
             IMPORTED #需要导入库的意思
              )
#设置导入库的位置
set_target_properties( avutil
                       PROPERTIES IMPORTED_LOCATION #导入本地文件的属性
                       ${FF}/libavutil.so #直接指向编译的文件
                       )

#像素格式转换库
add_library( swscale
             SHARED
             IMPORTED #需要导入库的意思
              )
#设置导入库的位置
set_target_properties( swscale
                       PROPERTIES IMPORTED_LOCATION #导入本地文件的属性
                       ${FF}/libswscale.so #直接指向编译的文件
                       )
#音频重采样
add_library( swresample
             SHARED
             IMPORTED #需要导入库的意思
              )
#设置导入库的位置
set_target_properties( swresample
                       PROPERTIES IMPORTED_LOCATION #导入本地文件的属性
                       ${FF}/libswresample.so #直接指向编译的文件
                       )


target_link_libraries(  native-lib
                        GLESv2#opengl用的
                        EGL#窗口交互使用
                        android
                        OpenSLES#音频库
                        avcodec
                        avformat
                        avfilter
                        avutil
                        swscale
                        swresample
                       ${log-lib} )
