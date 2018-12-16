#pragma once
#include<stdexcept>

using std::logic_error;
using std::out_of_range;


template<typename T>
class Binomial_Heap
{
public:
	class Pointer;

	Pointer insert(T key) {
		Binomial_Heap<T>::Pointer p;
		this->merge(new Binomial_Heap(new node(new tree(key, &p))));
		return p;
	}


	Binomial_Heap() {
		leftSon = new node();
		min = leftSon;
	}
	
	
	T extract_min() {
		if (is_empty()) {
			throw logic_error("Heap is empty!");
		}
		T res = (min->getTree())->key;

		eraseNode(min);
		node* tmp = min;

		this->merge(new Binomial_Heap(min->getTree()->leftSon));

		tmp->getTree()->leftSon = new node();
		delete tmp;

		return res;
	}


	bool is_empty() {
		return leftSon->getTree() == nullptr;
	}


	void erase(Pointer *Ptr) {
		if (!(Ptr->haveSon()))
			throw logic_error("there arent this pointer!");
		tree* tc = Ptr->get_son();
		while (tc->pr != nullptr)
		{
			swap(tc->pr, tc);
			tc = tc->pr;
		}
		min = tc->prNode;
		extract_min();
	}


	void change(Pointer *Ptr, T value) {
		if (!(Ptr->haveSon()))
			throw logic_error("there arent this pointer!");
		tree* tc = Ptr->get_son();
		if (tc->key < value)
			throw  logic_error("you are increase!"); // we cannot do increase for logn
		tc->key = value;
		while (tc->parent != nullptr)
		{
			if (tc->parent->key <= tc->key)
				break;
			swap(tc->parent, tc);
			tc = tc->parent;
		}
		if (tc->parent == nullptr)
			find_new_min();
	}


	void merge(Binomial_Heap &Heap_) {
		leftSon = merge(leftSon, Heap_.leftSon);
		while (leftSon->left != nullptr)
			leftSon = leftSon->left;

		Heap_.leftSon = new node();
		Heap_.min = Heap_.leftSon;

		find_new_min();
	}


	~Binomial_Heap() {
		while (leftSon->right != nullptr) {
			del(leftSon);
		}
		delete leftSon;
	}

private:
	struct node;
	struct tree;

	Binomial_Heap(node* a) {
		leftSon = a;
		find_new_min();
	}


	tree * merge_tree(tree* first, tree* second) {
		if (second->key < first->key) {
			return merge_tree(second, first);
		}
		first->rightSon->insertMe(second);
		first->rightSon = first->rightSon->right;
		first->st++;
		second->parent = first;
		return first;
	}


	void swap(tree* first, tree *second) {
		T value = first->key;
		first->key = second->key;
		second->key = value;

		Pointer* ptr = first->owner;
		if (second->owner != nullptr) second->owner->change_son(first);

		if (ptr != nullptr) ptr->change_son(second);
	}
	
	
	void find_new_min() {
		min = leftSon;
		node* cur = leftSon;
		while (cur->getTree() != nullptr) {
			if (min->getTree() == nullptr || (min->getTree())->key > (cur->getTree())->key) {
				min = cur;
			}
			cur = cur->right;
		}
	}


	void merge(Binomial_Heap *heap_) {
		merge(*heap_);
		delete heap_;
	}


	node* merge(node* first, node* second) {
		while (second->getTree() != nullptr) {
			second->getTree()->P = this;
			second->getTree()->parent = nullptr;
			
			if (first->getTree() == nullptr) {
				first->insertMe(second->getTree());
				first = first->right;
				second->changeTree(nullptr);
				del(second);
			}

			else {
				if (first->getTree()->st > second->getTree()->st) {
					first->insertLeft(second->getTree());

					second->changeTree(nullptr);
					del(second);
				}
				else {
					if (first->getTree()->st < second->getTree()->st) {
						first = first->right;
					}
					else {
						node* c = new node(merge_tree(first->getTree(), second->getTree()));

						node* ud = first;
						first = first->right;

						ud->eraseNode();
						ud->changeTree(nullptr);
						delete ud;

						first = merge(first, c);

						second->changeTree(nullptr);
						del(second);
					}
				}
			}
		}
		delete second;
		return first;
	}


	void eraseNode(node *a) {
		if (a == leftSon) {
			leftSon = leftSon->right;
		}
		a->eraseNode();
	}


	static void del(node* &a) {
		if (a->right == nullptr) {
			throw logic_error("i havent right!");
		}
		a = a->right;
		delete a->left;
	}

	node* min = nullptr;
	node* leftSon = nullptr;
};


template<typename T>
class Binomial_Heap<T>::Pointer
{
	friend Binomial_Heap;
public:
	Pointer(Pointer&& right) noexcept {
		son = right.son;
		son->owner = this;
		right.son = nullptr;
	}

	Pointer(const Pointer&) = delete;
	Pointer& operator=(const Pointer&) = delete;
	
	inline T get_value() {
		if (!haveSon()) {
			throw logic_error("i haven't son!");
		}
		return son->key;
	
	}
	bool haveSon() {
		return son != nullptr;
	}

	~Pointer() {
		if (son != nullptr) son->owner = nullptr;
	}


private:
	explicit Pointer(tree *son_ = nullptr) :son(son_)
	{
		if (son != nullptr)
			son_->owner = this;
	}

