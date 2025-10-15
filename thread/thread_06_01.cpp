// // 6、C++11 std::call_once 与其使用场景

// #include <iostream>
// #include <thread>
// #include <mutex>

// std::once_flag init_flag;

// void init()
// {
//     std::cout << "Initialized only once\n";
// }

// void worker()
// {
//     // init() 被多个线程各自调用，运行了 3 次。如果 init() 中做了 资源申请、配置文件加载、全局变量写入等敏感操作，就可能导致：
//     // 重复初始化（性能开销）
//     // 线程竞态（数据错乱、程序崩溃）
//     // init(); // 直接调用 init 函数


//     // std::call_once(init_flag, init); // 确保 init() 只被调用一次
//     std::cout << "Worker thread doing other things\n";
// }

// int main()
// {
//     std::thread t1(worker);
//     std::thread t2(worker);
//     std::thread t3(worker);

//     t1.join();
//     t2.join();
//     t3.join();

//     return 0;
// }
