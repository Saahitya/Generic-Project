#ifndef SPLAY_TREE
#define SPLAY_TREE
#include <iostream>
#include <utility>
#include <functional>

using namespace std;

template<typename value, typename comparator = 	less<value> >
class splay_tree
{
public:
	splay_tree(comparator comp = comparator());
	splay_tree(const splay_tree& rhs);
	splay_tree(splay_tree&& rhs);
	
	~splay_tree();

	splay_tree& operator=(const splay_tree& rhs);
	splay_tree& operator=(splay_tree&& rhs);

	size_t size()
	{
		return size_;
	}

	//Iterator Class
	class Iterator;

	Iterator begin() const;
	Iterator end() const;
	Iterator rbegin() const;
	Iterator rend() const;


	pair<Iterator, bool> insert(const value& val);
	pair<Iterator, bool> erase(const value& val);
	pair<Iterator, bool> find(const value& val) ;
	
	bool empty() const;

	friend bool operator==( splay_tree& lhs,  splay_tree& rhs)
	{
		if(lhs.size() != rhs.size())
		{
			return false;
		}
		auto it = lhs.begin();
		auto jt = rhs.begin();
		while(it != lhs.end() && jt != rhs.end() && *it == *jt)
		{
			++it; ++jt;
		}
		return(it == lhs.end() &&  jt == rhs.end());
	}

	friend bool operator!=( splay_tree& lhs,  splay_tree& rhs)
	{
		return !(lhs == rhs);
	}

	friend bool operator<(const splay_tree<value, comparator>& lhs, const splay_tree<value, comparator>& rhs)
	{
		auto it = lhs.begin();
		auto jt = rhs.begin();
		while(it != lhs.end() && jt != rhs.end() && *it < *jt)
		{
			++it; ++jt;
		}
		return(it == lhs.end() || jt == rhs.end());
	}

	friend bool operator<=(const splay_tree<value, comparator>& lhs, const splay_tree<value, comparator>& rhs)
	{
		auto it = lhs.begin();
		auto jt = rhs.begin();
		while(it != lhs.end() && jt != rhs.end() && *it <= *jt)
		{
			++it; ++jt;
		}
		return(it == lhs.end() || jt == rhs.end());
	}

	friend bool operator>(const splay_tree<value, comparator>& lhs, const splay_tree<value, comparator>& rhs)
	{
		auto it = lhs.begin();
		auto jt = rhs.begin();
		while(it != lhs.end() && jt != rhs.end() && *it > *jt)
		{
			++it; ++jt;
		}
		return(it == lhs.end() || jt == rhs.end());
	}

	friend bool operator>=(const splay_tree<value, comparator>& lhs, const splay_tree<value, comparator>& rhs)
	{
		auto it = lhs.begin();
		auto jt = rhs.begin();
		while(it != lhs.end() && jt != rhs.end() && *it >= *jt)
		{
			++it; ++jt;
		}
		return(it == lhs.end() || jt == rhs.end());
	}

private:
	struct node
	{
		value node_value_;
		node* left_child_;
		node* right_child_;
		node* parent_;

		node(const value& val): node_value_(val) {left_child_ = right_child_ = parent_ = nullptr;}
	};

	node* head, *tail;
	node* root;
	
	comparator comp;
	size_t size_;
	//REVIEW
	node* insert_node(node* node_, node* parent_node_, const value& val)
	{
		if (node_ == nullptr)
		{
			node *new_node_ = new splay_tree<value, comparator>::node(val);
			new_node_->parent_ = parent_node_;
			++size_;
			return new_node_;
		}
		int flag = 0;
		// if (val < node_->node_value_) 
		if(comp(val, node_->node_value_))
		{
			node_->left_child_  = insert_node(node_->left_child_, node_, val); 
		}
		// else if (val > node_->node_value_) 
		else if (comp(node_->node_value_, val))
		{
			node_->right_child_ = insert_node(node_->right_child_, node_ , val);    
			flag = 1;
		}
		return node_;
	}

