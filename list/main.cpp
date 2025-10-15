#include "list.h"
#include <iostream>
using namespace std;

int main()
{
    // 创建一个整型链表
    List<int> lst;

    cout << "== 测试 push_back() ==" << endl;
    lst.push_back(10);
    lst.push_back(20);
    lst.push_back(30);

    cout << "链表内容: ";
    for (auto it = lst.begin(); it != lst.end(); ++it)
        cout << *it << " ";
    cout << endl;

    cout << "== 测试 push_front() ==" << endl;
    lst.push_front(5);
    lst.push_front(1);

    cout << "链表内容: ";
    for (auto it = lst.begin(); it != lst.end(); ++it)
        cout << *it << " ";
    cout << endl;

    cout << "== 测试 front(), back() ==" << endl;
    cout << "front = " << lst.front() << ", back = " << lst.back() << endl;

    cout << "== 测试 pop_front() ==" << endl;
    lst.pop_front();

    cout << "链表内容: ";
    for (auto x : lst) // 支持 range-for 遍历（因为实现了 begin()/end()）
        cout << x << " ";
    cout << endl;

    cout << "== 测试 pop_back() ==" << endl;
    lst.pop_back();

    cout << "链表内容: ";
    for (auto x : lst)
        cout << x << " ";
    cout << endl;

    cout << "== 测试 erase() 删除第二个节点 ==" << endl;
    auto it = lst.begin();
    ++it; // 指向第二个元素
    lst.erase(it);

    cout << "链表内容: ";
    for (auto x : lst)
        cout << x << " ";
    cout << endl;

    cout << "== 测试 insert() 在第二个位置插入99 ==" << endl;
    it = lst.begin();
    ++it;
    lst.insert(it, 99);

    cout << "链表内容: ";
    for (auto x : lst)
        cout << x << " ";
    cout << endl;

    cout << "== 测试拷贝构造函数 ==" << endl;
    List<int> lst2 = lst;
    cout << "lst2内容: ";
    for (auto x : lst2)
        cout << x << " ";
    cout << endl;

    cout << "== 测试移动构造函数 ==" << endl;
    List<int> lst3 = std::move(lst2);
    cout << "lst3内容: ";
    for (auto x : lst3)
        cout << x << " ";
    cout << endl;

    cout << "== 测试拷贝赋值运算符 ==" << endl;
    List<int> lst4;
    lst4 = lst3;
    cout << "lst4内容: ";
    for (auto x : lst4)
        cout << x << " ";
    cout << endl;

    cout << "== 测试移动赋值运算符 ==" << endl;
    List<int> lst5;
    lst5 = std::move(lst4);
    cout << "lst5内容: ";
    for (auto x : lst5)
        cout << x << " ";
    cout << endl;

    cout << "== 测试 clear() ==" << endl;
    lst5.clear();
    cout << "是否为空: " << boolalpha << lst5.empty() << endl;

    return 0;
}
