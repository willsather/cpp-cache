#include "gtest/gtest.h"
#include "lru_cache.hpp"

TEST(LRUCacheTest, BasicPutAndGet) {
    LRUCache<int, std::string> cache(2);

    cache.put(1, "one");
    cache.put(2, "two");

    EXPECT_EQ(cache.get(1).value(), "one");
    EXPECT_EQ(cache.get(2).value(), "two");
}

TEST(LRUCacheTest, EvictionTest) {
    LRUCache<int, std::string> cache(2);

    cache.put(1, "one");
    cache.put(2, "two");
    cache.put(3, "three"); // this should remove key 1

    // verify key 1 has been evicted
    EXPECT_EQ(cache.get(1), std::nullopt);

    // verify key 2,3 are still present
    EXPECT_EQ(cache.get(2).value(), "two");
    EXPECT_EQ(cache.get(3).value(), "three");
}

TEST(LRUCacheTest, OrderTest) {
    LRUCache<int, std::string> cache(2);

    cache.put(1, "one");
    cache.put(2, "two");
    cache.get(1); // access key 1, making it most recently used
    cache.put(3, "three"); // this should remove key 2

    // verify key 2 has been evicted
    EXPECT_EQ(cache.get(2), std::nullopt);

    // verify key 1,3 are still present
    EXPECT_EQ(cache.get(1).value(), "one");
    EXPECT_EQ(cache.get(3).value(), "three");
}
