#include "bs_tree.cpp"
#include "rb_tree.h"

void testRBTree() {
    RBTree<int> tree;
    tree.insert(5);
    tree.insert(2);
    tree.insert(1);
    tree.insert(4);
    tree.insert(3);

    std::cout << "Pre: \n";
    tree.print(RBTree<int>::Order::PRE);

    std::cout << "In: \n";
    tree.print(RBTree<int>::Order::IN);

    std::cout << "Post: \n";
    tree.print(RBTree<int>::Order::POST);


//    RBTree<int> tree2(tree);

//    tree2.remove(2);

//    std::cout << "In Tree2: \n";
//    tree2.print(RBTree<int>::Order::IN);
//    std::cout << "In Tree1: \n";
//    tree.print(RBTree<int>::Order::IN);

//    typedef RBTree<int>::Node* IntNode;
//    IntNode e = tree.search(1);
//    IntNode p = e->parent;
//    IntNode g = tree2.grand_parent(e);
//    std::cout << "Number: " << e->value << " Parent: " << p->value << " Grandparent: " << g->value << "\n";
}


int main()
{
    //testBSTree();
    testRBTree();
    return 0;
}
