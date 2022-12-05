
#include <iostream>
#include <sstream>
#include <vector>
#include <stack>
using namespace std;
/* Note: 
	1. You will have to comment main() when unit testing your code because catch uses its own main().
	2. You will submit this main.cpp file and any header files you have on Gradescope. 
*/

class BST // using some code here from Aman's lecture
{
	private:
		struct TreeNode
		{
			int val, height;
			string name;
			TreeNode *left;
			TreeNode *right;
			TreeNode(int x, int h, string n) : val(x), height(h), name(n), left(nullptr), right(nullptr){}
		};

		TreeNode* root = nullptr;
		BST::TreeNode* HelperInsert(TreeNode* root, int key, string name);
		vector<int> HelperInorder(TreeNode* root);
		vector<int> HelperPreorder(TreeNode* root);
		vector<int> HelperPostorder(TreeNode* root);
		string HelperIDSearch(TreeNode* root, int ID);
		vector<int> HelperNameSearch(TreeNode* root, string name);
		BST::TreeNode* HelperRemoveID(TreeNode* root, int ID);
		BST::TreeNode* HelperRemoveInorder(TreeNode* root, int N);
		int GetBalance(TreeNode* root);
		int GetHeight(TreeNode* root);
		bool CheckDuplicateIDHelper(TreeNode* root, int ID);
		BST::TreeNode* RecalculateHeights(TreeNode* root);
		BST::TreeNode* RightRotation(TreeNode* root);
		BST::TreeNode* LeftRotation(TreeNode* root);
		BST::TreeNode* LeftRightRotation(TreeNode* root);
		BST::TreeNode* RightLeftRotation(TreeNode* root);
		BST::TreeNode* DestructHelper(TreeNode* root);

	public:
		vector<BST::TreeNode*> printResult;
		vector<int> catchResult;
		bool foundGlobal = false;
		bool dupID = false;
		int inorderRemovalCount = 0;
		vector<int> Inorder();
		vector<int> Preorder();
		vector<int> Postorder();
		int PrintLevelCount();
		string FindID(int ID);
		vector<int> FindName(string name);
		void RemoveID(int ID);
		void RemoveInorder(int N);
		void Insert(int key, string name);
		bool CheckDuplicateID(int ID);
		void Destruct();
};

// traversals
vector<int> BST::HelperInorder(BST::TreeNode* root)
{
	if (root == NULL)
	{
		cout << "";

	}

	else
	{
		HelperInorder(root->left);
		catchResult.push_back(root->val);
		printResult.push_back(root);
		HelperInorder(root->right);
	}
	return catchResult;
}

vector<int> BST::HelperPreorder(BST::TreeNode* root)
{
	if (root == NULL)
	{
		cout << "";

	}

	else
	{
		// visit root, left side, right side
		catchResult.push_back(root->val);
		printResult.push_back(root);
		HelperPreorder(root->left);
		HelperPreorder(root->right);
	}
	return catchResult;
}

vector<int> BST::HelperPostorder(BST::TreeNode* root)
{
	if (root == NULL)
	{
		cout << "";

	}

	else
	{
		// visit left side, right side, root
		HelperPostorder(root->left);
		HelperPostorder(root->right);
		catchResult.push_back(root->val);
		printResult.push_back(root);
	}
	return catchResult;
}


// searches
string BST::HelperIDSearch(BST::TreeNode* root, int ID)
{
	if (root == NULL)
	{
		//cout << "unsuccessful (NULL)" << endl; 
		return "unsuccessful";
	}
	else
	{
		// assume ID is unique
		string result = "unsuccessful";
		bool found = false;
		while (!found && root !=NULL)
		{
			if (root->val == ID)
			{
				found = true;
				foundGlobal = true;
				result = root->name;
			}

			else if (root->val < ID)
			{
				root = root->right;
			}

			else 
			{
				root = root->left;
			}
		}

		if (result != "unsuccessful")
		{
			cout << result << endl;
		}
		return result;
	}

}

vector<int> BST::HelperNameSearch(BST::TreeNode* root, string name)
{
	if (root == NULL)
	{
		cout << "";

	}

	else
	{
		HelperNameSearch(root->left, name);
		if (root->name == name)
		{
			catchResult.push_back(root->val);
			cout << root->val << endl;
			foundGlobal = true;
		}
		HelperNameSearch(root->right, name);
	}
	return catchResult;
}


