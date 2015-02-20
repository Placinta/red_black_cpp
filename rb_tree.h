#ifndef RB_TREE_H
#define RB_TREE_H

#include <iostream>
#include <functional>

template <typename T>
class RBTree {
public:
    enum class Color {RED, BLACK};
    enum class Order {PRE, IN, POST};

    typedef struct Node {
        T value;
        Node* left;
        Node* right;
        Node* parent;
        Color color;
        Node(T val) : value(val), left(nullptr), right(nullptr), parent(nullptr), color(Color::BLACK) {}
        Node(T val, Color c) : value(val), left(nullptr), right(nullptr), parent(nullptr), color(c) {}
        Node(T val, Node* p) : value(val), left(nullptr), right(nullptr), parent(p), color(Color::BLACK) {}
        Node(T val, Node* p, Color c) : value(val), left(nullptr), right(nullptr), parent(p), color(c) {}
        Node(T val, Node* l, Node* r, Node* p, Color c) : value(val), left(l), right(r), parent(p), color(c) {}
    } Node;

    Node* root;

    RBTree() : root(nullptr) {}

    void copyTree(Node* from, Node*& to, Node* parent) {
        if (from == nullptr) {
            return;
        }

        to = new Node(from->value, parent, from->color);
        copyTree(from->left, to->left, to);
        copyTree(from->right, to->right, to);
    }

    RBTree(const RBTree& rhs) {
        copyTree(rhs.root, root, nullptr);
    }

    RBTree& operator=(const RBTree& rhs) {
        if (this == &rhs) {
            return *this;
        }

        empty(root);
        copyTree(rhs.root, root, nullptr);

        return *this;
    }

    void empty(Node*& node) {
        if (node != nullptr) {
            empty(node->left);
            empty(node->right);
            delete node;
        }
        node = nullptr;
    }

    ~RBTree() {
        empty(this->root);
    }

    void insert(T val) {
        real_insert(root, val, nullptr);
    }

    void real_insert(Node*& node, T val, Node* parent) {
        if (node == nullptr) {
            node = new Node(val, parent);
        }
        else if (val < node->value) {
            real_insert(node->left, val, node);
        }
        else {
            real_insert(node->right, val, node);
        }
    }

    Node* search(T val) {
        return real_search(root, val);
    }

    Node* real_search (Node* node, T val) {
        while (node != nullptr) {
            if (node->value == val) {
                return node;
            }
            else if (val < node->value) {
                node = node->left;
            }
            else {
                node = node->right;
            }
        }
        return nullptr;
    }

    void remove(T val) {
        remove_real(root, val);
    }

    void remove_real(Node* node, T val) {
        if (node == nullptr) {
            return;
        }

        // Found the value we were looking for.
        if (node->value == val) {
            // Case 1 No children, just remove the node.
            if (node->left == nullptr && node->right == nullptr) {
                if (node->parent) {
                    // Clean up the parent's child pointer.
                    if (node->parent->left == node) {
                        node->parent->left = nullptr;
                    } else {
                        node->parent->right = nullptr;
                    }
                    delete node;
                } else {
                    // This is the root node we are deleting.
                    delete this->root;
                    this->root = nullptr;
                }
            }
            // Case 2 Only one child, remove the node, and replace it with the child.
            else if (node->left == nullptr || node->right == nullptr) {
                Node* child = node->left;
                if (node->right) {
                    child = node->right;
                }
                if (node->parent) {
                    // Clean up the parent's child pointer.
                    if (node->parent->left == node) {
                        node->parent->left = child;
                    } else {
                        node->parent->right = child;
                    }
                    delete node;
                } else {
                    // This is the root node we are deleting, just replace the child
                    this->root = child;
                    delete node;
                }
            }
            // Case 3 both children are present, choose successor, and replace current node with successor, then remove current node.
            else if (node->left != nullptr && node->right != nullptr) {
                Node* successor = node->right;
                while (successor->left) {
                    successor = successor->left;
                }
                T temp = node->value;
                node->value = successor->value;
                successor->value = temp;
                remove_real(node->right, temp);
            }
        } else {
            // Didn't find the value yet, recurse into subtrees to find the value.
            remove_real(node->left, val);
            remove_real(node->right, val);
        }
    }

    template <typename Func>
    auto traverse(Node* node, Func f, Order order = Order::PRE) -> decltype(f(node->value)) {
        if (node == nullptr) {
            return;
        }

        if (order == Order::PRE) {
            f(node->value);
        }

        if (node->left) {
            traverse(node->left, f, order);
        }

        if (order == Order::IN) {
            f(node->value);
        }

        if (node->right) {
            traverse(node->right, f, order);
        }

        if (order == Order::POST) {
            f(node->value);
        }
    }

    void print(Order order = Order::PRE) {
        return traverse(root, [](T value) {
            std::cout << value << "\n";
        }, order);
    }

};


#endif // RB_TREE_H
