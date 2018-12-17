#ifndef Binomial_Heap_Binomial_Heap_H
#define Binomial_Heap_Binomial_Heap_H

#include <memory>

using std::weak_ptr;
using std::shared_ptr;
using std::unique_ptr;

template <typename Key>
class Binomial_Heap {
private:
	struct Node {
	private:
		Node() {}
	public:
		Key key;
		shared_ptr<Node> right;
		shared_ptr<Node> left;
		shared_ptr<Node> left_child;
		weak_ptr<Node> par;
		size_t degree;
		bool is_negative_infinity = false;
		Node(Key key) {
			this->key = key;
			left = nullptr;
			right = nullptr;
			left_child = nullptr;
			par = weak_ptr<Node>();
			is_negative_infinity = false;
			degree = 0;
		}
		void AddLeftChild(shared_ptr<Node> new_child) {
			if (left_child != nullptr) {
				new_child->right = left_child;
				left_child->left = new_child;
			}
			else {
				new_child->right = nullptr;
			}
			left_child = new_child;
			new_child->left = nullptr;
			new_child->par = weak_ptr<Node>();
			++degree;
		}
	};
	shared_ptr<Node> right_;
	shared_ptr<Node> min_;
	Binomial_Heap(shared_ptr<Node> node) {
		right_ = node;
		min_ = node;
	}
	void ValidateMin() {
		shared_ptr<Node> current = right_;
		min_ = right_;
		while (current != nullptr) {
			if (current->key < min_->key) {
				min_ = current;
			}
			current = current->left;
		}
	}
public:
	class Pointer {
	private:
		weak_ptr<Node> ptr_;
	public:
		Pointer(shared_ptr<Node> ptr) {
			this->ptr_ = ptr;
		}
		weak_ptr<Node> GetPtr() const {
			return ptr_;
		}
		friend class Binomial_Heap;
	};
	Binomial_Heap() {
		right_ = nullptr;
		min_ = nullptr;
	}
	bool is_empty() const {
		return (right_ == nullptr);
	}
	void Clear() {
		right_ = nullptr;
		min_ = nullptr;
	}
	Key get_min() const {
		if (is_empty()) {
			throw std::logic_error("Heap is empty");
		}
		return min_->key;
	}
	Key extract_min() {
		Key res = get_min();
		Binomial_Heap children;
		shared_ptr<Node> ptr = min_->left_child;
		if (ptr != nullptr) {
			while (ptr->right != nullptr) {
				ptr->par = ptr;
				ptr = ptr->right;
			}
			ptr->par = ptr;
		}
		children.right_ = ptr;
		if (min_->left != nullptr) {
			min_->left->right = min_->right;
		}
		if (min_->right != nullptr) {
			min_->right->left = min_->left;
		}
		else {
			right_ = min_->left;
		}
		merge(children);
		ValidateMin();
		return res;
	}
	void merge(Binomial_Heap &other_heap) {
		if (is_empty()) {
			right_ = other_heap.right_;
			min_ = other_heap.min_;
			other_heap.Clear();
			ValidateMin();
		}
		else if (!other_heap.is_empty()) {
			shared_ptr<Node> ptr1 = right_;
			shared_ptr<Node> ptr2 = other_heap.right_;
			if (ptr2->degree < ptr1->degree || (ptr2->degree == ptr1->degree && ptr2->key < ptr1->key)) {
				right_ = ptr2;
			}
			while (ptr1 != nullptr && ptr2 != nullptr) {
				if (ptr1->degree < ptr2->degree) {
					if (ptr1->left != nullptr) {
						ptr1 = ptr1->left;
					}
					else {
						ptr1->left = ptr2;
						ptr2->right = ptr1;
						ptr1 = nullptr;
					}
				}
				else {
					shared_ptr<Node> tmp = ptr2->left;
					ptr2->right = ptr1->right;
					ptr2->left = ptr1;
					if (ptr1->right != nullptr) {
						ptr1->right->left = ptr2;
					}
					else {
						right_ = ptr2;
					}
					ptr1->right = ptr2;
					ptr2 = tmp;
				}
			}
			shared_ptr<Node> ptr = right_;
			while (ptr->left != nullptr) {
				if (ptr->degree == ptr->left->degree) {
					if (ptr->key < ptr->left->key) {
						shared_ptr<Node> tmp = ptr->left->left;
						ptr->AddLeftChild(ptr->left);
						if (tmp != nullptr) {
							tmp->right = ptr;
						}
						ptr->left = tmp;
					}
					else {
						shared_ptr<Node> new_ptr = ptr->left;
						if (ptr->right != nullptr) {
							ptr->right->left = new_ptr;
						}
						else {
							right_ = new_ptr;
						}
						new_ptr->right = ptr->right;
						new_ptr->AddLeftChild(ptr);
						ptr = new_ptr;
					}
				}
				else {
					ptr = ptr->left;
				}
			}
			ValidateMin();
			other_heap.Clear();
		}
	}
	Pointer insert(Key key) {
		Node* node_ptr = new Node(key);
		shared_ptr<Node> ptr(node_ptr);
		Pointer res(ptr);
		Binomial_Heap<Key> heap(ptr);
		merge(heap);
		return res;
	}
	void Delete(Pointer ptr) {
		if (ptr.GetPtr().expired()) {
			throw std::invalid_argument("Element does not exist");
		}
	}
	void change(Pointer& ptr, Key key) {
		Delete(ptr);
		ptr.ptr_ = Insert(key).ptr_;
	}
};

