/*!
 * @file list.hpp
 * @brief Dynamic array implementation with support for both dynamic and static memory allocation
 */

#ifndef LIST_HPP
#define LIST_HPP

#include <iostream>
#include <cstddef>
#include <cstring>
#include <stdexcept>

/*!
 * @brief Represents a dynamically sized array (List) structure.
 *
 * This class holds metadata for a homogeneous collection of elements
 * where memory management can be done either internally (via constructor)
 * or externally (via init_static), making it suitable for both
 * general and embedded systems.
 */
template <typename T>
class List {
private:
    T* elements;         ///< Pointer to the data array (external or internal allocation)
    size_t count;        ///< The current number of elements stored
    size_t capacity;     ///< The maximum number of elements currently allocated for
    bool is_dynamic;     ///< Flag to indicate if the list buffer (elements) was dynamically allocated

    /*!
     * @brief Internal utility to resize the list's data buffer.
     *
     * Uses reallocation and doubles the capacity. This is common algorithm
     * consolidated from the original add/insert functions.
     *
     * @param new_capacity The desired new capacity (must be >= count).
     * @return true on success, false if reallocation fails.
     */
    bool resize(size_t new_capacity) {
        // If the list was initialized statically, we do not allow resizing
        if (!is_dynamic) {
            return false;
        }

        if (new_capacity < count) {
            return false;
        }

        // Attempt to reallocate memory for the data array
        T* new_elements = new (std::nothrow) T[new_capacity];
        if (new_elements == nullptr) {
            return false;
        }

        // Copy existing elements to new array
        for (size_t i = 0; i < count; ++i) {
            new_elements[i] = elements[i];
        }

        // Delete old array if it was dynamically allocated
        if (is_dynamic && elements != nullptr) {
            delete[] elements;
        }

        elements = new_elements;
        capacity = new_capacity;
        return true;
    }

public:
    /*!
     * @defgroup Initialization Initialization and Destruction Functions
     * @{
     */

    /*!
     * @brief Default constructor that creates an empty list with zero capacity.
     */
    List() : elements(nullptr), count(0), capacity(0), is_dynamic(true) {}

    /*!
     * @brief Constructs and initializes a new List with dynamic memory allocation.
     *
     * @param capacity The initial maximum number of elements the list can hold. Must be > 0.
     * @throw std::invalid_argument if capacity is 0
     * @throw std::bad_alloc if memory allocation fails
     */
    explicit List(size_t capacity) : count(0), capacity(capacity), is_dynamic(true) {
        if (capacity == 0) {
            throw std::invalid_argument("Capacity must be greater than 0");
        }

        elements = new T[capacity];
    }

    /*!
     * @brief Destructor that frees the memory associated with the List and its data buffer.
     *
     * This is automatically called when the List object goes out of scope.
     * It safely handles both dynamically and statically allocated buffers.
     */
    ~List() {
        if (is_dynamic && elements != nullptr) {
            delete[] elements;
        }
        elements = nullptr;
    }

    /*!
     * @brief Initializes a List using a pre-allocated static/stack buffer.
     *
     * This function allows memory control outside of dynamic allocation, preferred in strict embedded environments.
     * The list will not resize itself.
     *
     * @param buffer Pointer to the pre-allocated data buffer.
     * @param capacity The maximum size of the buffer in elements.
     */
    void init_static(T* buffer, size_t capacity) {
        if (is_dynamic && elements != nullptr) {
            delete[] elements;
        }

        elements = buffer;
        this->count = 0;
        this->capacity = capacity;
        this->is_dynamic = false;
    }

    /*! @} */ // End of Initialization group

    /*!
     * @defgroup Information Basic Information Functions
     * @{
     */

    /*!
     * @brief Gets the current number of elements in the list.
     * @return The current number of elements.
     */
    size_t length() const {
        return count;
    }

    /*!
     * @brief Gets the current capacity of the list.
     * @return The maximum number of elements currently allocated for.
     */
    size_t get_capacity() const {
        return capacity;
    }