	inline tree* get_son() {
		if (!haveSon())
			throw logic_error("i haven't got son!");
		return son;
	}


	void change_son(tree* son_) {
		son = son_;
		if (son != nullptr)
			son->owner = this;
	}


	tree* son = nullptr;
};


template<typename T>
struct Binomial_Heap<T>::tree
{
	int st = 0;
	tree* parent = nullptr; // родитель
	Binomial_Heap* P = nullptr;
	node* leftSon = nullptr; // левый сын
	node* rightSon = nullptr; // правый сон
	node* prNode = nullptr;
	T key;
	Pointer *owner; // указатель на себя

	tree(T x, Pointer* p) : key(x), owner(p) {
		p->change_son(this);
		leftSon = new node();
		rightSon = leftSon;
	}
	tree(const tree&) = delete;
	tree& operator=(const tree&) = delete;
	

	~tree() {
		if (owner != nullptr)
			owner->change_son(nullptr);
		while (leftSon->right != nullptr) {
			del(leftSon);
		}
		delete leftSon;
	}
};

template<typename T>
struct Binomial_Heap<T>::node
{
	node* left = nullptr;
	node* right = nullptr;
	node() = default;
	
	node(node* left) : left(left)
	{}

	node(tree* Tree){
		changeTree(Tree);
		right = new node(this);
	}

	node(node* left, node* right, tree* Tree) : left(left), right(right) {
		changeTree(Tree);
	}

	void insertLeft(tree* a) {
		node *t = new node(this->left, this, a);
		if (this->left != nullptr)
		{
			this->left->right = t;
		}
		this->left = t;
	}

	tree* getTree(){
		return Tree;
	}

	void changeTree(tree* a) {
		Tree = a;
		if (Tree != nullptr) {
			Tree->prNode = this;
		}
	}

	void eraseNode() {
		if (this->right == nullptr)
			throw logic_error("i havent right!");
		this->right->left = this->left;
		if (this->left != nullptr)
			this->left->right = this->right;
	}

	void insertMe(tree* a) {
		this->changeTree(a);
		this->right = new node(this);
	}

	~node() {
		if (Tree != nullptr)
			delete Tree;
	}
private:
	tree* Tree = nullptr;
};








//#pragma once
//
//
//template<class T>
//
//class BinomialHeap {
//
//private:
//	struct node {
//		node* left;
//		node* right;
//		node* parent;
//		node* next;
//		T key;
//		int degree;
//		node* own;
//		~node() {
//			if (left != nullptr) {
//				~left();
//			}
//			if (next!= nullptr) {
//				~next();
//			}
//			delete own;
//		}
//		void merge(node* I, node* other) {
//			if (I->key < other->key) {
//				other->parent = I;
//				I->next = other->next;
//				other->next = I->left;
//				I->left = other;
//			}
//			else {
//				I->parent = I;
//				I->next = other->left;
//				other->left = I;
//			}
//					
//		}
//	};
//	class Pointer {
//	
//	private:
//		node* ptr;
//	
//	public:
//		friend class Binomial_Heap<T>;
//	};
//
//	node* head;
//	node* min;
//
//
//public:
//	BinomialHeap() {
//		this->head = nullptr;
//		this->min = nullptr;
//	}
//
//
//	T get_min() {
//		return min->key;
//	}
//
//
//	bool is_Empty() {
//		return (this->head == nullptr);
//	}
//
//	Pointer insert(T value) {
//		node* cur = new node;
//		cur->key = value;
//		cur->parent = nullptr; cur->next = nullptr; cur->right = nullptr; cur->degree = 0;
//		Pointer res = cur;
//		cur->own = res;
//		BinomialHeap<T> a;
//		a.head = cur;
//		a.min = cur;
//		merge(b);
//		return res;
//	}
//
//
//	void merge(BinomialHeap &other) {
//		if (head == nullptr) {
//			head = other.head;
//			min = other.min;
//		}
//		if (other.head == nullptr) return;
//		node* list1_cur = head; node* list2_cur = other.head;
//		node* last = list1_cur;
//		if (head->degree > other.degree) {
//			last = other.degree;
//			list2_cur = other.head->next;
//		}
//		else {
//			list1_cur = head->next;
//		}
//		head = last;
//		while (list1_cur != nullptr && list2_cur != nullptr) {
//			if (list1_cur->degree < list2->degree) {
//				last->next = list1_cur;
//				list1_cur = list1_cur->next;
//			}
//			else {
//				last->next = list2_cur;
//				list2_cur = list2_cur->next;
//			}
//		}
//		while (list1_cur != nullptr) {
//			last->next = list1_cur;
//			list1_cur = list1_cur->next;
//		}
//		while (list2_cur != nullptr) {
//			last->next = list1_cur;
//			list2_cur = list2_cur->next;
//		}
//		last = head;
//		node* now = last;
//		node* cur = last->next;
//		while (cur != nullptr) {
//			if (cur->degree < now->degree) {
//				last->next = cur;
//				last = cur;
//				cur = cur->next;
//			}
//			else {
//				if (cur->degree == now->degree) {
//					merge(now, cur);
//					cur = now->next;
//				}
//				else {
//					last->next = now;
//					last = now;
//					now = cur;
//					cur = cur->next;
//				}
//			}
//		}
//		last->next = now;
//    }
//};