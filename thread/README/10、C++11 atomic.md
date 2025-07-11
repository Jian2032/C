# 10、C++11 std::atomic

---

## 引言

`std::atomic` 是 C++11 标准库中的一个模板类，用于实现多线程环境下的原子操作。它提供了一种线程安全的方式来访问和修改共享变量，可以避免多线程环境中的数据竞争问题。

`std::atomic` 的使用方式类似于普通的 C++ 变量，但是它的操作是原子性的。也就是说，在多线程环境下，多个线程同时对同一个 `std::atomic` 变量进行操作时，不会出现数据竞争问题。

---

## 常用的 std::atomic 操作

- **load()**：将 `std::atomic` 变量的值加载到当前线程的本地缓存中，并返回这个值。

- **store(val)**：将 `val` 的值存储到 `std::atomic` 变量中，并保证这个操作是原子性的。

- **exchange(val)**：将 `val` 的值存储到 `std::atomic` 变量中，并返回原先的值。

- **compare_exchange_weak(expected, val)** 和 **compare_exchange_strong(expected, val)**：
  
  比较 `std::atomic` 变量的值和 `expected` 的值是否相同，如果相同，则将 `val` 的值存储到 `std::atomic` 变量中，并返回 `true`；否则，将 `std::atomic` 变量的值存储到 `expected` 中，并返回 `false`。

---

## 示例代码

```cpp
#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int> count = 0;

void increment() {
    for (int i = 0; i < 1000000; ++i) {
        count++;
    }
}

int main() {
    std::thread t1(increment);
    std::thread t2(increment);
    t1.join();
    t2.join();
    std::cout << count << std::endl; // 预期输出 2000000
    return 0;
}
