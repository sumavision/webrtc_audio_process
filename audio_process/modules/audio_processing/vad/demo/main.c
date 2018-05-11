#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#include "webrtc_vad.h"


/** webrtc 音频处理模块支持每次处理10ms(vad还支持20ms 30ms)的音频数据，采样率仅支持8k,16k,32k*/
#define WEBRTC_AUDIO_VAD_FRAME_TIME_MS     10
#define WEBRTC_AUDIO_SAMPLE_PER_8K      ((8000*WEBRTC_AUDIO_VAD_FRAME_TIME_MS)/1000)
#define WEBRTC_AUDIO_SAMPLE_PER_16K     ((16000*WEBRTC_AUDIO_VAD_FRAME_TIME_MS)/1000)
#define WEBRTC_AUDIO_SAMPLE_PER_32K     ((32000*WEBRTC_AUDIO_VAD_FRAME_TIME_MS)/1000)

/** webrtc 音频处理模块 (short)buff 最大值*/
#define WEBRTC_BUFF_MAX_SIZE             WEBRTC_AUDIO_SAMPLE_PER_32K

/** webrtc 噪声抑制，频带数目,8k,16k都是低频带，因而是1;32k既有低频带，也有高频带，因而是2*/
#define WEBRTC_VAD_BAND_MAX_NUM               2


/** webrtc 静音检测持续时间，此处为1s,即当1s内检测结果都是silence才认为是静音*/
#define WEBRTC_VAD_SILENCE_TIME_MS_CHECK     1000
#define WEBRTC_VAD_BUFF_OUT_SIZE     WEBRTC_VAD_SILENCE_TIME_MS_CHECK/WEBRTC_AUDIO_VAD_FRAME_TIME_MS

enum Aggressiveness {
    kVadNormal = 0,
    kVadLowBitrate = 1,
    kVadAggressive = 2,
    kVadVeryAggressive = 3
  };

/** 获取单次处理数据大小,resolution为8的倍数*/
int webrtc_audio_size_per(int sample,int resolution)
{
    if(sample == 8000)
    {
        return WEBRTC_AUDIO_SAMPLE_PER_8K * (resolution/8);
    }
    else if (sample == 16000)
    {
        return WEBRTC_AUDIO_SAMPLE_PER_16K * (resolution/8);
    }
    else if (sample == 32000)
    {
        return WEBRTC_AUDIO_SAMPLE_PER_32K * (resolution/8);
    }
    else
    {
        return 0;
    }
}

/** 语音活动检测vad*/
void vad_test(const char *fileIn,const char *fileOut,int sample,int resolution,int mode)
{
    VadInst *vadInst = NULL;

    FILE *pFileIn  = NULL;
    FILE *pFileOut = NULL;
    int sizePer = 0;

    short shBuffIn[WEBRTC_BUFF_MAX_SIZE] = {0};

    vadInst = WebRtcVad_Create();
    if(NULL == vadInst)
    {
        printf("vad instance create fail \n ");
        return;
    }
    if(WebRtcVad_Init(vadInst) != 0)
    {
        printf("vad instance init fail \n ");
        return;
    }
    //vad mode设置：0: Mild, 1: Medium , 2: Aggressive,3 :very Aggressive
    if(WebRtcVad_set_mode(vadInst,mode) != 0)
    {
        printf("vad level set fail \n ");
        return;
    }
    printf("WebRtcVad_set_mode,mode = %d\n",mode);

    sizePer = webrtc_audio_size_per(sample,resolution);
    if(0 == sizePer)
    {
        printf("vad the handling size per get err\n ");
        return;
    }
    int frameSize = sizePer/(resolution/8);
    printf("webrtc_audio_size_per,size = %d, frame_size = %d\n",sizePer,frameSize);

    //vad 采样率和framesize检测
    if (WebRtcVad_ValidRateAndFrameLength(sample,frameSize) != 0)
    {
        printf("vad the sample(%d) and framesize(%d) is invalid\n ",sample,frameSize);
        return;
    }

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
    int ret_bk = -1;
    while (1)
    {
        if(frameSize != (int)fread(shBuffIn,sizeof(short),frameSize,pFileIn))
        {
            printf("pFileIn read finish,counter = %d \n ",count);
            break;
        }
        int ret = WebRtcVad_Process(vadInst,sample,shBuffIn,frameSize);
        if(ret < -1)
        {
            printf("WebRtcVad_Process error\n ");
            break;
        }
        if(ret == 1 && ret != ret_bk)
        {
            printf("the time %d ,is Active Voice\n",count*WEBRTC_AUDIO_VAD_FRAME_TIME_MS);
        }
        else if (ret == 0 && ret != ret_bk)
        {
            printf("the time %d ,is non-active Voice\n",count*WEBRTC_AUDIO_VAD_FRAME_TIME_MS);
        }
        ret_bk = ret;
        if(ret)
        {
           fwrite(shBuffIn,sizeof(short),frameSize,pFileOut);
        }
        count++;
    }
    printf("webrtc_vad data handle end \n");

    WebRtcVad_Free(vadInst);
    fclose(pFileIn);
    fclose(pFileOut);
}
int main()
{
    const char *pathFileIn = "audio_8k_16bit_silence.pcm";
    const char *pathFileOut= "vad_audio_8k_16bit_silece.pcm";
    vad_test(pathFileIn,pathFileOut,8000,16,kVadAggressive);
    return 0;
}