#endif //Binomial_Heap_Binomial_Heap_H
//#pragma once
//#include<stdexcept>
//
//using std::logic_error;
//using std::out_of_range;
//
//
//template<typename T>
//class Binomial_Heap
//{
//public:
//	class Pointer;
//
//	Pointer insert(T key) {
//		Binomial_Heap<T>::Pointer p;
//		this->merge(new Binomial_Heap(new node(new tree(key, &p))));
//		return p;
//	}
//
//
//	Binomial_Heap() {
//		leftSon = new node();
//		min = leftSon;
//	}
//
//
//	T get_min() {
//		return (min->getTree())->key;
//	}
//	
//	
//	T extract_min() {
//		if (is_empty()) {
//			throw logic_error("Heap is empty!");
//		}
//		T res = (min->getTree())->key;
//
//		eraseNode(min);
//		node* tmp = min;
//
//		this->merge(new Binomial_Heap(min->getTree()->leftSon));
//
//		tmp->getTree()->leftSon = new node();
//		delete tmp;
//
//		return res;
//	}
//
//
//	bool is_empty() {
//		return leftSon->getTree() == nullptr;
//	}
//
//
//	void erase(Pointer *Ptr) {
//		if (!(Ptr->haveSon()))
//			throw logic_error("there arent this pointer!");
//		tree* tc = Ptr->get_son();
//		while (tc->parent != nullptr)
//		{
//			swap(tc->parent, tc);
//			tc = tc->parent;
//		}
//		min = tc->prNode;
//		extract_min();
//	}
//
//
//	void change(Pointer *ptr, T value) {
//		if (!(ptr->haveSon()))
//			throw logic_error("there arent this pointer!");
//		tree* cur = ptr->get_son();
//		if (cur->key < value)
//			throw  logic_error("you are increase!"); // we cannot do increase for logn
//		cur->key = value;
//		while (cur->parent != nullptr) {
//			if (cur->parent->key <= cur->key)
//				break;
//			swap(cur->parent, cur);
//			cur = cur->parent;
//		}
//		if (cur->parent == nullptr)
//			find_new_min();
//	}
//
//
//	void merge(Binomial_Heap &Heap_) {
//		leftSon = merge(leftSon, Heap_.leftSon);
//		while (leftSon->left != nullptr)
//			leftSon = leftSon->left;
//
//		Heap_.leftSon = new node();
//		Heap_.min = Heap_.leftSon;
//
//		find_new_min();
//	}
//
//
//	~Binomial_Heap() {
//		while (leftSon->right != nullptr) {
//			del(leftSon);
//		}
//		delete leftSon;
//	}
//
//private:
//	struct node;
//	struct tree;
//
//	explicit Binomial_Heap(node* a) {
//		leftSon = a;
//		find_new_min();
//	}
//
//
//	tree * merge_tree(tree* first, tree* second) {
//		if (second->key < first->key) {
//			return merge_tree(second, first);
//		}
//		first->rightSon->insertMe(second);
//		first->rightSon = first->rightSon->right;
//		first->st++;
//		second->parent = first;
//		return first;
//	}
//
//
//	void swap(tree* first, tree *second) {
//		T value = first->key;
//		first->key = second->key;
//		second->key = value;
//
//		Pointer* ptr = first->owner;
//		if (second->owner != nullptr) second->owner->change_son(first);
//
//		if (ptr != nullptr) ptr->change_son(second);
//	}
//	
//	
//	void find_new_min() {
//		min = leftSon;
//		node* cur = leftSon;
//		while (cur->getTree() != nullptr) {
//			if (min->getTree() == nullptr || (min->getTree())->key > (cur->getTree())->key) {
//				min = cur;
//			}
//			cur = cur->right;
//		}
//	}
//
//
//	void merge(Binomial_Heap *heap_) {
//		merge(*heap_);
//		delete heap_;
//	}
//
//
//	node* merge(node* first, node* second) {
//		while (second->getTree() != nullptr) {
//			second->getTree()->P = this;
//			second->getTree()->parent = nullptr;
//			
//			if (first->getTree() == nullptr) {
//				first->insertMe(second->getTree());
//				first = first->right;
//				second->changeTree(nullptr);
//				del(second);
//			}
//
//			else {
//				if (first->getTree()->st > second->getTree()->st) {
//					first->insertLeft(second->getTree());
//
//					second->changeTree(nullptr);
//					del(second);
//				}
//				else {
//					if (first->getTree()->st < second->getTree()->st) {
//						first = first->right;
//					}
//					else {
//						node* c = new node(merge_tree(first->getTree(), second->getTree()));
//
//						node* ud = first;
//						first = first->right;
//
//						ud->eraseNode();
//						ud->changeTree(nullptr);
//						delete ud;
//
//						first = merge(first, c);
//
//						second->changeTree(nullptr);
//						del(second);
//					}
//				}
//			}
//		}
//		delete second;
//		return first;
//	}
//
//
//	void eraseNode(node *a) {
//		if (a == leftSon) {
//			leftSon = leftSon->right;
//		}
//		a->eraseNode();
//	}
//
//
//	static void del(node* &a) {
//		if (a->right == nullptr) {
//			throw logic_error("i havent right!");
//		}
//		a = a->right;
//		delete a->left;
//	}
//
//	node* min = nullptr;
//	node* leftSon = nullptr;
//};
//
//
//template<typename T>
//class Binomial_Heap<T>::Pointer
//{
//	friend Binomial_Heap;
//public:
//	Pointer(Pointer&& right) noexcept {
//		son = right.son;
//		son->owner = this;
//		right.son = nullptr;
//	}
//
//	Pointer(const Pointer&) = delete;
//	Pointer& operator=(const Pointer&) = delete;
//	
//	inline T get_value() {
//		if (!haveSon()) {
//			throw logic_error("i haven't son!");
//		}
//		return son->key;
//	
//	}
//	bool haveSon() {
//		return son != nullptr;
//	}
//
//	~Pointer() {
//		if (son != nullptr) son->owner = nullptr;
//	}
//
//
//private:
//	explicit Pointer(tree *son_ = nullptr) :son(son_)
//	{
//		if (son != nullptr)
//			son_->owner = this;
//	}
//
//	inline tree* get_son() {
//		if (!haveSon())
//			throw logic_error("i haven't got son!");
//		return son;
//	}
//
//
//	void change_son(tree* son_) {
//		son = son_;
//		if (son != nullptr)
//			son->owner = this;
//	}
//
//
//	tree* son = nullptr;
//};
//
//
//template<typename T>
//struct Binomial_Heap<T>::tree
//{
//	int st = 0;
//	tree* parent = nullptr; // родитель
//	Binomial_Heap* P = nullptr;
//	node* leftSon = nullptr; // левый сын
//	node* rightSon = nullptr; // правый сон
//	node* prNode = nullptr;
//	T key;
//	Pointer *owner; // указатель на себя
//
//	tree(T x, Pointer* p) : key(x), owner(p) {
//		p->change_son(this);
//		leftSon = new node();
//		rightSon = leftSon;
//	}
//	tree(const tree&) = delete;
//	tree& operator=(const tree&) = delete;
//	
//
//	~tree() {
//		if (owner != nullptr)
//			owner->change_son(nullptr);
//		while (leftSon->right != nullptr) {
//			del(leftSon);
//		}
//		delete leftSon;
//	}
//};
//
//template<typename T>
//struct Binomial_Heap<T>::node
//{
//	node* left = nullptr;
//	node* right = nullptr;
//	node() = default;
//	node(const node&) = delete;
//	node& operator=(const node&) = delete;
//	
//	node(node* left) : left(left)
//	{}
//
//	node(tree* Tree){
//		changeTree(Tree);
//		right = new node(this);
//	}
//
//	node(node* left, node* right, tree* Tree) : left(left), right(right) {
//		changeTree(Tree);
//	}
//
//	void insertLeft(tree* a) {
//		node *t = new node(this->left, this, a);
//		if (this->left != nullptr)
//		{
//			this->left->right = t;
//		}
//		this->left = t;
//	}
//
//	tree* getTree(){
//		return Tree;
//	}
//
//	void changeTree(tree* a) {
//		Tree = a;
//		if (Tree != nullptr) {
//			Tree->prNode = this;
//		}
//	}
//
//	void eraseNode() {
//		if (this->right == nullptr)
//			throw logic_error("i havent right!");
//		this->right->left = this->left;
//		if (this->left != nullptr)
//			this->left->right = this->right;
//	}
//
//	void insertMe(tree* a) {
//		this->changeTree(a);
//		this->right = new node(this);
//	}
//
//	~node() {
//		if (Tree != nullptr)
//			delete Tree;
//	}
//private:
//	tree* Tree = nullptr;
//};
//
//
//
//
//
//
//
//
////#pragma once
////
////
////template<class T>
////
////class BinomialHeap {
////
////private:
////	struct node {
////		node* left;
////		node* right;
////		node* parent;
////		node* next;
////		T key;
////		int degree;
////		node* own;
////		~node() {
////			if (left != nullptr) {
////				~left();
////			}
////			if (next!= nullptr) {
////				~next();
////			}
////			delete own;
////		}
////		void merge(node* I, node* other) {
////			if (I->key < other->key) {
////				other->parent = I;
////				I->next = other->next;
////				other->next = I->left;
////				I->left = other;
////			}
////			else {
////				I->parent = I;
////				I->next = other->left;
////				other->left = I;
////			}
////					
////		}
////	};
////	class Pointer {
////	
////	private:
////		node* ptr;
////	
////	public:
////		friend class Binomial_Heap<T>;
////	};
////
////	node* head;
////	node* min;
////
////
////public:
////	BinomialHeap() {
////		this->head = nullptr;
////		this->min = nullptr;
////	}
////
////
////	T get_min() {
////		return min->key;
////	}
////
////
////	bool is_Empty() {
////		return (this->head == nullptr);
////	}
////
////	Pointer insert(T value) {
////		node* cur = new node;
////		cur->key = value;
////		cur->parent = nullptr; cur->next = nullptr; cur->right = nullptr; cur->degree = 0;
////		Pointer res = cur;
////		cur->own = res;
////		BinomialHeap<T> a;
////		a.head = cur;
////		a.min = cur;
////		merge(b);
////		return res;
////	}
////
////
////	void merge(BinomialHeap &other) {
////		if (head == nullptr) {
////			head = other.head;
////			min = other.min;
////		}
////		if (other.head == nullptr) return;
////		node* list1_cur = head; node* list2_cur = other.head;
////		node* last = list1_cur;
////		if (head->degree > other.degree) {
////			last = other.degree;
////			list2_cur = other.head->next;
////		}
////		else {
////			list1_cur = head->next;
////		}
////		head = last;
////		while (list1_cur != nullptr && list2_cur != nullptr) {
////			if (list1_cur->degree < list2->degree) {
////				last->next = list1_cur;
////				list1_cur = list1_cur->next;
////			}
////			else {
////				last->next = list2_cur;
////				list2_cur = list2_cur->next;
////			}
////		}
////		while (list1_cur != nullptr) {
////			last->next = list1_cur;
////			list1_cur = list1_cur->next;
////		}
////		while (list2_cur != nullptr) {
////			last->next = list1_cur;
////			list2_cur = list2_cur->next;
////		}
////		last = head;
////		node* now = last;
////		node* cur = last->next;
////		while (cur != nullptr) {
////			if (cur->degree < now->degree) {
////				last->next = cur;
////				last = cur;
////				cur = cur->next;
////			}
////			else {
////				if (cur->degree == now->degree) {
////					merge(now, cur);
////					cur = now->next;
////				}
////				else {
////					last->next = now;
////					last = now;
////					now = cur;
////					cur = cur->next;
////				}
////			}
////		}
////		last->next = now;
////    }
////};