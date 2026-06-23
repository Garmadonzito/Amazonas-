#pragma once
#include <vector>
#include <functional>

using namespace std;

// QUICK SORT generico (template)
// Ordena un vector de cualquier tipo T.
// Idea: se elige un "pivote", se mandan los menores a la izquierda y los
// mayores a la derecha (particion), y luego se ordena cada lado por separado
// de forma recursiva.

// Comparador por defecto: ordena de menor a mayor.
// Si quieres otro orden le pasas tu propia lambda como comparador.
template <class T>
void intercambiar(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

// Particion tipo Lomuto: usa el ultimo elemento como pivote.
// Devuelve la posicion final del pivote ya colocado en su lugar.
template <class T>
int particion(vector<T>& v, int inicio, int fin, function<bool(T, T)> esMenor) {
    T pivote = v[fin];
    int i = inicio - 1; // marca el limite de los "menores"

    for (int j = inicio; j < fin; j++) {
        // si v[j] va antes que el pivote, lo movemos a la zona de menores
        if (esMenor(v[j], pivote)) {
            i++;
            intercambiar(v[i], v[j]);
        }
    }
    // colocar el pivote justo despues de los menores
    intercambiar(v[i + 1], v[fin]);
    return i + 1;
}

// Quick sort recursivo (version interna con indices)
template <class T>
void quickSortRec(vector<T>& v, int inicio, int fin, function<bool(T, T)> esMenor) {
    if (inicio < fin) {
        int posPivote = particion(v, inicio, fin, esMenor);
        quickSortRec(v, inicio, posPivote - 1, esMenor); // lado izquierdo
        quickSortRec(v, posPivote + 1, fin, esMenor);    // lado derecho
    }
}

// Funcion publica que se llama desde afuera (ordena de menor a mayor).
template <class T>
void quickSort(vector<T>& v) {
    if (v.size() < 2) return; // 0 o 1 elemento ya esta ordenado
    quickSortRec<T>(v, 0, (int)v.size() - 1, [](T a, T b) { return a < b; });
}

// Version con comparador propio (lambda) por si quieres ordenar al reves
// o por algun campo especifico.
template <class T>
void quickSort(vector<T>& v, function<bool(T, T)> esMenor) {
    if (v.size() < 2) return;
    quickSortRec<T>(v, 0, (int)v.size() - 1, esMenor);
}
