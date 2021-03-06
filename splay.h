#ifndef SPLAY_TREE
#define SPLAY_TREE
#define SPACE_COUNT 6
#include <iostream>
#include <utility>
#include <functional>
#include <initializer_list>

using namespace std;

template<typename value_type, typename comparator = less<value_type> >
class splay_tree
{
public:
	// splay_tree(comparator comp = comparator()) : comp(comp) 
	// {
	// 	root = head = tail = nullptr;
	// 	size_ = 0;
	// }
	splay_tree() : comp(comparator()) 
	{
		root = head = tail = nullptr;
		size_ = 0;
	}

	splay_tree(initializer_list<value_type> init) : comp(comparator()) 
	{
		root = head = tail = nullptr;
		size_ = 0;
		for(auto i : init)
			insert(i);	
	}

	splay_tree(const splay_tree& rhs);
	splay_tree(splay_tree&& rhs);
	
	~splay_tree()
	{
		destroy_tree(root);
	}

	splay_tree& operator=(const splay_tree& rhs);
	splay_tree& operator=(splay_tree&& rhs);

	size_t size() const {	return size_;	}
	bool empty() const 	{	return (size_ == 0);	}

	void disp()
	{
		display_tree(root, 0);
		cout << endl << endl;
	}

	class Iterator;

	Iterator begin() const;
	Iterator end() const;
	Iterator rbegin() const;
	Iterator rend() const;

	pair<Iterator, bool> insert(const value_type& val);
	pair<Iterator, bool> find(const value_type& val) ;
	void erase(const value_type& val);
	
	// Non member comparison operator functions
	friend ostream& operator<<(ostream&, const splay_tree& st)
	{
		for(auto it = st.begin(); it!=st.end(); ++it)
			cout << *it << "\t";
		cout << endl;
	}

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

	friend bool operator<(const splay_tree<value_type, comparator>& lhs, const splay_tree<value_type, comparator>& rhs)
	{
		auto it = lhs.begin();
		auto jt = rhs.begin();
		while(it != lhs.end() && jt != rhs.end() && *it < *jt)
		{
			++it; ++jt;
		}
 		return(it == lhs.end() || jt == rhs.end());
	}

	friend bool operator<=(const splay_tree<value_type, comparator>& lhs, const splay_tree<value_type, comparator>& rhs)
	{
		auto it = lhs.begin();
		auto jt = rhs.begin();
		while(it != lhs.end() && jt != rhs.end() && *it <= *jt)
		{
			++it; ++jt;
		}
		return(it == lhs.end() || jt == rhs.end());
	}

	friend bool operator>(const splay_tree<value_type, comparator>& lhs, const splay_tree<value_type, comparator>& rhs)
	{
		auto it = lhs.begin();
		auto jt = rhs.begin();
		while(it != lhs.end() && jt != rhs.end() && *it > *jt)
		{
			++it; ++jt;
		}
		return(it == lhs.end() || jt == rhs.end());
	}

	friend bool operator>=(const splay_tree<value_type, comparator>& lhs, const splay_tree<value_type, comparator>& rhs)
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
		value_type node_value_;
		node* left_child_;
		node* right_child_;
		node* parent_;

