#pragma once
#include "Movie.h"
#include <string>
using namespace std;

class RedBlackTree {
  private:
    struct Node {
      Movie movie;
      bool isRed = true;
      Node* left = nullptr;
      Node* right = nullptr;
      Node* parent = nullptr;
    };

    Node* root = nullptr;
  
    void rotateLeft(Node* n);
    void rotateRight(Node* n);
    void balance(Node* n);
    void clear(Node* n);

  public:
    ~RedBlackTree();

    void insert(const Movie& m);
    Movie* search(const string& title);
      
};
