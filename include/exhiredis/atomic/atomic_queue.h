//
// Created by dguco on 19-1-5.
//

#ifndef EXHIREDIS_ATOMIC_QUEUE_H
#define EXHIREDIS_ATOMIC_QUEUE_H

#include <queue>
#include <atomic>
using namespace std;

deque<int> queue1;

namespace exhiredis
{
template<typename _Tp>
class atomic_queue
{
private:
    struct node
    {
        _Tp data;
        node *next;
        node(const _Tp &data)
            : data(data), next(nullptr)
        {}
    };
    typedef typename node *_Ptr;
    std::atomic<_Ptr> head;
    std::atomic<_Ptr> tail;
public:
    void push(const _Tp &data)
    {
        _Ptr new_node = new node(data);
        new_node->next = head.load(std::memory_order_relaxed);
        while (!head.compare_exchange_weak(new_node->next,
                                           new_node,
                                           std::memory_order_release,
                                           std::memory_order_relaxed));
    }

    _Tp pop()
    {

    }
};
}
#endif //EXHIREDIS_ATOMIC_QUEUE_H
