#include <iostream>
#include <cstddef>
#include <stdexcept>

template <typename T, std::size_t N>
class CustomArray {
private:
    T m_data[N];

public:
    constexpr std::size_t size() const noexcept { return N; }

    // Element access with bounds checking
    T& at(std::size_t index) {
        if (index >= N) {
            throw std::out_of_range("Index out of bounds");
        }
        return m_data[index];
    }

    const T& at(std::size_t index) const {
        if (index >= N) {
            throw std::out_of_range("Index out of bounds");
        }
        return m_data[index];
    }

    // Subscript operator (unchecked for performance)
    T& operator[](std::size_t index) noexcept { return m_data[index]; }
    const T& operator[](std::size_t index) const noexcept { return m_data[index]; }

    // Iterators for range-based for loops
    T* begin() noexcept { return m_data; }
    T* end() noexcept { return m_data + N; }
    const T* begin() const noexcept { return m_data; }
    const T* end() const noexcept { return m_data + N; }
};

int main() {
    CustomArray<int, 5> arr;
    for (std::size_t i = 0; i < arr.size(); ++i) {
        arr[i] = static_cast<int>((i + 1) * 10);
    }

    // Range-based for loop works because of begin()/end()
    for (const auto& val : arr) {
        std::cout << val << ' ';
    }
    std::cout << '\n';

    return 0;
}