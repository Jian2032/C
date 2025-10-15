#ifndef LIST_H
#define LIST_H

#include <utility>   // std::move
#include <stdexcept> // std::out_of_range

template <typename Object>
class List
{
private:
    // 节点结构体
    struct Node
    {
        Object data; // 存储元素
        Node *prev;  // 前驱节点指针
        Node *next;  // 后继节点指针

        // 拷贝构造
        Node(const Object &d = Object{}, Node *p = nullptr, Node *n = nullptr)
            : data{d}, prev{p}, next{n} {}

        // 移动构造
        Node(Object &&d, Node *p = nullptr, Node *n = nullptr)
            : data{std::move(d)}, prev{p}, next{n} {}
    };

public:
    // 常量迭代器
    class const_iterator
    {
    public:
        const_iterator() : current{nullptr} {} // 默认构造函数，current置空

        // 解引用操作符，返回节点数据的只读引用
        const Object &operator*() const { return retrieve(); }

        // 前置自增
        const_iterator &operator++()
        {
            current = current->next;
            return *this;
        }
        // 后置自增
        const_iterator operator++(int)
        {
            const_iterator old = *this;
            ++(*this);
            return old;
        }

        // 前置递减
        const_iterator &operator--()
        {
            current = current->prev;
            return *this;
        }
        // 后置递减
        const_iterator operator--(int)
        {
            const_iterator old = *this;
            --(*this);
            return old;
        }

        // 相等和不等
        bool operator==(const const_iterator &rhs) const { return current == rhs.current; }
        bool operator!=(const const_iterator &rhs) const { return !(*this == rhs); }

    protected:
        Node *current; // 当前节点指针

        // 返回节点数据
        Object &retrieve() const { return current->data; }

        // 构造函数，List可访问
        const_iterator(Node *p) : current{p} {}
        friend class List<Object>;
    };

    // 可修改迭代器
    class iterator : public const_iterator
    {
    public:
        iterator() {} // 默认构造

        // 解引用操作符
        Object &operator*() { return const_iterator::retrieve(); }
        const Object &operator*() const { return const_iterator::operator*(); }

        // 前置自增
        iterator &operator++()
        {
            this->current = this->current->next;
            return *this;
        }
        // 后置自增
        iterator operator++(int)
        {
            iterator old = *this;
            ++(*this);
            return old;
        }

        // 前置递减
        iterator &operator--()
        {
            this->current = this->current->prev;
            return *this;
        }
        // 后置递减
        iterator operator--(int)
        {
            iterator old = *this;
            --(*this);
            return old;
        }

        // 相等不等
        bool operator==(const iterator &rhs) const { return this->current == rhs.current; }
        bool operator!=(const iterator &rhs) const { return !(*this == rhs); }

    protected:
        iterator(Node *p) : const_iterator{p} {}
        friend class List<Object>;
    };

public:
    // 构造和析构
    List() { init(); }
    ~List()
    {
        clear();
        delete head;
        delete tail;
    }

    // 拷贝构造
    List(const List &rhs)
    {
        init();
        for (const Object &x : rhs)
            push_back(x);
    }
    // 拷贝赋值
    List &operator=(const List &rhs)
    {
        List copy = rhs;
        std::swap(*this, copy);
        return *this;
    }

    // 移动构造
    List(List &&rhs) : theSize{rhs.theSize}, head{rhs.head}, tail{rhs.tail}
    {
        rhs.theSize = 0;
        rhs.head = nullptr;
        rhs.tail = nullptr;
    }
    // 移动赋值
    List &operator=(List &&rhs)
    {
        std::swap(theSize, rhs.theSize);
        std::swap(head, rhs.head);
        std::swap(tail, rhs.tail);
        return *this;
    }

    // 迭代器接口
    iterator begin() { return {head->next}; }
    const_iterator begin() const { return {head->next}; }
    iterator end() { return {tail}; }
    const_iterator end() const { return {tail}; }

    int size() const { return theSize; }
    bool empty() const { return size() == 0; }

    // 清空列表
    void clear()
    {
        while (!empty())
            pop_front();
    }

    // 访问首尾元素
    Object &front() { return *begin(); }
    const Object &front() const { return *begin(); }
    Object &back() { return *--end(); }
    const Object &back() const { return *--end(); }

    // 插入元素
    void push_front(const Object &x) { insert(begin(), x); }
    void push_front(Object &&x) { insert(begin(), std::move(x)); }
    void push_back(const Object &x) { insert(end(), x); }
    void push_back(Object &&x) { insert(end(), std::move(x)); }

    // 删除元素
    void pop_front() { erase(begin()); }
    void pop_back() { erase(--end()); }

    // 插入操作
    iterator insert(iterator itr, const Object &x)
    {
        Node *p = itr.current;
        if (p == nullptr)
            throw std::out_of_range("Iterator out of bounds");
        theSize++;
        Node *newNode = new Node{x, p->prev, p};
        p->prev->next = newNode;
        p->prev = newNode;
        return iterator{newNode};
    }

    iterator insert(iterator itr, Object &&x)
    {
        Node *p = itr.current;
        if (p == nullptr)
            throw std::out_of_range("Iterator out of bounds");
        theSize++;
        Node *newNode = new Node{std::move(x), p->prev, p};
        p->prev->next = newNode;
        p->prev = newNode;
        return iterator{newNode};
    }

    // 删除操作
    iterator erase(iterator itr)
    {
        Node *p = itr.current;
        if (p == nullptr || p == tail)
            throw std::out_of_range("Iterator out of bounds");
        iterator retVal{p->next};
        p->prev->next = p->next;
        p->next->prev = p->prev;
        delete p;
        theSize--;
        return retVal;
    }

    iterator erase(iterator from, iterator to)
    {
        for (iterator itr = from; itr != to;)
            itr = erase(itr);
        return to;
    }

private:
    int theSize; // 链表长度
    Node *head;  // 头节点
    Node *tail;  // 尾节点

    // 初始化空链表
    void init()
    {
        theSize = 0;
        head = new Node;
        tail = new Node;
        head->next = tail;
        tail->prev = head;
    }
};

#endif // LIST_H
