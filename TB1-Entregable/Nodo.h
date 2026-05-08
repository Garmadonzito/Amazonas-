#pragma once

template <class T>
struct Nodo {
    T dato;
    Nodo<T>* siguiente;

    Nodo(T valor, Nodo<T>* sig = nullptr) {
        dato = valor;
        siguiente = sig;
    }
};
