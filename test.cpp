#include "pch.h"
#include<stdexcept>
#include<gtest/gtest.h>
#include "C:\Users\stepvah\Desktop\Heap\MyVector.h"
#include "C:\Users\stepvah\Desktop\Heap\UsualHeap.h"

using std::logic_error;
using std::out_of_range;

class ClassDeclaration : public testing::Test {
public:
	ClassDeclaration() {}
};



TEST_F(ClassDeclaration, pushback1) {
	vector<int> a;
	for (int  i = 0; i < 100; ++i)
		a.push_back(i + 1);
	for (int  i = 0; i < 100; ++i)
		ASSERT_EQ(i + 1, a[i]);
}


TEST_F(ClassDeclaration, pushback2) {
	vector<int> a;
	int* res = new int[100000];

	for (int i = 0; i < 100000; i++){
		res[i] = rand() % 1000;
		a.push_back(res[i]);
	}


	for (int i = 0; i < 100000; i++) {
		ASSERT_EQ(res[i], a[i]);
	}


	delete[] res;
}

TEST_F(ClassDeclaration, popback1) {
	vector<int> a;
	for (int i = 0; i < 100; i++) {
		a.push_back(i + 1);
	}
	for (int i = 0; i < 50; i++) {
		a.pop_back();
	}
	for (int i = 0; i < 50; i++) {
		ASSERT_EQ(i + 1, a[i]);
	}
}

TEST_F(ClassDeclaration, popback2) {
	vector<int> a;
	int* res = new int[100];
	for (int i = 0; i < 100; i++) {
		res[i] = rand() % 1000;
		a.push_back(res[i]);
	}
	for (int i = 0; i < 50; i++) {
		a.pop_back();
	}
	for (int i = 0; i < 50; i++) {
		res[i + 50] = rand() % 1000;
		a.push_back(res[i + 50]);
	}
	for (int i = 0; i < 100; i++) {
		ASSERT_EQ(res[i], a[i]);
	}
	delete[] res;
}


TEST_F(ClassDeclaration, Size_isEmpty) {
	vector<int> a;
	for (int i = 0; i < 100; i++) {
		ASSERT_EQ(a.Real_size(), i);
		a.push_back(i);
	}
	ASSERT_EQ(a.Real_size(), 100);
	for (int i = 0; i < 100; i++) {
		ASSERT_EQ(a.Real_size(), 100 - i);
		ASSERT_EQ(0, a.isEmpty());
		a.pop_back();
	}
	ASSERT_EQ(a.Real_size(), 0);
	ASSERT_EQ(1, a.isEmpty());
}

TEST_F(ClassDeclaration, Resize) {
	vector<int> a;
	for (int i = 1; i < 100; i++) {
		a.resize(i);
		ASSERT_EQ(a.Real_size(), i);
	}
	a.resize(0);
	ASSERT_EQ(1, a.isEmpty());
	for (int i = 0; i < 100; i++) {
		a.push_back(i);
		ASSERT_EQ(a.Real_size(), i + 1);
	}
	a.resize(50);
	for (int i = 0; i < 50; i++) {
		ASSERT_EQ(i, a[i]);
	}
}


TEST_F(ClassDeclaration, Throw) {
	vector<int> a;
	ASSERT_THROW(a.pop_back(), logic_error);
	for (int i = 0; i < 10; i++) {
		a.push_back(i);
	}
	ASSERT_THROW(a[10], out_of_range);
	ASSERT_ANY_THROW(a[21], out_of_range);
}


TEST_F(ClassDeclaration, some) {
	vector<double> a;
	double* r = new double[5];
	for (int i = 0; i < 5; i++) {
		r[i] = rand() % 1000;
		a.push_back(r[i]);
	}
	for (int i = 0; i < 5; i++) {
		ASSERT_EQ(a[i], r[i]);
	}
	a[3] = 34;
	ASSERT_EQ(a[3], 34);
	ASSERT_THROW(a[5], out_of_range);
	for (int i = 0; i < 5; i++) {
		a.pop_back();
	}
	ASSERT_THROW(a.pop_back(), logic_error);
	for (int i = 0; i < 5; i++) {
		a.push_back(rand() % 1000);
	}
	a.resize(2);
	ASSERT_THROW(a[2], out_of_range);
	a.resize(0);
	ASSERT_THROW(a.pop_back(), logic_error);
	delete[] r;
}



TEST_F(ClassDeclaration, get_value_insert) {
	heap<int> a;
	int n = rand() % 100;
	for (int i = 0; i < n; ++i)
	{
		a.insert(rand() % 100);
	}
	int tc = rand() % 100;
	heap<int>::Pointer ch = a.insert(tc);
	int y = a.get_value(ch);
	ASSERT_EQ(y, tc);
}


TEST_F(ClassDeclaration, get_min) {
	heap<int> a;
	for (int i = 0; i < 100; i++) {
		a.insert(100 - i - 1);
	}
	ASSERT_EQ(0, a.get_min());
}


TEST_F(ClassDeclaration, extract_min) {
	heap<int> a;
	for (int i = 0; i < 100; i++) {
		a.insert(100 - i - 1);
	}
	for (int i = 0; i < 100; i++) {
		ASSERT_EQ(a.extract_min(), i);
	}
}

