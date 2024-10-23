//
// Created by Will Sather on 10/22/24.
//
#pragma once

#include "cache.hpp"
#include <unordered_map>
#include <chrono>

template<typename Key, typename Value>
class TimeBasedCache : public Cache<Key, Value> {
public:
    TimeBasedCache(std::chrono::seconds ttl);

    void put(const Key &key, const Value &value) override;

    std::optional<Value> get(const Key &key) override;

private:
    std::chrono::seconds _ttl;
    std::unordered_map<Key, std::pair<Value, std::chrono::steady_clock::time_point> > _cache;

    void cleanup();
};

template<typename Key, typename Value>
TimeBasedCache<Key, Value>::TimeBasedCache(std::chrono::seconds ttl) : _ttl(ttl) {
}

template<typename Key, typename Value>
void TimeBasedCache<Key, Value>::put(const Key &key, const Value &value) {
    auto expiry = std::chrono::steady_clock::now() + _ttl;
    _cache[key] = std::make_pair(value, expiry);
    cleanup();
}

template<typename Key, typename Value>
std::optional<Value> TimeBasedCache<Key, Value>::get(const Key &key) {
    cleanup();
    auto it = _cache.find(key);
    if (it != _cache.end()) {
        return it->second.first;
    }
    return std::nullopt;
}

template<typename Key, typename Value>
void TimeBasedCache<Key, Value>::cleanup() {
    auto now = std::chrono::steady_clock::now();
    for (auto it = _cache.begin(); it != _cache.end();) {
        if (it->second.second <= now) {
            it = _cache.erase(it);
        } else {
            ++it;
        }
    }
}
