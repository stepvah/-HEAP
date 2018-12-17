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
		left = new node();
		min = left;
	}


	T get_min() {
		return (min->getTree())->key;
	}
	
	
	T extract_min() {
		if (is_empty()) {
			throw logic_error("Heap is empty!");
		}
		T res = (min->getTree())->key;

		erase(min);
		node* tmp = min;

		this->merge(new Binomial_Heap(min->getTree()->left));

		tmp->getTree()->left = new node();
		delete tmp;

		return res;
	}


	bool is_empty() {
		return left->getTree() == nullptr;
	}


	void erase(Pointer *Ptr) {
		if (!(Ptr->haveSon()))
			throw logic_error("there arent this pointer!");
		tree* tc = Ptr->get_son();
		while (tc->parent != nullptr)
		{
			swap(tc->parent, tc);
			tc = tc->parent;
		}
		min = tc->prNode;
		extract_min();
	}


	void change(Pointer *ptr, T value) {
		if (!(ptr->haveSon()))
			throw logic_error("there arent this pointer!");

		
		tree* cur = ptr->get_son();

		if (cur->key < value)
			throw  logic_error("you are increase!"); // we cannot do increase for logn
		cur->key = value;
		while (cur->parent != nullptr) {
			if (cur->parent->key <= cur->key)
				break;
			swap(cur->parent, cur);
			cur = cur->parent;
		}
		if (cur->parent == nullptr)
			find_new_min();
	}


	void merge(Binomial_Heap &Heap_) {
		left = merge(left, Heap_.left);
		while (left->left != nullptr)
			left = left->left;

		Heap_.left = new node();
		Heap_.min = Heap_.left;

		find_new_min();
	}


	~Binomial_Heap() {
		while (left->right != nullptr) {
			del(left);
		}
		delete left;
	}

private:
	struct node;
	struct tree;

	explicit Binomial_Heap(node* a) {
		left = a;
		find_new_min();
	}


	tree * merge_tree(tree* first, tree* second) {
		if (second->key < first->key) {
			return merge_tree(second, first);
		}
		first->right->add(second);
		first->right = first->right->right;
		first->degree++;
		second->parent = first;
		return first;
	}


	void swap(tree* first, tree *second) {
		T value = first->key;
		first->key = second->key;
		second->key = value;

		Pointer* ptr = first->Ptr;
		if (second->Ptr != nullptr) second->Ptr->change_son(first);

		if (ptr != nullptr) ptr->change_son(second);
	}
	
	
	void find_new_min() {
		min = left;
		node* cur = left;
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
				first->add(second->getTree());
				first = first->right;
				second->changeTree(nullptr);
				del(second);
			}

			else {
				if (first->getTree()->degree > second->getTree()->degree) {
					first->insertLeft(second->getTree());

					second->changeTree(nullptr);
					del(second);
				}
				else {
					if (first->getTree()->degree < second->getTree()->degree) {
						first = first->right;
					}
					else {
						node* c = new node(merge_tree(first->getTree(), second->getTree()));

						node* ud = first;
						first = first->right;

						ud->erase();
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


	void erase(node *vertex) {
		if (vertex == left) {
			left = left->right;
		}
		vertex->erase();
	}


	static void del(node* &vertex) {
		if (vertex->right == nullptr) {
			throw logic_error("i havent right!");
		}
		vertex = vertex->right;
		delete vertex->left;
	}

	node* min = nullptr;
	node* left = nullptr;
};


template<typename T>
class Binomial_Heap<T>::Pointer
{
	friend Binomial_Heap;
public:
	Pointer(Pointer&& right) noexcept {
		son = right.son;
		son->Ptr = this;
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
		if (son != nullptr) son->Ptr = nullptr;
	}


private:
	explicit Pointer(tree *son_ = nullptr) :son(son_)
	{
		if (son != nullptr)
			son_->Ptr = this;
	}

	inline tree* get_son() {
		if (!haveSon())
			throw logic_error("i haven't got son!");
		return son;
	}


	void change_son(tree* son_) {
		son = son_;
		if (son != nullptr)
			son->Ptr = this;
	}


	tree* son = nullptr;
};


template<typename T>
struct Binomial_Heap<T>::tree
{
	int degree = 0;
	tree* parent = nullptr; // родитель
	Binomial_Heap* P = nullptr;
	node* left = nullptr; // левый сын
	node* right = nullptr; // правый сон
	node* prNode = nullptr;
	T key;
	Pointer *Ptr; // указатель на себя

	
	
	tree(T x, Pointer* p) : key(x), Ptr(p) {
		p->change_son(this);
		left = new node();
		right = left;
	}



	tree(const tree&) = delete;
	tree& operator=(const tree&) = delete;
	

	~tree() {
		if (Ptr != nullptr)
			Ptr->change_son(nullptr);
		while (left->right != nullptr) {
			del(left);
		}
		delete left;
	}
};

template<typename T>
struct Binomial_Heap<T>::node
{
	node* left = nullptr;
	node* right = nullptr;
	node() = default;
	node(const node&) = delete;
	node& operator=(const node&) = delete;
	
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

	void erase() {
		if (this->right == nullptr)
			throw logic_error("i havent right!");
		this->right->left = this->left;
		if (this->left != nullptr)
			this->left->right = this->right;
	}

	void add(tree* a) {
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