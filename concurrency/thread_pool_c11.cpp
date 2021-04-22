#include<iostream>
#include<string>
#include<thread>
#include<mutex>
#include<vector>
#include<queue>
#include<condition_variable>
#include<functional>
#include<future>
#include<chrono>

class threadpool{
public:
    threadpool(size_t threadnum);
    template<class F, class... Args>
    auto enqueue(F&& func, Args... argv)
        -> std::future<typename std::result_of<F(Args...)>::type>;
    ~threadpool();

private:
    std::mutex queue_mutex;
    std::condition_variable queue_cv;
    bool hangon;

    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
};

inline threadpool::threadpool(size_t threadnum) : 
    hangon(false){
    for(size_t i=0; i<threadnum; i++){
        workers.emplace_back(
            [this]{
                for(;;){
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        this->queue_cv.wait(lock, [this]{return this->hangon || !this->tasks.empty();});
                        
                        if(this->hangon && this->tasks.empty()){
                            return;
                        }
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
                    task();
                }
            }
        );
    }
}

template<class F, class... Args>
auto threadpool::enqueue(F&& func, Args... argv)
    -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared<std::packaged_task<return_type()> >(
        std::bind(std::forward<F>(func), std::forward<Args>(argv)...)
    );

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex);

        if(hangon){
            throw std::runtime_error("enqueue stopped");
        }

        tasks.emplace([task](){(*task)();});
    }
    queue_cv.notify_one();
    return res;
}

inline threadpool::~threadpool(){
    {
        std::unique_lock<std::mutex> lock(this->queue_mutex);
        hangon = true;
    }

    this->queue_cv.notify_all();
    for(std::thread &worker:workers){
        worker.join();
    }
}

int main(int argc, char** argv){
    threadpool pool(4);
    std::vector< std::future<int> > results;

    for(int i = 0; i < 8; ++i) {
        results.emplace_back(
            pool.enqueue([i] {
                std::cout << "hello " << i << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::cout << "world " << i << std::endl;
                return i*i;
            })
        );
    }

    for(auto && result: results)
        std::cout << result.get() << ' ';
    std::cout << std::endl;
    
    return 0;
}