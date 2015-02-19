#include <iostream>
#include <functional>

template <typename T>
class BSTree {
public:
    typedef struct Node {
        T value;
        Node* left;
        Node* right;
        Node(T val) : value(val), left(nullptr), right(nullptr) {}
        Node(T val, Node* l, Node* r) : value(val), left(l), right(r) {}
    } Node;

    Node* root;

    enum class Order {PRE, IN, POST};
    typedef Node* ParentP;
    typedef Node* ChildP;
    typedef std::pair<ChildP, ParentP> ChildAndParentP;

    BSTree() : root(nullptr) {

    }

    void copyTree(Node* from, Node*& to) {
        if (from == nullptr) {
            return;
        }

        to = new Node(from->value);
        copyTree(from->left, to->left);
        copyTree(from->right, to->right);
    }


    BSTree(const BSTree& rhs) {
        copyTree(rhs.root, root);
    }

    BSTree& operator=(const BSTree& rhs) {
        if (this == &rhs) {
            return *this;
        }

        empty(root);
        copyTree(rhs.root, root);

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

    ~BSTree() {
        empty(this->root);
    }

    void insert(T val) {
        real_insert(root, val);
    }

    void real_insert(Node*& node, T val) {
        if (node == nullptr) {
            node = new Node(val);
        }
        else if (val < node->value) {
            real_insert(node->left, val);
        }
        else {
            real_insert(node->right, val);
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

    ChildAndParentP search_with_parent(T val) {
        return real_search_with_parent(ChildAndParentP(root, nullptr), val);
    }

    ChildAndParentP real_search_with_parent(ChildAndParentP child_and_parent, T val) {
        while (child_and_parent.first != nullptr) {
            if (child_and_parent.first->value == val) {
                return child_and_parent;
            }
            else if (val < child_and_parent.first->value) {
                child_and_parent.second = child_and_parent.first;
                child_and_parent.first = child_and_parent.first->left;
            }
            else {
                child_and_parent.second = child_and_parent.first;
                child_and_parent.first = child_and_parent.first->right;
            }
        }
        return ChildAndParentP(nullptr, nullptr);
    }

    void remove(T val) {
        remove_real(nullptr, root, val);
    }

    void remove_real(Node* parent, Node* node, T val) {
        if (node == nullptr) {
            return;
        }

        // Found the value we were looking for.
        if (node->value == val) {
            // Case 1 No children, just remove the node.
            if (node->left == nullptr && node->right == nullptr) {
                if (parent) {
                    // Clean up the parent's child pointer.
                    if (parent->left == node) {
                        parent->left = nullptr;
                    } else {
                        parent->right = nullptr;
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
                if (parent) {
                    // Clean up the parent's child pointer.
                    if (parent->left == node) {
                        parent->left = child;
                    } else {
                        parent->right = child;
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
                remove_real(node, node->right, temp);
            }
        } else {
            // Didn't find the value yet, recurse into subtrees to find the value.
            remove_real(node, node->left, val);
            remove_real(node, node->right, val);
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
        return BSTree::traverse(root, [](T value) {
            std::cout << value << "\n";
        }, order);
    }

};

int main()
{
    BSTree<int> tree;
    tree.insert(5);
    tree.insert(2);
    tree.insert(1);
    tree.insert(4);
    tree.insert(3);

    std::cout << "Pre: \n";
    tree.print(BSTree<int>::Order::PRE);

    std::cout << "In: \n";
    tree.print(BSTree<int>::Order::IN);

    std::cout << "Post: \n";
    tree.print(BSTree<int>::Order::POST);

    BSTree<int>::ChildAndParentP a = tree.search_with_parent(3);
    std::cout << "Child val: " << a.first->value << "Parent address: " << a.second << "\n";
    if (a.second != nullptr) {
        std::cout << "Parent val: " << a.second->value << "\n";
    }

    BSTree<int> tree2(tree);

    tree2.remove(2);

    std::cout << "In Tree2: \n";
    tree2.print(BSTree<int>::Order::IN);
    std::cout << "In Tree1: \n";
    tree.print(BSTree<int>::Order::IN);

    return 0;
}
