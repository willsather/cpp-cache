#include "gtest/gtest.h"
#include "time_based_cache.hpp"
#include <thread>

TEST(TimeBasedCacheTest, BasicPutAndGet) {
    TimeBasedCache<int, std::string> cache(std::chrono::seconds(5));

    cache.put(1, "one");
    cache.put(2, "two");

    EXPECT_EQ(cache.get(1).value(), "one");
    EXPECT_EQ(cache.get(2).value(), "two");
}

TEST(TimeBasedCacheTest, ExpirationTest) {
    TimeBasedCache<int, std::string> cache(std::chrono::seconds(2));

    cache.put(1, "one");
    cache.put(2, "two");

    EXPECT_EQ(cache.get(1).value(), "one");
    EXPECT_EQ(cache.get(2).value(), "two");

    std::this_thread::sleep_for(std::chrono::seconds(3));

    EXPECT_EQ(cache.get(1), std::nullopt);
    EXPECT_EQ(cache.get(2), std::nullopt);
}

TEST(TimeBasedCacheTest, UpdateExistingKeyRefreshTest) {
    TimeBasedCache<int, std::string> cache(std::chrono::seconds(3));

    cache.put(1, "one");
    std::this_thread::sleep_for(std::chrono::seconds(2));

    cache.put(1, "uno"); // this should refresh the expiration time
    std::this_thread::sleep_for(std::chrono::seconds(2));

    EXPECT_EQ(cache.get(1).value(), "uno"); // should still be available
}

TEST(TimeBasedCacheTest, CleanupTest) {
    TimeBasedCache<int, std::string> cache(std::chrono::seconds(2));

    cache.put(1, "one");
    cache.put(2, "two");
    cache.put(3, "three");

    std::this_thread::sleep_for(std::chrono::seconds(3));

    cache.put(4, "four"); // this should trigger cleanup

    EXPECT_EQ(cache.get(1), std::nullopt);
    EXPECT_EQ(cache.get(2), std::nullopt);
    EXPECT_EQ(cache.get(3), std::nullopt);
    EXPECT_EQ(cache.get(4).value(), "four");
}
