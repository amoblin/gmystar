#include <pthread.h>
#include <iostream>
#include <stdlib.h>
using namespace std;
//首先定义一个ThreadClass接口,所有回调类都直接或者间接继承自该类   
//它规定了回调接口   
class ThreadClass
{
    public:   
        virtual void *operator()(void *)=0;   
};  
//再定义一个线程信息结构,其中包含了要调用的class和出入的数据   
struct _threadInfo
{
    ThreadClass *threadClass;
    void *data;
};
typedef _threadInfo ThreadInfo;
class ThreadCall
{
    public:
        static void *Fun(void *data)
        {   
            ThreadInfo *info=(ThreadInfo *)data;   
            return info->threadClass->operator ()(info->data);   
        }   
};  
class MyThreadClass:ThreadClass
{
    public:
        virtual void *operator()(void *data)
        {   
            char *str=(char *)data;   
            cout<<str<<endl;   
            return NULL;   
        };   
};   
int main(int argc,char *argv[])
{
    MyThreadClass threadClass;   
    ThreadInfo info;   
    info.threadClass=(ThreadClass *)&threadClass;   
    char *temp="hello";
    info.data=temp;
    pthread_t pid;   
    pthread_create(&pid,NULL,&ThreadCall::Fun,&info);   
    //system("pause");   
}  
