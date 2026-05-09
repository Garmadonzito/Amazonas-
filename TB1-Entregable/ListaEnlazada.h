#pragma once
#include <iostream>
#include <functional> // Necesario para la lambda

//Chicos desde ahora he puesto comentarios en secciones del codigo para describir los cambios nuevos que he echo
//cambie el codigo ahora por lis simple enlazada para que todo funcione bien y le he puesto una validacion al DNI para 
//Que solo si tiene 8 digitos entre el DNI 

template <class T>
class Nodo {
public:
    T dato;
    Nodo<T>* siguiente;

    Nodo(T valor) {
        dato = valor;
        siguiente = nullptr;
    }
};


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

  
    ~ListaEnlazada() {
        Nodo<T>* actual = cabeza;
        while (actual != nullptr) {
            Nodo<T>* aBorrar = actual;
            actual = actual->siguiente;
            delete aBorrar;
        }
    }

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

    // Ordenamiento burbuja con comparador lambda (O(n^2))
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