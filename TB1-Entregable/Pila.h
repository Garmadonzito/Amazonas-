#pragma once
#include "Nodo.h"

template <class T>
class Pila {
private:
    Nodo<T>* tope;
    int cantidad;

public:
    Pila() : tope(nullptr), cantidad(0) {}
    ~Pila() { while (!estaVacia()) desapilar(); }

    void apilar(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor, tope);
        tope = nuevo;
        cantidad++;
    }

    bool desapilar() {
        if (estaVacia()) return false;
        Nodo<T>* aux = tope;
        tope = tope->siguiente;
        delete aux;
        cantidad--;
        return true;
    }

    T obtenerTope() {
        return (tope != nullptr) ? tope->dato : T();
    }

    bool estaVacia() { return tope == nullptr; }
};
