#include <iostream>
#include <string>

// Modern C++17 Variadic Template using compile-time 'if constexpr'
template <typename T, typename... Args>
void print_all(const T& first, const Args&... rest) {
    std::cout << first;
    
    if constexpr (sizeof...(rest) > 0) {
        std::cout << ", ";
        print_all(rest...); // Compile-time expansion
    } else {
        std::cout << '\n';
    }
}

int main() {
    print_all(1, 2.5, "C++20", 'A', std::string("Variadic Templates"));
    return 0;
}