#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#include "signal_processing_library.h"
#include "noise_suppression_x.h"
#include "noise_suppression.h"


/** webrtc 音频处理模块仅支持每次处理10ms的音频数据，采样率仅支持8k,16k,32k*/
#define WEBRTC_AUDIO_SAMPLE_PER_8K      ((8000*10)/1000)
#define WEBRTC_AUDIO_SAMPLE_PER_16K     ((16000*10)/1000)
#define WEBRTC_AUDIO_SAMPLE_PER_32K     ((32000*10)/1000)

/** webrtc 音频处理模块 (short)buff 最大值*/
#define WEBRTC_BUFF_MAX_SIZE             WEBRTC_AUDIO_SAMPLE_PER_32K

/** webrtc 噪声抑制，频带数目,8k,16k都是低频带，因而是1;32k既有低频带，也有高频带，因而是2*/
#define WEBRTC_NS_BAND_MAX_NUM               2

/** 获取单次处理数据大小,resolution为8的倍数*/
int noise_suppression_size_per(int sample,int resolution)
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
static void ns_FloatToShort(const float *src, ssize_t size, short *dest)
{
    int i = 0;
    for (i = 0;i < size; i++)
    {
        dest[i] = (short)src[i];
    }
}
static void ns_ShortToFloat(const short *src, ssize_t size, float *dest)
{
    int i = 0;
    for (i = 0;i < size; i++)
    {
        dest[i] = (float)src[i];
    }
}
typedef float *const webrtc_ns_frame;
/** 浮点噪声抑制*/
void noise_suppression_float(const char *fileIn,const char *fileOut,int sample,int resolution,int mode)
{
    NsHandle *nsInst = NULL;

    FILE *pFileIn  = NULL;
    FILE *pFileOut = NULL;
    int sizePer = 0;

    char *pBuffIn = NULL;
    char *pBuffOut= NULL;

    int num_bands = 1;
    int fileInLen = 0;

    nsInst = WebRtcNs_Create();
    if(NULL == nsInst)
    {
        printf("ns instance create fail \n ");
        return;
    }
    if(WebRtcNs_Init(nsInst,sample) < 0)
    {
        printf("ns instance init fail \n ");
        return;
    }
    //噪声抑制级别设置：0: Mild, 1: Medium , 2: Aggressive
    if(WebRtcNs_set_policy(nsInst,mode) < 0)
    {
        printf("ns level set fail \n ");
        return;
    }

    sizePer = noise_suppression_size_per(sample,resolution);
    if(0 == sizePer)
    {
        printf("ns the handling size per get err\n ");
        return;
    }
    int frameSize = sizePer/(resolution/8);
    printf("noise_suppression_size_per,size = %d, frame_size = %d\n",sizePer,frameSize);

    pFileIn = fopen(fileIn,"rb");
    if(NULL == pFileIn)
    {
        printf("fileIn %s open fail \n ",fileIn);
        return;
    }
    //输入音频文件大小
    fseek(pFileIn,0,SEEK_END);
    fileInLen = ftell(pFileIn);
    fseek(pFileIn,0,SEEK_SET);
    printf("ns input file size is %d\n",fileInLen);

    pBuffIn = (char *)malloc(sizeof(char)*fileInLen);
    if(NULL == pBuffIn)
    {
        printf("ns pBuffIn malloc err\n ");
        return;
    }
    memset(pBuffIn,0,fileInLen);
    if((int)fread(pBuffIn,sizeof(char),fileInLen,pFileIn) < fileInLen)
    {
        printf("ns input file fread err\n ");
        return;
    }

    pBuffOut = (char *)malloc(sizeof(char)*fileInLen);
    if(NULL == pBuffOut)
    {
        printf("ns pBuffOut malloc err\n ");
        return;
    }
    memset(pBuffOut,0,fileInLen);

    int i = 0;
    for(i = 0;i < fileInLen;i += sizePer)
    {
        if(fileInLen - i < sizePer)
        {
            printf("webrtc_ns fileIn data remaining size = %d \n",fileInLen - i);
            break;
        }
        short shBuffIn[WEBRTC_BUFF_MAX_SIZE] = {0};
        short shBuffOut[WEBRTC_BUFF_MAX_SIZE] = {0};
        float fBuffIn[WEBRTC_BUFF_MAX_SIZE] = {0.0};
        float fBuffOut[WEBRTC_BUFF_MAX_SIZE] = {0.0};

        memcpy(shBuffIn,(char*)(pBuffIn+i),frameSize*sizeof(short));
        ns_ShortToFloat(shBuffIn,frameSize,fBuffIn);

        float *nsIn[WEBRTC_NS_BAND_MAX_NUM-1] = {fBuffIn};
        float *nsOut[WEBRTC_NS_BAND_MAX_NUM-1] = {fBuffOut};

        WebRtcNs_Analyze(nsInst,nsIn[0]);
        WebRtcNs_Process(nsInst,nsIn,num_bands,nsOut);

        ns_FloatToShort(fBuffOut,frameSize,shBuffOut);
        memcpy(pBuffOut+i,shBuffOut,frameSize*sizeof(short));
    }

    printf("webrtc_ns data handle end \n");

    pFileOut = fopen(fileOut,"wb");
    if(NULL == pFileOut)
    {
        printf("fileOut %s open fail \n ",fileOut);
        return;
    }
    fwrite(pBuffOut,sizeof(char),fileInLen,pFileOut);

    WebRtcNs_Free(nsInst);
    fclose(pFileIn);
    fclose(pFileOut);
    free(pBuffIn);
    free(pBuffOut);
}

