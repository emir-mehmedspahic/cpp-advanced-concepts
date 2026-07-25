#include <iostream>
#include <cstddef>
#include <cstdint>
#include <utility>
#include <new>

class ArenaAllocator {
private:
    std::byte* m_buffer = nullptr;
    std::size_t m_capacity = 0;
    std::size_t m_offset = 0;

    // Helper: Align size/address to specified alignment boundary (default target architecture alignment)
    static std::size_t align_forward(std::size_t ptr, std::size_t alignment) {
        return (ptr + (alignment - 1)) & ~(alignment - 1);
    }

public:
    explicit ArenaAllocator(std::size_t capacity) : m_capacity(capacity) {
        m_buffer = static_cast<std::byte*>(::operator new(m_capacity));
    }

    ~ArenaAllocator() {
        ::operator delete(m_buffer);
    }

    // Non-copyable & Non-movable (Manages fixed-location raw buffer)
    ArenaAllocator(const ArenaAllocator&) = delete;
    ArenaAllocator& operator=(const ArenaAllocator&) = delete;
    ArenaAllocator(ArenaAllocator&&) = delete;
    ArenaAllocator& operator=(ArenaAllocator&&) = delete;

    // Allocate raw aligned memory
    void* allocate(std::size_t size, std::size_t alignment = alignof(std::max_align_t)) {
        std::size_t current_ptr = reinterpret_cast<std::size_t>(m_buffer + m_offset);
        std::size_t aligned_ptr = align_forward(current_ptr, alignment);
        std::size_t padding = aligned_ptr - current_ptr;

        if (m_offset + padding + size > m_capacity) {
            throw std::bad_alloc(); // Arena is out of memory
        }

        m_offset += padding + size;
        return reinterpret_cast<void*>(aligned_ptr);
    }

    // Construct an object directly inside arena memory (Placement New)
    template <typename T, typename... Args>
    T* create(Args&&... args) {
        void* mem = allocate(sizeof(T), alignof(T));
        return ::new (mem) T(std::forward<Args>(args)...);
    }

    // Reset arena pointer without freeing underlying buffer
    void reset() noexcept {
        m_offset = 0;
    }

    // Observers
    std::size_t used_bytes() const noexcept { return m_offset; }
    std::size_t capacity() const noexcept { return m_capacity; }
};

// --- Tests ---

struct Point {
    float x, y, z;
    Point(float x, float y, float z) : x(x), y(y), z(z) {}
};

int main() {
    // Allocate 1 KB Arena
    ArenaAllocator arena(1024);

    std::cout << "Arena initialized. Used: " << arena.used_bytes() << "/" << arena.capacity() << " bytes.\n";

    // Allocate objects sequentially
    int* numbers = static_cast<int*>(arena.allocate(sizeof(int) * 5, alignof(int)));
    for (int i = 0; i < 5; ++i) {
        numbers[i] = (i + 1) * 10;
    }

    Point* p1 = arena.create<Point>(1.0f, 2.0f, 3.0f);

    std::cout << "Allocated 5 ints and 1 Point. Used: " << arena.used_bytes() << " bytes.\n";
    std::cout << "Point values: (" << p1->x << ", " << p1->y << ", " << p1->z << ")\n";

    // Instant O(1) deallocation of everything
    arena.reset();
    std::cout << "Arena reset. Used: " << arena.used_bytes() << " bytes.\n";

    return 0;
}