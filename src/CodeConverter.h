//例子2: 用C++语言实现的转换示例程序

/* f.cpp : 代码转换示例C++程序 */
#include <iconv.h>
#include <iostream>
#include <string.h>

#define OUTLEN 255

using namespace std;

// 代码转换操作类
class CodeConverter
{
    private:
        iconv_t cd;
    public:
        // 构造
        CodeConverter(const char *from_charset,const char *to_charset)
        {
            cd = iconv_open(to_charset,from_charset);
        }

        // 析构
        ~CodeConverter()
        {
            iconv_close(cd);
        }

        // 转换输出
        int convert(char *inbuf,int inlen,char *outbuf,int outlen);
};
