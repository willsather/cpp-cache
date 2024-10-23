#include "lfu_cache.hpp"
#include <gtest/gtest.h>

TEST(LFUCacheTest, BasicPutAndGet) {
    LFUCache<int, std::string> cache(2);

    // insert key value pairs
    cache.put(1, "One");
    cache.put(2, "Two");

    // verify values can be retrieved
    EXPECT_EQ(cache.get(1), "One");
    EXPECT_EQ(cache.get(2), "Two");

    // verify invalid values cannot be retrieved
    EXPECT_EQ(cache.get(3), std::nullopt);
}

TEST(LFUCacheTest, EvictionTest) {
    LFUCache<int, std::string> cache(2);

    // insert key value pairs
    cache.put(1, "One");
    cache.put(2, "Two");

    // access key 1 multiple times to increase its frequency
    cache.get(1);
    cache.get(1);
    cache.get(1);

    // add a new item, key 2 should be evicted since key 1 is more frequently used
    cache.put(3, "Three");

    // verify that key 3 was evicted
    EXPECT_EQ(cache.get(2), std::nullopt);

    // verify that key 1,3 are present
    EXPECT_EQ(cache.get(1), "One");
    EXPECT_EQ(cache.get(3), "Three");
}

TEST(LFUCacheTest, VariableFrequencyEvictionTest) {
    LFUCache<int, std::string> cache(3);

    // insert key value pairs
    cache.put(1, "One");
    cache.put(2, "Two");
    cache.put(3, "Three");

    // access keys with different frequencies
    cache.get(1);
    cache.get(1);
    cache.get(2);

    // insert a new key, this should cause eviction of the least frequently used key (key 3)
    cache.put(4, "Four");

    // verify that key 3 was evicted
    EXPECT_EQ(cache.get(3), std::nullopt);

    // verify that key 1,2,4 are present
    EXPECT_EQ(cache.get(1), "One");
    EXPECT_EQ(cache.get(2), "Two");
    EXPECT_EQ(cache.get(4), "Four");
}

TEST(LFUCacheTest, UpdateExistingKeyTest) {
    LFUCache<int, std::string> cache(2);

    // insert key value pairs
    cache.put(1, "One");
    cache.put(2, "Two");

    // update the value of an existing key (key 1)
    cache.put(1, "Uno");

    // verify the value has been updated
    EXPECT_EQ(cache.get(1), "Uno");

    // verify the other value has not been changed
    EXPECT_EQ(cache.get(2), "Two");
}

TEST(LFUCacheTest, ZeroCapacityTest) {
    LFUCache<int, std::string> cache(0);

    // add item to cache of size 0
    cache.put(1, "One");

    // should not store anything
    EXPECT_EQ(cache.get(1), std::nullopt);
}

TEST(LFUCacheTest, CapacityOneEvictionTest) {
    LFUCache<int, std::string> cache(1);

    // insert key value pair into cache of size 1
    cache.put(1, "One");
    EXPECT_EQ(cache.get(1), "One");

    // insert another key value pair
    cache.put(2, "Two");

    // should evict the existing key (key 1)
    EXPECT_EQ(cache.get(2), "Two");
    EXPECT_EQ(cache.get(1), std::nullopt);
}
