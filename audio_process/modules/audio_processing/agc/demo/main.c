#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#include "signal_processing_library.h"
#include "gain_control.h"

/** webrtc 音频处理模块仅支持每次处理10ms的音频数据，采样率仅支持8k,16k*/
#define WEBRTC_AUDIO_SAMPLE_PER_8K      ((8000*10)/1000)
#define WEBRTC_AUDIO_SAMPLE_PER_16K     ((16000*10)/1000)

/** webrtc 音频处理模块 (short)buff 最大值*/
#define WEBRTC_BUFF_MAX_SIZE             WEBRTC_AUDIO_SAMPLE_PER_16K

/** webrtc 音频处理模块，频带数目,8k,16k都是低频带，因而是1;32k既有低频带，也有高频带，因而是2*/
#define WEBRTC_AGC_BAND_MAX_NUM               2

typedef void* AgcHandle;

/** 获取单次处理数据大小,resolution为8的倍数*/
int agc_size_per(int sample,int resolution)
{
    if(sample == 8000)
    {
        return WEBRTC_AUDIO_SAMPLE_PER_8K * (resolution/8);
    }
    else if (sample == 16000)
    {
        return WEBRTC_AUDIO_SAMPLE_PER_16K * (resolution/8);
    }
    else
    {
        return 0;
    }
}
/** 自动增益agc*/
void agc_test(const char *fileIn,const char *fileOut,int sample,int resolution)
{
    AgcHandle agcInst = NULL;

    FILE *pFileIn  = NULL;
    FILE *pFileOut = NULL;
    int sizePer = 0;

    short shBuffIn[WEBRTC_BUFF_MAX_SIZE] = {0};
    short shBuffOut[WEBRTC_BUFF_MAX_SIZE] = {0};

    if (NULL == fileIn || NULL == fileOut)
    {
        printf("agc_test file path is NULL \n");
        return;
    }

    agcInst = WebRtcAgc_Create();
    if(NULL == agcInst)
    {
        printf("agc instance create fail \n ");
        return;
    }
    int minLevel = 0;
    int maxLevel = 255;
    if(WebRtcAgc_Init(agcInst,minLevel,maxLevel,kAgcModeFixedDigital,sample) != 0)
    {
        printf("agc instance init fail \n ");
        return;
    }
    WebRtcAgcConfig cfg;
    memset(&cfg,0,sizeof(WebRtcAgcConfig));
    cfg.targetLevelDbfs = 3;
    cfg.compressionGaindB = 20;
    cfg.limiterEnable = 1;
    int ret = WebRtcAgc_set_config(agcInst,cfg);
    if(ret != 0)
    {
        printf("agc instance set config fail : %d\n ",ret);
        return;
    }


    sizePer = agc_size_per(sample,resolution);
    if(0 == sizePer)
    {
        printf("agc the handling size per get err\n ");
        return;
    }
    int frameSize = sizePer/(resolution/8);
    printf("agc_size_per,size = %d, frame_size = %d\n",sizePer,frameSize);

    pFileIn = fopen(fileIn,"rb");
    if(NULL == pFileIn)
    {
        printf("fileNear %s open fail \n ",fileIn);
        return;
    }
    pFileOut = fopen(fileOut,"wb");
    if(NULL == pFileOut)
    {
        printf("fileOut %s open fail \n ",fileOut);
        return;
    }

    int count = 0;
    int currMicLevel = 0;
    while (1)
    {
        count = fread(shBuffIn,sizeof(short),frameSize,pFileIn);
        if(count != frameSize)
        {
            printf("pFileIn read finish,counter = %d \n ",count);
            break;
        }
        short *agcIn[WEBRTC_AGC_BAND_MAX_NUM-1] = {shBuffIn};
        short *agcOut[WEBRTC_AGC_BAND_MAX_NUM-1] = {shBuffOut};

        int inMicLevel = currMicLevel;
        uint8_t saturationWarning= 0;
        int ret = WebRtcAgc_Process(agcInst,agcIn,1,frameSize,agcOut,inMicLevel,&currMicLevel,0,&saturationWarning);
        if(ret < -1)
        {
            printf("WebRtcAgc_Process fail\n ");
            break;
        }
        if(saturationWarning)
        {
            printf("warning : a saturation event has occurred and the volume cannot be further reduced\n ");
        }

        fwrite(shBuffOut,sizeof(short),frameSize,pFileOut);
    }
    WebRtcAgc_Free(agcInst);
    fclose(pFileIn);
    fclose(pFileOut);
}
int main()
{
    const char *pathFileIn = "audio_8k_16bit.pcm";
    const char *pathFileOut= "agc_audio_8k_16bit.pcm";
    agc_test(pathFileIn,pathFileOut,8000,16);
    return 0;
}
