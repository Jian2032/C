# 7、C++11 `condition_variable` 使用场景

---

## `std::condition_variable` 简介

`std::condition_variable` 是 C++11 提供的一种线程同步机制，常用于实现线程间的**等待与通知**，例如生产者-消费者模型。

它允许一个或多个线程等待某个条件成立，并在满足条件时被**另一个线程通知唤醒**。

---

## 使用步骤

1. 创建一个 `std::condition_variable` 对象；
2. 创建一个 `std::mutex` 互斥量，保护共享数据；
3. 在 **等待线程中**（如消费者）：
   - 使用 `std::unique_lock<std::mutex>` 加锁；
   - 使用 `.wait()`、`.wait_for()`、`.wait_until()` 等等待条件；
4. 在 **通知线程中**（如生产者）：
   - 修改共享数据；
   - 调用 `notify_one()` 或 `notify_all()` 通知等待线程。

---

## 示例：生产者 - 消费者模型

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

std::mutex g_mutex;
std::condition_variable g_cv;
std::queue<int> g_queue;

void Producer() {
    for (int i = 0; i < 10; i++) {
        {
            std::unique_lock<std::mutex> lock(g_mutex);
            g_queue.push(i);
            std::cout << "Producer: produced " << i << std::endl;
        }
        g_cv.notify_one(); // 通知一个等待线程
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Consumer() {
    while (true) {
        std::unique_lock<std::mutex> lock(g_mutex);
        g_cv.wait(lock, []() { return !g_queue.empty(); }); // 条件满足才继续
        int value = g_queue.front();
        g_queue.pop();
        std::cout << "Consumer: consumed " << value << std::endl;
    }
}

int main() {
    std::thread producer_thread(Producer);
    std::thread consumer_thread(Consumer);

    producer_thread.join();
    consumer_thread.join();

    return 0;
}
```

---

## 说明与关键点

- `std::unique_lock<std::mutex>` 用于和 `condition_variable` 协作；
- `cv.wait(lock, pred)` 会自动解锁互斥量并挂起线程，直到 `pred` 为 true；
- `notify_one()` 唤醒一个等待的线程，`notify_all()` 唤醒所有；
- **避免虚假唤醒**：`wait()` 必须配合条件判断使用；
- 消费者线程中使用 `while(true)`，注意实际开发中需加入退出机制；

---

## 小结

- `std::condition_variable` 适合用于线程间等待和通知；
- 在生产者-消费者模型中可防止资源抢占与资源浪费；
- 与 `std::mutex` 搭配使用，保护共享资源；
- 通过 `wait + notify`，实现线程之间高效而安全的通信。

> ✅ 在多线程程序中，`condition_variable` 是构建高效同步机制的重要工具。
