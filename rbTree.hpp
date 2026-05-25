#pragma once
#include <iostream>
using namespace std;

enum Color
{
	RED,
	BLACK
};

template <class K, class T>
struct Node
{
	K key;
	T val;

	Color color;
	Node* parent;
	Node* left;
	Node* right;

	Node() :parent(nullptr), left(nullptr), right(nullptr), color(RED) {}
};


template <class K, class T>
class rbTree
{
private:
	Node<K, T>* head;
	int size;
	void leftRotate(Node <K, T>* node);
	void rightRotate(Node <K, T>* node);
	void insertFixup(Node <K, T>* node);
	void removeFixup(Node <K, T>* node);
	void destroyTree(Node<K, T>* node);
	void getTree(Node<K, T>* node);
	Node<K, T>* grand(Node <K, T>* node);
	Node<K, T>* uncle(Node <K, T>* node);
	Node<K, T>* brother(Node <K, T>* node);
	Node<K, T>* minNode(Node <K, T>* node);
	Node<K, T>* maxNode(Node <K, T>* node);
public:
	rbTree() :head(nullptr), size(0) {}
	~rbTree();
	Node<K, T>* getMin();
	Node<K, T>* getMax();
	void insert(const K& key, const T& val);
	Node<K, T>* find(const K& key);
	bool remove(const K& k);
	void printTree();
};

template<class K, class T>
void rbTree<K, T>::destroyTree(Node<K, T>* node) {
	if (node) {
		destroyTree(node->left);
		destroyTree(node->right);
		delete node;
	}
}

template<class K, class T>
rbTree<K, T>::~rbTree() {
	destroyTree(head);
	head = nullptr;
	size = 0;
}

template <class K, class T>
Node<K, T>* rbTree<K, T>::grand(Node<K, T>* node) {
	if (node && node->parent) {
		return node->parent->parent;
	}
	else {
		return nullptr;
	}
}

template <class K, class T>
Node<K, T>* rbTree<K, T>::uncle(Node<K, T>* node) {
	Node<K, T>* grandparent = grand(node);
	if (!grandparent) {
		return nullptr;
	}
	if (node->parent == grandparent->left) {
		return grandparent->right;
	}
	else if (node->parent == grandparent->right) {
		return grandparent->left;
	}
	return nullptr;
}

template <class K, class T>
Node<K, T>* rbTree<K, T>::brother(Node <K, T>* node) {
	if (node == nullptr || node == head) {
		return nullptr;
	}

	if (node == node->parent->right) {
		return node->parent->left;
	}
	else {
		return node->parent->right;
	}
}

template <class K, class T>
Node<K, T>* rbTree<K, T>::minNode(Node <K, T>* node){
	if (node == nullptr) {
		return nullptr;
	}
	while (node->left != nullptr) {
		node = node->left;
	}
	return node;
}

template <class K, class T>
Node<K, T>* rbTree<K, T>::maxNode(Node <K, T>* node) {
	if (node == nullptr) {
		return nullptr;
	}
	while (node->right) {
		node = node->right;
	}
	return node;
}

template <class K, class T>
void rbTree<K, T>::rightRotate(Node<K, T>* node) {
	Node<K, T>* temp = node->left;

	node->left = temp->right;
	if (temp->right) {
		temp->right->parent = node;
	}

	temp->parent = node->parent;

	if (!node->parent) {
		head = temp;
	}

	else if (node->parent->left == node) {
		node->parent->left = temp;
	}
	else {
		node->parent->right = temp;
	}

	temp->right = node;
	node->parent = temp;
}

template <class K, class T>
void rbTree<K, T>::leftRotate(Node<K, T>* node) {
	Node<K, T>* temp = node->right;

	node->right = temp->left;
	if (temp->left) {
		temp->left->parent = node;
	}

	temp->parent = node->parent;

	if (!node->parent) {
		head = temp;
	}
	else if (node->parent->left == node) {
		node->parent->left = temp;
	}
	else {
		node->parent->right = temp;
	}

	temp->left = node;
	node->parent = temp;
}

template <class K, class T>
void rbTree<K, T>::insertFixup(Node<K, T>* node) {

	while (node->parent && node->parent->color == RED) {
		Node<K, T>* grand_node = grand(node);

		// ліва гілка відносно діда
		if (node->parent == grand_node->left) {

			Node<K, T>* uncle_node = uncle(node);
			 
			// дядько червоний
			if (uncle_node && uncle_node->color == RED) {
				node->parent->color = BLACK;
				uncle_node->color = BLACK;
				grand_node->color = RED;
				node = grand_node; // проблема йде вгору
			}
			else {
				// дядько чорний + злам
				if (node->parent->right == node) {
					node = node->parent;
					leftRotate(node);
				}
				// пряма лінія + дядько чорний
				node->parent->color = BLACK;
				grand_node->color = RED;
				rightRotate(grand_node);
			}
		}
		// права гілка відносно діда
		else {
			Node<K, T>* uncle_node = uncle(node);

			if (uncle_node && uncle_node->color == RED) {
				node->parent->color = BLACK;
				uncle_node->color = BLACK;
				grand_node->color = RED;
				node = grand_node;
			}
			else {
				if (node->parent->left == node) {
					node = node->parent;
					rightRotate(node);
				}
				node->parent->color = BLACK;
				grand_node->color = RED;
				leftRotate(grand_node);
			}

		}
	}
	head->color = BLACK;
}

