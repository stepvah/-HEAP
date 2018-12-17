#pragma once
#include<stdexcept>

using std::logic_error;
using std::invalid_argument;

template<typename T>
class Fibonacci_Heap
{
public:
	class Pointer;

	Pointer insert(T value) {
		Fibonacci_Heap<T>::Pointer p;
		addNode(new node(nullptr, nullptr, new tree(value, &p)));
		size++;
		return p;
	}


	Fibonacci_Heap() {
		left = new node();
		min = left;
		right = left;
	}


	T extract_min() {
		if (isEmpty()) {
			throw logic_error("Heap is Empty!");
		}

		T rez = min->getTree()->key;

		node* ptr = min;

		erase(ptr);
		update(ptr->getTree()->left);
		merge(ptr->getTree()->left);
		right = ptr->getTree()->right;

		size--;
		consolidate();

		ptr->getTree()->left = new node();
		delete ptr;

		return rez;
	}



	bool isEmpty() {
		return size == 0;
	}


	void decrease(Pointer *Ptr, T key) {
		if (!(Ptr->haveSon())) {
			throw logic_error("The arent this Point!");
		}

		tree* tc = Ptr->get_son();

		if (tc->key < key) {
			throw  logic_error("You cannot increase!"); // по условию можем только уменьшать
		}



		tc->key = key;

		if (tc->parent == nullptr) {
			updatemin(tc->parentNode);
		}

		if (tc->parent != nullptr && tc->parent->key > tc->key) {
			cut(tc);
		}
	}
	
	
	
	void merge(Fibonacci_Heap &other) {
		merge(other.left);
		updatemin(other.min);
		right = other.right;
		size += other.size;
		other.size = 0;
		other.left = new node();
		other.min = other.left;
		other.right = other.left;
	}

	Fibonacci_Heap(const Fibonacci_Heap&) = delete;
	Fibonacci_Heap& operator=(const Fibonacci_Heap&) = delete;
	
	
	~Fibonacci_Heap() {
		while (left->right != nullptr) {
			del(left);
		}
		delete left;
	}

private:

	struct tree;

	struct node {
		node* left = nullptr;
		node* right = nullptr;
		node() = default;
		node(const node&) = delete;
		node& operator=(const node&) = delete;
		node(node* left) : left(left)
		{}

		node(tree* Tree){
			change(Tree);
			right = new node(this);
		}


		node(node* left, node* right, tree* Tree) : left(left), right(right) {
			change(Tree);
		}

		void insertLeft(node* a) {
			a->left = this->left;
			a->right = this;
			if (this->left != nullptr) {
				this->left->right = a;
			}
			this->left = a;
		}


		tree* getTree() {
			return Tree;
		}


		void change(tree* a) {
			Tree = a;
			if (Tree != nullptr) {
				Tree->parentNode = this;
			}
		}

		void erase(){
			if (this->right == nullptr)
				throw logic_error("i not have right");
			this->right->left = this->left;
			if (this->left != nullptr)
				this->left->right = this->right;
			this->left = nullptr;
			this->right = nullptr;
		}


		~node(){
			if (Tree != nullptr) delete Tree;
		}



	private:
		tree* Tree = nullptr;
	};



	int size = 0;
	node* min = nullptr;
	node* left = nullptr;
	node* right = nullptr;



	void erase(node *a) {
		if (a == left) {
			left = left->right;
		}
		a->erase();
	}




	void cascading_cut(tree* a) {
		if (a->pr == nullptr) {
			return;
		}
		if (!a->mark) {
			a->mark = true;
		}
		else {
			cut(a);
		}
	}


	void cut(tree* a) {
		if (a->parent == nullptr) {
			return;
		}

		a->mark = false;
		a->parent->erase(a->parentNode);
		tree* ptr = a->parent;
		addNode(a->parentNode);
		a->parent = nullptr;
		cascading_cut(ptr);
	}


	void update(node* a) {
		while (a->getTree() != nullptr) {
			a->getTree()->parent = nullptr;
			a->getTree()->mark = false;
			a = a->right;
		}

	}


	void merge(node* a) {
		node* ptr = right;
		right = a;
		if (ptr->left != nullptr) {
			ptr->left->right = a;
		}

		else {
			left = a;
		}
		a->left = ptr->left;
		delete ptr;
	}



