//
//  ArrayList.hpp
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 30/11/25.
//


#ifndef ArrayList_h
#define ArrayList_h

#include <iostream>
#include <stdexcept>
#include <algorithm>

template <typename T>
class ArrayList
{
private:
    T *data;
    size_t size;
    size_t capacity;
    
    static const size_t DEFAULT_CAPACITY = 16;
    
    /// Resizes the internal array when capacity is reached
    void Resize(size_t newCapacity) {
        if (newCapacity < size) newCapacity = size;
        if (newCapacity == 0) newCapacity = 1;
        
        T *newData = new T[newCapacity];
        
        for (size_t i = 0; i < size; ++i) {
            newData[i] = data[i];
        }
        
        delete[] data;
        data = newData;
        this->capacity = newCapacity;
    }
    
public:
    /// Comparator function pointer type for sorting
    using Comparator = bool (*)(const T& a, const T& b);
    
    /// Default constructor
    ArrayList() : size(0), capacity(DEFAULT_CAPACITY) {
        data = new T[capacity];
    }
    
    /// Constructor with initial capacity
    explicit ArrayList(size_t initCap) : size(0) {
        capacity = (initCap == 0) ? DEFAULT_CAPACITY : initCap;
        data = new T[capacity];
    }
    
    /// Constructor from an existing C-array
    ArrayList(const T* other, size_t size) : size(size) {
        capacity = (size == 0) ? DEFAULT_CAPACITY : size * 2;
        data = new T[capacity];
        for (size_t i = 0; i < size; ++i) {
            data[i] = other[i];
        }
    }
    
    /// Constructor that initializes 'count' elements with 'initialValue'
    ArrayList(size_t count, const T& initialValue) : size(count), capacity(count * 1.5) {
        if (capacity == 0) capacity = 16;
        data = new T[capacity];
        for (size_t i = 0; i < size; ++i) {
            data[i] = initialValue;
        }
    }
    
    /// Copy constructor
    ArrayList(const ArrayList& other) : size(other.size), capacity(other.capacity) {
        data = new T[capacity];
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }
    
    /// Destructor
    ~ArrayList() {
        delete[] data;
    }
    
    /// Assignment operator (Deep copy)
    ArrayList& operator=(const ArrayList& other) {
        if (this != &other) {
            delete[] data;
            
            size = other.size;
            capacity = other.capacity;
            data = new T[capacity];
            for (size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }
    
    /// Index operator (mutable)
    T& operator[](size_t index) {
        if (index >= size) throw std::out_of_range("Index out of bounds");
        return data[index];
    }
    
    /// Index operator (read-only)
    const T& operator[](size_t index) const {
        if (index >= size) throw std::out_of_range("Index out of bounds");
        return data[index];
    }
    
    /// Returns current number of elements
    size_t Count() const { return size; }
    
    /// Checks if list is empty
    bool IsEmpty() const { return size == 0; }
    
    /// Gets element at index with bounds checking
    T Get(size_t index) const {
        if (index >= size) throw std::out_of_range("Index out of bounds");
        return data[index];
    }
    
    /// Sets element at index
    void Set(size_t index, T element) {
        if (index >= size) throw std::out_of_range("Index out of bounds");
        data[index] = element;
    }
    
    /// Adds element to end of list
    void Add(const T& element) {
        if (size >= capacity)
            Resize(capacity * 2);
        
        data[size++] = element;
    }
    
    /// Appends another list to this one
    void AddRange(const ArrayList<T>& other) {
        size_t newSize = size + other.size;
        if (newSize > capacity) {
            Resize(newSize * 1.5);
        }

        for (size_t i = 0; i < other.size; ++i) {
            data[size++] = other.data[i];
        }
    }
    
    /// Sets all existing elements to specific value
    void Fill(const T& value) {
        for (size_t i = 0; i < size; ++i) {
            data[i] = value;
        }
    }
    
    /// Inserts element at index, shifting others right
    void Insert(size_t index, const T& element) {
        if (index > size) throw std::out_of_range("Index out of bounds");
        
        if (size >= capacity) {
            Resize(capacity * 2);
        }

        for (size_t i = size; i > index; --i) {
            data[i] = data[i - 1];
        }

        data[index] = element;
        ++size;
    }
    
    /// Removes element at index by shifting
    void RemoveAt(size_t index) {
        if (index >= size) return;
        
        for (size_t i = index; i < size - 1; ++i) {
            data[i] = data[i + 1];
        }
        
        size--;
    }
    
    /// Removes first occurrence of element
    void Remove(const T& element) {
        int index = IndexOf(element);
        if (index >= 0) RemoveAt(static_cast<size_t>(index));
    }
    
    /// Finds index of element, returns -1 if not found
    int IndexOf(const T& element) const {
        for (size_t i = 0; i < size; ++i)
            if (data[i] == element) return static_cast<int>(i);
        return -1;
    }
    
    /// Checks if element exists
    bool Contains(const T& element) const {
        return IndexOf(element) != -1;
    }
    
    /// Iterator support
    T* begin() { return data; }
    T* end() { return data + size; }
    const T* begin() const { return data; }
    const T* end() const { return data + size; }
    
    /// Clears list (does not free memory)
    void Clear() { size = 0; }
    
    /// Sorts list using custom comparator
    void Sort(Comparator comp) {
        if (size > 1) std::sort(data, data + size, comp);
    }
};

#endif // !ArrayList_h


