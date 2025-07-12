// // 5、C++11 lock_guard 与 std::unique_lock

// #include <iostream>
// #include <thread>
// #include <mutex>
// #include <condition_variable>

// std::mutex mtx;
// std::condition_variable cv;
// bool ready = false;

// // lock()	加锁，若互斥量已被其他线程占用，则阻塞
// // try_lock()	尝试加锁，成功返回 true，失败立即返回 false
// // try_lock_for(duration)	尝试加锁指定时间，成功返回 true，超时返回 false
// // try_lock_until(time_point)	尝试加锁直到指定时间点
// // unlock()	解锁互斥量
// // std::unique_lock<std::mutex> lock;                          // 默认构造，不关联任何互斥量
// // std::unique_lock<std::mutex> lock(mtx);                     // 自动加锁
// // std::unique_lock<std::mutex> lock(mtx, std::defer_lock);    // 延迟加锁
// // std::unique_lock<std::mutex> lock(mtx, std::try_to_lock);   // 尝试加锁
// // std::unique_lock<std::mutex> lock(mtx, std::adopt_lock);    // 接管已加锁的互斥量

// void worker() {
//     // std::unique_lock	C++11 引入的模板类，用于管理互斥锁（mutex）的上锁和解锁
//     // <std::mutex>	模板参数，说明 lock 要管理的互斥锁类型是 std::mutex
//     // lock	声明的变量名，即这个锁对象的名称
//     // (mtx)	构造函数参数，表示对 mtx 互斥量进行加锁
//     std::unique_lock<std::mutex> lock(mtx); // 延迟锁可以配合条件变量

//     // cv	一个 std::condition_variable 条件变量对象，用于线程间的等待/通知
//     // wait(...)	阻塞当前线程，直到满足某个条件或被唤醒
//     // lock	一个 std::unique_lock<std::mutex> 对象，必须已持有互斥锁
//     // [] { return ready; }	一个lambda 表达式，用于判断等待条件是否满足（谓词）
//     // ready	通常是某个 bool 类型的共享状态变量，用于线程通信判断条件是否满足

//     // 当 ready == true 或 cv.notify_one() 被调用，重新加锁并检查条件。
//     cv.wait(lock, [] { return ready; });    // 自动解锁再加锁
//     std::cout << "Worker thread proceeds\n";
// }

// // Lambda 表达式语法结构：
// // [capture](parameter_list) -> return_type {
// //     function_body
// // }

// // []	捕获列表，用于访问外部变量
// // (…)	参数列表，像普通函数一样定义参数
// // -> T	返回类型（可省略，编译器自动推导）
// // {}	函数体

// // 简单示例：
// // auto add = [](int a, int b) {
// //     return a + b;
// // };

// // int x = 10;

// // auto printX = [x]() {
// //     std::cout << "x = " << x << std::endl;
// // };
// // printX();

// // 捕获方式详解：
// // [x]	捕获变量 x 的副本（值捕获）
// // [&x]	捕获变量 x 的引用（可修改外部变量）
// // [=]	值捕获所有用到的外部变量
// // [&]	引用捕获所有用到的外部变量
// // [=, &y]	默认值捕获，y 例外引用捕获
// // [this]	捕获当前类的 this 指针（类成员访问）

// // int count = 0;

// // auto inc = [&]() {
// //     count++;
// // };

// // inc();
// // std::cout << count << std::endl;  // 输出 1




// int main() {
//     std::thread t(worker);

//     // sleep_for(...)
//     // 这是 this_thread 里的一个函数，作用是：
//     // 让当前线程暂停（sleep）指定的时间段。
//     // void sleep_for(const std::chrono::duration<...>& rel_time);
//     // 参数是一个 std::chrono::duration 类型，表示“相对时间段”。

//     // std::chrono::milliseconds(500)	500 毫秒
//     // std::chrono::microseconds(1000)	1000 微秒
//     // std::chrono::nanoseconds(1)	1 纳秒
//     // std::chrono::minutes(1)	1 分钟

//     std::this_thread::sleep_for(std::chrono::seconds(1));
//     {
//         std::lock_guard<std::mutex> lock(mtx);
//         ready = true;
//     }
//     cv.notify_one();
//     t.join();
//     return 0;
// }


// // 主线程              子线程(worker)
// // --------            ---------------
// // 启动                加锁
// //                    检查 ready = false
// //                    --> wait(lock) --> 解锁 & 休眠
// // 睡眠 1 秒
// // 加锁 + ready = true
// // notify_one() ---> 唤醒子线程
// //                    重新加锁
// //                    输出 “proceeds”
// // 主线程 join 等待    子线程结束
