#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int height(Node* root){
	if(root == nullptr) return 0;



	return(1+ max(height(root->left), height(root->right)));
}


bool equalPaths(Node * root)
{
    // Add your code below
		if(root == nullptr) return true;
		if((root->left == nullptr) and (root->right == nullptr)) return true;
		
		int leftHeight = height(root->left);
		int rightHeight = height(root->right);

		if((root->left != nullptr) and (root->right != nullptr) and (leftHeight != rightHeight)) return false;

		return (equalPaths(root->left) and equalPaths(root->right));

		if(leftHeight == rightHeight){
			return true;
		}



}

