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
    int8_t balance_;    // effectively a signed char
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
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
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

    void rotateLeft(AVLNode<Key, Value>* node);
    void rotateRight(AVLNode<Key, Value>* node);
    void insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* child);
		void removeFix(AVLNode<Key, Value>* node, int8_t diff);
		AVLNode<Key, Value>* getSuccessor(AVLNode<Key, Value>* node);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
 
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO

		AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
		newNode->setBalance(0);
		newNode->setRight(nullptr);
		newNode->setLeft(nullptr);
		
		if(this->root_ == nullptr) {
			this->root_ = newNode;
			return;
		}

		AVLNode<Key, Value>* parent = nullptr;
		AVLNode<Key, Value>* next = static_cast<AVLNode<Key, Value>*>(this->root_);
		
		while(true){
			parent = next;
			if(new_item.first == parent->getKey()){
				parent->setValue(new_item.second);
				delete newNode;
				return;
			}
			else if(new_item.first < parent->getKey()){
				if(parent->getLeft() == nullptr){
					parent->setLeft(newNode);
					newNode->setParent(parent);
					break;
				}
				next = parent->getLeft();
			}
			else {
				if(parent->getRight() == nullptr){
					parent->setRight(newNode);
					newNode->setParent(parent);
					break;
				}
				next = parent->getRight();
			}
		}

		if((parent->getBalance() == -1) or (parent->getBalance() == 1)){
			parent->setBalance(0);
			return;
		}
		else{
			if(parent->getLeft() == newNode){
				parent->setBalance(-1);
			}
			else {
				parent->setBalance(1);
			}
			insertFix(parent, newNode);
		}
		
}



/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */

 template<class Key, class Value>
 void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* child)
 {
	if((parent == nullptr) or (parent->getParent() == nullptr)) return;

	AVLNode<Key, Value> *grandparent = parent->getParent();

	if(parent == grandparent->getLeft()){

		grandparent->setBalance(grandparent->getBalance() - 1);

		if(grandparent->getBalance() == 0) return; //already balanced

		if(grandparent->getBalance() == -1){
			insertFix(grandparent, parent);
			return;
		}

		if(child == parent->getLeft()){
			rotateRight(grandparent);
			parent->setBalance(0);
			grandparent->setBalance(0);
		}

		else {
			rotateLeft(parent);
			rotateRight(grandparent);

			if(child->getBalance() == -1){
				parent->setBalance(0);
				grandparent->setBalance(1);
			}
			else if(child->getBalance() == 0){
				parent->setBalance(0);
				grandparent->setBalance(0);

			} else {
				parent->setBalance(-1);
				grandparent->setBalance(0);
			}
			child->setBalance(0);
		}
	}

	else{
		grandparent->setBalance(grandparent->getBalance() +1);

		if(grandparent->getBalance() == 0) return;

		if(grandparent->getBalance() == 1){
			insertFix(grandparent, parent);
			return;
		}

		if(child == parent->getRight()){
			rotateLeft(grandparent);
			parent->setBalance(0);
			grandparent->setBalance(0);
		}

		else{
			rotateRight(parent);
			rotateLeft(grandparent);

			if(child->getBalance() == 1){
				parent->setBalance(0);
				grandparent->setBalance(-1);
			}

			else if(child->getBalance() == 0){
				parent->setBalance(0);
				grandparent->setBalance(0);
			}

			else {
				parent->setBalance(1);
				grandparent->setBalance(0);
			}
			child->setBalance(0);
		}
	}

 }

