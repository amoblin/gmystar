#include "Mystar.h"
#include <iconv.h>
/*
 * 这个未能实现，不知道为什么？
char *gbk2utf8(char *src)
{
    //printf("the previous string is:%s\n",src);
    iconv_t conv = iconv_open("utf-8","GB2312");
    if(conv == (iconv_t)-1)
    {
        printf("iconv_open:%d\n",errno);
    }

    size_t msgBuf_len = strlen(src);//不包含\0的长度
    //size_t *msgBuf_len_size_t = &msgBuf_len;
    size_t msgBuf_utf8_len = msgBuf_len * 2;
    //size_t *msgBuf_utf8_len_size_t = &msgBuf_utf8_len;
    char *msgBuf_utf8 = (char*)malloc(msgBuf_utf8_len);
    bzero(msgBuf_utf8,msgBuf_utf8_len);
    int ret=0;
    ret = iconv(conv, (char**)&src, &msgBuf_len, &msgBuf_utf8, &msgBuf_utf8_len);
    if(ret == -1)
    {
        printf("iconv:%d\n",errno);
    }
    else
    {
        printf("conv success!\n");
        //printf("now the string is:%s\n",msgBuf_utf8);
        return msgBuf_utf8;
    }
    iconv_close(conv);

}
*/

//代码转换:从一种编码转为另一种编码
int code_convert(char *from_charset,char *to_charset,char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
    iconv_t cd;
    int rc;
    char **pin = &inbuf;
    char **pout = &outbuf;

    cd = iconv_open(to_charset,from_charset);
    if (cd==0) return -1;
    memset(outbuf,0,outlen);
    if (iconv(cd,pin,&inlen,pout,&outlen)==-1) return -1;
    iconv_close(cd);
    return 0;
}
//UNICODE码转为GB2312码
int u2g(char *inbuf,int inlen,char *outbuf,int outlen)
{
    //char* to string
    char from_charset[] = "utf-8";
    char to_charset[] = "gb2312";
    return code_convert(from_charset,to_charset,inbuf,inlen,outbuf,outlen);
}
//GB2312码转为UNICODE码
int g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
    //char* to string
    char from_charset[] = "gb2312";
    char to_charset[] = "utf-8";
    return code_convert(from_charset,to_charset,inbuf,inlen,outbuf,outlen);
}
char *Mystar::gbk2utf8(char *src)//将gbk转化为utf8编码 2009-03-25 00:58成功!
{
    char utf8_str[255];
    int rc = g2u(src,strlen(src),utf8_str,255);
    char *utf8_p = utf8_str;
    return utf8_p;
}

/*
 * 另一种比较底层的编码转换算法,未用iconv,不会用,要求对编码字节很了解
 void Mystar::ConvertGBKToUtf8(CString &amp,CString strGBK)
 { 
 int len=MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, NULL,0); 
 unsigned short * wszUtf8 = new unsigned short[len+1]; 
 memset(wszUtf8, 0, len * 2 + 2); 
 MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, (LPWSTR)wszUtf8, len); 
 len = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)wszUtf8, -1, NULL, 0, NULL, NULL); 
 char *szUtf8=new char[len + 1]; 
 memset(szUtf8, 0, len + 1); 
 WideCharToMultiByte (CP_UTF8, 0, (LPCWSTR)wszUtf8, -1, szUtf8, len, NULL,NULL); 
 //strGBK = szUtf8; 
 amp=szUtf8;
 delete[] szUtf8; 
 delete[] wszUtf8; 
 } 

 void ConvertUtf8ToGBK(CString &amp,CString strUtf8)
 { 
 int len=MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, NULL,0); 
 unsigned short * wszGBK = new unsigned short[len+1]; 
 memset(wszGBK, 0, len * 2 + 2); 
 MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, (LPWSTR)wszGBK, len); 
 len = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, NULL, 0, NULL, NULL); 
 char *szGBK=new char[len + 1]; 
 memset(szGBK, 0, len + 1); 
 WideCharToMultiByte (CP_ACP, 0, (LPCWSTR)wszGBK, -1, szGBK, len, NULL,NULL); 
//strUtf8 = szGBK; 
amp=szGBK;
delete[] szGBK; 
delete[] wszGBK; 
}
 */
