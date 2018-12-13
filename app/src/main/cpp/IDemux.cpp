//
// Created by xj on 2018/5/31.
//

#include "IDemux.h"
#include "XLog.h"

void IDemux::Main() {
    while (!isExits){
       XData d= Read();
        if (d.size>0&&d.decodeType>=0){
            Notify(d);
        } else{
            d.Drop();
        }
    }
}