	void rotate_up(node* node_)
	{

		////cout <<"ROTATING: "<<node_->node_value_;
		struct node* parent;
		struct node* grandparent;
#if 1
		if(node_->parent_)
		{
			////cout << endl << "NODE HAS PARENT" << endl;
			parent = node_->parent_;
		}
		else
			return;

		if(node_->parent_->parent_)
		{
			////cout << endl << "Node has grandparent"<< endl;
			grandparent = node_->parent_->parent_;
		}
		else
		{
			grandparent = nullptr;
		}
		
		if(node_->parent_->left_child_ == node_)
		{
			////cout << endl << "NODE HAS LEFT CHILD" << endl;
			node* child = node_->right_child_;
			node_->right_child_ = parent;
			if(child)
			{
				parent->left_child_ = child;
				child->parent_ = parent;
			}
			else
				parent->left_child_ = nullptr;
		}

		else
		{
			////cout << endl << "NODE HAS RIGHT CHILD" << endl;
			node* child = node_->left_child_;
			node_->left_child_ = parent;
			////cout << "NEw child value"<<node_->left_child_->node_value_;
			if(child)
			{
				////cout << "node has child";brea
				parent->right_child_ = child;
				child->parent_ = parent;
			}
			else
				parent->right_child_ = nullptr;
		}
		parent->parent_ = node_;
		if(grandparent)
		{
			node_->parent_ = grandparent;
			if(grandparent->left_child_ == parent)
				grandparent->left_child_ = node_;
			else
				grandparent->right_child_ = node_;
		}
		else
		{
			////cout << "no gp" << endl;
			node_->parent_ = nullptr;
			root = node_;
		}
#endif
	//return 1;
	}

	void splay(node* node_) 
	{
		//Alread root
		while(node_ && node_->parent_)
		{
			//	////cout <<"INSIDE SPLAY";
			struct node* parent = node_->parent_;
			struct node* grandparent = parent->parent_;

			//parent is root
			if(grandparent == nullptr)
			{
				////cout << "ZIG";
				rotate_up(node_);
			//	break;
			}
			//If both are same side child
			else if(parent->left_child_ == node_ && grandparent->left_child_ == parent || parent->right_child_ == node_ && grandparent->right_child_ == parent)
			{
				rotate_up(parent);
				rotate_up(node_);
			}

			else
			{
				rotate_up(node_);
				rotate_up(node_);
			}
		}
	}
	
	node* clone_tree(node* clone_from, node* parent);
	
	void destroy_tree(node* parent);

	node* find_node(const value& val, node* curr_node) 
	{
		if(curr_node == nullptr)
			return curr_node;
		// else if(curr_node->node_value_ == val)
		else if(!comp(curr_node->node_value_ ,val) && !comp(val, curr_node->node_value_))
			return  curr_node;
//		else if(curr_node->node_value_ < val)
		else if(comp(curr_node->node_value_, val))
		{
			//cout << curr_node->node_value_<< "\n";
			return  find_node(val, curr_node->right_child_);
		}
			
		else
			return  find_node(val, curr_node->left_child_);
		//delete_node(head, 100);
	}
	node* delete_node(node* root, node* parent_node_, value val)
	{
		if(root == nullptr)return root;

		// if(root->node_value_ > val)
		if(comp(val, root->node_value_ ))
		{
			root->left_child_ = delete_node(root->left_child_, root, val); 
			return root;
		}
		// else if(root->node_value_ < val)
		else if(comp(root->node_value_, val))
		{
			root->right_child_ = delete_node(root->right_child_, root, val); 
			return root; 
		}

		if(!root->left_child_)
		{
			if(root->right_child_)
				(root->right_child_)->parent_ = parent_node_;
			return root->right_child_;
		}
		else if(!root->right_child_)
		{
			if(root->left_child_)
				(root->left_child_)->parent_ = parent_node_;
			return root->left_child_;
		}
		else
		{
			node* succ_parent = root->right_child_;
			node* succ = root->right_child_;
			if(succ->left_child_ == nullptr)
			{
				if(succ->right_child_)
					(succ->right_child_)->parent_ = root;
				root->node_value_ = succ->node_value_;
				root->right_child_ = (succ->right_child_)->parent_;
				return root;
			}
			while(succ->left_child_ != nullptr)
			{
				succ_parent = succ;
				succ = succ->left_child_;
			}
			succ_parent->left_child_ = succ->right_child_;
			if(succ->right_child_)
			{
				(succ->right_child_)->parent_ = succ_parent->left_child_;
			}
			root->node_value_ = succ->node_value_;
			return root;
		}
	} 
};


//Implementation

//Default constructor when given comparator
template<typename value, typename comparator>
splay_tree<value, comparator>::splay_tree(comparator comp) : comp(comp) 
{
	root = head = tail = nullptr;
	size_ = 0;
}

