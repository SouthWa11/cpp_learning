#include<iostream>
#include<string>
#include<pthread.h>
#include<unistd.h>

void* mainfunc1(void* arg);
void* mainfunc2(void* arg);

int cnt=0;
long param=0;

int main(){
    pthread_t pid1, pid2;

    if(pthread_create(&pid1, NULL, mainfunc1, (void*)param) != 0){
        std::cout<<"create thread func1 fail! Error cause: "<<pid1<<std::endl;
        return -1;
    }

    if(pthread_create(&pid2, NULL, mainfunc2, (void*)param) != 0){
        std::cout<<"create thread func2 fail! Error cause: "<<pid2<<std::endl;
        return -1;
    }

    std::cout<<"pid1 - "<<pid1<<"   pid2 - "<<pid2<<std::endl;
    pthread_detach(pid1);

    std::cout<<"thread running"<<std::endl;
    // pthread_join(pid1, NULL);
    // std::cout<<"thread1 done"<<std::endl;
    
    int iret=0;
    pthread_join(pid2, (void**)&iret);
    std::cout<<"thread2 done, return value - "<<iret<<std::endl;

    return 0;
}

void* mainfunc1(void* arg){
    for(int i=0; i<5; i++){
        sleep(1);
        std::cout<<"func1 sleep 1 sec, var1 "<<cnt++<<" - var2 "<<((long)arg++)<<std::endl;
    }
    // thread exit
    // pthread_exit(0);
    //or use the way below
    // return (void*) 0;
}

void* mainfunc2(void* arg){
    for(int i=0; i<10; i++){
        sleep(1);
        std::cout<<"func2 sleep 1 sec, var "<<cnt++<<" - var2 "<<((long)arg++)<<std::endl;
    }
    pthread_exit((void*)10);
}

// SUM UP
// 1. var transmission
// cnt as a global var, finally stop at 14, which is right, but is occupied by different threads in disorder
// arg as a var transmitted,finally stop at 9, which is wrong, due to the missing of mutex
// the way of arg is suggested, sockets transmission of client sockets connected on sercver for example
//
// 2. pthread_detach
// if join is called, ret_val will be 22, which means thread is detached
// the resource and pcb of the thread will be recycled by OS after return avoid of zombie state
// this is the way suggested
//
// 3. thread return_val
// can be acquired using pthread_join(pid, void**)
// how to acquire it in other ways? see C++ 11
//
// 4. thread cancel(not finished)
// main process: 
//      pthread_cancel(pid)
// thread:
//      pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, int* old_state)  
//      pthread_setcanceltype()-set thread to cancel at CANCEL_POINT(i/O func like send/recv, blocking func like sleep etc.)
//      PTHREAD_CANCEL_ASYNCHRONOUS / PTHREAD_CANCEL_DEFFERED
//
//
// 5. thread cleanup(not finished) - usded for recycling resource
// pthread_cleanup_push(void* routine(void*, as a clean func), void* arg)
// ptherad_cleanup_pop(int execute) 0 - pop but not execute, non 0 - pop and execute
//
// must be called 1 to 1, can be imagined as a destructor for cpp
// used to close the socket(file actually) recycle for example, better used in thread rather than main process
