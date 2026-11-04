#include "RedBlackTree.h"
using namespace std;

RedBlackTree::~RedBlackTree() {  //Destructor deletes all nodes when the tree destroyed
    clear(root);
}

void RedBlackTree::clear(Node* node) {  //Deletes children before deleting the parent
    if (node == nullptr) {
        return;
    }

    clear(node->left);
    clear(node->right);
    delete node;
}

Movie* RedBlackTree::search(const string& title) {  //searches for a movie using its exact title
    Node* current = root;       //start search at root

    while (current != nullptr) {     //continue moving left or right until title found
        if (title < current->movie.title) {
            current = current->left;
        } else if (title > current->movie.title) {
            current = current->right;
        } else {
            return &current->movie;
        }
    }

    return nullptr;     //title was not found
}


void RedBlackTree::rotateLeft(Node* node) {
    if (node == nullptr) {           //check for node and right child
        return;
    }
    if (node->right == nullptr) {
        return;
    }

    Node* rightChild = node->right;   // save right child before changing pointers
    node->right = rightChild->left;   // move child left side to node's right side

    if (rightChild->left != nullptr) { //update parent of subtree
        rightChild->left->parent = node;
    }

    rightChild->parent = node->parent; //connect right child to node's old parent

    if (node->parent == nullptr) {  //connect right child to the rest of the tree
        root = rightChild;
    } else if (node == node->parent->left) {
        node->parent->left = rightChild;
    } else {
        node->parent->right = rightChild;
    }

    rightChild->left = node;   //put original node on the left
    node->parent = rightChild;
}

void RedBlackTree::rotateRight(Node* node) {
    if (node == nullptr) {        //check for node and left child
        return;
    }
    if (node->left == nullptr) {
        return;
    }

    Node* leftChild = node->left;    //save left child before changing pointers
    node->left = leftChild->right;   //move child's right side to node's left side

    if (leftChild->right != nullptr) { //update parent of subtree
        leftChild->right->parent = node;
    }

    leftChild->parent = node->parent; //connect left child to node's old parent

    if (node->parent == nullptr) {    //connect left child to the rest of the tree
        root = leftChild;
    } else if (node == node->parent->left) {
        node->parent->left = leftChild;
    } else {
        node->parent->right = leftChild;
    }

    leftChild->right = node;        //put original node on the right
    node->parent = leftChild;
}
