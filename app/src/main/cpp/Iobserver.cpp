//
// Created by xj on 2018/6/19.
//

#include "Iobserver.h"


//主体函数
void Iobserver::AddObs(Iobserver *obs){
    if (!obs)//参数为空不处理
        return;
    mux.lock();
    obss.push_back(obs);
    mux.unlock();
}
//通知所有观察者
void Iobserver::Notify(XData data){
     mux.lock();
    int size=obss.size();
    for (int i = 0; i <size ; ++i) {
        obss[i]->Update(data);
    }
    mux.unlock();
}