#pragma once
#include <iostream>

template <class T>
class NodoCola {
public:
    T dato;
    NodoCola<T>* siguiente;

    NodoCola(T valor) {
        dato = valor;
        siguiente = nullptr;
    }
};

template <class T>
class Cola {
private:
    NodoCola<T>* frente;
    NodoCola<T>* fin;
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
        NodoCola<T>* nuevoNodo = new NodoCola<T>(valor);
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

        NodoCola<T>* aBorrar = frente;
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

    NodoCola<T>* getFrente() {
        return frente;
    }
};
