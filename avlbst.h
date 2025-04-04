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
		void restoreAVL(AVLNode<Key, Value>* ancestor, AVLNode<Key, Value>* node);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
 
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
		if(this->root_ == nullptr) {
			this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
			return;
		}

		AVLNode<Key, Value>* current = static_cast<AVLNode<Key, Value>*>(this->root_);
		AVLNode<Key, Value>* parent = nullptr;
		bool wentLeft = true;

		while (current != nullptr) {
			parent = current;

			if(new_item.first < current->getKey()){
				current = current->getLeft();
				wentLeft = true;
			} else if (new_item.first > current->getKey()){
				current = current->getRight();
				wentLeft = false;
			} else {
				current->setValue(new_item.second);
				return;
			}
		}

			AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
			if(wentLeft) {
				parent->setLeft(newNode);
			} else {
				parent->setRight(newNode);
			}

			if(parent->getBalance() == 0){
				if(wentLeft){
					parent->updateBalance(-1);
				} else {
					parent->updateBalance(1);
				}

				insertFix(parent, newNode);
			} else {
				parent->setBalance(0);
			}
		
}
/*
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item){
	AVLNode<Key, Value>* temp;
	AVLNode<Key, Value>* back;
	AVLNode<Key, Value>* ancestor;

	if(root_ == nullptr){
		root_ = newNode;
		return;
	}

	restoreAVL(ancestor, newNode);

}
*/


/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */

 template<class Key, class Value>
 void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* child)
 {
	/*
	if((parent == nullptr) or (parent->getBalance() == 0)) return;

	AVLNode<Key, Value>* grandparent = parent->getParent();

	if((parent->getBalance() == 1) or (parent->getBalance() == -1)){
		if(grandparent != nullptr){
			bool parentIsLeftChild = (grandparent->getLeft() == parent);

			if(parentIsLeftChild){
				grandparent->updateBalance(-1);
			} else {
				grandparent->updateBalance(1);
			}

			if(grandparent->getBalance() != 0){
				insertFix(grandparent, parent);
			}
		}
		return;
	}

	if((parent->getBalance() == -2) and (parent->getBalance() == -1)){
		rotateRight(parent);
		parent->setBalance(0);
		child->setBalance(0);
	}

	else if((parent->getBalance() == -2) and (parent->getBalance() == 1)){
		AVLNode<Key, Value>* grandchild = child->getRight();
		rotateLeft(child);
		rotateRight(parent);

		if(grandchild->getBalance() == -1){
			parent->setBalance(1);
			child->setBalance(0);
			grandchild->setBalance(0);
		} else if(grandchild->getBalance() == 0){
			parent->setBalance(0);
			child->setBalance(0);
			grandchild->setBalance(0);
		} else {
			parent->setBalance(0);
			child->setBalance(-1);
			grandchild->setBalance(0);
		}
	}

	else if((parent->getBalance() == 2) and (child->getBalance() == 1)) {
		rotateLeft(parent);
		parent->setBalance(0);
		child->setBalance(0);
	}

	else if ((parent->getBalance() == 2) and (child->getBalance() == -1)){
		AVLNode<Key, Value>* grandchild = child->getLeft();
		rotateRight(child);
		rotateLeft(parent);

		if(grandchild->getBalance() == 1){
			parent->setBalance(-1);
			child->setBalance(0);
			grandchild->setBalance(0);
		} else if (grandchild->getBalance() == 0){
			parent->setBalance(0);
			child->setBalance(0);
			grandchild->setBalance(0);
		} else {
			parent->setBalance(0);
			child->setBalance(1);
			grandchild->setBalance(0);
		}

	}
	*/

	if((parent == nullptr) or (parent->getParent() == nullptr)) return;

	AVLNode<Key, Value>* grandparent = parent->getParent();

	bool parentIsLeftChild = (grandparent->getLeft() == parent);

	int8_t dir;

	if (parentIsLeftChild){
		dir = -1;
	} else {
		dir = 1;
	}

	grandparent->updateBalance(dir);

	if(grandparent->getBalance() == 0){
		return;
	} else if (grandparent->getBalance() == dir * (-1)){
		grandparent->setBalance(dir*(-1));
		AVLNode<Key, Value>* greatGrandparent = grandparent->getParent();
		if(greatGrandparent != nullptr){
			//bool grandparentIsLeftCHild = (greatGrandparent->getLeft() == grandparent);
			insertFix(grandparent, parent);
		}
		return;
	} else {
		if(parentIsLeftChild){
			if(parent->getRight() == child){
				rotateLeft(parent);
				rotateRight(grandparent);

				if(child->getBalance() == 0){
					parent->setBalance(0);
					grandparent->setBalance(0);

				} else if(child->getBalance() == -1){
					parent->setBalance(0);
					grandparent->setBalance(1);
					child->setBalance(0);

				} else {
					//parent->setBalance(-1); //changed from 1 to -1
					rotateRight(grandparent);
					grandparent->setBalance(0);
					child->setBalance(0);
				}
			} else{
				rotateRight(grandparent);

				parent->setBalance(0);
				grandparent->setBalance(0);
			}
		} else {
			if(parent->getLeft()== child){
				rotateRight(parent);
				rotateLeft(grandparent);

				if(child->getBalance() == 0){
					parent->setBalance(0);
					grandparent->setBalance(0);
				} else if(child->getBalance() == 1){
					parent->setBalance(0);
					grandparent->setBalance(-1); 
					child->setBalance(0);
				} else {
					parent->setBalance(1); 
					grandparent->setBalance(0);
					child->setBalance(0);
					}
			} else {
				rotateLeft(grandparent);
				parent->setBalance(0);
				grandparent->setBalance(0);
			}
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
			diff = isLeftChild ? 1 : -1;
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

				//newTop->setBalance(0); removed this since other newTops were added

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

				//newTop->setBalance(0); remove this since other new tops were added

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
	if((node == nullptr) or (node->getRight() == nullptr)) return;

	AVLNode<Key, Value>* rightChild = node->getRight();
	AVLNode<Key, Value>* parent = node->getParent();

	node->setRight(rightChild->getLeft());
	if(rightChild->getLeft() != nullptr){
		rightChild->getLeft()->setParent(node);
	}

	rightChild->setLeft(node);
	node->setParent(rightChild);

	rightChild->setParent(parent);

	if(parent == nullptr){
		this->root_ = rightChild;
	} else {
		if(parent->getLeft() == node){
			parent->setLeft(rightChild);
		} else {
			parent->setRight(rightChild);
		}
	}
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node){
	if((node == nullptr) or (node->getLeft() == nullptr)) return;

	AVLNode<Key, Value>* leftChild = node->getLeft();
	AVLNode<Key, Value>* parent = node->getParent();

	node->setLeft(leftChild->getRight());
	if(leftChild->getRight() != nullptr){
		leftChild->getRight()->setParent(node);
	}

	leftChild->setRight(node);
	node->setParent(leftChild);

	leftChild->setParent(parent);

	if(parent == nullptr){
		this->root_ = leftChild;
	} else {
		if(parent->getLeft() == node){
			parent->setLeft(leftChild);
		} else {
			parent->setRight(leftChild);
		}
	}

}


#endif
