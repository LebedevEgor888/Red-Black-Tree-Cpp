#include <iostream>
#include "rbTree.hpp"
using namespace std;

int main() {

	rbTree<int, string> myTree;
	myTree.insert(50, "Root");
	myTree.insert(30, "Left Child");
	myTree.insert(70, "Right Child");
	myTree.insert(20, "Leaf 1");
	myTree.insert(40, "Leaf 2");
	myTree.insert(60, "Leaf 3");
	myTree.insert(80, "Leaf 4");

	cout << "Tree created" << endl;

	Node<int, string>* node = myTree.find(50);
	cout << node->val << endl;
	
	Node<int, string>* min_node = myTree.getMin();
	cout << min_node->key << " : " << min_node->val << endl;

	Node<int, string>* max_node = myTree.getMax();
	cout << max_node->key << " : " << max_node->val << endl;

	cout << endl;
	myTree.printTree();

	return 0;
}