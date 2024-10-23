//
// Created by Will Sather on 10/22/24.
//
#pragma once

#include <optional>

template<typename Key, typename Value>
class Cache {
public:
    virtual ~Cache() = default;

    virtual void put(const Key &key, const Value &value) = 0;

    virtual std::optional<Value> get(const Key &key) = 0;
};
