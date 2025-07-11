# 6、C++11 `std::call_once` 与其使用场景

---

## 单例模式的线程安全问题

**单例模式（Singleton Pattern）** 是一种常用的设计模式，用于确保某个类在程序中只存在一个实例，并提供全局访问点。

在多线程环境下，简单的单例实现可能会**造成多个实例被构造**，从而**破坏唯一性和线程安全性**。

### 示例：非线程安全的单例实现

```cpp
class Singleton {
public:
    static Singleton& getInstance() {
        static Singleton instance; // 非线程安全（C++11 前）
        return instance;
    }

    void setData(int data) { m_data = data; }
    int getData() const { return m_data; }

private:
    Singleton() {}
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    int m_data = 0;
};
```

在某些平台或旧编译器中，局部静态变量可能在多线程同时访问时**构造多次**，因此不能保证线程安全。

---

## 使用 `std::call_once` 实现线程安全的单例

`std::call_once` 可确保某个初始化函数只在**多个线程中被调用一次**，非常适合实现线程安全的单例模式。

### 示例：使用 `std::call_once` 实现线程安全单例

```cpp
#include <iostream>
#include <memory>
#include <mutex>

class Singleton {
public:
    static Singleton& getInstance() {
        std::call_once(m_onceFlag, &Singleton::init);
        return *m_instance;
    }

    void setData(int data) { m_data = data; }
    int getData() const { return m_data; }

private:
    Singleton() {}
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static void init() {
        m_instance.reset(new Singleton);
    }

    static std::unique_ptr<Singleton> m_instance;
    static std::once_flag m_onceFlag;

    int m_data = 0;
};

std::unique_ptr<Singleton> Singleton::m_instance;
std::once_flag Singleton::m_onceFlag;
```

### 说明：

- `std::once_flag`：标记是否已经初始化。
- `std::call_once(flag, func)`：确保多个线程中只有一个执行 `func`。
- `std::unique_ptr`：保证对象生命周期自动释放，避免内存泄漏。

---

## `std::call_once` 函数原型与说明

```cpp
template <class Callable, class... Args>
void std::call_once(std::once_flag& flag, Callable&& func, Args&&... args);
```

### 参数说明：

- `flag`：必须为 `std::once_flag` 类型，并在多次调用中为同一对象。
- `func`：需要被调用的函数或可调用对象。
- `args...`：传给 `func` 的参数（可选）。

### 行为：

- **只会有一个线程成功执行 `func`**；
- 其他线程会被阻塞，直到 `func` 执行完毕；
- 如果 `func` 抛出异常，`call_once` 会再次尝试执行；
- 如果成功返回，说明 `func` 已成功执行一次。

---

## 使用注意事项

- `flag` 参数必须为全局或静态的 `std::once_flag` 实例；
- `func` 必须是幂等的（可重复调用且无副作用）；
- 如果 `func` 抛出异常，调用者应做好异常处理；
- 避免在 `func` 中写过多逻辑，保持初始化的**简洁性和稳定性**。

---

## 总结

- `std::call_once` 是 C++11 提供的**线程安全的一次性初始化工具**；
- 特别适用于单例模式、懒加载初始化等场景；
- 它比手动加锁更简单、安全，避免了双重检查锁等繁琐写法；
- 搭配 `std::once_flag` 使用，线程安全、轻量高效。

> 建议在需要线程安全的一次性初始化场景中优先使用 `std::call_once`，尤其适用于跨线程全局对象或单例的构造。

