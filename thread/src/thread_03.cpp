// 3、C++11 互斥量解决多线程数据共享问题

// #include <iostream>
// #include <thread>
// #include <mutex>
// #include <chrono>  // 添加用于时间统计

// int shared_data = 0;
// std::mutex mtx;

// void func(int n) {
//     for (int i = 0; i < 100000; ++i) {
//         // 使用 std::lock_guard 自动管理互斥量的锁定和解锁
//         // std::lock_guard<std::mutex> lock(mtx);
        
//         mtx.lock();  // 锁定互斥量，保护共享数据
//         shared_data++;
//         mtx.unlock();  // 解锁互斥量
        
//         // 输出会严重拖慢线程执行速度，真实性能测试时可注释
//         // std::cout << "Thread " << n << " increment shared_data to " << shared_data << std::endl;
//     }
// }

// int main() {
//     auto start = std::chrono::high_resolution_clock::now();  // 开始计时

//     std::thread t1(func, 1);
//     std::thread t2(func, 2);

//     t1.join();
//     t2.join();

//     auto end = std::chrono::high_resolution_clock::now();    // 结束计时
//     std::chrono::duration<double> elapsed = end - start;

//     std::cout << "Final shared_data = " << shared_data << std::endl;
//     std::cout << "Execution time: " << elapsed.count() << " seconds" << std::endl;

//     return 0;
// }
