#include<iostream>
#include<fstream>
#include<string>
#include<pthread.h>
#include<unistd.h>
#include<signal.h>

void* threadfunc(void* arg);

class connection_pool{
public:
    connection_pool(const char* filename){
        outfile.open(filename, std::ios::app);
        pthread_mutex_init(&filelock, NULL);
    }

    bool get_connection(){
        if(pthread_mutex_trylock(&filelock)){
            return true;
        }

        return false;
    }

    void free_connection(){
        pthread_mutex_unlock(&filelock);
    }

    virtual ~connection_pool(){
        outfile.close();
        pthread_mutex_destroy(&filelock);
    }

    std::ofstream outfile;
private:
    pthread_mutex_t filelock;
    pthread_cond_t lockcond;
};

int main(){
    pthread_t pid1, pid2, pid3;
    std::cout<<"build connection pool !"<<std::endl;
    connection_pool* cp = new connection_pool("./test.log");

    std::cout<<"create thread !"<<std::endl;
    if(pthread_create(&pid1, NULL, &threadfunc, (void*) cp)){
        std::cout<<"create thread 1 failed"<<std::endl;
    }

    if(pthread_create(&pid2, NULL, &threadfunc, (void*) cp)){
        std::cout<<"create thread 2 failed"<<std::endl;
    }

    if(pthread_create(&pid3, NULL, &threadfunc, (void*) cp)){
        std::cout<<"create thread 3 failed"<<std::endl;
    }

    pthread_detach(pid1);pthread_detach(pid2);pthread_detach(pid3);
    std::cout<<"threads detach done, destroy self"<<std::endl;
    
    pthread_join(pid1, NULL);pthread_join(pid2, NULL);pthread_join(pid3, NULL);
    

    // sleep(20);
    delete cp;
    return 0;
}

void* threadfunc(void* arg){
    int pid = pthread_self();
    std::cout<<"thread "<<pid<<" working !"<<std::endl;

    int ii=0;
    connection_pool* conn = (connection_pool*)arg;
    while(! conn->get_connection()){
        std::cout<<"thread "<<pid<<" connecting failed !"<<std::endl;
    }

    std::cout<<"thread "<<pid<<" connecting success !"<<std::endl;
    while(ii < 3){
        conn->outfile <<"thread "<<pid<<" - log "<< ++ii <<std::endl;
    }
    
    conn->free_connection();
}