//Copy Constructor
template<typename value, typename comparator>
splay_tree<value, comparator>::splay_tree(const splay_tree& rhs) 
{
	size_ = rhs.size_;
	comp = rhs.comp;

	root = clone_tree(rhs.root, nullptr);
	
	tail = head = root;
	if(head != nullptr) {
		while(head->left_child_)head = head->left_child_;
		while(tail->right_child_)tail = tail->right_child_;
	}
}

//Copy Assignment
template<typename value, typename comparator>
splay_tree<value, comparator>& 
	splay_tree<value, comparator>::operator=(const splay_tree& rhs) 
{
	destroy_tree(root);
	
	size_ = rhs.size_;
	comp = rhs.comp;

	root = clone_tree(rhs.root, nullptr);
	tail = head = root;
	if(head != nullptr) {
		while(head->left_child_)	
			head = head->left_child_;
		while(tail->right_child_)	
			tail = tail->right_child_;
	}
}

//Move Constructor
//REVIEW Why are we not destroying the original tree?
template<typename value, typename comparator>
splay_tree<value, comparator>::splay_tree(splay_tree&& rhs)
: size_(rhs.size_), comp(rhs.comp), root(rhs.root), tail(rhs.tail), head(rhs.head)
{
	rhs.root = rhs.tail = rhs.head = nullptr;
}

//Move Assignment
template<typename value, typename comparator>
splay_tree<value, comparator>& 
splay_tree<value, comparator>::operator=(splay_tree<value, comparator>&& rhs)
{
	if(this != &rhs)
	{
		destroy_tree(root);
//		delete head;
//		delete tail;
		root = rhs.root;
		head = rhs.head;
		tail = rhs.tail;
		rhs.root = rhs.tail = rhs.head = nullptr;
		size_ = rhs.size_;
		comp = rhs.comp;
	}
	return *this;
}

//Destructor
template<typename value, typename comparator>
splay_tree<value, comparator>::~splay_tree()
{
	// destroy_tree(root);
}

//Clone tree
template<typename value, typename comparator>
typename splay_tree<value, comparator>::node* 
	splay_tree<value, comparator>::clone_tree(node* clone_from, node* parent) 
{
	if(clone_from == nullptr)	
		return nullptr;
	node* new_node = new node(clone_from->node_value_);
	new_node->node_value_ = clone_from->node_value_;
	new_node->parent_ = parent;
	new_node->left_child_ = clone_tree(clone_from->left_child_,new_node);
	new_node->right_child_ = clone_tree(clone_from->right_child_, new_node);
	return new_node;
}

//Destroy Tree
template<typename value, typename comparator>
void splay_tree<value, comparator>::destroy_tree(node* parent)
{
	if(parent == nullptr)	
		return;
	destroy_tree(parent->left_child_);
	destroy_tree(parent->right_child_);
	delete parent;
}

//Return Iterator begin
template<typename value, typename comparator>
typename splay_tree<value, comparator>::Iterator
	splay_tree<value, comparator>::begin() const
{
	//cout << "root is"<<root->node_value_<<endl;
	node* temp = root;
	while(temp->left_child_ != nullptr)temp = temp->left_child_;
	return Iterator(temp);
	//return Iterator(head);
}

//Return Iterator end
template<typename value, typename comparator>
typename splay_tree<value, comparator>::Iterator
	splay_tree<value, comparator>::end() const
{
	/*node* temp = root;
	while(temp->right_child_ != nullptr)temp = temp->right_child_;
	return Iterator(temp);*/
	return Iterator(nullptr);
	// return Iterator(tail);
}

template<typename value, typename comparator>
typename splay_tree<value, comparator>::Iterator
	splay_tree<value, comparator>::rbegin() const
{
	//cout << "root is"<<root->node_value_<<endl;
	node* temp = root;
	while(temp->right_child_ != nullptr)temp = temp->right_child_;
	return Iterator(temp);
	//return Iterator(head);
}

//Return Iterator end
template<typename value, typename comparator>
typename splay_tree<value, comparator>::Iterator
	splay_tree<value, comparator>::rend() const
{
	/*node* temp = root;
	while(temp->right_child_ != nullptr)temp = temp->right_child_;
	return Iterator(temp);*/
	return Iterator(nullptr);
	// return Iterator(tail);
}

//Return if size_ of tree is null
template<typename value, typename comparator>
bool splay_tree<value, comparator>::empty() const
{
	return (size_ == 0);
}

