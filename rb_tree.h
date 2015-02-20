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
        Node(T val) : value(val), left(nullptr), right(nullptr), parent(nullptr), color(Color::RED) {}
        Node(T val, Color c) : value(val), left(nullptr), right(nullptr), parent(nullptr), color(c) {}
        Node(T val, Node* p) : value(val), left(nullptr), right(nullptr), parent(p), color(Color::RED) {}
        Node(T val, Node* p, Color c) : value(val), left(nullptr), right(nullptr), parent(p), color(c) {}
        Node(T val, Node* l, Node* r, Node* p, Color c) : value(val), left(l), right(r), parent(p), color(c) {
            if (left != nullptr) {
                left->parent = this;
            }
            if (right != nullptr) {
                right->parent = this;
            }
        }
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

    Node* grand_parent(Node* node) {
        if (node == nullptr || node->parent == nullptr) {
            return nullptr;
        }
        return node->parent->parent;
    }

    Node* uncle(Node* node) {
        Node* g = grand_parent(node);
        if (g == nullptr) {
            return NULL;
        } else if (g->left == node->parent) {
            return g->right;
        } else {
            return g->left;
        }
    }

    void insert(T val) {
        real_insert(root, nullptr, val);
    }

    void real_insert(Node*& node, Node* parent, T val) {
        if (node == nullptr) {
            node = new Node(val, parent);
            // Rebalance tree.
            insert_case1(node);
        }
        else if (val < node->value) {
            real_insert(node->left, node, val);
        }
        else {
            real_insert(node->right, node, val);
        }
    }

    Color node_color(Node* n) {
        return n == nullptr ? Color::BLACK : n->color;
    }

    void insert_case1(Node* n) {
        if (n->parent == nullptr) {
            n->color = Color::BLACK;
        } else {
            insert_case2(n);
        }
    }

    void insert_case2(Node* n) {
        if (node_color(n->parent) == Color::BLACK) {
            return;
        } else {
            insert_case3(n);
        }
    }

    void insert_case3(Node* n) {
        Node* u = uncle(n);
        if (u != nullptr && node_color(u) == Color::RED) {
            n->parent->color = Color::BLACK;
            u->color = Color::BLACK;
            Node* g = grand_parent(n);
            g->color = Color::RED;
            insert_case1(g);
        } else {
            insert_case4(n);
        }
    }

    void replace_node(Node* from, Node* to) {
        if (from->parent == nullptr) {
            root = to;
        } else {
            if (from->parent->left == from) {
                from->parent->left = to;
            } else {
                from->parent->right = to;
            }
        }
        if (to != nullptr) {
            to->parent = from->parent;
        }
    }

    void rotate_left(Node* n) {
        Node* r = n->right;
        replace_node(n, r);
        n->right = r->left;
        if (r->left != nullptr) {
            r->left->parent = n;
        }
        r->left = n;
        n->parent = r;
    }

    void rotate_right(Node* n) {
        Node* l = n->left;
        replace_node(n, l);
        n->left = l->right;
        if (l->right != nullptr) {
            l->right->parent = n;
        }
        l->right = n;
        n->parent = l;
    }

    void insert_case4(Node* n) {
        Node* g = grand_parent(n);
        if (n == n->parent->right && n->parent == g->left) {
            rotate_left(n->parent);
            n = n->left;
        } else if (n == n->parent->left && n->parent == g->right) {
            rotate_right(n->parent);
            n = n->right;
        }
        insert_case5(n);
    }

    void insert_case5(Node* n) {
        Node* g = grand_parent(n);
        n->parent->color = Color::BLACK;
        g->color = Color::RED;
        if (n == n->parent->left) {
            rotate_right(g);
        } else {
            rotate_left(g);
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

    Node* maximum_in_tree(Node* node) {
        if (node == nullptr) {
            return nullptr;
        }

        while (node->right != nullptr) {
            node = node->right;
        }

        return node;
    }

    Node* sibling(Node* node) {
        if (node->parent->left == node) {
            return node->parent->right;
        } else {
            return node->parent->left;
        }
    }

    void remove_real(Node* start_node, T val) {
        Node* node = real_search(start_node, val);
        if (node == nullptr) {
            return;
        }

        if (node->left != nullptr && node->right != nullptr) {
            // Copy pred value into current node, then delete the predecessor.
            Node* pred = maximum_in_tree(node->left);
            node->value = pred->value;
            node = pred;
        }

        if (node->left == nullptr || node->right == nullptr) {
            Node* child = node->right == nullptr ? node->left : node->right;
            if (node_color(node) == Color::BLACK) {
                node->color = node_color(child);
                delete_case1(node);
            }
            replace_node(node, child);
            if (node->parent == nullptr && child != nullptr) {
                // Root should be black.
                child->color = Color::BLACK;
            }
        } else {
            // This is fishy, is this needed? It feels like it.
            replace_node(node, nullptr);
        }

        delete node;
    }

    void delete_case1(Node* node) {
        if (node->parent == nullptr) {
            return;
        }
        else {
            delete_case2(node);
        }
    }

    void delete_case2(Node* node) {
        Node* s = sibling(node);
        if (node_color(s) == Color::RED) {
            node->parent->color = Color::RED;
            s->color = Color::BLACK;
            if (node == node->parent->left) {
                rotate_left(node->parent);
            } else {
                rotate_right(node->parent);
            }
        }
        delete_case3(node);
    }

    void delete_case3(Node* node) {
        Node* s = sibling(node);
        if (
                node_color(node->parent) == Color::BLACK &&
                node_color(s) == Color::BLACK &&
                node_color(s->left) == Color::BLACK &&
                node_color(s->right) == Color::BLACK
           ) {
            s->color = Color::RED;
            delete_case1(node->parent);
        }
        else {
            delete_case4(node);
        }
    }

    void delete_case4(Node* node) {
        Node* s = sibling(node);
        if (
                node_color(node->parent) == Color::RED &&
                node_color(s) == Color::BLACK &&
                node_color(s->left) == Color::BLACK &&
                node_color(s->right) == Color::BLACK
           ) {
            s->color = Color::RED;
            node->parent->color = Color::BLACK;
        }
        else {
            delete_case5(node);
        }
    }

    void delete_case5(Node* node) {
        Node* s = sibling(node);
        if (node_color(s) == Color::BLACK) {
            if (
                    node == node->parent->left &&
                    node_color(s->right) == Color::BLACK &&
                    node_color(s->left) == Color::RED
               ) {
                s->color = Color::RED;
                s->left->color = Color::BLACK;
                rotate_right(s);
            } else if (
                   node == node->parent->right &&
                   node_color(s->left) == Color::BLACK &&
                   node_color(s->right) == Color::RED
              ) {
               s->color = Color::RED;
               s->right->color = Color::BLACK;
               rotate_left(s);
           }
        }
        delete_case6(node);
    }

    void delete_case6(Node* node) {
        Node* s = sibling(node);
        s->color = node->parent->color;
        node->parent->color = Color::BLACK;
        if (node == node->parent->left) {
            s->right->color = Color::BLACK;
            rotate_left(node->parent);
        } else {
            s->left->color = Color::BLACK;
            rotate_right(node->parent);
        }
    }

    template <typename Func>
    auto traverse(Node* node, Func f, Order order = Order::PRE) -> decltype(f(node)) {
        if (node == nullptr) {
            return;
        }

        if (order == Order::PRE) {
            f(node);
        }

        if (node->left) {
            traverse(node->left, f, order);
        }

        if (order == Order::IN) {
            f(node);
        }

        if (node->right) {
            traverse(node->right, f, order);
        }

        if (order == Order::POST) {
            f(node);
        }
    }

    template <typename Enumeration>
    auto as_integer(Enumeration const value)
        -> typename std::underlying_type<Enumeration>::type
    {
        return static_cast<typename std::underlying_type<Enumeration>::type>(value);
    }

    void print(Order order = Order::PRE) {
        return traverse(root, [](Node* node) {
            int color = static_cast<typename std::underlying_type<Color>::type>(node->color);
            char color_name = ' ';
            if (color == 0) {
                color_name = 'R';
            } else {
                color_name = 'B';
            }
            std::cout << node->value << ":" << color_name << "\n";
        }, order);
    }

};


#endif // RB_TREE_H
