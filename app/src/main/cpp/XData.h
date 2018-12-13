//
// Created by xj on 2018/5/31.
//

#ifndef FFMPEGONEDEMO_XDATA_H
#define FFMPEGONEDEMO_XDATA_H
enum SdataType{
    AVPACKET_TYPE=0,
    UCHAR_TYPE=1
};

struct XData {
    int type=0;
    //时间数据
    long pts=0;
    //保存数据指针地址
    unsigned char *data=0;
    unsigned char *datas[8]={0};
    int size=0;
    int width=0;//视频图像宽
    int height=0;//视频图像高
    //0代表视频，1代表音频，2代表其他
    int decodeType=-1;
    //添加硬解码后处理
    int format=0;//视频类型
    void Drop();//清理函数
    //分配数据空间
    bool Alloc(int size, const char *data=0);
};


#endif //FFMPEGONEDEMO_XDATA_H