		node(const value_type& val): node_value_(val) {left_child_ = right_child_ = parent_ = nullptr;}
	};

	node* head, *tail;
	node* root;
	
	comparator comp;
	size_t size_;
	
	node* clone_tree(node* clone_from, node* parent);
	void destroy_tree(node* parent);

	void display_tree(node* root_node, int space)
	{
		if(root_node == NULL)
			return;
		space += SPACE_COUNT;
		display_tree(root_node->right_child_, space);
		cout << endl;
		for(int i = SPACE_COUNT; i < space; ++i)
			cout << " ";
		cout << root_node->node_value_;
		display_tree(root_node->left_child_, space);
	}

	node* insert_node(node* node_, node* parent_node_, const value_type& val)
	{
		if (node_ == nullptr)
		{
			node *new_node_ = new splay_tree<value_type, comparator>::node(val);
			new_node_->parent_ = parent_node_;
			++size_;
			return new_node_;
		}

		if(comp(val, node_->node_value_))
			node_->left_child_  = insert_node(node_->left_child_, node_, val);
		else if (comp(node_->node_value_, val))
			node_->right_child_ = insert_node(node_->right_child_, node_ , val);

		return node_;
	}

	node* find_node(const value_type& val, node* curr_node) 
	{
		if(curr_node == nullptr)
			return curr_node;
		else if(!comp(curr_node->node_value_ ,val) && !comp(val, curr_node->node_value_))
			return  curr_node;
		else if(comp(curr_node->node_value_, val))
			return  find_node(val, curr_node->right_child_);
		else
			return  find_node(val, curr_node->left_child_);
	}

	node* delete_node(node* root, node* parent_node_, value_type val)
	{
		if(root == nullptr)return root;

		if(comp(val, root->node_value_ ))
		{
			root->left_child_ = delete_node(root->left_child_, root, val); 
			return root;
		}
		else if(comp(root->node_value_, val))
		{
			root->right_child_ = delete_node(root->right_child_, root, val); 
			return root; 
		}

		if(!root->left_child_)
		{
			if(root->right_child_)
				(root->right_child_)->parent_ = parent_node_;
			auto temp = root->right_child_;
			delete root;
			return temp;
		}
		else if(!root->right_child_)
		{
			if(root->left_child_)
				(root->left_child_)->parent_ = parent_node_;
			auto temp = root->left_child_;
			delete root;
			return temp;
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
				root->right_child_ = succ->right_child_;
				delete succ;
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
			delete succ;
			return root;
		}
	} 


	void rotate_up(node* node_)
	{

		struct node* parent;
		struct node* grandparent;
		if(node_->parent_)
		{
			parent = node_->parent_;
		}
		else
			return;

		if(node_->parent_->parent_)
		{
			grandparent = node_->parent_->parent_;
		}
		else
		{
			grandparent = nullptr;
		}
		
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
				parent->left_child_ = nullptr;
		}

		else
		{
			node* child = node_->left_child_;
			node_->left_child_ = parent;
			if(child)
			{
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
			node_->parent_ = nullptr;
			root = node_;
		}
	}

	void splay(node* node_) 
	{
		while(node_ && node_->parent_)
		{
			struct node* parent = node_->parent_;
			struct node* grandparent = parent->parent_;

			if(grandparent == nullptr)
			{
				rotate_up(node_);
			}
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
};


template<typename value_type, typename comparator>
splay_tree<value_type, comparator>::splay_tree(const splay_tree& rhs) 
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

template<typename value_type, typename comparator>
splay_tree<value_type, comparator>& 
	splay_tree<value_type, comparator>::operator=(const splay_tree& rhs) 
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

template<typename value_type, typename comparator>
splay_tree<value_type, comparator>::splay_tree(splay_tree&& rhs)
: size_(rhs.size_), comp(rhs.comp), root(rhs.root), tail(rhs.tail), head(rhs.head)
{
	rhs.root = rhs.tail = rhs.head = nullptr;
}

template<typename value_type, typename comparator>
splay_tree<value_type, comparator>& 
splay_tree<value_type, comparator>::operator=(splay_tree<value_type, comparator>&& rhs)
{
	if(this != &rhs)
	{
		destroy_tree(root);
		root = rhs.root;
		head = rhs.head;
		tail = rhs.tail;
		rhs.root = rhs.tail = rhs.head = nullptr;
		size_ = rhs.size_;
		comp = rhs.comp;
	}
	return *this;
}


template<typename value_type, typename comparator>
typename splay_tree<value_type, comparator>::node* 
	splay_tree<value_type, comparator>::clone_tree(node* clone_from, node* parent) 
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


template<typename value_type, typename comparator>
void splay_tree<value_type, comparator>::destroy_tree(node* parent)
{
	if(parent == nullptr)	
		return;
	destroy_tree(parent->left_child_);
	destroy_tree(parent->right_child_);
	delete parent;
}

template<typename value_type, typename comparator>
typename splay_tree<value_type, comparator>::Iterator
	splay_tree<value_type, comparator>::begin() const
{
	node* temp = root;
	while(temp->left_child_ != nullptr)temp = temp->left_child_;
	return Iterator(temp);
}

template<typename value_type, typename comparator>
typename splay_tree<value_type, comparator>::Iterator
	splay_tree<value_type, comparator>::end() const
{
	return Iterator(nullptr);
}

template<typename value_type, typename comparator>
typename splay_tree<value_type, comparator>::Iterator
	splay_tree<value_type, comparator>::rbegin() const
{
	node* temp = root;
	while(temp->right_child_ != nullptr)temp = temp->right_child_;
	return Iterator(temp);
}

template<typename value_type, typename comparator>
typename splay_tree<value_type, comparator>::Iterator
	splay_tree<value_type, comparator>::rend() const
{
	return Iterator(nullptr);
}

template<typename value_type, typename comparator>
pair<typename splay_tree<value_type, comparator>::Iterator, bool>
splay_tree<value_type, comparator>::insert(const value_type& val)
{
	auto find_pair = find(val);
	if(find_pair.second)
		return pair<typename splay_tree<value_type, comparator>::Iterator, bool>(find_pair.first, false);

	if(empty())
	{
			node* new_node = new node(val);
			head = tail = root = new_node;
			++size_;
			return pair<typename splay_tree<value_type, comparator>::Iterator, bool>(typename splay_tree<value_type, comparator>::Iterator(head), true);
	}

	node* new_node = insert_node(root, nullptr,  val);
	splay(new_node);
	find_pair = find(val);
	return find_pair;
}

template<typename value_type, typename comparator>
pair<typename splay_tree<value_type, comparator>::Iterator, bool> 
splay_tree<value_type, comparator>::find(const value_type& val) 
{
	node* target_node = find_node(val, root);
	splay(target_node);
	if(target_node == nullptr)
		return pair<typename splay_tree<value_type, comparator>::Iterator, bool>(splay_tree<value_type, comparator>::Iterator(target_node), false);
	else
		return pair<typename splay_tree<value_type, comparator>::Iterator, bool>(splay_tree<value_type, comparator>::Iterator(target_node), true);
}

template<typename value_type, typename comparator>
void splay_tree<value_type, comparator>::erase(const value_type& val)
{
		root = delete_node(root, nullptr, val);
		--size_;
}

//Iterator class definition
template<typename value_type, typename comparator>
class splay_tree<value_type, comparator>::Iterator : public std::iterator<std::forward_iterator_tag, splay_tree<value_type, comparator>::node*>
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
	value_type operator*()const
	{
		if(iter) return iter->node_value_;
		return value_type(); //IMPORTANT : value_type class needs default constructor
	}
	Iterator& operator++()	//pre increment
	{
		if(iter->right_child_)
		{

			iter = iter->right_child_;
			while(iter->left_child_)
			{
				iter = iter->left_child_;
			}
		}
		else
		{
			if(iter->parent_)
			{
				while(iter->parent_)
				{
					if(iter->parent_->left_child_ == iter)
					{
						iter = iter->parent_;
						return *this;
					}
					iter = iter->parent_;
				}				
			}	
			iter = nullptr;
					
		}
		return *this;
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
			iter = iter->left_child_;
			while(iter->right_child_)
			{
				iter = iter->right_child_;
			}
		}
		else
		{
			if(iter->parent_)
			{
				while(iter->parent_)
				{
					if(iter->parent_->right_child_ == iter)
					{
						iter = iter->parent_;
						return *this;
					}
					iter = iter->parent_;
				}				
			}	
			iter = nullptr;
					
		}
		return *this;
		
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

#endif
