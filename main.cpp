/*!
 * @file main.cpp
 * @brief Unit tests for the List class
 */

#include "list.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>
#include <string>

/*!
 * @brief Runs all unit tests for the List data structure.
 *
 * This program tests all functions of the List data structure
 * with various scenarios including edge cases and error conditions.
 *
 * @return 0 if all tests pass, non-zero if any test fails
 */
int main() {
    std::cout << "=== List Data Structure Unit Tests ===" << std::endl;

    // Test suite 1: Basic Initialization and Destruction
    std::cout << "\n=== Test Suite 1: Initialization & Destruction ===" << std::endl;

    // Test 1.1: Create and destroy dynamic list
    {
        std::cout << "Test 1.1: Create and destroy dynamic list... ";
        List<int> list(10);
        assert(list.get_capacity() == 10);
        assert(list.length() == 0);
        assert(list.is_dynamic_allocation() == true);
        std::cout << "PASSED" << std::endl;
    }

    // Test 1.2: Create list with invalid parameters (should throw exception)
    {
        std::cout << "Test 1.2: Create list with invalid parameters... ";
        bool caught_exception = false;
        try {
            List<int> list(0);
        } catch (const std::invalid_argument& e) {
            caught_exception = true;
        }
        assert(caught_exception);
        std::cout << "PASSED" << std::endl;
    }

    // Test 1.3: Static initialization
    {
        std::cout << "Test 1.3: Static initialization... ";
        static const size_t STATIC_CAPACITY = 5;
        int static_buffer[STATIC_CAPACITY];
        List<int> static_list;
        static_list.init_static(static_buffer, STATIC_CAPACITY);
        assert(static_list.get_capacity() == STATIC_CAPACITY);
        assert(static_list.length() == 0);
        assert(static_list.is_dynamic_allocation() == false);
        assert(static_list.get_element(0) == &static_buffer[0]);
        std::cout << "PASSED" << std::endl;
    }

    // Test suite 2: Basic Operations
    std::cout << "\n=== Test Suite 2: Basic Operations ===" << std::endl;

    // Test 2.1: List length and empty check
    {
        std::cout << "Test 2.1: List length and empty check... ";
        List<int> list(5);
        assert(list.length() == 0);
        assert(list.is_empty() == true);

        list.add(42);
        assert(list.length() == 1);
        assert(list.is_empty() == false);

        list.clear();
        assert(list.length() == 0);
        assert(list.is_empty() == true);

        std::cout << "PASSED" << std::endl;
    }

    // Test 2.2: Get and set elements
    {
        std::cout << "Test 2.2: Get and set elements... ";
        List<int> list(3);
        int values[] = {10, 20, 30};

        // Add elements
        for (int i = 0; i < 3; i++) {
            list.add(values[i]);
        }

        // Get elements
        for (int i = 0; i < 3; i++) {
            assert(*list.get_element(i) == values[i]);
        }

        // Set elements
        list.set_element(1, 99);
        assert(*list.get_element(1) == 99);

        // Test invalid indices (should throw exceptions)
        bool caught_exception = false;
        try {
            list.get_element(3);
        } catch (const std::out_of_range& e) {
            caught_exception = true;
        }
        assert(caught_exception);

        caught_exception = false;
        try {
            list.set_element(3, 100);
        } catch (const std::out_of_range& e) {
            caught_exception = true;
        }
        assert(caught_exception);

        std::cout << "PASSED" << std::endl;
    }

    // Test suite 3: Element Manipulation
    std::cout << "\n=== Test Suite 3: Element Manipulation ===" << std::endl;

    // Test 3.1: Add elements and automatic resizing
    {
        std::cout << "Test 3.1: Add elements and automatic resizing... ";
        List<int> list(2);
        list.add(10);
        list.add(20);
        assert(list.length() == 2);
        assert(list.get_capacity() == 2);

        // This should trigger resize
        list.add(30);
        assert(list.length() == 3);
        assert(list.get_capacity() >= 4); // Should double capacity

        std::cout << "PASSED" << std::endl;
    }

    // Test 3.2: Insert elements at various positions
    {
        std::cout << "Test 3.2: Insert elements at various positions... ";
        List<int> list(5);
        list.add(10);
        list.add(20);
        list.add(30);

        // Insert at beginning
        list.insert(0, 5);
        assert(list.length() == 4);

        // Insert in middle
        list.insert(2, 15);
        assert(list.length() == 5);

        // Insert at end
        list.insert(5, 40);
        assert(list.length() == 6);

        // Verify elements
        int expected[] = {5, 10, 15, 20, 30, 40};
        for (size_t i = 0; i < list.length(); i++) {
            assert(*list.get_element(i) == expected[i]);
        }

        // Test invalid index (should throw exception)
        bool caught_exception = false;
        try {
            list.insert(7, 50); // Out of bounds
        } catch (const std::out_of_range& e) {
            caught_exception = true;
        }
        assert(caught_exception);

        std::cout << "PASSED" << std::endl;
    }

    // Test 3.3: Remove elements from various positions
    {
        std::cout << "Test 3.3: Remove elements from various positions... ";
        List<int> list(10);

        // Add elements 10, 20, 30, 40, 50
        for (int i = 1; i <= 5; i++) {
            list.add(i * 10);
        }

        // Remove from beginning
        list.remove_at(0);
        assert(list.length() == 4);

        // Remove from middle
        list.remove_at(1);
        assert(list.length() == 3);

        // Remove from end
        list.remove_at(2);
        assert(list.length() == 2);

        // Verify remaining elements
        int expected[] = {20, 40};
        for (size_t i = 0; i < list.length(); i++) {
            assert(*list.get_element(i) == expected[i]);
        }

        // Test invalid index (should throw exception)
        bool caught_exception = false;
        try {
            list.remove_at(2); // Out of bounds
        } catch (const std::out_of_range& e) {
            caught_exception = true;
        }
        assert(caught_exception);

        std::cout << "PASSED" << std::endl;
    }

    // Test suite 4: Search Operations
    std::cout << "\n=== Test Suite 4: Search Operations ===" << std::endl;

    // Test 4.1: Find index of elements
    {
        std::cout << "Test 4.1: Find index of elements... ";
        List<int> list(5);
        int values[] = {10, 20, 30, 40, 50};
        for (size_t i = 0; i < 5; i++) {
            list.add(values[i]);
        }

        assert(list.index_of(30) == 2);
        assert(list.index_of(10) == 0);
        assert(list.index_of(50) == 4);
        assert(list.index_of(60) == -1);

        std::cout << "PASSED" << std::endl;
    }

    // Test 4.2: Check if list contains elements
    {
        std::cout << "Test 4.2: Check if list contains elements... ";
        List<int> list(3);
        list.add(10);
        list.add(20);
        assert(list.contains(10) == true);
        assert(list.contains(20) == true);
        assert(list.contains(30) == false);

        list.add(30);
        assert(list.contains(30) == true);

        std::cout << "PASSED" << std::endl;
    }

    // Test suite 5: Edge Cases and Error Handling
    std::cout << "\n=== Test Suite 5: Edge Cases & Error Handling ===" << std::endl;

    // Test 5.1: Static list behavior (no resizing)
    {
        std::cout << "Test 5.2: Static list behavior (no resizing)... ";
        static const size_t STATIC_BUF_SIZE = 3;
        int static_buffer[STATIC_BUF_SIZE];
        List<int> static_list;
        static_list.init_static(static_buffer, STATIC_BUF_SIZE);

        static_list.add(10);
        assert(static_list.length() == 1);

        static_list.add(20);
        assert(static_list.length() == 2);

        static_list.add(30);
        assert(static_list.length() == 3);

        // This should fail - static list cannot resize
        bool caught_exception = false;
        try {
            static_list.add(40);
        } catch (const std::runtime_error& e) {
            caught_exception = true;
        }
        assert(caught_exception);
        assert(static_list.length() == 3);

        // Insert should also fail when full
        caught_exception = false;
        try {
            static_list.insert(1, 40);
        } catch (const std::runtime_error& e) {
            caught_exception = true;
        }
        assert(caught_exception);
        assert(static_list.length() == 3);

        // But operations that don't require resizing should work
        static_list.remove_at(1);
        assert(static_list.length() == 2);

        static_list.insert(1, 40);
        assert(static_list.length() == 3);

        std::cout << "PASSED" << std::endl;
    }

    // Test suite 6: Different Data Types
    std::cout << "\n=== Test Suite 6: Different Data Types ===" << std::endl;

    // Test 6.1: List with float elements
    {
        std::cout << "Test 6.1: List with float elements... ";
        List<float> list(3);
        list.add(1.1f);
        list.add(2.2f);
        list.add(3.3f);

        const float* f_elem = list.get_element(1);
        assert(std::fabs(*f_elem - 2.2f) < 0.001f); // Allow for floating-point error

        std::cout << "PASSED" << std::endl;
    }

    // Test 6.2: List with struct elements
    {
        std::cout << "Test 6.2: List with struct elements... ";
        struct Person {
            int id;
            std::string name;

            bool operator==(const Person& other) const {
                return id == other.id && name == other.name;
            }
        };

        List<Person> list(2);
        Person p1 = {1, "Alice"};
        Person p2 = {2, "Bob"};

        list.add(p1);
        list.add(p2);

        const Person* p_elem = list.get_element(0);
        assert(p_elem->id == 1);
        assert(p_elem->name == "Alice");

        p_elem = list.get_element(1);
        assert(p_elem->id == 2);
        assert(p_elem->name == "Bob");

        // Test modifying a struct element
        Person p3 = {3, "Charlie"};
        list.set_element(1, p3);

        p_elem = list.get_element(1);
        assert(p_elem->id == 3);
        assert(p_elem->name == "Charlie");

        std::cout << "PASSED" << std::endl;
    }

    std::cout << "\n=== All tests passed successfully! ===" << std::endl;
    return 0;
}
