#pragma once
#include <cstddef>
#include <vector>


template<class T>
class TreeNode {
private:
	T data;
	TreeNode<T>* parent;
	vector< TreeNode<T>* > children;
public:
	TreeNode();
	TreeNode(TreeNode<T>* parent);
	TreeNode(TreeNode<T>* parent, T data);
	~TreeNode();

	T& getData() const;
	void setData(const T& data);
	void addChild(const T& data);
	void removeChild(const size_t& indx);
	TreeNode<T>* findChild(const T& data) const;
	TreeNode<T>* getChild(const size_t& indx) const;
	TreeNode<T>* getParent() const;
	int getNumChildren() const;

};

template<class T>
TreeNode<T>::TreeNode() : parent(),data() {}

template<class T>
TreeNode<T>::TreeNode(TreeNode<T>* parent) : parent(parent) {}

template<class T>
TreeNode<T>::TreeNode(TreeNode<T>* parent, T dat) : parent(parent) {
	data = T(dat);
}

template<class T>
TreeNode<T>::~TreeNode() {
	//delete data;
	for (TreeNode<T>* childNode : children)
		delete childNode;
}

template<class T>
T& TreeNode<T>::getData() const {
	return data;
}

template<class T>
void TreeNode<T>::setData(const T& dd) {
	data = dd;
}

template<class T>
void TreeNode<T>::addChild(const T& dat) {
	children.push_back(new TreeNode<T>(this, dat));
}

template<class T>
void TreeNode<T>::removeChild(const size_t& indx) {
	children.erase(children.begin() + indx);
}

template<class T>
TreeNode<T>* TreeNode<T>::findChild(const T& dat) const {
	for (int i = 0; i<children.size(); i++)
		if (children[i]->getData() == dat)
			return children[i];
	return nullptr;
}

template<class T>
TreeNode<T>* TreeNode<T>::getChild(const size_t& indx) const {
	return children[indx];
}

template<class T>
TreeNode<T>* TreeNode<T>::getParent() const {
	return parent;
}

template<class T>
int TreeNode<T>::getNumChildren() const {
	return children.size();
}
