//
// Created by xj on 2018/6/19.
//

#ifndef FFMPEGONEDEMO_XTHREAD_H
#define FFMPEGONEDEMO_XTHREAD_H

void XSleep(int mis);

class XThread {
public:
    //启动线程
    virtual bool Start();
    //安全停止线程（不一定成功）
    virtual void Stop();
    //入口主函数
    virtual void Main(){};
protected:
    bool isExits= false;
    bool  isRuning= false;
private:
    void XThreadMain();
};


#endif //FFMPEGONEDEMO_XTHREAD_H
