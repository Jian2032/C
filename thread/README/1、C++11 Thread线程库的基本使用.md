# 1、C++11 Thread线程库的基本使用

本文详细介绍 C++11 Thread 线程库的基本使用，包括如何创建线程、启动线程、等待线程完成以及如何分离线程等。

---

## 创建线程

要创建线程，我们需要一个可调用的函数或函数对象，作为线程的入口点。在 C++11 中，我们可以使用函数指针、函数对象或 lambda 表达式来实现。

创建线程的基本语法如下：

```cpp
#include <thread>
std::thread t(function_name, args...);
```

- `function_name` 是线程入口点的函数或可调用对象  
- `args...` 是传递给函数的参数  

创建线程后，我们可以使用 `t.join()` 等待线程完成，或者使用 `t.detach()` 分离线程，让它在后台运行。

例如，下面的代码创建了一个线程，输出一条消息：

```cpp
#include <iostream>
#include <thread>

void print_message() {
    std::cout << "Hello, world!" << std::endl;
}

int main() {
    std::thread t(print_message);
    t.join();
    return 0;
}
```

在这个例子中，我们定义了一个名为 `print_message` 的函数，它输出一条消息。然后，我们创建了一个名为 `t` 的线程，将 `print_message` 函数作为入口点。最后，我们使用 `t.join()` 等待线程完成。

---

## 传递参数

我们可以使用多种方式向线程传递参数，例如使用函数参数、全局变量、引用等。例如：

```cpp
#include <iostream>
#include <thread>

void print_message(const std::string& message) {
    std::cout << message << std::endl;
}

void increment(int& x) {
    ++x;
}

int main() {
    std::string message = "Hello, world!";
    std::thread t(print_message, message);
    t.join();

    int x = 0;
    std::thread t2(increment, std::ref(x));
    t2.join();
    std::cout << x << std::endl;

    return 0;
}
```

- 在第一个例子中，我们使用了一个字符串作为函数参数，传递给线程。
- 在第二个例子中，我们使用了一个引用来传递一个整数变量。**需要注意**的是，当我们使用引用传递参数时，我们需要使用 `std::ref` 来包装引用，否则编译器会报错。

---

## 等待线程完成

当我们创建一个线程后，可能需要等待它完成，以便获取线程的执行结果或执行清理操作。我们可以使用 `t.join()` 方法来等待线程完成。

```cpp
#include <iostream>
#include <thread>

void print_message(const std::string& message) {
    std::cout << message << std::endl;
}

int main() {
    std::thread t1(print_message, "Thread 1");
    std::thread t2(print_message, "Thread 2");
    t1.join();
    t2.join();
    std::cout << "All threads joined" << std::endl;
    return 0;
}
```

在这个例子中，我们创建了两个线程 `t1` 和 `t2`，它们都调用 `print_message` 函数输出一条消息。然后，使用 `t1.join()` 和 `t2.join()` 等待它们完成。最后，输出一条消息表示所有线程都已经完成。

---

## 分离线程

有时候我们可能不需要等待线程完成，而是希望它在后台运行。这时候可以使用 `t.detach()` 方法来分离线程。

```cpp
#include <iostream>
#include <thread>

void print_message(const std::string& message) {
    std::cout << message << std::endl;
}

int main() {
    std::thread t(print_message, "Thread 1");
    t.detach();
    std::cout << "Thread detached" << std::endl;
    return 0;
}
```

在这个例子中，我们创建了一个线程并分离它，让它在后台运行。**注意事项**：

- 一旦线程被分离，就不能再使用 `t.join()` 等待它完成；
- 确保线程不会在主线程结束前退出，否则可能会导致未定义行为。

---

## `joinable()`

`joinable()` 方法返回一个布尔值，如果线程可以被 `join()` 或 `detach()`，则返回 `true`，否则返回 `false`。如果试图对一个不可加入的线程调用 `join()` 或 `detach()`，会抛出 `std::system_error` 异常。

示例：

```cpp
#include <iostream>
#include <thread>

void foo() {
    std::cout << "Thread started" << std::endl;
}

int main() {
    std::thread t(foo);
    if (t.joinable()) {
        t.join();
    }
    std::cout << "Thread joined" << std::endl;
    return 0;
}
```

---

## 常见错误

在使用 C++11 线程库时，有一些常见的错误需要注意：

1. **忘记等待或分离线程**：创建线程后如果既没 join 也没 detach，主线程结束时可能会导致未定义行为。
2. **访问共享数据未同步**：多线程访问共享资源时应使用互斥锁等机制，否则容易导致数据竞争和死锁。
3. **异常未处理**：在线程中抛出异常但没有处理，可能导致程序崩溃。应使用 `try-catch` 捕获并妥善处理。

---

## 总结

C++11 提供了一个强大的线程库 —— `std::thread`。它支持创建和管理线程，为 C++ 多线程编程提供了现代化的解决方案。

本文介绍了 `std::thread` 库的基本用法，包括如何创建、启动、传参、管理线程，等待线程完成、分离线程等操作。同时也列举了使用过程中的常见错误及注意事项。
