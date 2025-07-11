# 9、异步并发——async、future、packaged_task、promise

---

## 1. async 与 future

`std::async` 和 `std::future` 是 C++11 引入的功能，允许我们异步执行一个函数，并返回一个 `std::future` 对象，表示异步操作的结果。使用 `std::async` 可以方便地进行异步编程，避免手动创建线程和管理线程的麻烦。

### 示例代码

```cpp
#include <iostream>
#include <future>
#include <thread>
#include <chrono>

int calculate() {
    // 模拟一个耗时的计算
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 42;
}

int main() {
    std::future<int> future_result = std::async(std::launch::async, calculate);
    // 在这里可以做其他的事情
    int result = future_result.get(); // 获取异步操作的结果
    std::cout << result << std::endl; // 输出42
    return 0;
}
