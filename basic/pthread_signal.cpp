#include<iostream>
#include<string>
#include<pthread.h>
#include<unistd.h>
#include<signal.h>

void* mainfunc1(void* arg);
void* mainfunc2(void* arg);
void sigfunc(int sig);

int cnt=0;
long param=0;

int main(){
    pthread_t pid1, pid2;
    signal(2, sigfunc);
    
    std::cout<<"pid - "<<getpid()<<" ongoing"<<std::endl;
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
    pthread_kill(pid2, 2);

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

void sigfunc(int sig){
    std::cout<<"sigfunc pid: "<<getpid()<<" get signal-"<<sig<<std::endl;
    sleep(1);
}

// SUM UP
// 1. sigfunc location
// signal handler function are the same anywhere in multi-thread program, due to the handler belonging to the main process.
// so normally the func is located in the mainfunc
//
// 2. sigfunc is globally unified for the whole process group
// which means one signal refers to certain sigfunc in one program
// unless the signal set is changed
//
// other APIs include pthread_kill(pid, signal) which block the certain thread to handle sigfunc.
// sigwait, sigwaitinfo, sigtimedwait, pthread_signalmask(used to block different signals)

