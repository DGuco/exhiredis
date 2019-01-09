//
// Created by dguco on 19-1-5.
//

#ifndef EXHIREDIS_ATOMIC_QUEUE_H
#define EXHIREDIS_ATOMIC_QUEUE_H

#include <queue>
#include <atomic>
#include <type_traits>
#include <functional>
using namespace std;

namespace exhiredis
{
template<typename T>
class concurrent_queue
{
    struct node
    {
        typedef typename node *_Ptr;
        atomic<_Ptr> _next;
        T _data;
    };

    typedef typename node::_Ptr _Ptr;
public:
    concurrent_queue(void)
        :
        _head(nullptr),
        _tail(nullptr)
    {
    }

//    bool push(T const &t)
//    {
//        for (;;) {
//            _Ptr tail = _tail.load(memory_order_acquire);
//            _Ptr next = tail->_next.load(memory_order_acquire);
//            _Ptr tail2 = _tail.load(memory_order_acquire);
//            if (tail == tail2) {
//                if (next == nullptr) {
//                    tagged_node_handle new_tail_next(node_handle, next.get_next_tag());
//                    if (tail_node->next.compare_exchange_weak(next, new_tail_next)) {
//                        tagged_node_handle new_tail(node_handle, tail.get_next_tag());
//                        tail_.compare_exchange_strong(tail, new_tail);
//                        return true;
//                    }
//                }
//                else {
//                    tagged_node_handle new_tail(pool.get_handle(next_ptr), tail.get_next_tag());
//                    tail_.compare_exchange_strong(tail, new_tail);
//                }
//            }
//        }
//    }

private:
    atomic<_Ptr> _head;
    atomic<_Ptr> _tail;
};
}
#endif //EXHIREDIS_ATOMIC_QUEUE_H
