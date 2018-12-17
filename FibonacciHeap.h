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


	void erase(node *a) {
		if (a == left) {
			left = left->right;
		}
		a->erase();
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

		node* tc = left;
		node* ptr;
		while (tc->right != nullptr) {
			tc = tc->right;

			ptr = tc->left;
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


	void updatemin(node* other) {
		if (other->getTree() == nullptr) {
			throw logic_error("Invalid other - don't have tree!");
		}

		if (min->getTree() == nullptr || min->getTree()->key > other->getTree()->key)
			min = other;
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




	static void del(node* &a) {
		if (a->right == nullptr) {
			throw logic_error("i havnt right!");
		}
		a = a->right;
		delete a->left;
	}


	
	
	void conflict(node** key, node* a) {
		if (key[a->getTree()->degree] == nullptr)
			key[a->getTree()->degree] = a;
		else {
			node* ptr = key[a->getTree()->degree];
			key[a->getTree()->degree] = nullptr;

			tree* c = mergetree(ptr->getTree(), a->getTree());
			a->change(c);

			ptr->change(nullptr);
			delete ptr;

			conflict(key, a);
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

	int size = 0;
	node* min = nullptr;
	node* left = nullptr;
	node* right = nullptr;
};



template<typename T>
class Fibonacci_Heap<T>::Pointer
{
	friend Fibonacci_Heap;
public:
	Pointer(Pointer&& right) noexcept
	{
		son = right.son;
		son->owner = this;
		right.son = nullptr;
	}
	Pointer(const Pointer&) = delete;
	Pointer& operator=(const Pointer&) = delete;
	inline T get_value() const
	{
		if (!haveSon())
			throw logic_error("i haven't got son!!!");
		return son->key;
	}
	bool haveSon() const
	{
		return son != nullptr;
	}
	~Pointer()
	{
		if (son != nullptr)
			son->owner = nullptr;
	}
private:
	Pointer(tree *son_ = nullptr) :son(son_) {
		if (son != nullptr) {
			son_->owner = this;
		}
	}


	inline tree* get_son() {
		if (!haveSon()) {
			throw invalid_argument("There arent this Pointer");
		}
		return son;
	}


	void change_son(tree* vertex){
		son = vertex;
		if (son != nullptr) {
			son->owner = this;
		}
	}

	tree* son = nullptr;
};



template<typename T>
struct Fibonacci_Heap<T>::tree
{
	int degree = 0;
	bool mark = false;
	tree* parent = nullptr;
	node* left = nullptr;
	node* right = nullptr;
	node* parentNode = nullptr;
	T key;
	Pointer *owner;
	tree(T T, Pointer* p) : key(T), owner(p) {
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


	~tree() {
		if (owner != nullptr) {
			owner->change_son(nullptr);
		}
		while (left->right != nullptr) {
			del(left);
		}
		delete left;
	}
};


//#pragma once
//#include<memory>
//#include<stdexcept>
//#include "MyVector.h"
//
//
//
//using std:: logic_error;
//using std::invalid_argument;
//
//
//using std::weak_ptr;       // вроде можем использовать умные указатели в лабе?
//using std::shared_ptr;
//
//
//template<typename T>
//class FibonacciHeap {
//private:
//	struct node{
//		T key;
//		int degree; // степень вершины
//		bool mark; // метка для вершины, что у нее удалили 1 ребенка
//		shared_ptr<node> child; // произвольный ребенок
//		weak_ptr<node> parent; // родитель
//		shared_ptr<node> left;
//		weak_ptr<node> right;
//		
//		node(T key) : key(key), degree(0), mark(false) { // делаем конструктор
//			child = shared_ptr<node>(); parent = shared_ptr<node>(); left = shared_ptr<node>(); right = shared_ptr<node>();
//		}
//
//	};
//
//
//
//	void union_(shared_ptr<node> first, shared_ptr<node> second) { // объединяю 2 списка, перевешивая указатели на соседей
//		if (second == nullptr) return;
//		shared_ptr<node> L = first->left;
//		shared_ptr<node> R = second->right.lock(); // перехожу к shared ptr cпомощью метода lock
//		second->right = first;
//		first->left = second;
//		L->right = R;
//		R->left = L;
//	}
//	
//	
//	shared_ptr<node> min; // вершина с минимумом, который находится в одной из вершин верхнего уровня
//
//	/*shared_ptr<node> tree_merge(shared_ptr<node> root1, shared_ptr<node> root2) {
//		if (root1 == nullptr)
//			return root2;
//		if (root2 == nullptr)
//			return root1;
//		if (root1->key > root2->key)
//		{
//			tree_merge(root2, root1);
//		}
//		else
//		{
//			root1->left = root1;
//			root2->left = root2;
//			root1->right.lock() = root1;
//			root2->right.lock() = root2;
//			shared_ptr<node> curNode1 = root1->child;
//			shared_ptr<node> curNode2 = root2;
//			root1->child = root2;
//			root2->parent = root1;
//			(root1->degree)++;
//			if (curNode1 != nullptr)
//			{
//				curNode2->left = curNode1;
//				curNode1->right.lock() = curNode2;
//			}
//			return root1;
//		}
//	}*/
//
//
//
//	void consolidate() { // улучшаем кучу до примерно похожего на биномиальное
//		/*vector<shared_ptr<node>> A;
//		for (int i = 0; i <= min->degree; i++) {
//			A.push_back(nullptr);
//		}
//		A[min->degree] = min;
//		shared_ptr<node> current = min->left;
//		for (int i = A.Real_size(); i <= current->degree; i++) {
//			A.push_back(nullptr);
//		}
//		while (current != A[current->degree])
//		{
//			if (A[current->degree] == nullptr)
//			{
//				A[current->degree] = current;
//				current = current->left;
//			}
//			else
//			{
//				shared_ptr<node> conflict;
//				shared_ptr<node> L = (A[current->degree]->right).lock();
//				shared_ptr<node> R = A[current->degree]->left;
//				L->left = R;
//				R->right = L;
//				if (L->left == L)
//					min = nullptr;
//				else
//				{
//					L = current->right.lock();
//					R = current->left;
//				}
//
//				conflict = tree_merge(A[current->degree], current);
//				if (min = nullptr)
//				{
//					min = conflict;
//					return;
//				}
//				conflict->right = L;
//				conflict->left = R;
//				(conflict->left)->right.lock() = conflict;
//				(conflict->right.lock())->left = conflict;
//				A[current->degree] = nullptr;
//				current = conflict;
//
//
//
//			}
//			for (int i = A.Real_size(); i <= current->degree; i++) {
//				A.push_back(nullptr);
//			}
//		}
//		shared_ptr<node> tmp = current;
//		min = current;
//		while (current != tmp)
//		{
//			if (current->key < min->key)
//				min = current;
//			current = current->left;
//		}*/
//
//		vector<shared_ptr<node>> Array;
//		shared_ptr<node> cur = min;        // возможно, что сейчас это не минимум. Но в итоге мы его обновим
//		bool end = false;
//		while (end == false) {
//			//std:: cout << cur->key << ' ';
//			if (cur->key < min->key) min = cur; // обновляем минимум
//
//			if (Array.Real_size() <= cur->degree) { // изменяем размер массива - до наибольшей возможной степени дерева
//				for (int i = Array.Real_size(); i <= cur->degree; i++) {
//					Array.push_back(nullptr);
//				}
//			}
//			if (cur->left == cur) end = true; // двусвязный список зациклился. Мы ,возможно, сможем победить положив в список эту вершину
//
//			shared_ptr<node> next = cur->left; // иначе смотрим на следующего соседа
//			
//			cur->left->right = cur->right; // переносим ссылки 
//			cur->right.lock()->left = cur->left; // через cur
//
//
//			cur->left = cur; // оставляем cur одного 
//			cur->right = cur; // в своем списке
//
//			if (Array[cur->degree] == nullptr) { // если нет дерева такой степени, то просто сейчас оно единственное и мы его кладем
//				Array[cur->degree] = cur;
//			}
//
//			else {
//				shared_ptr<node> now_degree = Array[cur->degree]; // с чем придется объединять cur
//
//				shared_ptr<node> add, to; // что к чему добавляем 
//
//
//				if (now_degree->key < cur->key) { // в зависимости
//					to = now_degree;      // от того, чей
//					add = cur;         // приоритет меньше
//				}
//				else {
//					add = now_degree;  // понимаем, что к 
//					to = cur;          // кому подвешивать
//				}
//
//
//				if (to->child == nullptr) { // если нет ребенка у него, то 
//					to->child = add;        // подвесить легко
//				}
//
//				else {
//					union_(to->child, add); // иначе объединяем всех детей со списком двусвязным соседей  и получаем следующий уровень
//				}
//
//				add->parent = to; // присваиваем родителя
//
//
//
//				Array[to->degree] = nullptr; // мы объединили, и потому нет больше дерева с такой степенью 
//
//				to->degree++; // степень увеличилась
//
//				if (end) {        // мы на самом деле не победили 
//					next = to;    // и текущее полученное дерево надо
//					end = false;  // надо тоже куда-то засунуть
//				}
//
//				else {
//					union_(next, to); // если тогда не была одна вершина в списке, то сейчас следует объединить
//				}
//
//			}
//			cur = next;
//		} // сейчас у нас есть по не более чем одному дереву каждой степени
//
//		//std::cout << '\n';
//		//std::cout << 'y' << '\n';
//		for (int i = 0; i < Array.Real_size(); i++) {              // объединяем все деревья в один
//			if (Array[i] != nullptr && Array[i] != min) {// большой список
//				//std::cout << Array[i]->key << ' ';
//				union_(min, Array[i]);
//			}
//		}
//		//std::cout << '\n';
//	}
//
//
//
//
//	void cut(shared_ptr<node> vertex) {  // вырезаем веришну
//		shared_ptr<node> L = vertex->left;
//		shared_ptr<node> R = (vertex->right).lock();
//		
//		
//		R->left = L;
//		L->right = R;               // аккуратно перевешиваю все указатели,
//		vertex->right = vertex;    //  чтобы все связи пропали
//		vertex->left = vertex;
//
//		vertex->parent.lock()->degree--; // уменьшаем степень сына
//
//
//		if (vertex->parent.lock().child == vertex) { // проверяем, чтобы родитель не потерял ссылку на детей
//			
//			
//			if (vertex->right.lock() == vertex) {                       // случай единственного сына
//				vertex->parent.lock()->child = shared_ptr<node>(nullptr);
//			}
//
//			else {
//				vertex->parent.lock().child = vertex->right.lock(); // иначе делаем ссылку на любого другого сына
//			}
//		}
//
//		vertex->parent = weak_ptr<node>();
//		union_(min, vertex); // сливаем поддерево нашей вершины со всем верхним списком деревьев
//	}
//
//
//
//
//	void cascading_cut(shared_ptr<node> vertex) { // так сохраняется свойство того, что из биномиального дерева из каждой вершины удалили не более одного ребенка
//		
//		while (vertex->mark) {       // пока вершина помечена, вырезаем ее
//			cut(vertex);
//			vertex = vertex->parent.lock(); // и переходим к родителю
//		}
//		vertex->mark = true; // пометим последнюю невырезанную вершину - в следующий раз будем вырезать
//	}
//
//
//public:
//	class Pointer {
//	public:
//		friend class FibonacciHeap;
//		Pointer() {}
//	private:
//		weak_ptr<node> Ptr;
//		Pointer(weak_ptr<node> ptr_) {
//			Ptr = ptr_;
//		}
//	};
//
//	
//	FibonacciHeap() {
//		min = shared_ptr<node>(nullptr); // пустой конструктор - просто говорим, чему равен минимум - пустому указателю
//	}
//
//
//	bool is_empty() {
//		return min == nullptr; // проверяем, что минимума просто нет
//	}
//
//	Pointer insert(T key) {
//		node* ptr = new node(key); // создали новую вершину
//		shared_ptr<node> node_(ptr); // создаем указатель
//
//		if (is_empty()) {       // если куча пустая, то все понятно
//			min = node_;
//			min->left = node_;
//			min->right = node_;
//		}
//
//		else {
//			weak_ptr<node> prevRight = min->right;
//			
//			min->right = node_; //- вставляем справа от минимального и 
//			node_->left = min; // аккуратно меняем указатели
//			node_->right = prevRight;
//			prevRight.lock()->left = node_;
//		}
//		
//
//		if (node_->key < min->key) { // обновляем минимум, не забываем
//			min = node_;
//		}
//
//		return Pointer(node_); // возвращаем с помощью конструктора ссылку на новую вершину
//	}
//
//
//	T get_min() { // он находится в минимуме - все легко
//		return min->key;
//	}
//
//
//	void write() {
//		shared_ptr<node> now = min;
//		for (int i = 0; i <= 7; i++) {
//			if (now == nullptr) {
//				std::cout << 'r';
//			}
//			std::cout << now->key << ' ';
//			now = now->right.lock();
//			if (now->child != nullptr) {
//				std::cout << 'r' << now->child->key << ' ';
//			}
//		}
//		std::cout << '\n';
//	}
//
//	T extract_min() {
//        if (is_empty()) { // обрабатываем неправильный запрос
//	        throw logic_error("Heap is empty");
//        }
//
//        shared_ptr<node> prev_min = min;
//        if (min->child != nullptr) {
//	        union_(min, min->child); // объединяем детей минимума с верхним списком, так как мы сейчас выкинем их родителя
//        }
//
//
//        if (prev_min->right.lock() == prev_min) { //рассмотрим случай с одним элементом
//			min = shared_ptr<node>(nullptr);  // - просто непонятно, чему равен - потом обновим в consolidate
//	        return prev_min->key;
//        }
//        
//		
//		shared_ptr<node> L = min->left;
//        shared_ptr<node> R = min->right.lock();
//        
//		
//		L->right = R; // аккуратно меняем указатели, чтобы удалить минимум
//        R->left = L;
//        
//		
//		min = min->right.lock(); // тоже пока что обновляем минимум на что-то
//        prev_min->left = prev_min;
//        prev_min->right = prev_min;
//
//		//std:: cout << min->key << '\n';
//		consolidate(); // перестраиваем получше дерево
//		//write();
//        return prev_min->key; // возвращаем минимум
//    }
//
//
//	void merge(FibonacciHeap& other) {
//		if (other.is_empty()) return; // разбираем случаи с пустыми кучами
//
//		if (is_empty()) {
//			min = other.min;
//		}
//		else {
//			union_(min, other.min); // просто сливаем 2 списка и не паримся, как в биномиальной
//
//			// обновим минимум:
//			if (other.min->key < min->key) {
//				min = other.min;
//			}
//		}
//	}
//
//
//	void decrease(Pointer ptr, T key) {
//		if (ptr.Ptr.expired()) { // если неправильный вызов
//			throw invalid_argument("There arent this Pointer!");
//		}
//
//
//		if (ptr.Ptr.lock()->key < key) { // если увеличили, что плохо делать
//			throw logic_error("We cannot increase!");
//		}
//
//
//		if (ptr.Ptr.lock()->parent.expired() || ptr.Ptr.lock()->parent.lock()->key < key) { // если свойство кучи сохранилось
//
//			ptr.Ptr.lock()->key = key;                           // то просто обновляем key и минимум.
//			if (ptr.Ptr.lock()->key < min->key) {
//				min = ptr.Ptr.lock();
//			}
//		}
//
//		else {              // вызываем consolidate и тоже обновляем минимум
//			ptr.Ptr.lock()->key = key;
//			cut(ptr.Ptr.lock());
//			cascading_cut(ptr.Ptr.lock()->parent.lock());
//			if (ptr.Ptr.lock()->key < min->key) {
//				min = ptr.Ptr.lock();
//			}
//		}
//
//	}
//
//};