// deletions
BST::TreeNode* BST::HelperRemoveID(BST::TreeNode* root, int ID)
{
	// base case
	if (root == NULL)
	{
		return root;
	}

	// if the ID is less than root, go to left subtree to delete
	if (ID < root->val)
	{
		root->left = HelperRemoveID(root->left, ID);
	}

	// if the ID is greater than the root, go to the right subtree
	else if (ID > root->val)
	{
		root->right = HelperRemoveID(root->right, ID);
	}

	// otherwise this node is the ID to delete
	else
	{
		// no children case
		if (root->right == NULL && root->left == NULL)
		{
			return NULL;
		}

		// left child case
		else if (root->right == NULL && root->left != NULL)
		{
			// delete the current root node and return its left child
			BST::TreeNode* temp = root->left;
			delete root;
			return temp;
		}

		// right child case
		else if (root->right != NULL && root->left == NULL)
		{
			// delete the current root node and return its right child
			BST::TreeNode* temp = root->right;
			delete root;
			return temp;
		}

		// two children case
		else 
		{
			// find the inorder successor
			BST::TreeNode* successor = root->right;
			BST::TreeNode* temp = successor;
			bool found = false;
			while(!found && successor->left != NULL)
			{
				if (successor->left != NULL)
				{
					successor = successor->left;
					found = true;
				}

				else if (successor->right != NULL)
				{
					successor = successor->right;
				}
			}
			if(!found)
			{
				successor = temp;
			}
			// replace root's value AND NAME with successor's
			root->val = successor->val;
			root->name = successor->name;
			// delete inorder successor
			root->right = HelperRemoveID(root->right, successor->val);
		}
	}
	root = RecalculateHeights(root);
	return root;
}

BST::TreeNode* BST::HelperRemoveInorder(BST::TreeNode* root, int N)
{
	if (root == NULL)
	{
		cout << "";

	}

	else
	{
		root->left = HelperRemoveInorder(root->left, N);
		inorderRemovalCount++;
		if (inorderRemovalCount == N + 1)
		{
			root = HelperRemoveID(root, root->val);
			return root;
		}
		root->right = HelperRemoveInorder(root->right, N);
	}
	return root;
}


// Misc functions
int BST::GetBalance(BST::TreeNode* root)
{
	if (root == NULL)
	{
		return 0;
	}
	else
	{
		return (GetHeight(root->left) - GetHeight(root->right));
	}
}

bool BST::CheckDuplicateIDHelper(TreeNode* root, int ID)
{
	if (root == NULL)
	{
		cout << "";

	}

	else
	{
		CheckDuplicateIDHelper(root->left, ID);
		if (root->val == ID)
		{
			dupID = true;
		}
		CheckDuplicateIDHelper(root->right, ID);
	}

	return dupID;
}

// got idea for this function from geeksforgeeks.org
int BST::GetHeight(TreeNode* root) 
{
	if (root == NULL)
	{
		return 0;
	}
	else
	{
		return root->height;
	}
}

BST::TreeNode* BST::RecalculateHeights(TreeNode* root)
{
	if (root->left !=NULL)
	{
		root->left->height = 1 + max(GetHeight(root->left->left), GetHeight(root->left->right));
	}

	if (root->right !=NULL)
	{
		root->right->height = 1 + max(GetHeight(root->right->left), GetHeight(root->right->right));
	}
	return root;
}

// This will be called in main to deallocate memory
BST::TreeNode* BST::DestructHelper(TreeNode* root) 
{
  if (root->left) 
    DestructHelper(root->left);
  if (root->right) 
    DestructHelper(root->right);
  delete root;
  return root;
}

// rotations
BST::TreeNode* BST::RightRotation(TreeNode* root)
{
	// take as example the left heavy tree  25
	//									  22
	//									4
	TreeNode* result;
	// take the result to be the root->left (just 22 and 4)
	result = root->left;
	// then take the left side of the old root to be the right side of the result (new root) (the right in this case would be null)
	root->left = result->right;
	// the right side of the result is now the adjusted old root (with the left side being the results old right)
	result->right = root;

	// recalculate height values
	return RecalculateHeights(result);
}

BST::TreeNode* BST::LeftRotation(TreeNode* root)
{
	// take as example the right heavy tree  25
	//									  		30
	//												35
	TreeNode* result;
	// take the result to be the root->right (just 30 and 35)
	result = root->right;
	// then take the right side of the old root to be the left side of the result (new root) (the left in this case would be null)
	root->right = result->left;
	// the left side of the result is now the adjusted old root (with the right side being the results old left)
	result->left = root;

	// recalculate height values
	return RecalculateHeights(result);
}

BST::TreeNode* BST::RightLeftRotation(TreeNode* root)
{
	// take as example this tree  10
	//								  30
	//							  20
	TreeNode* temp;
	temp = RightRotation(root->right);
	// temp should now be  20
	// 						   30
	root->right = temp;
	// root is now:
	// 10
	//		20
	// 			30
	return LeftRotation(root);
}

