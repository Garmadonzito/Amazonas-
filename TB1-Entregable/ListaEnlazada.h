#pragma once

#include "Nodo.h"
#include <iostream>
#include <functional>

template <class T>
class ListaEnlazada {
private:
    Nodo<T>* cabeza;
    Nodo<T>* cola;
    int cantidad;

public:
    // Clase Iterador para cumplir con la rúbrica del TB1
    class Iterator {
    private:
        Nodo<T>* actual;
    public:
        Iterator(Nodo<T>* nodo) : actual(nodo) {}
        Iterator& operator++() { if (actual) actual = actual->siguiente; return *this; }
        T& operator*() { return actual->dato; }
        bool operator!=(const Iterator& otro) const { return actual != otro.actual; }
    };

    ListaEnlazada() : cabeza(nullptr), cola(nullptr), cantidad(0) {}

    void vaciar() {
        Nodo<T>* actual = cabeza;
        while (actual != nullptr) {
            Nodo<T>* aBorrar = actual;
            actual = actual->siguiente;
            delete aBorrar;
        }
        cabeza = nullptr; cola = nullptr; cantidad = 0;
    }

    ~ListaEnlazada() { vaciar(); }

    void agregar(T valor) {
        Nodo<T>* nuevoNodo = new Nodo<T>(valor);
        if (cabeza == nullptr) { cabeza = cola = nuevoNodo; }
        else { cola->siguiente = nuevoNodo; cola = nuevoNodo; }
        cantidad++;
    }

    bool eliminarSi(std::function<bool(T)> condicion) {
        if (cabeza == nullptr) return false;
        if (condicion(cabeza->dato)) {
            Nodo<T>* aBorrar = cabeza;
            cabeza = cabeza->siguiente;
            if (cabeza == nullptr) cola = nullptr;
            delete aBorrar; cantidad--; return true;
        }
        Nodo<T>* actual = cabeza;
        while (actual->siguiente != nullptr && !condicion(actual->siguiente->dato)) {
            actual = actual->siguiente;
        }
        if (actual->siguiente != nullptr) {
            Nodo<T>* aBorrar = actual->siguiente;
            actual->siguiente = aBorrar->siguiente;
            if (aBorrar == cola) cola = actual;
            delete aBorrar; cantidad--; return true;
        }
        return false;
    }

    Nodo<T>* getCabeza() { return cabeza; }
    int getCantidad() { return cantidad; }

    Iterator begin() { return Iterator(cabeza); }
    Iterator end() { return Iterator(nullptr); }

    // Ordenamiento burbuja con comparador lambda 
    void ordenar(std::function<bool(T, T)> comparador) {
        if (cabeza == nullptr || cabeza->siguiente == nullptr) return;
        bool huboIntercambio;
        do {
            huboIntercambio = false;
            Nodo<T>* actual = cabeza;
            while (actual->siguiente != nullptr) {
                if (comparador(actual->dato, actual->siguiente->dato)) {
                    T temp = actual->dato;
                    actual->dato = actual->siguiente->dato;
                    actual->siguiente->dato = temp;
                    huboIntercambio = true;
                }
                actual = actual->siguiente;
            }
        } while (huboIntercambio);
    }
};
