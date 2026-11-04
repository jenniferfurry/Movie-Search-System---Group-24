#include "SplayTree.h"


// NODE CONSTRUCTOR
SplayTree::Node::Node(const Movie& m) {
    movie = m; // create node using existing movie record
    left = nullptr; // empty so all point to nothing
    right = nullptr;
    parent = nullptr;
};

// CONSTRUCTOR
SplayTree::SplayTree() {
    root = nullptr; // empty so point to nothing
    treeSize = 0; // set size to 0 since empty
}

// DECONSTRUCTOR
SplayTree::~SplayTree() {
    destroyTree(root); // starting at root reaches every other node in tree
}

// DESTROY TREE FUNCTION
void SplayTree::destroyTree(Node* node) { // left->right->parent
    if (node == nullptr) {
        return; // no more nodes so stop
    }
    destroyTree(node->left); // recursively visit left and right children
    destroyTree(node->right);

    delete node; // delete current node (deletes children from bottom up to root)
}

// CLEAR FUNCTION
void SplayTree::clear() {
    destroyTree(root); // delete all nodes in tree
    root = nullptr; // set root to null so tree is empty
    treeSize = 0; // reset size to 0
}

// SIZE FUNCTION
int SplayTree::size() const {
    return treeSize; // return number of movies in tree
}

// ROTATION FUNCTIONS
void SplayTree::rotateLeft(Node* node){
    if (node == nullptr || node->right == nullptr) {
        return; // cannot perform left rotation if node or its right child is null
    }

    Node* rightChild = node->right; // store right child of node
    node->right = rightChild->left; // move right child's left subtree to node's right subtree
    
    if (rightChild->left != nullptr) {
        rightChild->left->parent = node; // update parent of right child's left subtree
    }

    rightChild->left = node; // make node the left child of rightChild
    rightChild->parent = node->parent; // update parent of rightChild
    node->parent = rightChild; // update parent of node

    if (rightChild->parent == nullptr) {
        root = rightChild; // update root if node was root
    } else if (node == rightChild->parent->left) {
        rightChild->parent->left = rightChild; // update parent's left child
    } else {
        rightChild->parent->right = rightChild; // update parent's right child
    }
}

void SplayTree::rotateRight(Node* node){
    if (node == nullptr || node->left == nullptr) {
        return; // cannot perform right rotation if node or its left child is null
    }

    Node* leftChild = node->left; // store left child of node
    node->left = leftChild->right; // move left child's right subtree to node's left subtree
    
    if (leftChild->right != nullptr) {
        leftChild->right->parent = node; // update parent of left child's right subtree
    }

    leftChild->right = node; // make node the right child of leftChild
    leftChild->parent = node->parent; // update parent of leftChild
    node->parent = leftChild; // update parent of node

    if (leftChild->parent == nullptr) {
        root = leftChild; // update root if node was root
    } else if (node == leftChild->parent->right) {
        leftChild->parent->right = leftChild; // update parent's right child
    } else {
        leftChild->parent->left = leftChild; // update parent's left child
    }
}

// SPLAY FUNCTION
void SplayTree::splay(Node* node) {
    if (node == nullptr) {
        return; // cannot splay a null node
    }

    while (node->parent != nullptr) { // while node is not root
        Node* parent = node->parent;
        Node* grandparent = parent->parent;

        if (grandparent == nullptr) { // node's parent is root
            if (node == parent->left) {
                rotateRight(parent); // zig rotation
            } else {
                rotateLeft(parent); // zag rotation
            }
        } else if (node == parent->left && parent == grandparent->left) {
            rotateRight(grandparent); // zig-zig rotation
            rotateRight(parent);
        } else if (node == parent->right && parent == grandparent->right) {
            rotateLeft(grandparent); // zag-zag rotation
            rotateLeft(parent);
        } else if (node == parent->right && parent == grandparent->left) {
            rotateLeft(parent); // zig-zag rotation
            rotateRight(grandparent);
        } else {
            rotateRight(parent); // zag-zig rotation
            rotateLeft(grandparent);
        }
    }
}

// FIND NODE FUNCTION
SplayTree::Node* SplayTree::findNode(int id) {
    Node* current = root;
    while (current != nullptr) {
        if (id < current->movie.id) { // if id is smaller than current node's id, go left
            current = current->left;
        } else if (id > current->movie.id) { // if id is larger than current node's id, go right
            current = current->right;
        } else { // if id matches current node's id, return the node
            return current; // node found
        }
    }
    return nullptr; // node not found
}

// INSERT FUNCTION
void SplayTree::insert(const Movie& movie) {
    Node* newNode = new Node(movie); // create a new node with the movie record
    if (root == nullptr) { // if tree is empty, set new node as root
        root = newNode;
        treeSize++;
        return;
    }

    Node* current = root;
    Node* parent = nullptr;

    while (current != nullptr) { // traverse the tree to find the correct position for the new node
        parent = current;
        if (movie.id < current->movie.id) {
            current = current->left; // go left if new movie's id is smaller
        } else if (movie.id > current->movie.id) {
            current = current->right; // go right if new movie's id is larger
        } else {
            delete newNode; // duplicate id found, do not insert
            return;
        }
    }

    newNode->parent = parent; // set the parent of the new node
    if (movie.id < parent->movie.id) {
        parent->left = newNode; // insert as left child
    } else {
        parent->right = newNode; // insert as right child
    }

    splay(newNode); // splay the newly inserted node to the root
    treeSize++; // increment the size of the tree
}

// SEARCH FUNCTION
Movie* SplayTree::search(int id) {
    Node* node = findNode(id); // find the node with the given id
    if (node != nullptr) { // if node is found, splay it to the root
        splay(node); // splay the found node to the root
        return &(node->movie); // return pointer to the movie record
    }
    return nullptr;
}

// REMOVE FUNCTION
bool SplayTree::remove(int id) {
    Node* node = findNode(id);
    if (node == nullptr) {
        return false; // node not found
    }

    splay(node); // splay the node to the root
    
    Node* leftSubtree = node->left; // store left subtree
    Node* rightSubtree = node->right; // store right subtree

    // Remove the node (simplified - actual implementation may vary)
    if (leftSubtree == nullptr) {
        leftSubtree->parent = nullptr; // update parent of left subtree to null
    }
    if (rightSubtree == nullptr) {
        rightSubtree->parent = nullptr; // update parent of right subtree to null
    }

    delete node; // delete the node

    if (leftSubtree == nullptr) {
        root = rightSubtree; // set right subtree as new root
    } else {
        root = leftSubtree; // set left subtree as new root if left is null
        Node* maxNode = root;

        while (maxNode->right != nullptr) { // find the maximum node in the left subtree
            maxNode = maxNode->right;
        }
        splay(maxNode); // splay the maximum node to the root
        root->right = rightSubtree; // attach the right subtree to the new root

        if (rightSubtree != nullptr) {
            rightSubtree->parent = root; // update parent of right subtree
        }
    }
    treeSize--; // decrement the size of the tree
    return true;
}