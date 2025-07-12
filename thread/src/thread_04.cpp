// // 4、C++11 互斥量死锁

// #include <iostream>
// #include <thread>
// #include <mutex>

// std::mutex mtx1, mtx2;

// void taskA() {
//     // std::lock(mtx1, mtx2);
//     // 是 C++ 11 中用于 **一次性加锁多个互斥量（std::mutex） **的函数，作用是：
//     // ✅ 同时对多个互斥量加锁，并避免死锁问题。
//     std::lock(mtx1, mtx2);  // 一次性锁两个互斥量，避免死锁

//     // std::lock_guard<std::mutex>	C++ RAII（资源管理）类，用于自动加锁和解锁 mutex
//     // lock1	创建的锁管理对象（构造时加锁，析构时解锁）
//     // mtx1	要管理的互斥锁
//     // std::adopt_lock	表示 mtx1 已经被锁了，lock_guard 不再调用 .lock()，只负责 .unlock()
//     std::lock_guard<std::mutex> lock1(mtx1, std::adopt_lock);
//     std::lock_guard<std::mutex> lock2(mtx2, std::adopt_lock);

//     std::cout << "Task A acquired both locks\n";
//     // 处理共享资源...
// }

// void taskB() {
//     std::lock(mtx2, mtx1);  // 即使顺序不一致也不会死锁
//     std::lock_guard<std::mutex> lock1(mtx2, std::adopt_lock);
//     std::lock_guard<std::mutex> lock2(mtx1, std::adopt_lock);

//     std::cout << "Task B acquired both locks\n";
// }

// int main()
// {
//     std::thread t1(taskA);
//     std::thread t2(taskB);
//     t1.join();
//     t2.join();
//     return 0;
// }