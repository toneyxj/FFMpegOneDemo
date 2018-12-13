//
// Created by xj on 2018/5/31.
//

#include "XData.h"
extern "C"
{
#include <libavformat/avformat.h>
}
//删除数据
void XData::Drop() {
    if(!data)return;
    if (type==AVPACKET_TYPE){
        av_packet_free((AVPacket **) &data);
    } else{
        delete data;
    }
    data=0;
    size=0;
}
bool XData::Alloc(int size, const char *d){
    Drop();//先清理数据 再分配空间
    type=UCHAR_TYPE;
    if(size<=0)return false;
    this->data=new unsigned char[size];
    if(!this->data)return false;
    if (d){
        //参数一：保存数据的地址，参数二：复制数据的地址,参数三：数据的大小
        memcpy(this->data,d,size);
    }
    this->size=size;
    return true;
}