//
// Created by Will Sather on 10/22/24.
//
#pragma once

#include "cache.hpp"
#include <unordered_map>
#include <list>

template<typename Key, typename Value>
class LFUCache : public Cache<Key, Value> {
public:
    LFUCache(int capacity);

    void put(const Key &key, const Value &value) override;

    std::optional<Value> get(const Key &key) override;

private:
    void updateFrequency(const Key &key);

    struct CacheEntry {
        Value value;
        int frequency;
    };

    int _capacity;
    int _min_frequency;

    std::unordered_map<Key, CacheEntry> _key_value_map; // stores key and {value, frequency}
    std::unordered_map<int, std::list<Key> > _frequency_list_map; // stores frequency and list of keys
    std::unordered_map<Key, typename std::list<Key>::iterator> _key_iter_map;
    // stores key and its position in the frequency list
};

template<typename Key, typename Value>
LFUCache<Key, Value>::LFUCache(int capacity) : _capacity(capacity), _min_frequency(0) {
}

template<typename Key, typename Value>
std::optional<Value> LFUCache<Key, Value>::get(const Key &key) {
    if (!_key_value_map.contains(key)) {
        return std::nullopt;
    }

    // update the frequency of the key
    updateFrequency(key);

    // Return the value associated with the key
    return _key_value_map[key].value;
}

template<typename Key, typename Value>
void LFUCache<Key, Value>::put(const Key &key, const Value &value) {
    if (_capacity == 0)
        return;

    // If the key is already in the cache, update its value and frequency
    if (_key_value_map.contains(key)) {
        _key_value_map[key].value = value;
        updateFrequency(key);
        return;
    }

    // If the cache is full, evict the least frequently used key
    if (_key_value_map.size() >= _capacity) {
        // Evict the least frequently used key
        Key evictKey = _frequency_list_map[_min_frequency].back();
        _frequency_list_map[_min_frequency].pop_back();
        if (_frequency_list_map[_min_frequency].empty()) {
            _frequency_list_map.erase(_min_frequency); // Remove empty list
        }
        _key_value_map.erase(evictKey);
        _key_iter_map.erase(evictKey);
    }

    // Insert the new key with value and frequency 1
    _key_value_map[key] = {value, 1};
    _frequency_list_map[1].push_front(key);
    _key_iter_map[key] = _frequency_list_map[1].begin();
    _min_frequency = 1; // Reset minFrequency to 1 for new key
}

template<typename Key, typename Value>
void LFUCache<Key, Value>::updateFrequency(const Key &key) {
    int freq = _key_value_map[key].frequency;

    // remove the key from its current frequency list
    _frequency_list_map[freq].erase(_key_iter_map[key]);
    if (_frequency_list_map[freq].empty()) {
        // remove empty frequency list
        _frequency_list_map.erase(freq);

        // increment minFrequency if necessary
        if (_min_frequency == freq) {
            _min_frequency++;
        }
    }

    // increment the key's frequency and move it to the new list
    ++_key_value_map[key].frequency;
    int newFreq = _key_value_map[key].frequency;
    _frequency_list_map[newFreq].push_front(key);
    _key_iter_map[key] = _frequency_list_map[newFreq].begin();
}