	void updatemin(node* other) {
		if (other->getTree() == nullptr) {
			throw logic_error("Invalid other - don't have tree!");
		}

		if (min->getTree() == nullptr || min->getTree()->key > other->getTree()->key)
			min = other;
	}


	void addNode(node *a) {
		right->insertLeft(a);
		if (left == right) {
			left = a;
		}
		updatemin(a);
	}



	void consolidate() {
		int r = siz();     /// находим максимальный размер массива
		node** st = new node*[r];  // это будет массив указателей на деревья с различными степенями
		for (int i = 0; i < r; ++i) {
			st[i] = nullptr;
		}

		node* cur = left;
		node* ptr;
		while (cur->right != nullptr) {
			cur = cur->right;

			ptr = cur->left;
			ptr->erase();
			conflict(st, ptr);
		}
		min = right;
		left = right;
		for (int i = 0; i < r; ++i) {
			if (st[i] == nullptr)
				continue;
			addNode(st[i]);
		}
		delete[] st;
	}





	static void del(node* &vertex) {
		if (vertex->right == nullptr) {
			throw logic_error("i havnt right!");
		}
		vertex = vertex->right;
		delete vertex->left;
	}


	
	
	void conflict(node** key, node* vertex) {
		if (key[vertex->getTree()->degree] == nullptr)
			key[vertex->getTree()->degree] = vertex;
		else {
			node* ptr = key[vertex->getTree()->degree];
			key[vertex->getTree()->degree] = nullptr;

			tree* c = mergetree(ptr->getTree(), vertex->getTree());
			vertex->change(c);

			ptr->change(nullptr);
			delete ptr;

			conflict(key, vertex);
		}
	}


	tree* mergetree(tree* first, tree* second) {
		if (second->key < first->key) {
			return mergetree(second, first);
		}

		node* ptr = new node(nullptr, nullptr, second);
		first->right->insertLeft(ptr);
		
		if (first->left == first->right) {
			first->left = ptr;
		}


		first->degree++;
		second->parent = first;
		return first;
	}


	int siz() { /// ищет наибольшую текущую степень для размера массива
		int x = 0, y = 1, z, n = 1;
		x = 0;
		y = 1;
		while (y < size) {
			z = x + y;
			x = y;
			y = z;
			n++;
		}
		return n + 1;
	}
};



template<typename T>
class Fibonacci_Heap<T>::Pointer
{
	friend Fibonacci_Heap;
public:
	Pointer(Pointer&& right) noexcept
	{
		son = right.son;
		son->Ptr = this;
		right.son = nullptr;
	}
	Pointer(const Pointer&) = delete;
	Pointer& operator=(const Pointer&) = delete;


	inline T get_value() const{
		if (!haveSon())
			throw logic_error("i haven't got son!!!");
		return son->key;
	}


	bool haveSon() const{
		return son != nullptr;
	}


	~Pointer(){
		if (son != nullptr) {
			son->Ptr = nullptr;
		}
	}


private:
	Pointer(tree *son_ = nullptr) :son(son_) {
		if (son != nullptr) {
			son_->Ptr = this;
		}
	}


	inline tree* get_son() {
		if (!haveSon()) {
			throw invalid_argument("There arent this Pointer!");
		}
		return son;
	}


	void change_son(tree* vertex){
		son = vertex;
		if (son != nullptr) {
			son->Ptr = this;
		}
	}

	tree* son = nullptr;
};



template<typename T>
struct Fibonacci_Heap<T>::tree
{    

	Pointer *Ptr;
	T key;
	node* left = nullptr;
	node* right = nullptr;
	node* parentNode = nullptr;
	int degree = 0;
	bool mark = false;
	tree* parent = nullptr;

	~tree() {
		if (Ptr != nullptr) {
			Ptr->change_son(nullptr);
		}
		while (left->right != nullptr) {
			del(left);
		}
		delete left;
	}


	tree(T key, Pointer* p) : key(key), Ptr(p) {
		p->change_son(this);
		left = new node();
		right = left;
	}


	tree(const tree&) = delete;
	tree& operator=(const tree&) = delete;
	
	
	void erase(node* a) {
		if (a == right) {
			throw invalid_argument("It's wrong!");
		}


		if (a == left){
			left = left->right;
		}


		degree--;
		a->erase();
	}

};



