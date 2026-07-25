#include <iostream>
#include <utility>

template <typename T>
class UniquePtr {
private:
    T* m_ptr = nullptr;

public:
    constexpr UniquePtr() noexcept = default;
    
    explicit UniquePtr(T* ptr) noexcept : m_ptr(ptr) {}

    ~UniquePtr() {
        delete m_ptr;
    }

    // Non-copyable
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    // Move semantics
    UniquePtr(UniquePtr&& other) noexcept : m_ptr(other.m_ptr) {
        other.m_ptr = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            delete m_ptr;
            m_ptr = other.m_ptr;
            other.m_ptr = nullptr;
        }
        return *this;
    }

    // Observers
    T& operator*() const noexcept { return *m_ptr; }
    T* operator->() const noexcept { return m_ptr; }
    T* get() const noexcept { return m_ptr; }
    explicit operator bool() const noexcept { return m_ptr != nullptr; }

    // Modifiers
    T* release() noexcept {
        T* temp = m_ptr;
        m_ptr = nullptr;
        return temp;
    }

    void reset(T* ptr = nullptr) noexcept {
        T* old_ptr = m_ptr;
        m_ptr = ptr;
        delete old_ptr;
    }
};

// --- Tests ---

struct Dummy {
    int value = 42;
    void print() const { std::cout << "Value: " << value << '\n'; }
};

int main() {
    UniquePtr<Dummy> ptr1(new Dummy());
    if (ptr1) {
        ptr1->print();
    }

    // Ownership transfer
    UniquePtr<Dummy> ptr2 = std::move(ptr1);
    
    if (!ptr1 && ptr2) {
        std::cout << "Ownership successfully transferred.\n";
    }

    return 0;
}