# C++ Cache

This repository contains implementations of various caching strategies in C++, including LRU (Least Recently Used), LFU (Least Frequently Used), and Time-Based caching.

## Table of Contents

1. [Setup](#setup)
   - [Build](#build)
   - [Testing](#testing)

2. [Caching Strategies](#caching-strategies)
   - [LRU Cache](#lru-cache)
   - [LFU Cache](#lfu-cache)
   - [Time-Based Cache](#time-based-cache)

## Setup

To set up this project locally, follow these steps:

1. Ensure you have CMake (version 3.29 or higher) and a C++20 compatible compiler installed.

### Build

To build the project, you can use the provided build script:

```bash
./scripts/build.sh
```

This script will create a `build` directory, run CMake, and build the project.

### Testing

To run the tests, use the provided test script:

```bash
./scripts/run_tests.sh
```

This script will build the project and run all tests using CTest.

## Caching Strategies

> [!NOTE]  
> Each caching strategy is implemented as a separate class that inherits from a common [`Cache`](./src/cache.hpp) interface, allowing for easy interchangeability and extension.


### LRU Cache

The Least Recently Used (LRU) cache discards the least recently used items first when the cache reaches its capacity. This implementation uses a combination of a hash map and a doubly linked list to achieve O(1) time complexity for both insertion and lookup.

### LFU Cache

The Least Frequently Used (LFU) cache discards the least frequently used items first. If there's a tie, it removes the least recently used among the candidates. This implementation uses multiple hash maps to keep track of frequencies and achieve O(1) time complexity for both insertion and lookup.

### Time-Based Cache

The Time-Based cache automatically expires entries after a specified time-to-live (TTL) duration. It uses a hash map to store key-value pairs along with their expiration times. The cleanup of expired entries is performed lazily during insertion and lookup operations.