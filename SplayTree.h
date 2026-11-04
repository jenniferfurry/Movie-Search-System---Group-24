#pragma once

#include "Movie.h"

class SplayTree {
    private:

        struct Node {
            Movie movie; // object of the Movie struct
            Node* left; // pointer to left node (smaller ID)
            Node* right; // pointer to right node (larger ID)
            Node* parent; // pointer to the parent fo current node

            // CONSTRUCTOR
            Node(const Movie& m); // creates node using an existing Movie record
        };

        Node* root; // pointer to root of tree
        int treeSize; // tracks how many movie records are currently in tree

        // ROTATION FUNCTIONS
        void rotateLeft(Node* node);
        void rotateRight(Node* node);

        // SPLAYING FUNCTION
        void splay(Node* node);

        // SEARCH HELPER
        Node* findNode(const std::string& title);

        // DELETION FUNCTION
        void destroyTree(Node* node);

    public:

        // CONSTRUCTOR
        SplayTree(); // creates new empty splay tree

        // DECONSTRUCTOR
        ~SplayTree(); // deallocates memory when deleteing

        // REST OF TREE FUNCTIONS
        void insert(const Movie& movie); // inserts a new movie into tree with title as key
        Movie* search(const std::string& title); // searches for movie by title and returns most popular
        bool remove(const std::string& title); // removes movie by title -> returns true if removed and false if not found
        int size() const; // gets number of movies currently in tree
        void clear(); // removes all movies from tree so it is empty
};