TEST_F(ClassDeclaration, extract_min_2) {
	heap<int> a;
	a.insert(-2); a.insert(-12); a.insert(18);
	a.insert(18); a.insert(8); a.insert(-2);
	ASSERT_EQ(a.extract_min(), -12);
	a.insert(-12); a.insert(-2); a.insert(-2);
	ASSERT_EQ(a.extract_min(), -12);
	ASSERT_EQ(a.extract_min(), -2);
	ASSERT_EQ(a.extract_min(), -2);
	ASSERT_EQ(a.extract_min(), -2);
	ASSERT_EQ(a.extract_min(), -2);
	ASSERT_EQ(a.extract_min(), 8);
	ASSERT_EQ(a.extract_min(), 18);
	ASSERT_EQ(a.extract_min(), 18);
    ASSERT_EQ(a.is_empty(), true);
}

TEST_F(ClassDeclaration, extract_min_3) {
	heap<int> a;
	a.insert(28); a.insert(18);
	ASSERT_EQ(a.extract_min(), 18);
	ASSERT_EQ(a.extract_min(), 28);
	ASSERT_THROW(a.extract_min(), logic_error);
	a.insert(-12); a.insert(8);
	ASSERT_EQ(a.extract_min(), -12);
	ASSERT_EQ(a.extract_min(), 8);
	a.insert(-12);
	ASSERT_EQ(a.extract_min(), -12);
	ASSERT_EQ(a.is_empty(), true);
}


TEST_F(ClassDeclaration, random1) {
	heap<int> a;
	a.insert(-2);
	a.insert(-12);
	a.insert(18);
	ASSERT_EQ(a.extract_min(), -12);
	ASSERT_EQ(a.extract_min(), -2);
	ASSERT_EQ(a.extract_min(), 18);
	ASSERT_THROW(a.extract_min(), logic_error);
	a.insert(8);
	a.insert(8);
	ASSERT_EQ(a.extract_min(), 8);
	a.insert(-2);
	a.insert(-12);
	a.insert(8);
	ASSERT_EQ(a.extract_min(), -12);
	ASSERT_EQ(a.extract_min(), -2);
	ASSERT_EQ(a.extract_min(), 8);
	ASSERT_EQ(a.extract_min(), 8);
	ASSERT_EQ(a.is_empty(), 1);
}


//tests for insert, extract_min, change, erase:



TEST_F(ClassDeclaration, random2) {
	heap<int> a;
	heap<int>::Pointer ch0 = a.insert(-2);
	heap<int>::Pointer ch1 = a.insert(-12);
	heap<int>::Pointer ch3 = a.insert(18);
	a.delet(ch0);
	ASSERT_EQ(a.extract_min(), -12);
	ASSERT_EQ(a.extract_min(), 18);
	ASSERT_THROW(a.extract_min(), logic_error);
	heap<int>:: Pointer ch5 = a.insert(8);
	heap<int>::Pointer ch4 = a.insert(8);
	a.insert(10);
	a.change(ch4, 2);
	ASSERT_EQ(a.extract_min(), 2);
	a.insert(-2);
	a.insert(-12);
	a.insert(8);
	a.delet(ch5);
	ASSERT_EQ(a.extract_min(), -12);
	ASSERT_EQ(a.extract_min(), -2);
	//ASSERT_EQ(a.extract_min(), 8);
	ASSERT_EQ(a.extract_min(), 8);
	////ASSERT_EQ(a.is_empty(), 1);
}


TEST_F(ClassDeclaration, constructor) {
	int* arr = new int[5]{ 2, 3, 5, 1, 4 };
	heap<int> heap(arr + 0, arr + 5);
	ASSERT_EQ(1, heap.get_min());
	ASSERT_EQ(1, heap.extract_min());
	ASSERT_EQ(0, heap.is_empty());
	ASSERT_EQ(2, heap.extract_min());
	heap.insert(-1);
	ASSERT_EQ(-1, heap.extract_min());
	ASSERT_EQ(3, heap.extract_min());
	ASSERT_EQ(4, heap.extract_min());
	ASSERT_EQ(5, heap.extract_min());
	delete[] arr;
}


TEST_F(ClassDeclaration, constructor_time) {
	int* arr = new int[100000];
	for (int i = 0; i < 100000; i++) {
		arr[i] = 100000 - 1 - i;
	}
	heap<int> heap(arr, arr + 100000);
	delete[] arr;
}



TEST_F(ClassDeclaration, constructor_time_withcheck) {
	int* arr = new int[100000];
	for (int i = 0; i < 100000; i++) {
		arr[i] = 100000 - 1 - i;
	}
	heap<int> heap(arr, arr + 100000);
	for (int i = 0; i < 100000; i++) {
		ASSERT_EQ(i, heap.extract_min());
	}
	delete[] arr;
}


TEST_F(ClassDeclaration, optimize)
{
	heap<int> a;
    a.optimize(16, 4);
	a.insert(3);
	a.insert(10);
	a.insert(-1);
	a.insert(4);
	a.insert(-3);
	ASSERT_EQ(-3, a.extract_min());
	ASSERT_EQ(-1, a.extract_min());
	a.insert(8);
	a.insert(2);
	a.insert(16);
	a.insert(1);
	a.insert(6);
	a.insert(9);
	a.insert(11);
	a.insert(9);
	a.insert(8);
	a.insert(3);
	ASSERT_EQ(1, a.extract_min());
	a.insert(5);
	a.insert(3);
	a.insert(23);
	ASSERT_EQ(2, a.extract_min());
}


TEST_F(ClassDeclaration, optimize_time) {
	heap<int> a;
	a.optimize(200000, 50);
	for (int i = 0; i < 50; i++) {
		for (int j = 1; j < 4000; j++) {
			a.insert(rand() % 10000);
		}
		a.extract_min();
	}
}