// Insert value into tree
template<typename value, typename comparator>
pair<typename splay_tree<value, comparator>::Iterator, bool>
splay_tree<value, comparator>::insert(const value& val)
{
	auto find_pair = find(val);
	//cout << boolalpha;
	//cout << find_pair.second << endl;
	if(find_pair.second)
		return pair<typename splay_tree<value, comparator>::Iterator, bool>(find_pair.first, false);

	if(empty())
	{
			node* new_node = new node(val);
			head = tail = root = new_node;
			++size_;
			return pair<typename splay_tree<value, comparator>::Iterator, bool>(typename splay_tree<value, comparator>::Iterator(head), true);
	}
	node* new_node = insert_node(root, nullptr,  val);
	//splay(new_node);
	find_pair = find(val);
	////cout << endl << "CALLING ROTATE";
	////cout << "New root "<<root->node_value_ << endl;
	return find_pair;
}

//Finds value from tree. Have to add splaying
template<typename value, typename comparator>
pair<typename splay_tree<value, comparator>::Iterator, bool> 
splay_tree<value, comparator>::find(const value& val) 
{
	// begin();
	//g++ << "FIND "<<root->node_value_;
	node* target_node = find_node(val, root);
	//splay(target_node);
	if(target_node == nullptr)
		return pair<typename splay_tree<value, comparator>::Iterator, bool>(splay_tree<value, comparator>::Iterator(target_node), false);
	else
		return pair<typename splay_tree<value, comparator>::Iterator, bool>(splay_tree<value, comparator>::Iterator(target_node), true);
}

template<typename value, typename comparator>
pair<typename splay_tree<value, comparator>::Iterator, bool> 
splay_tree<value, comparator>::erase(const value& val)
{
		//cout << "Went in alive" << endl;
		root = delete_node(root, nullptr, val);
		--size_;
		cout << "Got out alive" << endl;
}
//Shifts node to its parent
/*template<typename value, typename comparator>
void splay_tree<value, comparator>::rotate_up(splay_tree<value, comparator>::node* node_)
{
	////cout <<"ROTATING";
	node* parent, grandparent;
	if(node_->parent_)
		parent = node_->parent_;
	else
		return;

	if(node_->parent_->parent_)
		grandparent = node_->parent_->parent_;

	if(node_->parent_->left_child_ == node_)
	{
		node* child = node_->right_child_;
		node_->right_child_ = parent;
		if(child)
		{
			parent->left_child_ = child;
			child->parent_ = parent;
		}
		else
			parent->left_child = nullptr;
	}

	else
	{
		node* child = node_->left_child_;
		node_->left_child_ = parent;
		if(child)
		{
			parent->right_child = child;
			child->parent_ = parent;
		}
		else
			parent->right_child = nullptr;
	}
	parent->parent_ = node_;
	if(grandparent)
	{
		node_->parent_ = grandparent;
		if(grandparent->left_child_ == parent)
			grandparent->left_child_ = node_;
		else
			grandparent->right_child_ = node_;
	}
	else
		node_->parent_ = nullptr;	
}*/