template <class K, class T>
void rbTree<K, T>::removeFixup(Node <K, T>* node) {

	while (node->parent && node->color == BLACK) {
		Node<K, T>* brother_node = brother(node);

		// ліва гілка
		if (node == node->parent->left) {
			if (brother_node->color == RED) {
				brother_node->color = BLACK;
				brother_node->parent->color = RED;
				leftRotate(brother_node->parent);
				brother_node = node->parent->right;
			}

			bool left_black = (brother_node->left == nullptr || brother_node->left->color == BLACK);
			bool right_black = (brother_node->right == nullptr || brother_node->right->color == BLACK);

			if (left_black && right_black) {
				brother_node->color = RED;
				node = node->parent;
			}
			else {
				if (right_black) {
					if (brother_node->left != nullptr) brother_node->left->color = BLACK;
					brother_node->color = RED;
					rightRotate(brother_node);
					brother_node = node->parent->right;
				}
				brother_node->color = node->parent->color;
				node->parent->color = BLACK;
				if (brother_node->right != nullptr) brother_node->right->color = BLACK;
				leftRotate(node->parent);
				node = head;
			}
		}
		// права гілка
		else {
			if (brother_node->color == RED) {
				brother_node->color = BLACK;
				brother_node->parent->color = RED;
				rightRotate(brother_node->parent);
				brother_node = node->parent->left;
			}

			bool left_black = (brother_node->left == nullptr || brother_node->left->color == BLACK);
			bool right_black = (brother_node->right == nullptr || brother_node->right->color == BLACK);

			if (left_black && right_black) {
				brother_node->color = RED;
				node = node->parent;
			}
			else {
				if (left_black) {
					if(brother_node->right != nullptr) brother_node->right->color = BLACK;
					brother_node->color = RED;
					leftRotate(brother_node);
					brother_node = node->parent->left;
				}
				brother_node->color = node->parent->color;
				node->parent->color = BLACK;
				if (brother_node->left != nullptr) brother_node->left->color = BLACK;
				rightRotate(node->parent);
				node = head;
			}
		}
	}
	node->color = BLACK;
}

template <class K, class T>
void rbTree<K, T>::insert(const K& key, const T& val) {

	Node<K, T>* node = new Node<K, T>;
	node->key = key;
	node->val = val;

	if (head == nullptr) {
		head = node;
		node->color = BLACK;
		size++;
		return;
	}

	Node<K, T>* curr = head;
	Node<K, T>* parent = nullptr;
	while (curr) {
		parent = curr;

		if (key == curr->key) {
			curr->val = val;
			delete node;
			return;
		}

		if (node->key < curr->key) {
			curr = curr->left;
		}
		else {
			curr = curr->right;
		}
	}
	if (node->key < parent->key) {
		parent->left = node;
	}
	else {
		parent->right = node;
	}
	node->parent = parent;
	size++;

	insertFixup(node);
}

template <class K, class T>
Node<K, T>* rbTree<K, T>::find(const K& key) {
	Node<K, T>* curr = head;

	while (curr) {
		if (curr->key == key) {
			return curr;
		}
		else {
			if (key < curr->key) {
				curr = curr->left;
			}
			else {
				curr = curr->right;
			}
		}
	}
	return nullptr;
}

template <class K, class T>
bool rbTree<K, T>::remove(const K& key) {
	Node<K, T>* curr = head;

	while (curr) {
		if (curr->key == key) {
			// вузол без дітей
			if (!curr->left && !curr->right) {
				if (curr == head) {
					head = nullptr;
				}
				else {
					if (curr->color == BLACK) {
						removeFixup(curr);
					}

					if (curr == curr->parent->right) {
						curr->parent->right = nullptr;
					}
					else {
						curr->parent->left = nullptr;
					}
				}
				delete curr;
				size--;
				return true;
			}
			//вузол має 1 дитину
			else if ((curr->left && !curr->right) || (!curr->left && curr->right)) {
				
				Node<K, T>* child = (curr->left != nullptr) ? curr->left : curr->right;

				if (curr == head) {
					head = child;
					head->parent = nullptr;
				}
				else {
					if (curr == curr->parent->left) {
						curr->parent->left = child;
					}
					else {
						curr->parent->right = child;
					}
					child->parent = curr->parent;
				}
				child->color = BLACK;

				delete curr;
				size--;
				return true;
			}
			//вузол має 2 дитини
			else {

				Node<K, T>* successor = minNode(curr->right);

				curr->key = successor->key;
				curr->val = successor->val;
				// наступник має дитину 
				if (successor->right != nullptr) {
					
					Node<K, T>* child = successor->right;

					if (successor == successor->parent->left) {
						successor->parent->left = child;
					}
					else {
						successor->parent->right = child;
					}
					child->parent = successor->parent;
					child->color = BLACK;
				}
				else {
					//наступник не має дітей
					removeFixup(successor);

					if (successor == successor->parent->left) {
						successor->parent->left = nullptr;
					}
					else {
						successor->parent->right = nullptr;
					}
				}
				
				delete successor;
				size--;
				return true;
			}
		}
		else {
			if (key < curr->key) {
				curr = curr->left;
			}
			else {
				curr = curr->right;
			}
		}
	}
	return false;
}

template <class K, class T>
Node<K, T>* rbTree<K, T>::getMin() {
	Node<K, T>* node = minNode(head);
	if (node) {
		return node;
	}
}

template <class K, class T>
Node<K, T>* rbTree<K, T>::getMax()  {
	Node<K, T>* node = maxNode(head);
	if (node) {
		return node;
	}
}

template <class K, class T>
void rbTree<K, T>::getTree(Node<K, T>* node) {
	if (node == nullptr) {
		return;
	}
	getTree(node->left);
	cout << "[" << node->key << " : " << node->val << "] ";
	getTree(node->right);
}

template <class K, class T>
void rbTree<K, T>::printTree() {
	getTree(head);
	cout << endl;
}