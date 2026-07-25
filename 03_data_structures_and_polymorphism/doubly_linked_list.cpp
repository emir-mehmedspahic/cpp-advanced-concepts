#include <iostream>

template <typename T>
class DoublyLinkedList {
private:
    struct Node {
        T data;
        Node* next = nullptr;
        Node* prev = nullptr;
        explicit Node(const T& val) : data(val) {}
    };

    Node* m_head = nullptr;
    Node* m_tail = nullptr;
    std::size_t m_size = 0;

public:
    DoublyLinkedList() = default;

    ~DoublyLinkedList() {
        clear();
    }

    void push_back(const T& value) {
        Node* new_node = new Node(value);
        if (!m_tail) {
            m_head = m_tail = new_node;
        } else {
            m_tail->next = new_node;
            new_node->prev = m_tail;
            m_tail = new_node;
        }
        ++m_size;
    }

    void push_front(const T& value) {
        Node* new_node = new Node(value);
        if (!m_head) {
            m_head = m_tail = new_node;
        } else {
            new_node->next = m_head;
            m_head->prev = new_node;
            m_head = new_node;
        }
        ++m_size;
    }

    void clear() {
        Node* current = m_head;
        while (current) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        m_head = m_tail = nullptr;
        m_size = 0;
    }

    void print_forward() const {
        Node* current = m_head;
        while (current) {
            std::cout << current->data << " <-> ";
            current = current->next;
        }
        std::cout << "null\n";
    }

    std::size_t size() const noexcept { return m_size; }
};

int main() {
    DoublyLinkedList<int> list;
    list.push_back(20);
    list.push_back(30);
    list.push_front(10);

    std::cout << "List contents (size " << list.size() << "): ";
    list.print_forward();

    return 0;
}