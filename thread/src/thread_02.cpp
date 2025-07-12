// // 2、C++11 线程函数中的数据未定义错误

// #include <iostream>
// #include <thread>
// #include <functional>
// #include <memory>

// void foo_1(int &x)
// {
//     x += 1;
// }

// void foo_2(std::shared_ptr<int> ptr)
// {
//     std::cout << *ptr << std::endl;
// }

// class MyClass
// {
// private:
//     // 友元函数声明
//     // friend
//     // 这是关键字，表示后面的函数（或类）是当前类的友元。友元拥有访问该类私有（private）和保护（protected）成员的权限。
//     // void myThreadFunc(MyClass* obj);
//     // 声明了一个函数 myThreadFunc，它返回类型是 void，参数是指向 MyClass 类型对象的指针。
//     friend void myThreadFunc(MyClass *obj);
//     void privateFunc()
//     {

//         // std::this_thread::get_id() 可以用来：
//         // 获取当前线程的 ID，用于线程标识和调试。
//         // 比较线程是否相同。
//         // 记录日志时标明是哪个线程执行的。
//         std::cout << "Thread " << std::this_thread::get_id() << " privateFunc" << std::endl;
//     }
// };

// void myThreadFunc(MyClass *obj)
// {
//     obj->privateFunc();
// }

// int main()
// {
//     int x = 0;

//     // std::thread t1(foo, 1);   // 报错
//     // foo 函数接受一个整数引用作为参数，并对其加 1。但在线程创建时，传入的 1 是一个临时变量
//     // 在 std::thread 解析参数时，该临时变量会被销毁，导致 foo 访问了已销毁的对象，产生未定义行为。
//     std::thread t1(foo_1, std::ref(x));

//     // 局部变量 x 在 main 函数作用域结束后被销毁，但 std::thread 可能尚未运行或访问了无效的地址，导致未定义行为。
//     // int y = 0;
//     // std::thread t2(foo_2, &y);

//     auto ptr = std::make_shared<int>(1);
//     std::thread t2(foo_2, ptr);

//     MyClass obj;
//     std::thread t3(myThreadFunc, &obj);

//     t1.join(); // 等待线程 t1 执行完毕
//     t2.join(); // 等待线程 t2 执行完毕
//     t3.join(); // 等待线程 t3 执行完毕

//     return 0;
// }