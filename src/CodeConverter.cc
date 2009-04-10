#include "CodeConverter.h"
int CodeConverter::convert(char *inbuf,int inlen,char *outbuf,int outlen)
{
    char **pin = &inbuf;
    char **pout = &outbuf;

    memset(outbuf,0,outlen);
    return iconv(cd,pin,(size_t *)&inlen,pout,(size_t *)&outlen);
}
/*
int main(int argc, char **argv)
{
    char *in_gb2312 = "姝ｅ?ㄥ??瑁?";
    char *in_utf8 = "正在安装";
    char out[OUTLEN];

    // utf-8-->gb2312
    CodeConverter cc = CodeConverter("utf-8","gb2312");
    cc.convert(in_utf8,strlen(in_utf8),out,OUTLEN);
    cout << "utf-8-->gb2312 in=" << in_utf8 << ",out=" << out << endl;

    // gb2312-->utf-8
    CodeConverter cc2 = CodeConverter("gb2312","utf-8");
    cc2.convert(in_gb2312,strlen(in_gb2312),out,OUTLEN);
    cout << "gb2312-->utf-8 in=" << in_gb2312 << ",out=" << out << endl;
}
*/