template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
	
	

    // TODO
		
		if(this->root_ == nullptr) return;

		AVLNode<Key, Value>* nodeToRemove = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
		if(nodeToRemove == nullptr) return;

		if((nodeToRemove->getLeft() != nullptr) and (nodeToRemove->getRight() != nullptr)){
			AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(nodeToRemove));
			nodeSwap(nodeToRemove, pred);
		}

		AVLNode<Key, Value>* parent = nodeToRemove->getParent();
		AVLNode<Key, Value>* child = (nodeToRemove->getLeft() != nullptr) ? nodeToRemove->getLeft() : nodeToRemove->getRight();

		int8_t diff = 0;
		bool isLeftChild = false;

		if(parent != nullptr){
			isLeftChild = (parent->getLeft() == nodeToRemove);
			if(isLeftChild){
				diff = 1;
			} else{
				diff = -1;
			}
		}

		if(parent == nullptr){
			this->root_ = child;
			if(child != nullptr){
				child->setParent(nullptr);
			}
		} else {
			if(isLeftChild){
				parent->setLeft(child);
			} else {
				parent->setRight(child);
			}

			if(child != nullptr){
				child->setParent(parent);
			}
		}

		delete nodeToRemove;

		if(parent != nullptr){
			removeFix(parent, diff);
		}
		
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* node, int8_t diff){
	

	
	if(node == nullptr) return;

	AVLNode<Key, Value>* parent = node->getParent();
	int8_t nextDiff = 0;

	if(parent != nullptr){
		bool isLeftChild = (parent->getLeft() == node);
		nextDiff = isLeftChild ? 1 : -1;
	}

	node->updateBalance(diff);

	if(node->getBalance() == 0){
		if(parent != nullptr){
			removeFix(parent, nextDiff);
		}
	}

	else if((node->getBalance() == 1) or (node->getBalance() == -1)) return;

	else {
		if(node->getBalance() == 2){
			AVLNode<Key, Value>* rightChild = node->getRight();

			if(rightChild->getBalance() == -1){
				rotateRight(rightChild);
				rotateLeft(node);

				AVLNode<Key, Value>* newTop = node->getParent();

				if(newTop->getBalance() == 0){
					node->setBalance(0);
					rightChild->setBalance(0);
				} else if(newTop->getBalance() == 1){
					node->setBalance(-1);
					rightChild->setBalance(1); // changed from 1 to 0
					newTop->setBalance(0);
				} else {
					node->setBalance(0);
					rightChild->setBalance(1);
					newTop->setBalance(0);
				}


				if(parent != nullptr){
					removeFix(parent, nextDiff);
				}
			}
			else{
				rotateLeft(node);

				if(rightChild->getBalance() == 0){
					node->setBalance(1);
					rightChild->setBalance(-1);
					return;
				} else {
					node->setBalance(0);
					rightChild->setBalance(0);

					if(parent != nullptr){
						removeFix(parent, nextDiff);
					}
				}
			}
		} else {
			AVLNode<Key, Value>* leftChild = node->getLeft();

			if(leftChild->getBalance() == 1){
				rotateLeft(leftChild);
				rotateRight(node);

				AVLNode<Key, Value>* newTop = node->getParent();

				if(newTop->getBalance() == 0){
					node->setBalance(0);
					leftChild->setBalance(0);
				} else if (newTop->getBalance() == -1){
					node->setBalance(1);
					leftChild->setBalance(0);
					newTop->setBalance(0);
				} else {
					node->setBalance(0);
					leftChild->setBalance(-1);
					newTop->setBalance(0);
				}

				if(parent != nullptr){
					removeFix(parent, nextDiff);
				}
			}
			else {
				rotateRight(node);

				if(leftChild->getBalance() == 0){
					node->setBalance(-1);
					leftChild->setBalance(1);
					return;
				} else {
					node->setBalance(0);
					leftChild->setBalance(0);

					if(parent != nullptr){
						removeFix(parent, nextDiff);
					}
				}
			}
		}
	}
	
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node){
	AVLNode<Key, Value>* y = node->getRight();
	AVLNode<Key, Value>* rootParent = node->getParent();
	y->setParent(rootParent);

	if(node->getParent() == nullptr){
		this->root_ = node->getRight();
	}
	else if(rootParent->getRight() == node){
		rootParent->setRight(y);
	}
	else{
		rootParent->setLeft(y);
	}

	AVLNode<Key, Value>* c = y->getLeft();

	y->setLeft(node);
	node->setParent(y);
	node->setRight(c);

	if(c != nullptr){
		c->setParent(node);
	}

}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node){

	AVLNode<Key, Value>* y = node->getLeft();
	AVLNode<Key, Value>* rootParent = node->getParent();
	y->setParent(rootParent);

	if(node->getParent() == nullptr){
		this->root_ = y;
	}
	else if(rootParent->getRight() == node){
		rootParent->setRight(y);
	}
	else{
		rootParent->setLeft(y);
	}

	AVLNode<Key, Value>* c = y->getRight();

	y->setRight(node);
	node->setParent(y);
	node->setLeft(c);
	
	if(c != nullptr){
		c->setParent(node);
	}

}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::getSuccessor(AVLNode<Key, Value>* node){
	if(node->getRight() != nullptr){
		node = node->getRight();
		while (node->getLeft() != nullptr){
			node = node->getLeft();
		}
		return node;
	}
	else{
		AVLNode<Key, Value>* parent = node->getParent();
		while((parent != nullptr) and (node == parent->getRight())){
			node = parent;
			parent = parent->getParent();
		}
		return parent;
	}
}


#endif
