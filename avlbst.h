#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
   
    int8_t balance_;    // effectively a signed char, signed 8-bit integer (-128 to 127)
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0) // Initializes balance_ to 0.
{

}

template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode. Returns the node’s height.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.set the height
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
* Adds diff to current balance. (Used during tree rebalancing.)
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
   
    void rotateRight(AVLNode<Key, Value>* node);
    void rotateLeft(AVLNode<Key, Value>* node);
    void insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n);
    void removeFix(AVLNode<Key, Value>* p, int8_t diff);
    
};



// Helper function: Rotates subtree to the right
template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node)
{
    AVLNode<Key, Value>* leftChild = node->getLeft();
    if (!leftChild) return;

    // two directions link
    node->setLeft(leftChild->getRight());
    if (leftChild->getRight()) {
        leftChild->getRight()->setParent(node);
    }

    leftChild->setParent(node->getParent());
    if (!node->getParent()) {
        this->root_ = leftChild; // if node is root, then the new root will be leftChild
    }
    else if (node == node->getParent()->getRight()) {
        node->getParent()->setRight(leftChild);
    }
    else {
        node->getParent()->setLeft(leftChild);
    }

    // two directions link
    leftChild->setRight(node);
    node->setParent(leftChild);
}
// Helper function: Rotates subtree to the left
template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node)
{
    AVLNode<Key, Value>* rightChild = node->getRight();
    if (!rightChild) return;

    node->setRight(rightChild->getLeft());
    if (rightChild->getLeft()) 
    {
        rightChild->getLeft()->setParent(node);
    }

    rightChild->setParent(node->getParent());
    if (!node->getParent()) 
    {
        this->root_ = rightChild;
    }
    else if (node == node->getParent()->getLeft()) 
    {
        node->getParent()->setLeft(rightChild);
    }
    else {
        node->getParent()->setRight(rightChild);
    }

    rightChild->setLeft(node);
    node->setParent(rightChild);
}

