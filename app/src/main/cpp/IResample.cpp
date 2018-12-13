//
// Created by xj on 2018/6/29.
//

#include "IResample.h"

void IResample::Update(XData data) {
    XData d=this->Resample(data);
    if (d.size>0){
        this->Notify(d);
    }
}
