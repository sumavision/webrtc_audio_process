#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#include "signal_processing_library.h"
#include "echo_control_mobile.h"


/** webrtc 音频处理模块仅支持每次处理10ms的音频数据，采样率仅支持8k,16k*/
#define WEBRTC_AUDIO_SAMPLE_PER_8K      ((8000*10)/1000)
#define WEBRTC_AUDIO_SAMPLE_PER_16K     ((16000*10)/1000)

/** webrtc 音频处理模块 (short)buff 最大值*/
#define WEBRTC_BUFF_MAX_SIZE             WEBRTC_AUDIO_SAMPLE_PER_16K

/** webrtc delay */
#define WEBRTC_AECM_DELAY_MS             40

typedef void* AecmHandle;

/** 获取单次处理数据大小,resolution为8的倍数*/
int aecm_size_per(int sample,int resolution)
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

/** 回声消除aecm*/
void aecm_test(const char *fileFar,const char *fileNear,const char *fileOut,int sample,int resolution)
{
    AecmHandle aecmInst = NULL;

    FILE *pFileFar  = NULL;
    FILE *pFileNear  = NULL;
    FILE *pFileOut = NULL;
    int sizePer = 0;

    short shBuffFar[WEBRTC_BUFF_MAX_SIZE] = {0};
    short shBuffNear[WEBRTC_BUFF_MAX_SIZE] = {0};
    short shBuffOut[WEBRTC_BUFF_MAX_SIZE] = {0};

    if (NULL == fileFar || NULL == fileNear || NULL == fileOut)
    {
        printf("aecm_test file path is NULL \n");
        return;
    }

    aecmInst = WebRtcAecm_Create();
    if(NULL == aecmInst)
    {
        printf("aecm instance create fail \n ");
        return;
    }
    if(WebRtcAecm_Init(aecmInst,sample) != 0)
    {
        printf("aecm instance init fail \n ");
        return;
    }
    AecmConfig cfg;
    cfg.cngMode = AecmTrue;
    cfg.echoMode = 1;
    if(WebRtcAecm_set_config(aecmInst,cfg) != 0)
    {
        printf("aecm instance set config fail \n ");
        return;
    }


    sizePer = aecm_size_per(sample,resolution);
    if(0 == sizePer)
    {
        printf("aecm the handling size per get err\n ");
        return;
    }
    int frameSize = sizePer/(resolution/8);
    printf("aecm_size_per,size = %d, frame_size = %d\n",sizePer,frameSize);

    pFileFar = fopen(fileFar,"rb");
    if(NULL == pFileFar)
    {
        printf("fileFar %s open fail \n ",fileFar);
        return;
    }
    pFileNear = fopen(fileNear,"rb");
    if(NULL == pFileFar)
    {
        printf("fileNear %s open fail \n ",fileNear);
        return;
    }
    pFileOut = fopen(fileOut,"wb");
    if(NULL == pFileOut)
    {
        printf("fileOut %s open fail \n ",fileOut);
        return;
    }

    int count = 0;
    while (1)
    {
        count = fread(shBuffFar,sizeof(short),frameSize,pFileFar);
        if(count != frameSize)
        {
            printf("pFileFar read finish,counter = %d \n ",count);
            break;
        }
        count = fread(shBuffNear,sizeof(short),frameSize,pFileNear);

        WebRtcAecm_BufferFarend(aecmInst,shBuffFar,frameSize);
        WebRtcAecm_Process(aecmInst,shBuffNear,NULL,shBuffOut,frameSize,WEBRTC_AECM_DELAY_MS);

        fwrite(shBuffOut,sizeof(short),frameSize,pFileOut);
    }
    WebRtcAecm_Free(aecmInst);
    fclose(pFileFar);
    fclose(pFileNear);
    fclose(pFileOut);
}
int main()
{
    const char *pathFileFar = "speaker.pcm";
    const char *pathFileNear= "micin.pcm";
    const char *pathFileOut= "aecm_out.pcm";
    aecm_test(pathFileFar,pathFileNear,pathFileOut,8000,16);
    return 0;
}
