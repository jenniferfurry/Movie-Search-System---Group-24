#include "RedBlackTree.h"
using namespace std;

RedBlackTree::~RedBlackTree() {
    clear(root);
}

void RedBlackTree::clear(Node* node) {
    if (node == nullptr) {
        return;
    }

    clear(node->left);
    clear(node->right);
    delete node;
}

Movie* RedBlackTree::search(const string& title) {
    Node* current = root;

    while (current != nullptr) {
        if (title < current->movie.title) {
            current = current->left;
        } else if (title > current->movie.title) {
            current = current->right;
        } else {
            return &current->movie;
        }
    }

    return nullptr;
}
