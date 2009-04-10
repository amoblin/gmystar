#include <pthread.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

class processer
{
    public:
        processer();
        ~processer();

        int thread(int *in);

    private:
        pthread_t pid;
        int i;
        static void * thread_crust(void *p)
        {
            static_cast <processer *>(i)->thread();
        };
};

processer::processer() : i(5)
{
    pthread_create(&pid, NULL, thread_crust, this);
}

processer::~processer()
{
    pthread_join(pid, NULL);
}

int processer::thread( int *in)
{
    while(i--)
    {
        cout << in << endl;
        sleep(1);
    }
}

int main()
{
    processer p;

    return 0;
}
