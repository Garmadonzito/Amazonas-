#pragma once
#include <iostream>

//Chicos inclui esta nueva clase para que el almacenamiento en inventario sea dinamico y se pueda agregar y eliminar y cumplir con los requisitos
//del entregable 

//Tambien he uesto comentarios en los cambios que he echo para que cuando lean el codigo sepan que estoy haciendo aqui y en Vendedor.h
// - Rafael


// Nodo
template <class T>
class Nodo {
public:
    T dato;
    Nodo<T>* siguiente;
    Nodo<T>* anterior;

    Nodo(T valor) {
        dato = valor;
        siguiente = nullptr;
        anterior = nullptr;
    }
};

//Lista Doblemente Enlazada
template <class T>
class ListaDoble {
private:
    Nodo<T>* cabeza;
    Nodo<T>* cola;
    int cantidad;

public:
    ListaDoble() {
        cabeza = nullptr;
        cola = nullptr;
        cantidad = 0;
    }

    // Destructor 
    ~ListaDoble() {
        Nodo<T>* actual = cabeza;
        while (actual != nullptr) {
            Nodo<T>* aBorrar = actual;
            actual = actual->siguiente;
            delete aBorrar;
        }
    }

    // Agregar elemento al final de la lista
    void agregar(T valor) {
        Nodo<T>* nuevoNodo = new Nodo<T>(valor);
        if (cabeza == nullptr) {
            cabeza = nuevoNodo;
            cola = nuevoNodo;
        }
        else {
            cola->siguiente = nuevoNodo;
            nuevoNodo->anterior = cola;
            cola = nuevoNodo;
        }
        cantidad++;
    }

    int getCantidad() {
        return cantidad;
    }

    // Método necesario para que funcione la recursividad en el Inventario
    Nodo<T>* getCabeza() {
        return cabeza;
    }
};