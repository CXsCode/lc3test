#include "include/lc3.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
    //PCM格式
    enum lc3_pcm_format pcmFormat = LC3_PCM_FORMAT_S16;
    //帧长10ms
    int dtUs = 10000;
    //采样率48K
    int srHz =48000;
    //单帧编码后输出字节数
    uint16_t output_byte_count = 120;
    //编码器需占用缓存大小
    unsigned encodeSize = lc3_encoder_size(dtUs, srHz);
    //解码器需占用缓存大小
    unsigned decodeSize = lc3_decoder_size(dtUs, srHz);
    //单帧的采样数
    uint16_t sampleOfFrames = lc3_frame_samples(dtUs, srHz);
    //单帧字节数，一个采样占用两个字节
    uint16_t bytesOfFrames = sampleOfFrames*2;
    //编码器缓存
    void* encMem = NULL;
    //解码器缓存
    void* decMem = NULL;
    //输入文件的文件描述符
    int inFd = -1;
    //输出文件的文件描述符
    int outFd = -1;
    //输入帧缓冲
    unsigned char *inBuf = (unsigned char *)malloc(bytesOfFrames);
    //输出帧缓冲
    unsigned char *outBuf = (unsigned char *)malloc(bytesOfFrames);
    encMem = malloc(encodeSize);
    /*                      编码                           */
    lc3_encoder_t lc3_encoder = lc3_setup_encoder(dtUs, srHz, 0, encMem);
    if((inFd = open("./test_48K_16.pcm", O_RDONLY))<=0)
    {
        printf("encode open inFd err\n");
        return -1;
    }
    if((outFd = open("./encodedata", O_CREAT|O_WRONLY|O_TRUNC, 0666))<=0)
    {
        printf("encode open outFd err\n");
        return -1;
    }
    while(read(inFd,inBuf,bytesOfFrames)==bytesOfFrames)
    {
        lc3_encode(lc3_encoder, pcmFormat, (const int16_t*)inBuf, 1,output_byte_count, outBuf);
        // memcpy(outBuf,inBuf,bytesOfFrames);
        if(write(outFd,outBuf,output_byte_count)!=output_byte_count)
        {
            printf("encode write err\n");
            break;
        }
        memset(inBuf,0,bytesOfFrames);
        memset(outBuf,0,bytesOfFrames);
    }
    free(encMem);
    encMem = NULL;
    close(inFd);
    close(outFd);
    /*                      解码                           */
    decMem = malloc(decodeSize);
    lc3_decoder_t lc3_decoder = lc3_setup_decoder(dtUs, srHz, 0, decMem);    
    if((inFd = open("./encodedata", O_RDONLY))<=0)
    {
        printf("decode open inFd err\n");
        return -1;
    }
    if((outFd = open("./decode.pcm", O_CREAT|O_WRONLY|O_TRUNC, 0666))<=0)
    {
        printf("decode open outFd err\n");
        return -1;
    }  
    while(read(inFd,inBuf,output_byte_count)==output_byte_count)
    {
        lc3_decode(lc3_decoder, inBuf, output_byte_count, pcmFormat,outBuf, 1);
        if(write(outFd,outBuf,bytesOfFrames)!=bytesOfFrames)
        {
            printf("decode write err\n");
            break;
        }
        memset(inBuf,0,bytesOfFrames);
        memset(outBuf,0,bytesOfFrames);
    }
    free(decMem);
    close(inFd);
    close(outFd);
    free(inBuf);
    free(outBuf);
    inBuf = NULL;
    outBuf = NULL;
    return 0;
}









