//
// Created by Will Sather on 10/22/24.
//
#pragma once

#include "cache.hpp"
#include <unordered_map>
#include <list>

template<typename Key, typename Value>
class LRUCache : public Cache<Key, Value> {
public:
    LRUCache(size_t capacity);

    void put(const Key &key, const Value &value) override;

    std::optional<Value> get(const Key &key) override;

private:
    size_t _capacity;
    std::unordered_map<Key, typename std::list<std::pair<Key, Value> >::iterator> _cache_map;
    std::list<std::pair<Key, Value> > _cache_items;

    void move_to_front(typename std::list<std::pair<Key, Value> >::iterator it);
};

template<typename Key, typename Value>
LRUCache<Key, Value>::LRUCache(const size_t capacity) : _capacity(capacity) {
}

template<typename Key, typename Value>
void LRUCache<Key, Value>::put(const Key &key, const Value &value) {
    // look up the key in the cache
    auto it = _cache_map.find(key);

    // if the key already exists
    if (it != _cache_map.end()) {
        // update the value items' value and move to front
        it->second->second = value;
        move_to_front(it->second);
        return;
    }

    // if cache is full
    if (_cache_items.size() == _capacity) {
        // remove the last item
        auto last = _cache_items.back();
        _cache_map.erase(last.first);
        _cache_items.pop_back();
    }

    // insert the new key value pair at the front of the list
    _cache_items.push_front(std::make_pair(key, value));
    _cache_map[key] = _cache_items.begin();
}

template<typename Key, typename Value>
std::optional<Value> LRUCache<Key, Value>::get(const Key &key) {
    // look up the key in the cache
    auto it = _cache_map.find(key);

    // if the key exists cannot be found
    if (it == _cache_map.end()) {
        return std::nullopt;
    }

    // move the accessed item to the front of the list (most recently used)
    move_to_front(it->second);
    return it->second->second;
}

template<typename Key, typename Value>
void LRUCache<Key, Value>::move_to_front(typename std::list<std::pair<Key, Value> >::iterator it) {
    _cache_items.splice(_cache_items.begin(), _cache_items, it);
}
