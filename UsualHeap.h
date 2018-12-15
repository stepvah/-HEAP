#pragma once
#include <stdexcept>
#include "MyVector.h"

using std::logic_error;
using std::out_of_range;
template < class T>
struct node {
	T key;
	int* ptr;
	bool INF_;
	/*node(int* ind, T value): key(value), ptr(ind), INF_(0) {
	}*/

};

template<class T>

class heap {

public:
	class Pointer {
	public:
		template <class T>
		friend class heap;
	private:
		int* place;
		Pointer() {

		}
	};

private:
	int ord = 2;
	bool optimaz = false;
	int num = 0;
	vector<node<T>> v;
	template<class T1>
	void swap1(T1 a, T1 b) {
		T1 c = a;
		a = b;
		b = c;
	}


	void swap(int ind1, int ind2) {
		bool f = v[ind1].INF_;
		v[ind1].INF_ = v[ind2].INF_;
		v[ind2].INF_ = f;
		T c = v[ind1].key;
		v[ind1].key = v[ind2].key;
		v[ind2].key = c;
		int* a;
		a = v[ind1].ptr;
		v[ind1].ptr = v[ind2].ptr;
		v[ind2].ptr = a;
		*(v[ind1].ptr) = ind1;
		*(v[ind2].ptr) = ind2;
	}

	void sift_down(unsigned int index) {
		unsigned int cur = index;
		while (1) {
			unsigned int can = cur;
			for (int i = ord * cur + 1; i <= ord * cur + ord && i < v.Real_size(); i++) {
				if (v[i].key < v[can].key) can = i;
			}

			if (cur == can) break;
			swap(cur, can);
			cur = can;
		}
	}


	void sift_up(unsigned int index) {
		while (index > 0 && (v[index].key < v[(index - 1) / ord].key || v[index].INF_ == 1)) {
			swap(index, (index - 1) / ord);
			index--; index = index / ord;
		}
	}

public:
	heap() {

	}

	Pointer insert(T key) {
		int* element = new int;
		*element = v.Real_size();
		node<T> add;
		add.key = key; add.ptr = element; add.INF_ = 0;
		v.push_back(add);
		Pointer res;
		res.place = element;
		sift_up(v.Real_size() - 1);
		if (optimaz) {
			num--;
			if (num == 0) optimaz = false;
		}
		return res;
	}



	T extract_min() {
		if (is_empty())
			throw logic_error("Heap is Empty!!!");
		T ans = v[0].key;
		delete v[0].ptr;
		v[0] = v[v.Real_size() - 1];
		*v[0].ptr = 0;
		v.pop_back();
		sift_down(0);
		if (optimaz) {
			num--;
			if (num == 0) optimaz = false;
 		}
		return ans;
	}



	void delet(Pointer ptr) {
		v[*ptr.place].INF_ = 1;
		sift_up(*ptr.place);
		extract_min();
	}


	void change(Pointer ptr, T key) {
		if (v[*ptr.place].key > key) {
			v[*ptr.place].key = key;
			sift_up(*ptr.place);
		}
		else {
			v[*ptr.place].key = key;
			sift_down(*ptr.place);
		}
	}


	T get_value(Pointer ptr) {
		return (v[*(ptr.place)]).key;
	}


	template<typename Iterator>
	heap(Iterator begin, Iterator end);

	template<class Iterator>
	heap IPriorityQueue(Iterator begin, Iterator end) {
		heap a();
		for (; begin != end; begin++) {
			v.push_back(new node<T>(*begin, v.Real_size()*, 0));
		}
		for (int i = v.Real_size() - 1; i >= 0; i--) {
			sift_down(i);
		}
		return a;S
	}
	T get_min() {
		return v[0].key;
	}
	bool is_empty() {
		return v.isEmpty();
	}

	void optimize(size_t insertCount, size_t extractCount) {
		if (!is_empty()) {
			throw logic_error("Heap is not empty!");
		}
		if (extractCount == 0) extractCount = 1;
		if ((insertCount / extractCount) > 2) {
			ord = insertCount / extractCount;
		}
		else { 
			ord = 2; 
		}
		num = insertCount + extractCount;
		optimaz = true;
	}

};


template<typename T>
template<typename Iterator>
heap<T>::heap(Iterator begin, Iterator end)
{
	heap a();
	for (; begin != end; begin++) {
		node<T> *s = new node<T>;
		(*s).key = *begin;
		int* y = new int;
		*y = v.Real_size();
		(*s).ptr = y;
		(*s).INF_ = 0;
		v.push_back(*s);
	}
	for (int i = v.Real_size() - 1; i >= 0; i--) {
		sift_down(i);
	}
}