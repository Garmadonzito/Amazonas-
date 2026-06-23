#pragma once
#include <iostream>
#include <functional>

using namespace std;

// Arbol Binario de Busqueda AUTO-BALANCEADO (AVL)
// Se balancea solo con rotaciones para que la altura no se descontrole
// y las busquedas sigan siendo rapidas. Hecho con template para que sirva
// con cualquier tipo (int, string, etc).
template <class T>
class ArbolAVL {
private:
    // Nodo propio del arbol (guarda su altura para calcular el balance)
    struct NodoAVL {
        T dato;
        NodoAVL* izquierda;
        NodoAVL* derecha;
        int altura;

        NodoAVL(T valor) {
            dato = valor;
            izquierda = nullptr;
            derecha = nullptr;
            altura = 1; // un nodo nuevo (hoja) tiene altura 1
        }
    };

    NodoAVL* raiz;
    int cantidad;

    // Altura de un nodo (si es nullptr cuenta como 0)
    int alturaDe(NodoAVL* nodo) {
        if (nodo == nullptr) return 0;
        return nodo->altura;
    }

    int maximo(int a, int b) {
        return (a > b) ? a : b;
    }

    // Factor de balance = altura izquierda - altura derecha
    // Si esta entre -1 y 1 esta balanceado, sino hay que rotar
    int factorBalance(NodoAVL* nodo) {
        if (nodo == nullptr) return 0;
        return alturaDe(nodo->izquierda) - alturaDe(nodo->derecha);
    }

    // Rotacion simple a la derecha
    NodoAVL* rotarDerecha(NodoAVL* y) {
        NodoAVL* x = y->izquierda;
        NodoAVL* aux = x->derecha;

        x->derecha = y;
        y->izquierda = aux;

        // recalcular alturas (primero el de abajo)
        y->altura = maximo(alturaDe(y->izquierda), alturaDe(y->derecha)) + 1;
        x->altura = maximo(alturaDe(x->izquierda), alturaDe(x->derecha)) + 1;

        return x; // x queda como nueva raiz de este sub-arbol
    }

    // Rotacion simple a la izquierda
    NodoAVL* rotarIzquierda(NodoAVL* x) {
        NodoAVL* y = x->derecha;
        NodoAVL* aux = y->izquierda;

        y->izquierda = x;
        x->derecha = aux;

        x->altura = maximo(alturaDe(x->izquierda), alturaDe(x->derecha)) + 1;
        y->altura = maximo(alturaDe(y->izquierda), alturaDe(y->derecha)) + 1;

        return y;
    }

    // Insertar recursivo: baja, inserta y al regresar balancea
    NodoAVL* insertarRec(NodoAVL* nodo, T valor) {
        // 1. Insercion normal de un ABB
        if (nodo == nullptr) {
            cantidad++;
            return new NodoAVL(valor);
        }
        if (valor < nodo->dato) {
            nodo->izquierda = insertarRec(nodo->izquierda, valor);
        }
        else if (nodo->dato < valor) {
            nodo->derecha = insertarRec(nodo->derecha, valor);
        }
        else {
            return nodo; // dato repetido, no se inserta
        }

        // 2. Actualizar altura de este nodo
        nodo->altura = maximo(alturaDe(nodo->izquierda), alturaDe(nodo->derecha)) + 1;

        // 3. Revisar el balance y rotar si hace falta (los 4 casos)
        int balance = factorBalance(nodo);

        // Caso Izquierda-Izquierda
        if (balance > 1 && valor < nodo->izquierda->dato) {
            return rotarDerecha(nodo);
        }
        // Caso Derecha-Derecha
        if (balance < -1 && nodo->derecha->dato < valor) {
            return rotarIzquierda(nodo);
        }
        // Caso Izquierda-Derecha
        if (balance > 1 && nodo->izquierda->dato < valor) {
            nodo->izquierda = rotarIzquierda(nodo->izquierda);
            return rotarDerecha(nodo);
        }
        // Caso Derecha-Izquierda
        if (balance < -1 && valor < nodo->derecha->dato) {
            nodo->derecha = rotarDerecha(nodo->derecha);
            return rotarIzquierda(nodo);
        }

        return nodo; // ya estaba balanceado
    }

    // Buscar recursivo
    bool buscarRec(NodoAVL* nodo, T valor) {
        if (nodo == nullptr) return false;
        if (valor < nodo->dato) return buscarRec(nodo->izquierda, valor);
        if (nodo->dato < valor) return buscarRec(nodo->derecha, valor);
        return true; // son iguales
    }

    // Recorrido inorden: izquierda - raiz - derecha (sale ORDENADO)
    void inordenRec(NodoAVL* nodo, function<void(T)> visitar) {
        if (nodo == nullptr) return;
        inordenRec(nodo->izquierda, visitar);
        visitar(nodo->dato);
        inordenRec(nodo->derecha, visitar);
    }

    // Liberar memoria recursivo (post-orden para borrar hijos primero)
    void liberarRec(NodoAVL* nodo) {
        if (nodo == nullptr) return;
        liberarRec(nodo->izquierda);
        liberarRec(nodo->derecha);
        delete nodo;
    }

public:
    ArbolAVL() {
        raiz = nullptr;
        cantidad = 0;
    }

    ~ArbolAVL() {
        liberarRec(raiz);
    }

    void insertar(T valor) {
        raiz = insertarRec(raiz, valor);
    }

    bool buscar(T valor) {
        return buscarRec(raiz, valor);
    }

    // Recorre en orden y por cada dato llama a la funcion/lambda que le pases
    void recorrerInorden(function<void(T)> visitar) {
        inordenRec(raiz, visitar);
    }

    int getCantidad() {
        return cantidad;
    }

    // Altura total del arbol (sirve para comprobar que esta balanceado)
    int getAltura() {
        return alturaDe(raiz);
    }

    bool estaVacio() {
        return raiz == nullptr;
    }
};
