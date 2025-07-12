// 7、C++11 condition_variable 使用场景

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

std::mutex g_mutex;
std::condition_variable g_cv;
std::queue<int> g_queue;

void Producer()
{
    for (int i = 0; i < 10; ++i)
    {
        std::unique_lock<std::mutex> lock(g_mutex);
        g_queue.push(i);
        std::cout << "Produced: " << i << std::endl;
    }
    g_cv.notify_one();                                         // Notify one waiting thread
    std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Allow time for consumer to process
}

void Consumer()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(g_mutex);
        g_cv.wait(lock, []
                  { return !g_queue.empty(); }); // Wait until queue is not empty
        int value = g_queue.front();
        g_queue.pop();
        std::cout << "Consumed: " << value << std::endl;
    }
}

int main()
{
    std::thread producer_thread(Producer);
    std::thread consumer_thread(Consumer);

    producer_thread.join();
    consumer_thread.join();

    return 0;
}