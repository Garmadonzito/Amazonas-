#pragma once
#include "Nodo.h"
#include <iostream>

template <class T>
class Cola {
private:
    Nodo<T>* frente;
    Nodo<T>* fin;
    int cantidad;

public:
    Cola() {
        frente = nullptr;
        fin = nullptr;
        cantidad = 0;
    }

    ~Cola() {
        while (!estaVacia()) {
            desencolar();
        }
    }

    void encolar(T valor) {
        Nodo<T>* nuevoNodo = new Nodo<T>(valor);
        if (frente == nullptr) {
            frente = nuevoNodo;
            fin = nuevoNodo;
        }
        else {
            fin->siguiente = nuevoNodo;
            fin = nuevoNodo;
        }
        cantidad++;
    }

    bool desencolar() {
        if (frente == nullptr) return false;

        Nodo<T>* aBorrar = frente;
        frente = frente->siguiente;
        if (frente == nullptr) fin = nullptr;

        delete aBorrar;
        cantidad--;
        return true;
    }

    T* verFrente() {
        if (frente == nullptr) return nullptr;
        return &(frente->dato);
    }

    bool estaVacia() {
        return frente == nullptr;
    }

    int getCantidad() {
        return cantidad;
    }

    Nodo<T>* getFrente() {
        return frente;
    }
};