//Iterator class definition
template<typename value, typename comparator>
class splay_tree<value, comparator>::Iterator
{
public:
	explicit Iterator(node* node_ = nullptr) : iter(node_){	}
	bool operator==(const Iterator& rhs) const
	{
		return iter == rhs.iter;
	}
	bool operator!=(const Iterator& rhs) const
	{
		return !(*this == rhs);
	}
	value operator*()const
	{
		if(iter) return iter->node_value_;
		return value();
	}
	Iterator& operator++()	//pre increment
	{
		//cout << iter->node_value_;
		if(iter->right_child_)
		{
			//cout << "has right child"<< endl;
			iter = iter->right_child_;
			while(iter->left_child_)
			{
				//cout << "going left";
				iter = iter->left_child_;
			}
		}
		else
		{
			if(iter->parent_)
			{
				//cout << "has parent" << iter->parent_->node_value_ << endl;
				while(iter->parent_)
				{
					if(iter->parent_->left_child_ == iter)
					{
						//cout << "is left child of" << iter->parent_->node_value_ << endl;
						iter = iter->parent_;
						return *this;
					}
					iter = iter->parent_;
				}				
			}	
			iter = nullptr;
					
		}
		return *this;
		////cout <<"++";
		////cout << iter->node_value_<<endl;
		//logic to move forward in binary tree
		/*if(iter->right_child_)
		{
			//cout << "has right child";
			iter = iter->right_child_;
			return *this;
		}
		if(iter->parent_)
		{
			if(iter->parent_->left_child_ == iter) //left_child with no children
			{
				//cout << "is left child";
				iter = iter->parent_;
				return *this;
			}
			else
			{
				//cout << "is right child";
				if(iter->parent_->parent_)
					iter = iter->parent_->parent_;
				else
					//cout << "Cannot increment further";
			}	
		}
		return *this;*/
	}
	Iterator operator++(int)	//post increment
	{
		Iterator temp(*this);
		++*this;
		return temp;
	}
	Iterator& operator--()	//pre decrement
	{
		if(iter->left_child_)
		{
			//cout << "has right child"<< endl;
			iter = iter->left_child_;
			while(iter->right_child_)
			{
				//cout << "going left";
				iter = iter->right_child_;
			}
		}
		else
		{
			if(iter->parent_)
			{
				//cout << "has parent" << iter->parent_->node_value_ << endl;
				while(iter->parent_)
				{
					if(iter->parent_->right_child_ == iter)
					{
						//cout << "is left child of" << iter->parent_->node_value_ << endl;
						iter = iter->parent_;
						return *this;
					}
					iter = iter->parent_;
				}				
			}	
			iter = nullptr;
					
		}
		return *this;
		/*//logic to move backward in binary tree
		if(iter->left_child_)
		{
			iter = iter->left_child_;
			return *this;
		}
		if(iter->parent_)
		{
			if(iter->parent_->right_child_ == iter) //right child with no children
			{
				iter = iter->parent_;
				return *this;
			}
			else
			{
				if(iter->parent_->parent_)
					iter = iter->parent_;
				/*else
					////cout << "Cannot decrement further";*
			}
			
		}
		return *this;*/
	}
	Iterator operator--(int)	//post decrement
	{
		Iterator temp(*this);
		--*this;
		return temp;
	}

private:
	node* iter;
};

// template<typename value, typename comparator>
// bool operator==(const splay_tree<value, comparator>& lhs, const splay_tree<value, comparator>& rhs)
// {
// 	if(lhs.size() != rhs.size())
// 	{
// 		return false;
// 	}
// 	auto it = lhs.begin();
// 	auto jt = rhs.begin();
// 	while(it != lhs.end() && jt != rhs.end() && *it == *jt)
// 	{
// 		++it; ++jt;
// 	}
// 	return(it == lhs.end() &&  jt == rhs.end());
// }

// template <typename value, typename comparator>
// bool operator!=(const splay_tree<value, comparator>& lhs, const splay_tree<value, comparator>& rhs)
// {
// 	return !(lhs == rhs);
// }

// template <typename value, typename comparator>
// bool operator<(const splay_tree<value, comparator>& lhs, const splay_tree<value, comparator>& rhs)
// {
// 	auto it = lhs.begin();
// 	auto jt = rhs.begin();
// 	while(it != nullptr && jt != nullptr && *it < *jt)
// 	{
// 		++it; ++jt;
// 	}
// 	return(it == nullptr || jt == nullptr);
// }

// template <typename value, typename comparator>
// bool operator<=(const splay_tree<value, comparator>& lhs, const splay_tree<value, comparator>& rhs)
// {
// 	auto it = lhs.begin();
// 	auto jt = rhs.begin();
// 	while(it != nullptr && jt != nullptr && *it <= *jt)
// 	{
// 		++it; ++jt;
// 	}
// 	return(it == nullptr || jt == nullptr);
// }

// template <typename value, typename comparator>
// bool operator>=(const splay_tree<value, comparator>& lhs, const splay_tree<value, comparator>& rhs)
// {
// 	auto it = lhs.begin();
// 	auto jt = rhs.begin();
// 	while(it != nullptr && jt != nullptr && *it >= *jt)
// 	{
// 		++it; ++jt;
// 	}
// 	return(it == nullptr &&  jt == nullptr);
// }

// template <typename value, typename comparator>
// bool operator>(const splay_tree<value, comparator>& lhs, const splay_tree<value, comparator>& rhs)
// {
// 	auto it = lhs.begin();
// 	auto jt = rhs.begin();
// 	while(it != nullptr && jt != nullptr && *it > *jt)
// 	{
// 		++it; ++jt;
// 	}
// 	return(it == nullptr &&  jt == nullptr);
// }

#endif