    /*!
     * @brief Checks if the list contains no elements.
     *
     * @return true if the list is empty (count is 0), false otherwise.
     */
    bool is_empty() const {
        return count == 0;
    }

    /*!
     * @brief Returns whether the list uses dynamic memory allocation.
     *
     * @return true if the list buffer was dynamically allocated, false otherwise.
     */
    bool is_dynamic_allocation() const {
        return is_dynamic;
    }

    /*! @} */ // End of Information group

    /*!
     * @defgroup Access Element Access and Modification
     * @{
     */

    /*!
     * @brief Gets a pointer to the element at the specified index.
     *
     * The return value is a direct pointer into the internal array.
     *
     * @param index The zero-based index of the element to retrieve.
     * @return A pointer to the element.
     * @throw std::out_of_range if the index is out of bounds
     */
    T* get_element(size_t index) {
        if (index >= count) {
            throw std::out_of_range("Index out of bounds");
        }
        return &elements[index];
    }

    /*!
     * @brief Gets a const pointer to the element at the specified index.
     *
     * @param index The zero-based index of the element to retrieve.
     * @return A const pointer to the element.
     * @throw std::out_of_range if the index is out of bounds
     */
    const T* get_element(size_t index) const {
        if (index >= count) {
            throw std::out_of_range("Index out of bounds");
        }
        return &elements[index];
    }

    /*!
     * @brief Sets the value of the element at the specified index.
     *
     * @param index The zero-based index of the element to modify.
     * @param value The new value to set.
     * @throw std::out_of_range if the index is out of bounds
     */
    void set_element(size_t index, const T& value) {
        if (index >= count) {
            throw std::out_of_range("Index out of bounds");
        }
        elements[index] = value;
    }

    /*!
     * @brief Resets the list's element count to zero, effectively clearing it.
     *
     * The allocated memory buffer remains intact.
     */
    void clear() {
        count = 0;
    }

    /*! @} */ // End of Access group

    /*!
     * @defgroup Manipulation Element Manipulation Functions
     * @{
     */

    /*!
     * @brief Adds a new element to the end of the list.
     *
     * If capacity is reached, it attempts to double the capacity using realloc (if dynamically allocated).
     *
     * @param element The element to add.
     * @throw std::runtime_error if list is invalid or memory reallocation fails
     */
    void add(const T& element) {
        // Check for capacity and resize if needed
        if (count >= capacity) {
            size_t new_capacity = (capacity == 0) ? 1 : capacity * 2; // Doubling strategy
            if (!resize(new_capacity)) {
                throw std::runtime_error("Failed to reallocate memory for list expansion");
            }
        }

        // Add new element
        elements[count] = element;
        count++;
    }

    /*!
     * @brief Inserts a new element at the specified index, shifting subsequent elements right.
     *
     * If capacity is reached, it attempts to double the capacity using realloc (if dynamically allocated).
     *
     * @param index The index where the new element should be inserted. Must be <= length().
     * @param element The element to insert.
     * @throw std::out_of_range if index is out of bounds
     * @throw std::runtime_error if memory reallocation fails
     */
    void insert(size_t index, const T& element) {
        if (index > count) {
            throw std::out_of_range("Index out of bounds");
        }

        // Check for capacity and resize if needed
        if (count >= capacity) {
            size_t new_capacity = (capacity == 0) ? 1 : capacity * 2;
            if (!resize(new_capacity)) {
                throw std::runtime_error("Failed to reallocate memory for list expansion");
            }
        }

        // Shift elements to the right to make space
        if (index < count) {
            for (size_t i = count; i > index; --i) {
                elements[i] = elements[i - 1];
            }
        }

        // Insert new element
        elements[index] = element;
        count++;
    }

