# C++ Advanced Concepts & Data Structures

A hands-on exploration of modern C++ (C++17/20) internals — how smart pointers, memory allocators, and STL-style containers actually work under the hood, implemented from scratch.

## Why I built this

Most C++ courses teach you to *use* `std::vector` or `std::unique_ptr` — this project is about understanding what they're doing underneath, by rebuilding the core mechanics myself. It's a working reference for memory management, move semantics, and template metaprogramming as I prepare for systems-level and backend engineering roles.

## What it demonstrates

- **Manual memory management**: custom RAII-based smart pointer, bump/arena allocator with explicit alignment handling
- **Modern template mechanics**: perfect forwarding with universal references, variadic templates, compile-time evaluation with `if constexpr`
- **Data structures from first principles**: dynamic array with exponential heap reallocation (`std::vector` mechanics), doubly linked list with heap-allocated nodes
- **OOP design**: dynamic polymorphism via a virtual-dispatch example

## Tech stack

C++17 / C++20, compiled with GCC/Clang, no external dependencies.

## Repository Structure

```
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
```

## Building & Running

Each file is a standalone, self-contained example. Compile and run any of them individually:

```bash
g++ -std=c++20 -Wall -Wextra 01_memory_management/unique_ptr.cpp -o unique_ptr
./unique_ptr
```

Replace the path with any file in the repo to run that example.