BST::TreeNode* BST::LeftRightRotation(TreeNode* root)
{
	// take as example this tree  	  30
	//							  10
	//							  	  20
	TreeNode* temp;
	temp = LeftRotation(root->left);
	// temp should now be  		20
	// 						 10
	root->left = temp;
	// root is now:
	//			30
	//		20
	// 10
	return RightRotation(root);
}


// insertion
BST::TreeNode* BST::HelperInsert(TreeNode* root, int key, string name)
{
	if (root == NULL)
	{
		// 1 == height
		return new TreeNode(key, 1, name);
	}

	// if key is less than the root, go to the left
	else if (key < root->val)
	{
		root->left = HelperInsert(root->left, key, name);
	}

	// otherwise go to the right side
	else
	{
		root->right = HelperInsert(root->right, key, name);
	}

	// since we are recursively going up the tree here, we can update the heights as we go up and check their balances
	// read about this at geeksforgeeks.org
	root->height = 1 + max(GetHeight(root->left), GetHeight(root->right)); 
	int balance = GetBalance(root);
	if (balance == 2)
	{
		// check if you were going down the left tree...
		if (key < root->val)
		{
			// then check if the child is right heavy...
			if (GetBalance(root->left) == -1)
			{
				// case P:2 C:-1 == Left Right Rotation
				return LeftRightRotation(root);
			}

			// or left heavy...
			else if (GetBalance(root->left) == 1)
			{
				// case P:2 C:1  Right Rotation
				return RightRotation(root);
			}
		}

		else
		{
			if (GetBalance(root->right) == -1)
			{
				// case P:2 C:-1 == Left Right Rotation
				return LeftRightRotation(root);
			}

			else if (GetBalance(root->right) == 1)
			{
				// case P:2 C:1 Right Rotation
				return RightRotation(root);
			}
		}
	}

	if (balance == -2)
	{
		// check if you were going down the left tree...
		if (key < root->val)
		{
			// then check if the child is right heavy...
			if (GetBalance(root->left) == 1)
			{
				// case P:-2 C:1 == Right Left Rotation
				return RightLeftRotation(root);
			}

			// or left heavy...
			else if (GetBalance(root->left) == -1)
			{
				// case P:-2 C:-1 Left Rotation
				return LeftRotation(root);
			}
		}

		else
		{
			if (GetBalance(root->right) == 1)
			{
				// case P:-2 C:1 == Right Left Rotation
				return RightLeftRotation(root);
			}

			else if (GetBalance(root->right) == -1)
			{
				// case P:-2 C:-1 Left Rotation
				return LeftRotation(root);
			}
		}
	}
	return root;
}


// public functions
vector<int> BST::Inorder()
{
	vector<int> result = HelperInorder(this->root);
	return result;
}

vector<int> BST::Preorder()
{
	vector<int> result = HelperPreorder(this->root);
	return result;
}

vector<int> BST::Postorder()
{
	vector<int> result = HelperPostorder(this->root);
	return result;
}

int BST::PrintLevelCount()
{
	if (root == NULL)
	{
		cout << 0 << endl;
		return 0;
	}

	else
	{
		cout << this->root->height << endl;
		return this->root->height;
	}
}

string BST::FindID(int ID)
{
	return HelperIDSearch(this->root, ID);
}

vector<int> BST::FindName(string name)
{
	vector<int> result = HelperNameSearch(this->root, name);
	return result;
}

void BST::Insert(int key, string name)
{
	// starting the height always at 0
	this->root = HelperInsert(this->root, key, name);
}

void BST::RemoveID(int ID)
{
	this->root = HelperRemoveID(this->root, ID);
}

void BST::RemoveInorder(int N)
{
	this->root = HelperRemoveInorder(this->root, N);
}

bool BST::CheckDuplicateID(int ID)
{
	return CheckDuplicateIDHelper(this->root, ID);
}

bool checkID(string ID)
{
	if (ID.length() != 8)
		{
			//cout << "Unsuccessful" << endl;
			return false;
		}

		// check that UFID is only digits
	else 
	{
		for (char c : ID)
		{
			if (!isdigit(c))
			{
				//cout << "Unsuccessful" << endl;
				return false;
			}
		}
	}
	return true;
}

bool checkName(string name)
{
	// ensure the name consists of only characters a-z, A-Z, or spaces
		for (char c : name)
		{
			// if not a valid character print unsuccessful
			if (!isalpha(c) && !isspace(c))
			{
				//cout << "unsuccessful" << endl;
				return false;
			}
		}
		return true;
}

void BST::Destruct()
{
	this->root = DestructHelper(this->root);
}