    /*!
     * @brief Removes the element at the specified index, shifting subsequent elements left.
     *
     * Does not reduce the list's capacity.
     *
     * @param index The index of the element to remove. Must be < length().
     * @throw std::out_of_range if index is out of bounds
     */
    void remove_at(size_t index) {
        if (index >= count) {
            throw std::out_of_range("Index out of bounds");
        }

        // Shift elements to the left to close gap
        for (size_t i = index; i < count - 1; ++i) {
            elements[i] = elements[i + 1];
        }

        count--;
    }

    /*! @} */ // End of Manipulation group

    /*!
     * @defgroup Search Search Functions
     * @{
     */

    /*!
     * @brief Finds the index of the first occurrence of a matching value.
     *
     * Comparison is done using operator== for the type T.
     *
     * @param value The value to search for.
     * @return The zero-based index of the found element, or -1 if not found.
     */
    long index_of(const T& value) const {
        for (size_t i = 0; i < count; ++i) {
            if (elements[i] == value) {
                return static_cast<long>(i);
            }
        }
        return -1;
    }

    /*!
     * @brief Checks if the list contains a specific value.
     *
     * @param value The value to check for.
     * @return true if the element is found, false otherwise.
     */
    bool contains(const T& value) const {
        return index_of(value) >= 0;
    }

    /*! @} */ // End of Search group

    /*!
     * @brief Overloaded array subscript operator for element access.
     *
     * @param index The zero-based index of the element to retrieve.
     * @return A reference to the element at the specified index.
     * @throw std::out_of_range if the index is out of bounds
     */
    T& operator[](size_t index) {
        if (index >= count) {
            throw std::out_of_range("Index out of bounds");
        }
        return elements[index];
    }

    /*!
     * @brief Overloaded const array subscript operator for element access.
     *
     * @param index The zero-based index of the element to retrieve.
     * @return A const reference to the element at the specified index.
     * @throw std::out_of_range if the index is out of bounds
     */
    const T& operator[](size_t index) const {
        if (index >= count) {
            throw std::out_of_range("Index out of bounds");
        }
        return elements[index];
    }

private:
    // Disable copy constructor and assignment operator to prevent shallow copying
    List(const List&) = delete;
    List& operator=(const List&) = delete;

    // Enable move semantics
public:
    /*!
     * @brief Move constructor.
     *
     * @param other The List to move resources from.
     */
    List(List&& other) noexcept
        : elements(other.elements),
          count(other.count),
          capacity(other.capacity),
          is_dynamic(other.is_dynamic) {
        // Reset the source object
        other.elements = nullptr;
        other.count = 0;
        other.capacity = 0;
        other.is_dynamic = true;
    }

    /*!
     * @brief Move assignment operator.
     *
     * @param other The List to move resources from.
     * @return A reference to this List.
     */
    List& operator=(List&& other) noexcept {
        if (this != &other) {
            // Clean up existing resources
            if (is_dynamic && elements != nullptr) {
                delete[] elements;
            }

            // Transfer resources
            elements = other.elements;
            count = other.count;
            capacity = other.capacity;
            is_dynamic = other.is_dynamic;

            // Reset the source object
            other.elements = nullptr;
            other.count = 0;
            other.capacity = 0;
            other.is_dynamic = true;
        }
        return *this;
    }
};

/*!
 * @brief Utility function to print the contents of a list (for demonstration purposes).
 *
 * This function requires that the element type T has operator<< defined for std::ostream.
 *
 * @tparam T The type of elements stored in the list.
 * @param os The output stream to write to.
 * @param list The list to print.
 * @return The output stream.
 */
template <typename T>
std::ostream& operator<<(std::ostream& os, const List<T>& list) {
    if (list.is_empty()) {
        os << "List is empty (count: 0)";
        return os;
    }

    os << "List (count: " << list.length() << ", capacity: " << list.get_capacity() << "): [";
    for (size_t i = 0; i < list.length(); ++i) {
        os << list.get_element(i);
        if (i < list.length() - 1) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

#endif // LIST_HPP
