# 5、C++11 `lock_guard` 与 `std::unique_lock`

---

## std::lock_guard

`std::lock_guard` 是 C++11 标准库中提供的轻量级互斥量封装类，用于保护共享资源，防止数据竞争。

### 特点

- 当构造函数被调用时，互斥量自动加锁。
- 当析构函数被调用时，互斥量自动解锁（**RAII 原则**）。
- `std::lock_guard` 不支持复制和移动，只能在**局部作用域中使用**。
- 适用于简单、作用域内的互斥操作。

### 示例代码

```cpp
#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx;

void safe_print(const std::string& msg) {
    std::lock_guard<std::mutex> lock(mtx); // 构造时加锁，析构时自动释放
    std::cout << msg << std::endl;
}

int main() {
    std::thread t1(safe_print, "Hello from thread 1");
    std::thread t2(safe_print, "Hello from thread 2");
    t1.join();
    t2.join();
    return 0;
}
```

---

## std::unique_lock

`std::unique_lock` 是一个功能更强大、使用更灵活的互斥量封装类，适用于需要**手动控制加锁与解锁、条件变量、延迟加锁**等场景。

### 特点

- 支持手动加锁、延迟加锁、超时加锁等操作。
- 可与 `std::condition_variable` 配合使用。
- 支持移动构造和移动赋值，可转移锁的所有权。
- 占用资源比 `lock_guard` 略大，但提供更多功能。

---

### 常用成员函数

| 成员函数 | 说明 |
|----------|------|
| `lock()` | 加锁，若互斥量已被其他线程占用，则阻塞 |
| `try_lock()` | 尝试加锁，成功返回 true，失败立即返回 false |
| `try_lock_for(duration)` | 尝试加锁指定时间，成功返回 true，超时返回 false |
| `try_lock_until(time_point)` | 尝试加锁直到指定时间点 |
| `unlock()` | 解锁互斥量 |

---

### 常用构造函数

```cpp
std::unique_lock<std::mutex> lock;                          // 默认构造，不关联任何互斥量
std::unique_lock<std::mutex> lock(mtx);                     // 自动加锁
std::unique_lock<std::mutex> lock(mtx, std::defer_lock);    // 延迟加锁
std::unique_lock<std::mutex> lock(mtx, std::try_to_lock);   // 尝试加锁
std::unique_lock<std::mutex> lock(mtx, std::adopt_lock);    // 接管已加锁的互斥量
```

---

### 示例代码

#### 延迟加锁与条件变量使用

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void worker() {
    std::unique_lock<std::mutex> lock(mtx); // 延迟锁可以配合条件变量
    cv.wait(lock, [] { return ready; });    // 自动解锁再加锁
    std::cout << "Worker thread proceeds\n";
}

int main() {
    std::thread t(worker);

    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
        std::lock_guard<std::mutex> lock(mtx);
        ready = true;
    }
    cv.notify_one();
    t.join();
    return 0;
}
```

---

## lock_guard vs unique_lock 对比

| 对比项 | `lock_guard` | `unique_lock` |
|--------|--------------|----------------|
| 加锁时机 | 构造时立即加锁 | 可延迟加锁、尝试加锁 |
| 是否可移动 | ❌ 不可复制/移动 | ✅ 可移动 |
| 适用范围 | 简单场景，RAII | 条件变量、复杂控制 |
| 成本 | 较低（轻量） | 略高（功能强） |
| 支持条件变量 | ❌ 不支持 | ✅ 支持 |

---

## 总结

- 如果只是需要简单的**作用域加锁/解锁**，推荐使用 `std::lock_guard`。
- 如果需要更灵活的锁管理，例如：**延迟加锁、超时等待、与条件变量配合**等，请使用 `std::unique_lock`。

> 两者都能有效防止数据竞争，关键在于使用场景的不同。
