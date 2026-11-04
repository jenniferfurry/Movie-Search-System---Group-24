#include "RedBlackTree.h"
using namespace std;

void RedBlackTree::insert(const Movie& movie) {    //inserts a movie by title
    Node* newNode = new Node;    //creates a new node and stores the movie
    newNode->movie = movie;

    if (root == nullptr) {       //first node becomes black root
        root = newNode;
        root->isRed = false;
        return;
    }

    Node* current = root;
    Node* parent = nullptr;

    while (current != nullptr) {    //move through tree until empty position found
        parent = current;

        if (movie.title < current->movie.title) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    newNode->parent = parent;        //connect new node to last node visited

    if (movie.title < parent->movie.title) {    //place new node on the correct side of parent
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }

    balance(newNode);                //balance red-black-tree
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


void RedBlackTree::balance(Node* node) {    //balance red-black-tree
    if (node->parent == nullptr) {    // If node is root, make it black
        node->isRed = false;
        return;
    }

    if (!node->parent->isRed) {        // No balancing is needed if parent is black
        return;
    }

    Node* parent = node->parent;        //Intialize parent, grandparent, and uncle
    Node* grandparent = parent->parent;
    Node* uncle = nullptr;

    if (parent == grandparent->left) {    //find uncle
        uncle = grandparent->right;
    } else {
        uncle = grandparent->left;
    }

    if (uncle != nullptr && uncle->isRed) {    //if uncle red, flip colors and check grandparent
        parent->isRed = false;
        uncle->isRed = false;
        grandparent->isRed = true;

        balance(grandparent);
        return;
    }

    if (node == parent->right && parent == grandparent->left) {    //fix left-right case
        rotateLeft(parent);
        node = parent;
        parent = node->parent;
    } else if (node == parent->left && parent == grandparent->right) {    //fix right-left case
        rotateRight(parent);
        node = parent;
        parent = node->parent;
    }

    parent->isRed = false;        //change colors before final rotation
    grandparent->isRed = true;

    if (node == parent->left) {    //rotate grandparent in correct direction for left-left or right-right cases
        rotateRight(grandparent);
    } else {
        rotateLeft(grandparent);
    }
}

void RedBlackTree::clear(Node* node) {  //Deletes children before deleting the parent
    if (node == nullptr) {
        return;
    }

    clear(node->left);
    clear(node->right);
    delete node;
}

RedBlackTree::~RedBlackTree() {  //Destructor deletes all nodes when tree is destroyed
    clear(root);
}
