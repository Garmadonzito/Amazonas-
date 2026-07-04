#pragma once
#include "ListaEnlazada.h"
#include <string>
#include <vector>
#include <functional>


template <class T>
struct ElementoHash {
    std::string clave;
    T valor;

    ElementoHash() : clave(""), valor(T()) {}
    ElementoHash(std::string _clave, T _valor) : clave(_clave), valor(_valor) {}
};

template <class T>
class TablaHash {
private:
    int capacidad;
    
    ListaEnlazada<ElementoHash<T>>** buckets;

    
    int calcularHash(const std::string& clave) {
        unsigned long hashValue = 5381;
        for (char c : clave) {
            hashValue = ((hashValue << 5) + hashValue) ^ c;
        }
        return hashValue % capacidad;
    }

public:
    TablaHash(int _capacidad = 150) {
        this->capacidad = _capacidad;
        buckets = new ListaEnlazada<ElementoHash<T>>*[capacidad];
        for (int i = 0; i < capacidad; i++) {
            buckets[i] = new ListaEnlazada<ElementoHash<T>>();
        }
    }

    ~TablaHash() {
        for (int i = 0; i < capacidad; i++) {
            delete buckets[i];
        }
        delete[] buckets;
    }

    void insertar(std::string clave, T valor) {
        int indice = calcularHash(clave);

        bool actualizada = false;
        Nodo<ElementoHash<T>>* actual = buckets[indice]->getCabeza();
        while (actual != nullptr) {
            if (actual->dato.clave == clave) {
                actual->dato.valor = valor;
                actualizada = true;
                break;
            }
            actual = actual->siguiente;
        }

        if (!actualizada) {
            buckets[indice]->agregar(ElementoHash<T>(clave, valor));
        }
    }

    T* buscar(std::string clave) {
        int indice = calcularHash(clave);
        Nodo<ElementoHash<T>>* actual = buckets[indice]->getCabeza();

        while (actual != nullptr) {
            if (actual->dato.clave == clave) {
                return &(actual->dato.valor);
            }
            actual = actual->siguiente;
        }
        return nullptr;
    }

    
    void recorrerEstructura(std::function<void(std::string, T&)> funcionVisitar) {
        for (int i = 0; i < capacidad; i++) {
            Nodo<ElementoHash<T>>* actual = buckets[i]->getCabeza();
            while (actual != nullptr) {
                funcionVisitar(actual->dato.clave, actual->dato.valor);
                actual = actual->siguiente;
            }
        }
    }
};