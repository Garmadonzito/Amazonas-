#pragma once

#include <vector>
#include <functional>

using namespace std;


template <class T>
void mergeArreglo(vector<T>& arr, int left, int mid, int right, function<bool(T, T)> comp) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<T> L(n1);
    vector<T> R(n2);

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    
    while (i < n1 && j < n2) {
        if (comp(L[i], R[j])) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) { arr[k] = L[i]; i++; k++; }
    while (j < n2) { arr[k] = R[j]; j++; k++; }
}


template <class T>
void mergeSortRec(vector<T>& arr, int left, int right, function<bool(T, T)> comp) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;

    mergeSortRec(arr, left, mid, comp); 
    mergeSortRec(arr, mid + 1, right, comp); 

    mergeArreglo(arr, left, mid, right, comp); 
}


template <class T>
void mergeSort(vector<T>& arr, function<bool(T, T)> comp) {
    if (arr.empty()) return;
    mergeSortRec(arr, 0, (int)arr.size() - 1, comp);
}