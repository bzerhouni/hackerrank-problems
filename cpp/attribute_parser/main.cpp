#include <cmath>
#include <limits>
#include <vector>
#include <map>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <optional>

#define DEBUG_TRACE true

class Node {
  public:
    Node(const std::string_view node_key, Node *parent)
        : m_parent(parent), m_node_key(std::string(node_key)) {}
    const std::string &get_key() const { return m_node_key; }
    void add_child(std::unique_ptr< Node > child) {
        auto key = child->m_node_key;
        m_children.emplace(std::move(key), std::move(child));
    }
    void add_attribute(std::string_view key, std::string_view value) { m_attributes.emplace(key, value); }
    Node *get_parent() const { return m_parent; }
    Node *get_child(const std::string &key) const {
        const auto it = m_children.find(key);
        if (it == m_children.end()) {
            return nullptr;
        }
        return it->second.get();
    }
    std::optional< std::string_view > get_attribute(const std::string &key) const {
        const auto it = m_attributes.find(key);
        if (it == m_attributes.end()) {
            return std::nullopt;
        }
        return it->second;
    }
    void display_node(const size_t level = 0) const {
        std::string left_space;
        for (size_t i = 0; i < level; i++) {
            left_space += "    ";
        }
        std::cout << left_space << "Node(key=\"" << m_node_key << "\"";
        if (!m_attributes.empty()) {
            std::cout << ", attributes=[";
            for (const auto &[name, value] : m_attributes) {
                std::cout << name << ": " << value << ", ";
            }
            std::cout << "]";
        }
        std::cout << ")" << std::endl;
        if (!m_children.empty()) {
            std::cout << left_space << "Children:" << std::endl;
            for (const auto &[_, child] : m_children) {
                child->display_node(level + 1);
            }
        }
    }

  private:
    Node *const m_parent;
    std::string m_node_key;
    std::map< std::string, std::unique_ptr< Node > > m_children;
    std::map< std::string, std::string > m_attributes;
};

using Tree = std::vector< std::unique_ptr< Node > >;

std::string sanitize_value(std::string_view s) {
    size_t start = s.find('"');
    if (start == std::string::npos) {
        start = 0;
    } else {
        ++start; // skip the opening quote
    }

    std::string result;
    for (size_t i = start; i < s.size() && s[i] != '"' && s[i] != '>'; ++i) {
        result += s[i];
    }
    return result;
}

Node *read_tree(Tree &tree, const std::string &line, Node *parent) {
    std::istringstream iss(line);
    std::string token;
    Node *current_node = parent;
    while (std::getline(iss, token, ' ')) {
        if (token.rfind("</", 0) == 0) {
            current_node = current_node->get_parent();
        } else if (token.rfind('<', 0) == 0) {
            // new tag
            auto child = std::make_unique< Node >(sanitize_value(token.substr(1)), current_node);
            Node *child_ptr = child.get();
            if (parent == nullptr) {
                tree.emplace_back(std::move(child));
            } else {
                current_node->add_child(std::move(child));
            }
            current_node = child_ptr;
        } else {
            // read attributes
            std::string key = token;
            std::getline(iss, token, ' '); // =
            std::string value;
            std::getline(iss, value, ' ');
            current_node->add_attribute(key, sanitize_value(value));
        }
    }
    return current_node;
}

void display_tree(const Tree &tree) {
    for (const auto &node : tree) {
        node->display_node();
    }
}

void execute_query(const std::string &query, const Tree &tree) {
    std::istringstream iss(query);
    std::string token;
    Node *current_node = nullptr;
    while (std::getline(iss, token, '.')) {
        bool token_exists = false;
        const size_t attribute_delimiter = token.find('~');
        std::string attribute_key;
        if (attribute_delimiter != std::string::npos) {
            attribute_key = token.substr(attribute_delimiter + 1);
            token = token.substr(0, attribute_delimiter);
        }
        if (current_node == nullptr) {
            for (const auto &node : tree) {
                if (node->get_key() == token) {
                    current_node = node.get();
                    token_exists = true;
                }
            }
        } else {
            if (Node *child = current_node->get_child(token); child != nullptr) {
                token_exists = true;
                current_node = child;
            }
        }
        if (token_exists && !attribute_key.empty()) {
            if (const auto val = current_node->get_attribute(attribute_key); val.has_value()) {
                std::cout << val.value() << std::endl;
                break;
            }
            token_exists = false;
        }
        if (not token_exists) {
            std::cout << "Not Found!" << std::endl;
            break;
        }
    }
}

int main() {
    /* Read input from STDIN. Print output to STDOUT */
    int N;
    int Q;
    std::cin >> N;
    std::cin >> Q;
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    size_t line_num = 0;
    Tree tree;
    Node *current_node = nullptr;
    for (std::string line; std::getline(std::cin, line); ++line_num) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (line_num < N) {
            current_node = read_tree(tree, line, current_node);
        } else {
            execute_query(line, tree);
        }
    }
#if DEBUG_TRACE
    std::cout << std::endl;
    display_tree(tree);
#endif
    return 0;
}
