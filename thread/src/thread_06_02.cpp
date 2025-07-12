// // 6、C++11 std::call_once 与其使用场景

// #include <iostream>
// #include <thread>
// #include <mutex>
// #include <memory>

// class Singleton
// {
// public:
//     // 这是一个静态成员函数，属于类 Singleton，不是对象。
//     // 返回类型是 Singleton& —— 返回单例对象的引用。
//     // 通过 Singleton::getInstance() 可以在程序任何地方调用这个函数来获取唯一实例。
//     // 那么 &getInstance 表示：
//     // 获取函数 getInstance 的地址，即：函数指针
//     // getInstance()	直接调用函数，返回实例引用
//     // &getInstance	获取函数的地址（函数指针）
//     // *(&getInstance)	解引用函数地址，再次得到函数本身
//     static Singleton &getInstance()
//     {
//         // std::call_once(flag, &Singleton::init);	✅	init 是 static 且无参
//         // std::call_once(flag, &Singleton::init, &obj);	✅	init 是非 static，需要对象
//         // std::call_once(flag, std::bind(&Singleton::init, &obj));	✅	支持绑定任意参数
//         // std::call_once(flag, init);	✅	init 是自由函数（非成员）
//         std::call_once(m_onceflag, &Singleton::init);
//         return *m_instance;
//     }

//     void setdata(int data) { m_data = data; }

//     // int：返回类型，表示该函数返回一个 int 类型的值。
//     // getData()：成员函数名称，无参数。
//     // const：常量成员函数修饰符，表示该函数不会修改类的成员变量（除非它们是 mutable 或是通过 const_cast 修改，这种一般不推荐）表示这个函数不会修改类的成员变量，可被 const 对象调用。
//     // { return m_data; }：函数体，返回类中的成员变量 m_data 的值。
//     int getdata() const { return m_data; }

// private:
//     Singleton() {}

//     // 这是删除拷贝构造函数的语法。
//     // 这条语句告诉编译器：不允许用另一个 Singleton 实例来构造本对象
//     // Singleton s1;
//     // Singleton s2 = s1;  // ❌ 报错：拷贝构造被禁用
//     Singleton(const Singleton &) = delete;

//     // 这是删除拷贝赋值运算符的语法。
//     // 这表示 不允许使用 = 运算符给 Singleton 赋值，例如：
//     // Singleton s1;
//     // Singleton s2;
//     // s2 = s1;  // ❌ 报错：赋值运算符被禁用
//     Singleton &operator=(const Singleton &) = delete;

//     static void init()
//     {
//         // m_instance：一个 std::unique_ptr<Singleton> 智能指针变量，管理 Singleton 类的唯一实例。
//         // .reset(...)：unique_ptr 的成员函数，用于 重置指针，释放原来管理的对象（如果有），然后管理新的对象指针。
//         // new Singleton()：动态分配一个新的 Singleton 对象，返回其裸指针。
//         m_instance.reset(new Singleton());
//         std::cout << "Singleton instance initialized\n";
//     }

//     // static	静态变量，属于类本身而非对象，全类共享一份
//     // std::unique_ptr<Singleton>	智能指针，只能独占一个 Singleton 实例的所有权
//     // m_instance	变量名，通常代表 Singleton 的唯一实例指针
//     static std::unique_ptr<Singleton> m_instance;
//     static std::once_flag m_onceflag;

//     int m_data = 0;
// };

// int main()
// {
//     return 0;
// }


// // template <class Callable, class... Args>
// // void std::call_once(std::once_flag& flag, Callable&& func, Args&&... args);