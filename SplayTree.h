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
        Node* findNode(int id);

        // DELETION FUNCTION
        void destroyTree(Node* node);

    public:

        // CONSTRUCTOR
        SplayTree(); // creates new empty splay tree

        // DECONSTRUCTOR
        ~SplayTree(); // deallocates memory when deleteing

        // REST OF TREE FUNCTIONS
        void insert(const Movie& movie); // inserts a new movie into tree with ID as key
        Movie* search(int id); // searches for movie by ID and returns pointer to its
        bool remove(int id); // removes move by ID -> returns true if removed and false if not found
        int size() const; // gets number of movies currently in tree
        void clear(); // removes all movies from tree so it is empty
};