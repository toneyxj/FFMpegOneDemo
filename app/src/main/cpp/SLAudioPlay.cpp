//
// Created by xj on 2018/6/29.
//

#include "SLAudioPlay.h"
#include "XLog.h"

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

//创建音频处理对象-只在本cpp文件中有效
static SLObjectItf engineSL = NULL;
static SLEngineItf eng = NULL;
//输出的混音器
static SLObjectItf mix = NULL;
//声明播放器
static SLObjectItf player = NULL;
static SLPlayItf iplayer = NULL;
//缓存队列
static SLAndroidSimpleBufferQueueItf pcmque = NULL;

SLAudioPlay::SLAudioPlay() {
    buf = new unsigned char[1024 * 1024];//申请一兆空间
}

SLAudioPlay::~SLAudioPlay() {
    delete buf;
    buf = 0;
}

/**
 * 创建音频处理引擎
 */
static SLEngineItf CreateSL() {

    SLresult re;
    SLEngineItf en;
    re = slCreateEngine(&engineSL, 0, 0, 0, 0, 0);
    //如果创建不成功
    if (re != SL_RESULT_SUCCESS)return NULL;
    //取到具体对象
    re = (*engineSL)->Realize(engineSL, SL_BOOLEAN_FALSE);//SL_BOOLEAN_FALSE 等待创建
    if (re != SL_RESULT_SUCCESS)return NULL;
    //获取接口  en：获取到引擎接口存放对象
    re = (*engineSL)->GetInterface(engineSL, SL_IID_ENGINE, &en);
    if (re != SL_RESULT_SUCCESS)return NULL;
    return en;
}

void SLAudioPlay::PlayCall(void *bufq) {
    if (!bufq)
        return;
    SLAndroidSimpleBufferQueueItf bf = (SLAndroidSimpleBufferQueueItf) bufq;

    XData d = getData();
    if (d.size <= 0) {
        XLOGE("PlayCall getData() size is 0");
        return;
    }
    if (!buf) {
        return;
    }
    memcpy(buf, d.data, d.size);
    //发送音频数据进行播放
    mux.lock();
    if (pcmque && (*pcmque))
        (*bf)->Enqueue(bf, buf, d.size);
    mux.unlock();
    d.Drop();
}

static void pcmCall(SLAndroidSimpleBufferQueueItf bf, void *contex) {
    SLAudioPlay *ap = (SLAudioPlay *) contex;
    if (!ap) {
        XLOGE("pcmCall failed contex is null");
        return;
    }
    ap->PlayCall((void *) bf);
}

void SLAudioPlay::Close() {
    IAudioPlay::Clear();
    mux.lock();
    //停止播放
    if (iplayer && (*iplayer)) {
        (*iplayer)->SetPlayState(iplayer, SL_PLAYSTATE_STOPPED);
    }
    //清理播放队列
    if (pcmque && (*pcmque)) {
        (*pcmque)->Clear(pcmque);
    }
    //销毁player播放器对象
    if (player && (*player)) {
        (*player)->Destroy(player);
    }
    //销毁混音器
    if (mix && (*mix)) {
        (*mix)->Destroy(mix);
    }
    //销毁播放引擎
    if (engineSL && (*engineSL)) {
        (*engineSL)->Destroy(engineSL);
    }

    //创建音频处理对象-只在本cpp文件中有效
    engineSL = NULL;
    eng = NULL;
    //输出的混音器
    mix = NULL;
    //声明播放器
    player = NULL;
    iplayer = NULL;
    //缓存队列
     pcmque = NULL;

    mux.unlock();
}

bool SLAudioPlay::StartPlay(XParameter out) {
    Close();
    mux.lock();
    //创建引擎
    eng = CreateSL();

    if (!eng) {
        XLOGE("create SLEngineItf false!");
        mux.unlock();
        return false;
    }
    //创建混音器
    SLresult re = 0;
    (*eng)->CreateOutputMix(eng, &mix, 0, 0, 0);
    //实例化对象,SL_BOOLEAN_FALSE:阻塞等待创建完毕
    re = (*mix)->Realize(mix, SL_BOOLEAN_FALSE);
    if (re != SL_RESULT_SUCCESS) {//不成功进入
        XLOGE("初始化混音器失败");
        mux.unlock();
        return false;
    }
    //成立的接口提供给播放器回调
    SLDataLocator_OutputMix outmix = {SL_DATALOCATOR_OUTPUTMIX, mix};
    SLDataSink audiosink = {&outmix, 0};

    //配置音频信息
    //配置缓冲队列
    SLDataLocator_AndroidSimpleBufferQueue que = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 10};
    //音频格式
    SLDataFormat_PCM pcm = {SL_DATAFORMAT_PCM,
                            (SLuint32) out.channnels,//声道数
                            (SLuint32) out.sample_rate * 1000,//采样率
                            SL_PCMSAMPLEFORMAT_FIXED_16,//位数
                            SL_PCMSAMPLEFORMAT_FIXED_16,
                            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,//左右声道
                            SL_BYTEORDER_LITTLEENDIAN,//字节序，小端
    };
    //生成数据源结构体供播放器使用
    SLDataSource ds = {&que, &pcm};

    //创建播放器------代码

    //除SL_IID_PLAY ，其余接口获取都需要在这里注册
    const SLInterfaceID ids[] = {SL_IID_BUFFERQUEUE};//表示待会需要获取的接口
    const SLboolean req[] = {SL_BOOLEAN_TRUE};//true 代表要开放这个接口
    re = (*eng)->CreateAudioPlayer(eng, &player, &ds, &audiosink,
                                   sizeof(ids) / sizeof(SLInterfaceID),//开放几个接口
                                   ids,//设置可能开放的接口
                                   req//是否开放
    );
    if (re != SL_RESULT_SUCCESS) {//不成功进入
        XLOGE("CreateAudioPlayer failed!");
        mux.unlock();
        return false;
    }
    //初始化
    (*player)->Realize(player, SL_BOOLEAN_FALSE);
    //获取player接口
    re = (*player)->GetInterface(player, SL_IID_PLAY, &iplayer);
    if (re != SL_RESULT_SUCCESS) {//不成功进入
        XLOGE("GetInterface failed!");
        mux.unlock();
        return false;
    }
    //开始获取队列接口
    re = (*player)->GetInterface(player, SL_IID_BUFFERQUEUE, &pcmque);
    if (re != SL_RESULT_SUCCESS) {//不成功进入
        XLOGE("GetInterface SL_IID_BUFFERQUEUE failed!");
        mux.unlock();
        return false;
    }

    //设置回调函数,播放队列空调用,参数三是代表传递的值
    (*pcmque)->RegisterCallback(pcmque, pcmCall, this);
    //设置队列为播放状态
    (*iplayer)->SetPlayState(iplayer, SL_PLAYSTATE_PLAYING);

    //启动队列的回调
    (*pcmque)->Enqueue(pcmque, "", 1);
    XLOGI("StartPlay audio sucess");
    mux.unlock();
    return true;
}