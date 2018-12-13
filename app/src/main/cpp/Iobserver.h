//
// Created by xj on 2018/6/19.
//观察者
//

#ifndef FFMPEGONEDEMO_IOBSERVER_H
#define FFMPEGONEDEMO_IOBSERVER_H

#include <vector>
//互斥变量c++ 11支持
#include <mutex>

#include "XData.h"
#include "XThread.h"

class Iobserver : public XThread{
public:
    //观察者接收数据函数
    virtual void Update(XData data){};//空代表可以实现也可以不实现
    //主体函数
    void AddObs(Iobserver *obs);
    //通知所有观察者
    void Notify(XData data);

protected:
    std::vector<Iobserver *> obss;//存储观察者
    std::mutex mux;//互斥
};


#endif //FFMPEGONEDEMO_IOBSERVER_H
