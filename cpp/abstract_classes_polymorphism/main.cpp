#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <set>

#define DEBUG_TRACE true

struct Node {
    Node *prev;
    Node *next;
    int key;
    int value;
    Node(Node *p, Node *n, int k, int val) : prev(p), next(n), key(k), value(val) {};
    Node(int k, int val) : prev(NULL), next(NULL), key(k), value(val) {};
};

class Cache {
  public:
    virtual ~Cache() {
        for (auto &[_, node] : mp) {
            if (node != nullptr) {
                delete node;
            }
        }
    }
    virtual void set(int, int) = 0; // set function
    virtual int get(int) = 0;       // get function

  protected:
    std::map< int, Node * > mp; // map the key to the node in the linked list
    size_t cp{};                // capacity
    Node *tail{nullptr};        // double linked list tail pointer
    Node *head{nullptr};        // double linked list head pointer
};

class LRUCache : public Cache {
  public:
    explicit LRUCache(const int capacity) { cp = capacity; }
    void set(const int key, const int value) override {
        if (const auto it = mp.find(key); it != mp.end()) {
            // case where key is already present, we only insert value.
            it->second->value = value;
            return;
        }
        // case where we are inserting a new key
        Node *new_node = new Node(key, value);
        mp[key] = new_node;
        if (head == nullptr) {
            head = new_node;
        } else {
            head->prev = new_node;
            new_node->next = head;
            head = new_node;
        }
        if (tail == nullptr) {
            tail = new_node;
        } else if (mp.size() > cp) {
            // case where cache capacity is exceeded : erase from mp and shift tail
            Node *new_tail = tail->prev;
            new_tail->next = nullptr;
            mp.erase(tail->key);
            delete tail;
            tail = new_tail;
        }
    }
    int get(const int key) override {
        const auto it = mp.find(key);
        if (it == mp.end()) {
            return -1;
        }
        Node *node = it->second;
        if (node != head) {
            Node *prev = node->prev;
            if (node == tail) {
                prev->next = nullptr;
                tail = prev;
            } else {
                Node *next = node->next;
                prev->next = next;
                next->prev = prev;
            }
            node->prev = nullptr;
            node->next = head;
            head->prev = node;
            head = node;
        }
        return node->value;
    }
    void displayCache() const {
        std::cout << "=====================================" << std::endl;
        std::cout << "Capacity : " << cp << std::endl;
        std::cout << "Cache( ";
        Node *node = head;
        while (node != nullptr) {
            std::cout << " |key: " << node->key << " - value: " << node->value << "| ";
            node = node->next;
            if (node != nullptr) {
                std::cout << "->";
            }
        }
        std::cout << " )" << std::endl;
        std::cout << "=====================================" << std::endl;
    }
};

int main() {
    int n, capacity;
    std::cin >> n >> capacity;
    LRUCache l(capacity);
    for (int i = 0; i < n; i++) {
        std::string command;
        std::cin >> command;
        if (command == "get") {
            int key;
            std::cin >> key;
#if DEBUG_TRACE
            std::cout << command << " " << key << std::endl;
#endif
            std::cout << l.get(key) << std::endl;
#if DEBUG_TRACE
            l.displayCache();
#endif
        } else if (command == "set") {
            int key, value;
            std::cin >> key >> value;
#if DEBUG_TRACE
            std::cout << command << " " << key << " " << value << std::endl;
#endif
            l.set(key, value);
#if DEBUG_TRACE
            l.displayCache();
#endif
        }
    }
    return 0;
}