template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value>& new_item)
{
    if (this->root_ == nullptr) {
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        return;
    }
    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* parent = nullptr;

    while (curr != nullptr) {
        if (new_item.first == curr->getKey()) {
            curr->setValue(new_item.second);
            return;
        }

        parent = curr;

        if (new_item.first < curr->getKey())
            curr = curr->getLeft();
        else
            curr = curr->getRight();
    }
    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);

    if (new_item.first < parent->getKey())
        parent->setLeft(newNode);
    else
        parent->setRight(newNode);
    // Fix balance
    if (parent->getBalance() == -1 || parent->getBalance() == 1) {
        parent->setBalance(0); 
    }
    else if (parent->getBalance() == 0) 
    {
      if (newNode == parent->getLeft())
          parent->setBalance(-1);
      else
          parent->setBalance(1);
      insertFix(parent, newNode);
    }
}
/*
Takes the parent (p) of the newly inserted node n,
Checks if the grandparent (g) is now out of balance,
Performs the appropriate rotations to fix the balance,
And continues recursively upward if needed 
*/
template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n)
{
    if (p == nullptr || p->getParent() == nullptr) return;

    AVLNode<Key, Value>* g = p->getParent();
    if (p == g->getLeft()) {  // parent node is left child
        g->updateBalance(-1);
        if (g->getBalance() == 0) return;
        else if (g->getBalance() == -1) {
            insertFix(g, p);
        }
        else if (g->getBalance() == -2) 
        {
          if (n == p->getLeft()) 
          { 
            rotateRight(g);
            p->setBalance(0);
            g->setBalance(0);
          }
          else 
          { 
            rotateLeft(p);
            rotateRight(g);

            if (n->getBalance() == -1) { // n is left heavy
                p->setBalance(0);
                g->setBalance(1);
            }
            else if (n->getBalance() == 0) {
                p->setBalance(0);
                g->setBalance(0);
            }
            else { // +1, n is right heavy
                p->setBalance(-1);
                g->setBalance(0);
            }
            n->setBalance(0);// after rotation, n is the new subtree root and is balanced
          }
        }
    }
    else 
    { // p is right child
      g->updateBalance(1);

      if (g->getBalance() == 0) return;
      else if (g->getBalance() == 1) 
      {
        insertFix(g, p);
      }
      else if (g->getBalance() == 2) 
      {
        if (n == p->getRight()) 
        {
          rotateLeft(g);
          p->setBalance(0);
          g->setBalance(0);
        }
        else 
        {
          rotateRight(p);
          rotateLeft(g);

          if (n->getBalance() == 1) {
              p->setBalance(0);
              g->setBalance(-1);
          }
          else if (n->getBalance() == 0) {
              p->setBalance(0);
              g->setBalance(0);
          }
          else { // -1
              p->setBalance(1);
              g->setBalance(0);
          }
          n->setBalance(0);
        }
      }
    }
}
/*
Find the node with the given key
Remove it from the AVL tree
Restore the AVL balance using removeFix(...)
*/
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if (!node) return;

    AVLNode<Key, Value>* parent = node->getParent();
    int8_t diff = 0;

    // If node has two children, swap with predecessor
    if (node->getLeft() && node->getRight()) 
    {
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(node));
        nodeSwap(node, pred);
        parent = node->getParent(); // node now points to predecessor
    }

    // Now node has at most one child
    AVLNode<Key, Value>* child = nullptr;
    if (node->getLeft()) 
    {
        child = node->getLeft();
    }
    else if (node->getRight()) 
    {
        child = node->getRight();
    }
    if (child) 
    {
        child->setParent(parent);
    }

    // Reconnect parent's child pointer
    if (!parent) 
    {
        // node is root
        this->root_ = child;
    }
    else 
    {
      if (parent->getLeft() == node) 
      {
          parent->setLeft(child);
          diff = 1;
      }
      else {
          parent->setRight(child);
          diff = -1;
      }
    }
    delete node;
    removeFix(parent, diff);
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* n, int8_t diff)
{
    if (n == nullptr) return;

    AVLNode<Key, Value>* parent = n->getParent();
    int8_t nextDiff = 0;
    if (parent != nullptr) 
    {
      nextDiff = (n == parent->getLeft()) ? 1 : -1;
    }
    n->updateBalance(diff);
    int8_t balance = n->getBalance();
    if (balance == -2) 
    {
      AVLNode<Key, Value>* c = n->getLeft();
      if (c == nullptr) return;
      if (c->getBalance() == -1) 
      {
        rotateRight(n);
        n->setBalance(0);
        c->setBalance(0);
        removeFix(parent, nextDiff);
      }
      else if (c->getBalance() == 0) 
      {
        rotateRight(n);
        n->setBalance(-1);
        c->setBalance(+1);
        return; // stop
      }
      else 
      { 
        AVLNode<Key, Value>* g = c->getRight();
        rotateLeft(c);
        rotateRight(n);

        int8_t gBal = g->getBalance();
        if (gBal == +1) {
            n->setBalance(0);
            c->setBalance(-1);
        }
        else if (gBal == 0) {
            n->setBalance(0);
            c->setBalance(0);
        }
        else { // -1
            n->setBalance(+1);
            c->setBalance(0);
        }
        g->setBalance(0);
        removeFix(parent, nextDiff);
      }
    }
    else if (balance == +2) 
    {
      AVLNode<Key, Value>* c = n->getRight();
      if (c == nullptr) return; 
      if (c->getBalance() == +1) 
      {
        rotateLeft(n);
        n->setBalance(0);
        c->setBalance(0);
        removeFix(parent, nextDiff);
      }
      else if (c->getBalance() == 0) 
      {
        rotateLeft(n);
        n->setBalance(+1);
        c->setBalance(-1);
        return; // stop
      }
      else 
      { 
        AVLNode<Key, Value>* g = c->getLeft();
        rotateRight(c);
        rotateLeft(n);

        int8_t gBal = g->getBalance();
        if (gBal == -1) {
            n->setBalance(0);
            c->setBalance(+1);
        }
        else if (gBal == 0) {
            n->setBalance(0);
            c->setBalance(0);
        }
        else { // +1
            n->setBalance(-1);
            c->setBalance(0);
        }
        g->setBalance(0);
        removeFix(parent, nextDiff);
      }
  }
  else if (balance == -1 || balance == +1) 
  {
    return;
  }
  else if (balance == 0) 
  {
    removeFix(parent, nextDiff);
  }
}

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */


// Swaps positions of two nodes.
// Also swaps their balance values, because balance info is tied to physical structure.
template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    // use base class nodeSwap function to swap two nodes first
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    // swaps their balance (height) too
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

#endif