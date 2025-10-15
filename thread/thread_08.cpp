// // 8、C++11 跨平台线程池

// #include <iostream>
// #include <thread>
// #include <mutex>
// #include <condition_variable>
// #include <queue>
// #include <vector>
// #include <functional>
// // std::function	通用的函数包装器，可以存储函数、lambda、函数指针、成员函数等
// // std::bind	用于绑定参数，创建新的可调用对象
// // std::ref / std::cref	用于将变量包装为引用，用于避免拷贝
// // 算术函数对象	如 std::plus<>, std::minus<>, std::multiplies<>, std::divides<> 等
// // 关系函数对象	如 std::less<>, std::greater<>, std::equal_to<> 等
// // 逻辑函数对象	如 std::logical_and<>, std::logical_or<>, std::logical_not<>

// class ThreadPool
// {
// public:
//     // ThreadPool(int numThreads)：构造函数，接受线程数量作为参数。
//     // : stop(false)：初始化成员变量 stop 为 false（表示线程池还没关闭）。
//     ThreadPool(int numThreads) : stop(false)
//     {
//         for (int i = 0; i < numThreads; i++)
//         {
//             // threads 是一个 std::vector<std::thread>，用于存储所有线程对象。
//             // emplace_back 创建一个新线程，运行的是 lambda 函数。
//             // [this] 是 lambda 捕获列表，表示捕获当前对象指针 this，以便访问成员变量（如 mutex, tasks 等）。

//             // threads	线程对象容器（std::vector<std::thread>）
//             // emplace_back()	在容器中直接构造一个新 std::thread 对象
//             // [this] { ... }	线程执行函数，捕获了 this 访问成员变量

//             // [this] { ... }
//             // 这是一个无参数的 lambda 表达式，其中：
//             // [this]：捕获当前类对象指针 this，使得 lambda 可以访问当前类的成员变量（如 mutex, condition, stop, tasks）。
//             // { ... }：lambda 的函数体，定义了线程执行的逻辑。
//             // 没有参数和返回类型（会自动推断为 void()）。
//             threads.emplace_back([this]
//                                  {
//                 while(true){
//                     std::unique_lock<std::mutex> lock(mutex);
//                     condition.wait(lock,[this]{return stop || !tasks.empty();});
//                     if(stop && tasks.empty()){
//                         return;
//                     }
//                     // 🔹 std::function<void()>
//                     // 这是一个通用可调用对象包装器。
//                     // 它可以存储任何可调用对象（函数指针、lambda 表达式、std::bind 返回值、函数对象等）。
//                     // 这里指定的类型是：无参数、无返回值的函数（即 void()）。
//                     // 🔹 task(...)
//                     // 定义一个局部变量 task，类型为 std::function<void()>。
//                     // 用于存储当前将要执行的任务。
//                     // 🔹 std::move(...)
//                     // std::move 是一个类型转换，将 lvalue 转为 rvalue reference。
//                     // 意思是：将 tasks.front() 里的内容 “移动” 给 task，而不是复制。
//                     // 避免开销大的复制，提升性能，尤其适用于 lambda 或大对象。
//                     // 🔹 tasks.front()
//                     // tasks 是一个队   列（std::queue<std::function<void()>>）。
//                     // .front() 获取队头的任务，但不移除它。
//                     // 慢，复制：
//                     // std::function<void()> task = tasks.front();
//                     // // 快，移动（避免复制）：
//                     // std::function<void()> task(std::move(tasks.front()));
//                     std::function<void()> task(std::move(tasks.front()));
//                     tasks.pop();
//                     lock.unlock();
//                     task();
//                 } });
//         }
//     }

//     ~ThreadPool()
//     {
//         {
//             std::unique_lock<std::mutex> lock(mutex);
//             stop = true;
//         }
//         condition.notify_all();
//         // threads
//         // 是一个 std::vector<std::thread> 类型的容器，存放多个线程对象。
//         // std::thread &thread
//         // 循环变量，表示对 threads 中每个元素的 引用。
//         // 用引用避免复制线程对象（线程对象不可复制，只能移动，且复制无意义），同时对容器里的元素进行操作。
//         // C++11 新引入的范围基 for 语法，用于遍历容器或数组。
//         // 循环体
//         // 里面对每个线程对象进行操作，比如调用 thread.join() 等。
//         for (std::thread &thread : threads)
//         {
//             thread.join();
//         }
//     }

//     // template <typename F, typename... Args>
//     // 模板定义，接受一个函数类型 F，以及任意数量和类型的参数 Args...
//     template <typename F, typename... Args>
//     // void enqueue(F &&f, Args &&...args)
//     // 函数模板，f 是函数或可调用对象的右值或左值引用，args... 是函数参数的参数包，同样支持完美转发。
//     void enqueue(F &&f, Args &&...args)
//     {
//         // std::function<void()> task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
//         // std::bind：将传入的函数 f 和参数 args... 绑定，生成一个无参数的可调用对象。
//         // std::forward 保持传入参数的值类别（左值或右值），实现完美转发。
//         // 将 bind 返回的结果赋值给 std::function<void()>，统一任务的调用接口。
//         std::function<void()> task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
//         {
//             std::unique_lock<std::mutex> lock(mutex);
//             tasks.emplace(std::move(task));
//         }
//         condition.notify_one();
//     }

// private:
//     std::vector<std::thread> threads;
//     std::queue<std::function<void()>> tasks;
//     std::mutex mutex;
//     std::condition_variable condition;
//     bool stop = false;
// };

// int main()
// {
//     ThreadPool pool(4); // 创建一个包含4个线程的线程池
//     for (int i = 0; i < 8; i++)
//     {
//         pool.enqueue([i]
//                      {
//                          std::cout << "Task " << i << " is running in thread " << std::this_thread::get_id() << std::endl;
//                          std::this_thread::sleep_for(std::chrono::seconds(1)); // 模拟任务执行时间
//                      });
//     }
//     return 0;
// }

// // main()
// // │
// // ├── 创建 ThreadPool(4)
// // │   ├── 初始化 stop = false
// // │   ├── 启动 4 个线程（threads[0] ~ threads[3]）
// // │   │   └── 每个线程进入 while(true) 循环：
// // │   │       ├── 加锁 mutex
// // │   │       ├── 等待 condition 条件满足（有任务 or stop）
// // │   │       └── 如果满足条件：
// // │   │           ├── 从队列取出任务
// // │   │           ├── 解锁
// // │   │           └── 执行任务（task()）
// // │   │
// // ├── 向线程池添加 8 个任务（enqueue 0~7）
// // │   └── 对每个任务：
// // │       ├── 封装为 std::function<void()>，绑定参数
// // │       ├── 加锁 mutex
// // │       ├── 放入任务队列 tasks
// // │       ├── 解锁
// // │       └── 唤醒一个等待中的线程（condition.notify_one()）
// // │
// // ├── 工作线程收到通知后：
// // │   ├── 醒来，取出任务
// // │   ├── 解锁 mutex
// // │   └── 执行任务（打印 Task i 开始，sleep 1s，打印结束）
// // │   （最多同时运行 4 个任务）
// // │
// // └── main() 结束 → ThreadPool 析构
// //     ├── 加锁 mutex，设置 stop = true
// //     ├── 解锁
// //     ├── condition.notify_all() → 唤醒所有线程
// //     └── 所有线程返回 while，发现 stop = true 且队列空 → 退出线程
// //         └── 主线程 join 所有线程
