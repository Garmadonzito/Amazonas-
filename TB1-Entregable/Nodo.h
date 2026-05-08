#pragma once

template <class T>
class Nodo {
public:
    T dato;
    Nodo<T>* siguiente;

    Nodo(T valor, Nodo<T>* sig = nullptr) {
        dato = valor;
        siguiente = sig;
    }
};
