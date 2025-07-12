// // 1、C++11 Thread线程库的基本使用

// #include <iostream>
// #include <thread>

// // std::string：标准字符串类，来自 <string> 头文件。
// // &：表示引用传参，不拷贝字符串，提高效率。
// // const：表示函数内不会修改这个字符串内容。
// // 相比传值（std::string message），传引用可以避免 复制大对象开销。加 const 则提高 安全性。
// void printMessage(const std::string &message)
// {
//     // std::cout
//     // 全称是 std::character output，来自 <iostream> 头文件。
//     // 表示 标准输出流，通常输出到控制台（终端、命令行窗口）。
//     // std:: 是命名空间限定符，表示 cout 来自 std 命名空间。
//     // <<
//     // 输出运算符，也叫“插入运算符”。
//     // 它的作用是将右边的内容输出到 std::cout 中。
//     // 可以连续使用多个 << 来依次输出多个变量或字符串。
//     // std::endl
//     // 表示“输出一个换行符，并刷新输出缓冲区”。
//     // 等价于 '\n'（换行）+ std::flush（刷新缓冲区）。
//     // 每次使用 std::endl 都会刷新输出流，可能稍慢。如果你只想换行可以用 '\n' 替代：
//     std::cout << message << std::endl;
// }

// // void f(int value)	值传递	❌	拷贝一份变量，函数内部修改不影响外部
// // void f(int &value)	引用传递	✅	直接访问原变量，允许修改
// // void f(const int &v)	引用传递	❌	可访问但不能修改，常用于避免拷贝开销
// void increment(int &value)
// {
//     ++value;
// }

// void printNumber(int &number)
// {
//     for (int i = 0; i < number; ++i)
//     {
//         std::cout << "Number: " << i << std::endl;
//         ++number;
//     }
// }

// void foo()
// {
//     std::cout << "This function does nothing." << std::endl;
// }

// int main()
// {
//     std::string message = "Hello from the thread!";
//     // thread_1	新线程对象的名称
//     // printMessage	线程函数（线程执行的入口）
//     // message	传递给线程函数的参数
//     std::thread thread_1(printMessage, message);

//     int value = 0;
//     // std::ref(value) 的作用是将一个值包装为引用类型，主要用于传引用给接受按值的函数
//     // std::ref() 接收的是 T&，也就是左值引用。
//     // 所以 std::ref(x) 中，x 必须是左值。
//     std::thread thread_2(increment, std::ref(value));
//     std::cout << "Value after increment: " << value << std::endl;

//     int number = 500;
//     std::thread thread_3(printNumber, std::ref(number));

//     std::thread thread_4(foo);

//     // .join()	等待线程执行完毕，主线程再继续	✅ 是
//     // .detach()	分离线程，后台运行，主线程不再等待
//     thread_1.join();
//     thread_2.join();
//     std::cout << "All threads joined" << std::endl;
//     thread_3.detach();
//     std::cout << "Thread detached" << std::endl;

//     // 判断 thread_4 是否关联了一个活动线程且尚未被 join 或 detach。
//     // 如果没有启动线程或已经被 join/detach，那么它就是不可 joinable 的，调用 join() 会抛出异常。
//     // thread_4.join()：
//     // 阻塞当前主线程，直到 thread_4 线程运行完成。
//     // 用于线程同步，避免主线程提前结束或资源未正确释放。
//     thread_4.joinable();
//     if (thread_4.joinable())
//     {
//         thread_4.join();
//     }
//     std::cout << "Thread joined" << std::endl;

//     return 0;
// }
