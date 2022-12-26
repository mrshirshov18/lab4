
#ifndef __STACK_H__
#define __STACK_H__

#include <vector>
#include <cstddef>
#include <stdexcept>

using namespace std;
template <typename T>
class Stack
{
    vector<T> vec;
    size_t sz;
public:
    Stack()
    {
        sz = 0;
    }
    Stack(size_t size) : Stack()
    {
        vec.reserve(size);
    }
    Stack(size_t size, const T* arr) : Stack(size)
    {
        for (int i = 0; i < size; i++)
            vec.push_back(arr[i]);
        sz = size;
    }

    size_t size()
    {
        return sz;
    }
    bool empty()
    {
        return sz == 0;
    }

    bool operator==(const Stack& st)
    {
        if (this == &st)
            return true;
        return st.vec == vec;
    }
    void push(const T& elem)
    {
        vec.push_back(elem);
        sz++;
    }
    T top()
    {
        if (empty())
            throw std::out_of_range("Empty stack");
        return vec[sz - 1];
    }
    void pop()
    {
        if (empty())
            throw std::out_of_range("Empty stack");
        vec.pop_back();
        sz--;
    }

};

#endif