/** 定点噪声抑制*/
void noise_suppression_x(const char *fileIn,const char *fileOut,int sample,int resolution,int mode)
{
    NsxHandle *nsInst = NULL;

    FILE *pFileIn  = NULL;
    FILE *pFileOut = NULL;
    int sizePer = 0;

    char *pBuffIn = NULL;
    char *pBuffOut= NULL;

    int num_bands = 1;
    int fileInLen = 0;

    nsInst = WebRtcNsx_Create();
    if(NULL == nsInst)
    {
        printf("ns instance create fail \n ");
        return;
    }
    if(WebRtcNsx_Init(nsInst,sample) < 0)
    {
        printf("ns instance init fail \n ");
        return;
    }
    //噪声抑制级别设置：0: Mild, 1: Medium , 2: Aggressive
    if(WebRtcNsx_set_policy(nsInst,mode) < 0)
    {
        printf("ns level set fail \n ");
        return;
    }

    sizePer = noise_suppression_size_per(sample,resolution);
    if(0 == sizePer)
    {
        printf("ns the handling size per get err\n ");
        return;
    }
    int frameSize = sizePer/(resolution/8);
    printf("noise_suppression_size_per,size = %d, frame_size = %d\n",sizePer,frameSize);

    pFileIn = fopen(fileIn,"rb");
    if(NULL == pFileIn)
    {
        printf("fileIn %s open fail \n ",fileIn);
        return;
    }
    //输入音频文件大小
    fseek(pFileIn,0,SEEK_END);
    fileInLen = ftell(pFileIn);
    fseek(pFileIn,0,SEEK_SET);
    printf("ns input file size is %d\n",fileInLen);

    pBuffIn = (char *)malloc(sizeof(char)*fileInLen);
    if(NULL == pBuffIn)
    {
        printf("ns pBuffIn malloc err\n ");
        return;
    }
    memset(pBuffIn,0,fileInLen);
    if((int)fread(pBuffIn,sizeof(char),fileInLen,pFileIn) < fileInLen)
    {
        printf("ns input file fread err\n ");
        return;
    }

    pBuffOut = (char *)malloc(sizeof(char)*fileInLen);
    if(NULL == pBuffOut)
    {
        printf("ns pBuffOut malloc err\n ");
        return;
    }
    memset(pBuffOut,0,fileInLen);

    int i = 0;
    for(i = 0;i < fileInLen;i += sizePer)
    {
        if(fileInLen - i < sizePer)
        {
            printf("webrtc_ns fileIn data remaining size = %d \n",fileInLen - i);
            break;
        }
        short shBuffIn[WEBRTC_BUFF_MAX_SIZE] = {0};
        short shBuffOut[WEBRTC_BUFF_MAX_SIZE] = {0};

        short *nsIn[WEBRTC_NS_BAND_MAX_NUM-1] = {shBuffIn};
        short *nsOut[WEBRTC_NS_BAND_MAX_NUM-1] = {shBuffOut};

        memcpy(shBuffIn,(char*)(pBuffIn+i),frameSize*sizeof(short));
        WebRtcNsx_Process(nsInst,nsIn,num_bands,nsOut);
        memcpy(pBuffOut+i,shBuffOut,frameSize*sizeof(short));
    }

    printf("webrtc_ns data handle end \n");

    pFileOut = fopen(fileOut,"wb");
    if(NULL == pFileOut)
    {
        printf("fileOut %s open fail \n ",fileOut);
        return;
    }
    fwrite(pBuffOut,sizeof(char),fileInLen,pFileOut);

    WebRtcNsx_Free(nsInst);
    fclose(pFileIn);
    fclose(pFileOut);
    free(pBuffIn);
    free(pBuffOut);
}
int main()
{
    const char *pathFileIn = "audio_16k_16bit.pcm";
    const char *pathFileOut= "ns_audio_16k_16bit.pcm";
    const char *pathFileOutX= "nsx_audio_16k_16bit.pcm";
    noise_suppression_float(pathFileIn,pathFileOut,16000,16,2);
    noise_suppression_x(pathFileIn,pathFileOutX,16000,16,2);
    return 0;
}
