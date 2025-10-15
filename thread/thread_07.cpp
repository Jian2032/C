// // 7、C++11 condition_variable 使用场景

// #include <iostream>
// #include <thread>
// #include <mutex>
// #include <condition_variable>
// #include <queue>

// std::mutex g_mutex;
// std::condition_variable g_cv;
// std::queue<int> g_queue;

// // ┌──────────────────────────────┐
// // │ for i = 0 to 9               │
// // ├──────────────────────────────┤
// // │ 1. 加锁 mutex                │
// // │ 2. 向 g_queue 中 push(i)     │
// // │ 3. 打印 "produced i"         │
// // │ 4. 解锁 mutex                │
// // │ 5. 通知一个等待线程（cv）   │
// // │ 6. 线程 sleep 100 毫秒       │
// // └──────────────────────────────┘
// void Producer()
// {
//     for (int i = 0; i < 10; ++i)
//     {
//         std::unique_lock<std::mutex> lock(g_mutex);
//         g_queue.push(i);
//         std::cout << "Produced: " << i << std::endl;
//     }
//     g_cv.notify_one();                                           // Notify one waiting thread
//     std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Allow time for consumer to process
// }

// // ┌─────────────────────────────────────────────────────┐
// // │ while (true) 无限循环                               │
// // ├─────────────────────────────────────────────────────┤
// // │ 1. 加锁 mutex                                       │
// // │ 2. 条件等待：                                       │
// // │    - 如果 g_queue 为空，则等待（自动解锁 mutex）    │
// // │    - 被 notify 后重新加锁                           │
// // │    - 如果队列非空，继续                             │
// // ├─────────────────────────────────────────────────────┤
// // │ 3. 读取 g_queue.front()                             │
// // │ 4. 执行 g_queue.pop()                               │
// // │ 5. 打印 "consumed value"                            │
// // │ 6. 解锁 mutex                                       │
// // │ 7. 继续下一轮                                       │
// // └─────────────────────────────────────────────────────┘
// void Consumer()
// {
//     while (true)
//     {
//         std::unique_lock<std::mutex> lock(g_mutex);

//         // g_cv.wait(lock, [] { return !g_queue.empty(); });
//         // 这是 std::condition_variable::wait 的 重载版本
//         // cv.wait(lock, pred) 会自动解锁互斥量并挂起线程，直到 pred 为 true；
//         g_cv.wait(lock, []
//                   { return !g_queue.empty(); }); // Wait until queue is not empty
//         int value = g_queue.front();
//         g_queue.pop();
//         std::cout << "Consumed: " << value << std::endl;
//     }
// }

// // ┌─────────────────────────────┐
// // │ 启动 producer_thread        │
// // │ （执行 Producer 函数）      │
// // ├─────────────────────────────┤
// // │ 启动 consumer_thread        │
// // │ （执行 Consumer 函数）      │
// // ├─────────────────────────────┤
// // │ 等待 producer_thread 结束   │
// // │（join）                     │
// // ├─────────────────────────────┤
// // │ 等待 consumer_thread 结束   │
// // │（join，实际上阻塞无限）     │
// // └─────────────────────────────┘
// int main()
// {
//     std::thread producer_thread(Producer);
//     std::thread consumer_thread(Consumer);

//     producer_thread.join();
//     consumer_thread.join();

//     return 0;
// }