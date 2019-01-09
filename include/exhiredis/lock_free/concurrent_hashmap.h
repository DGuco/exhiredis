//
// Created by dguco on 19-1-8.
//

#ifndef EXHIREDIS_CONCURRENT_MAP_H
#define EXHIREDIS_CONCURRENT_MAP_H
#include <hash_fun.h>
#include <atomic>
#include <mutex>
#include <functional>

using namespace std;
namespace exhiredis
{
template<
    typename KeyType,
    typename ValueType,
    typename HashFn = std::hash<KeyType>,
    typename KeyEqual = std::equal_to<KeyType>,
    typename Allocator = std::allocator<uint8_t>,
    uint8_t ShardBits = 8,
    template<typename> class Atom = std::atomic,
    class Mutex = std::mutex>
class ConcurrentHashMap
{
public:
    class ConstIterator;

    typedef KeyType key_type;
    typedef ValueType mapped_type;
    typedef std::pair<const KeyType, ValueType> value_type;
    typedef std::size_t size_type;
    typedef HashFn hasher;
    typedef KeyEqual key_equal;
    typedef ConstIterator const_iterator;
};
}
#endif //EXHIREDIS_CONCURRENT_MAP_H
