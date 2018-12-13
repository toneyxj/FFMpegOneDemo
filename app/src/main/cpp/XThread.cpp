//
// Created by xj on 2018/6/19.
//

#include "XThread.h"'
#include "XLog.h"
#include <thread>
using namespace std;
//线程的sleep，毫秒级别
void XSleep(int mis){
    chrono::milliseconds du(mis);
    this_thread::sleep_for(du);
}
//启动线程
bool XThread::Start(){
    isExits= false;
    //创建一个线程
    thread th(&XThread::XThreadMain,this);
    th.detach();//当前线程放弃对新创建线程的控制
    return true;
}
//安全停止线程（不一定成功）
 void XThread::Stop(){
    isExits= true;
    for (int i = 0; i <200 ; ++i) {
        if (!isRuning){
            XLOGI("stop Thread Sucess");
            return;
        }
        XSleep(1);
    }
    XLOGI("stop Thread out time");
}
void XThread::XThreadMain() {
    isRuning= true;
    XLOGI("线程函数进入");
    Main();
    XLOGI("线程函数退出");
    isRuning= false;
}
