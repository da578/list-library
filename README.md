# Dynamic List Library for C++

![C++](https://img.shields.io/badge/C++-17%2B-blue?logo=c%2B%2B)
![License](https://img.shields.io/badge/License-MIT-green)
![Build](https://img.shields.io/badge/build-passing-brightgreen)
![Platform](https://img.shields.io/badge/platform-cross--platform-lightgrey)

A modern, type-safe, and efficient dynamic array implementation in C++ with support for both dynamic and static memory allocation. This library provides a flexible List container suitable for both general applications and resource-constrained embedded systems.

## Features

- **Type Safety**: Fully templated implementation with compile-time type checking
- **Dual Allocation Modes**:
  - Dynamic allocation with automatic resizing
  - Static allocation using pre-allocated buffers (ideal for embedded systems)
- **Exception Safety**: Proper exception handling with strong guarantees
- **Memory Efficient**: Optimized memory usage with move semantics support
- **Comprehensive API**: Full set of operations including insertion, removal, search, and more
- **Modern C++**: Built with C++17 features and follows RAII principles

## Installation

### Option 1: Header-only Integration

Simply copy the `list.hpp` file into your project's include directory.

### Option 2: CMake Integration

Add this repository as a submodule:

```bash
git submodule add hhttps://github.com/da578/list-library.git
```

Then in your CMakeLists.txt:

```cmake
add_subdirectory(cpp-dynamic-list)
target_link_libraries(your_target PRIVATE dynamic_list)
```

## Usage

Dynamic Allocation Example

```cpp
#include "list.hpp"

int main() {
    // Create a list with initial capacity of 10 integers
    List<int> numbers(10);

    // Add elements (automatic resizing when needed)
    numbers.add(42);
    numbers.add(24);
    numbers.add(7);

    // Access elements
    int first = *numbers.get_element(0);  // 42

    // Modify elements
    numbers.set_element(1, 99);

    // Insert at specific position
    numbers.insert(1, 55);  // [42, 55, 99, 7]

    // Remove element
    numbers.remove_at(2);   // [42, 55, 7]

    // Search operations
    long index = numbers.index_of(55);  // 1
    bool contains = numbers.contains(7); // true

    return 0;
}
```

Static Allocation Example (Embedded Systems)
```cpp
#include "list.hpp"

int main() {
    // Pre-allocated buffer for embedded systems
    constexpr size_t MAX_ELEMENTS = 5;
    int static_buffer[MAX_ELEMENTS];
    
    List<int> static_list;
    static_list.init_static(static_buffer, MAX_ELEMENTS);
    
    // Operations work the same way but won't resize
    static_list.add(10);
    static_list.add(20);
    
    // Will fail if buffer is full (no dynamic resizing)
    try {
        for (int i = 0; i < 10; ++i) {
            static_list.add(i);  // Will throw exception when full
        }
    } catch (const std::runtime_error& e) {
        // Handle capacity exceeded
    }
    
    return 0;
}
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
