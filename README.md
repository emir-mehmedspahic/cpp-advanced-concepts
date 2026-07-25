# C++ Advanced Concepts & Data Structures

A collection of lightweight, well-structured C++17/C++20 examples demonstrating key language features, low-level memory management techniques, and fundamental data structure implementations.

## 📁 Repository Structure

```text
.
├── 01_memory_management/
│   ├── unique_ptr.cpp         # Custom smart pointer implementing RAII and Move Semantics
│   └── arena_allocator.cpp    # Bump/Arena memory allocator with explicit alignment handling
│
├── 02_templates_and_move/
│   ├── perfect_forwarding.cpp # Modern std::forward and universal references (T&&)
│   ├── variadic_templates.cpp # Variadic templates and compile-time evaluation (if constexpr)
│   └── custom_array.cpp       # Fixed-size compile-time array implementation
│
└── 03_data_structures_and_polymorphism/
    ├── meals_polymorphism.cpp # Object-Oriented Design and dynamic polymorphism
    ├── custom_vector.cpp      # Dynamic array with exponential heap reallocation
    └── doubly_linked_list.cpp # Doubly linked list structure with heap-allocated nodes
