#pragma once
#include<stdexcept>

using std::logic_error;
using std::out_of_range;


template<class T>

class vector {
private:
	unsigned int MaxSize;
	unsigned int curSize;
	T* Array;
	void resize_in(unsigned int Size) {
		T* newArray = new T[Size];
		for (unsigned int i = 0; i < curSize; i++) {
			newArray[i] = Array[i];
		}
		delete[] Array;
		Array = newArray;
		MaxSize = Size;
	}
public:
	vector() {
		MaxSize = 1; curSize = 0; Array = new T[1];
	}
	bool isEmpty() {
		return !curSize;
	}
	unsigned int size() {
		return MaxSize;
	}
	unsigned int Real_size() {
		return curSize;
	}

	~vector() {
		delete[] Array;
	}

	T& operator [] (int ind) {
		if (ind < 0 || ind >= curSize) {
			throw out_of_range("Out of range");
		}
		return Array[ind];
	}

	void push_back(T key) {
		if (curSize == MaxSize) {
			resize_in(2 * MaxSize);
		}
		Array[curSize] = key;
		curSize++;
	}

	void pop_back() {
		if (curSize == 0) {
			throw logic_error("Empty");
		}
		curSize--;
		if (curSize * 4 <= MaxSize) {
			resize_in(MaxSize / 2);
		}
	}

	void resize(unsigned int Size) {
		bool f = false;
		if (Size == 0) {
			Size += 1;
			f = true;
		}
		T* newArray = new T[Size];
		for (unsigned int i = 0; i < Size; i++) {
			if (i >= curSize) {
				newArray[i] = 0;
			}
			else {
				newArray[i] = Array[i];
			}
		}
		delete[] Array;
		Array = newArray;
		curSize = Size;
		MaxSize = Size;
		if (f) curSize--;
	}
};
#pragma once