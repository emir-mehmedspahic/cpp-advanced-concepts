#include <iostream>
#include <cstddef>
#include <utility>
#include <stdexcept>

template <typename T>
class CustomVector {
private:
    T* m_data = nullptr;
    int m_size = 0;
    int m_capacity = 0;

    void reallocate(int new_capacity) {
        T* new_data = new T[new_capacity];
        for (int i = 0; i < m_size; ++i) {
            new_data[i] = std::move(m_data[i]);
        }
        delete[] m_data;
        m_data = new_data;
        m_capacity = new_capacity;
    }

public:
    CustomVector() = default;

    ~CustomVector() {
        delete[] m_data;
    }

    // Copy Constructor
    CustomVector(const CustomVector& other) : m_size(other.m_size), m_capacity(other.m_capacity) {
        m_data = new T[m_capacity];
        for (int i = 0; i < m_size; ++i) {
            m_data[i] = other.m_data[i];
        }
    }

    // Move Constructor
    CustomVector(CustomVector&& other) noexcept 
        : m_data(other.m_data), m_size(other.m_size), m_capacity(other.m_capacity) {
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }

    void push_back(const T& value) {
        if (m_size >= m_capacity) {
            reallocate(m_capacity == 0 ? 2 : m_capacity * 2);
        }
        m_data[m_size++] = value;
    }

    void pop_back() {
        if (m_size > 0) {
            --m_size;
        }
    }

    T& operator[](int index) noexcept { return m_data[index]; }
    const T& operator[](int index) const noexcept { return m_data[index]; }

    int size() const noexcept { return m_size; }
    int capacity() const noexcept { return m_capacity; }
};

int main() {
    CustomVector<int> vec;
    for (int i = 1; i <= 5; ++i) {
        vec.push_back(i * 10);
    }

    std::cout << "Vector size: " << vec.size() << ", capacity: " << vec.capacity() << '\n';
    for (int i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << ' ';
    }
    std::cout << '\n';

    return 0;
}