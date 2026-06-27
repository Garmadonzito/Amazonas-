#pragma once

#include <iostream>
using namespace std;

int parent(int i) {
	return (i - 1) / 2;
}

int left(int i) {
	return 2 * i + 1;
}

int rigth(int i) {
	return 2 * i + 2;
}

template <class T>
void maxHeapify(T A[], int n, int i) {
	int l = left(i);
	int r = rigth(i);
	int largest(0);

	if (l <= (n - 1) && A[l] > A[i]) {
		largest = l;
	}
	else
		largest = i;

	if (r <= (n - 1) && A[r] > A[largest]) {
		largest = r;
	}

	if (largest != i) {
		swap(A[i], A[largest]);
		maxHeapify(A, n, largest);
	}
}

template <class T>
void buildMaxHeap(T A[], int n) {
	for (int i = n / 2 - 1; i >= 0; --i) {
		maxHeapify(A, n, i);
	}
}

template <class T>
void print(T A[], int n) {
	for (int i = 0; i < n; ++i) {
		cout << A[i] << " ";
	}
}

//aplicación de ordenamiento
template <class T>
void heapsort(T A[], int n) {
	buildMaxHeap(A, n);
	for (int i = n - 1; i > 0; --i) {
		swap(A[0], A[i]);
		maxHeapify(A, --n, 0);
	}
}
