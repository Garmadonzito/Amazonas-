#pragma once

#include "Nodo.h"
#include <iostream>
#include <functional> // Necesario para lambdas

template <class T>
class ListaEnlazada {
private:
    Nodo<T>* cabeza;
    Nodo<T>* cola;
    int cantidad;

public:
    ListaEnlazada() {
        cabeza = nullptr;
        cola = nullptr;
        cantidad = 0;
    }

    // Nuevo método O(n) para limpiar memoria sin destruir el objeto principal
    void vaciar() {
        Nodo<T>* actual = cabeza;
        while (actual != nullptr) {
            Nodo<T>* aBorrar = actual;
            actual = actual->siguiente;
            delete aBorrar;
        }
        cabeza = nullptr;
        cola = nullptr;
        cantidad = 0;
    }

    ~ListaEnlazada() {
        vaciar();
    }

    // Complejidad O(1) gracias al puntero "cola"
    void agregar(T valor) {
        Nodo<T>* nuevoNodo = new Nodo<T>(valor);
        if (cabeza == nullptr) {
            cabeza = nuevoNodo;
            cola = nuevoNodo;
        }
        else {
            cola->siguiente = nuevoNodo;
            cola = nuevoNodo;
        }
        cantidad++;
    }

    // Complejidad O(n)
    bool eliminarSi(std::function<bool(T)> condicion) {
        if (cabeza == nullptr) return false;

        if (condicion(cabeza->dato)) {
            Nodo<T>* aBorrar = cabeza;
            cabeza = cabeza->siguiente;
            if (cabeza == nullptr) cola = nullptr;
            delete aBorrar;
            cantidad--;
            return true;
        }

        Nodo<T>* actual = cabeza;
        while (actual->siguiente != nullptr && !condicion(actual->siguiente->dato)) {
            actual = actual->siguiente;
        }

        if (actual->siguiente != nullptr) {
            Nodo<T>* aBorrar = actual->siguiente;
            actual->siguiente = aBorrar->siguiente;
            if (aBorrar == cola) cola = actual;
            delete aBorrar;
            cantidad--;
            return true;
        }
        return false;
    }

    Nodo<T>* getCabeza() { return cabeza; }
    int getCantidad() { return cantidad; }
};
