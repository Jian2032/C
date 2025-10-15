// // 9、异步并发——async、future、packaged_task、promise

// #include <iostream>
// #include <future>
// #include <thread>
// #include <chrono>

// int calculate()
// {
//     // 模拟一个耗时的计算
//     std::this_thread::sleep_for(std::chrono::seconds(1));
//     return 42;
// }

// int main()
// {
//     // std::async：C++11 标准库函数模板，用于异步调用一个可调用对象（函数、lambda等）。它会启动一个任务，执行传入的函数。
//     // 参数
//     // std::launch::async
//     // 这是一个启动策略，表示async必须在新线程中异步执行任务（不会同步调用）。
//     // calculate
//     // 这是要执行的函数（无参数）。
//     std::future<int> future_result = std::async(std::launch::async, calculate);
//     // 在这里可以做其他的事情
//     int result = future_result.get(); // 获取异步操作的结果
//     std::cout << result << std::endl; // 输出42
//     return 0;
// }