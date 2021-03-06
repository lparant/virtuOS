#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <assert.h>
#include "../src/thread.h"

#define N 100

struct timeval time1;
struct timeval time2;
int i;
int stop = 0;

int getpreemptingtime(int time) {
    return ((time%12000 + 500)/1000)*1000;
}

void *thread1_func()
{
    while (!stop)
    {

    }
    return NULL;
}

void *thread2_func()
{
    long int S = 0;
    int preempting_time;
    for (i = 0; i < N; i++)
    {
        gettimeofday(&time1, NULL);
        thread_yield();
        gettimeofday(&time2, NULL);
        double diff_us = difftime(time2.tv_usec, time1.tv_usec);
        if (diff_us < 0)
            diff_us = 1000000 + diff_us;
        S += diff_us;
        //printf("%f\n",diff_us);
        if(i == 0) { preempting_time = getpreemptingtime(diff_us);} // Calculate the preemption constant while the first iteration
    }
    stop = 1;
    int moy = (int) S/N - preempting_time; // Removing the "preemption constant" specific to each architecture
    printf("Timeslice moyenne: %d (wanted : 12000)\n", moy);
    assert(12000*0.95 < moy && moy < 12000*1.05); // 12000 is the timeslice expected for a normal priority
    return NULL;
}


int main()
{
    thread_t thread1;
    thread_t thread2;
    thread_create(&thread1, thread1_func, NULL);
    thread_create(&thread2, thread2_func, NULL);
    return 0;
}
