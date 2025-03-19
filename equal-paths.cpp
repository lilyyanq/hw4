#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

//check if all paths have the same height
bool checkEqualPaths(Node* root, int depth, int& leafDepth) {
    if (!root) 
    {
      return true; //empty node is always valid
    }
    // if leaf node, check depth
    if (!root->left && !root->right) 
    {
      if (leafDepth == -1)
      {
        leafDepth = depth; // Set first leaf's depth
      }
      return depth == leafDepth; // Compare with the stored depth
    }
    //check left and right children (only if they exist)
    bool leftValid = root->left ? checkEqualPaths(root->left, depth + 1, leafDepth) : true;
    bool rightValid = root->right ? checkEqualPaths(root->right, depth + 1, leafDepth) : true;

    return leftValid && rightValid;
}

bool equalPaths(Node* root) {
    if (!root) 
    {
        return true;//empty tree valid
    }
    int leafDepth = -1;
    if (root->left == nullptr && root->right == nullptr) 
    {
      return true;
    }
    return checkEqualPaths(root, 0, leafDepth);
}
