#pragma once
#include <vector>
#include <functional>

using namespace std;


template <class T>
void intercambiar(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

template <class T>
int particion(vector<T>& v, int inicio, int fin, function<bool(T, T)> esMenor) {
    T pivote = v[fin];
    int i = inicio - 1;

    for (int j = inicio; j < fin; j++) {
        if (esMenor(v[j], pivote)) {
            i++;
            intercambiar(v[i], v[j]);
        }
    }
    intercambiar(v[i + 1], v[fin]);
    return i + 1;
}

template <class T>
void quickSortRec(vector<T>& v, int inicio, int fin, function<bool(T, T)> esMenor) {
    if (inicio < fin) {
        int posPivote = particion(v, inicio, fin, esMenor);
        quickSortRec(v, inicio, posPivote - 1, esMenor);
        quickSortRec(v, posPivote + 1, fin, esMenor);
    }
}

template <class T>
void quickSort(vector<T>& v) {
    if (v.size() < 2) return;
    quickSortRec<T>(v, 0, (int)v.size() - 1, [](T a, T b) { return a < b; });
}

template <class T>
void quickSort(vector<T>& v, function<bool(T, T)> esMenor) {
    if (v.size() < 2) return;
    quickSortRec<T>(v, 0, (int)v.size() - 1, esMenor);
}
