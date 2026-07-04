#pragma once
#include <iostream>
#include <functional>

using namespace std;

template <class T>
class ArbolAVL {
private:
    struct NodoAVL {
        T dato;
        NodoAVL* izquierda;
        NodoAVL* derecha;
        int altura;

        NodoAVL(T valor) {
            dato = valor;
            izquierda = nullptr;
            derecha = nullptr;
            altura = 1;
        }
    };

    NodoAVL* raiz;
    int cantidad;

    int alturaDe(NodoAVL* nodo) {
        if (nodo == nullptr) return 0;
        return nodo->altura;
    }

    int maximo(int a, int b) {
        return (a > b) ? a : b;
    }

    int factorBalance(NodoAVL* nodo) {
        if (nodo == nullptr) return 0;
        return alturaDe(nodo->izquierda) - alturaDe(nodo->derecha);
    }

    NodoAVL* rotarDerecha(NodoAVL* y) {
        NodoAVL* x = y->izquierda;
        NodoAVL* aux = x->derecha;

        x->derecha = y;
        y->izquierda = aux;

        y->altura = maximo(alturaDe(y->izquierda), alturaDe(y->derecha)) + 1;
        x->altura = maximo(alturaDe(x->izquierda), alturaDe(x->derecha)) + 1;

        return x;
    }

    NodoAVL* rotarIzquierda(NodoAVL* x) {
        NodoAVL* y = x->derecha;
        NodoAVL* aux = y->izquierda;

        y->izquierda = x;
        x->derecha = aux;

        x->altura = maximo(alturaDe(x->izquierda), alturaDe(x->derecha)) + 1;
        y->altura = maximo(alturaDe(y->izquierda), alturaDe(y->derecha)) + 1;

        return y;
    }

    NodoAVL* insertarRec(NodoAVL* nodo, T valor) {
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
            return nodo;
        }

        nodo->altura = maximo(alturaDe(nodo->izquierda), alturaDe(nodo->derecha)) + 1;

        int balance = factorBalance(nodo);

        if (balance > 1 && valor < nodo->izquierda->dato) {
            return rotarDerecha(nodo);
        }
        if (balance < -1 && nodo->derecha->dato < valor) {
            return rotarIzquierda(nodo);
        }
        if (balance > 1 && nodo->izquierda->dato < valor) {
            nodo->izquierda = rotarIzquierda(nodo->izquierda);
            return rotarDerecha(nodo);
        }
        if (balance < -1 && valor < nodo->derecha->dato) {
            nodo->derecha = rotarDerecha(nodo->derecha);
            return rotarIzquierda(nodo);
        }

        return nodo;
    }

    bool buscarRec(NodoAVL* nodo, T valor) {
        if (nodo == nullptr) return false;
        if (valor < nodo->dato) return buscarRec(nodo->izquierda, valor);
        if (nodo->dato < valor) return buscarRec(nodo->derecha, valor);
        return true;
    }

    void inordenRec(NodoAVL* nodo, function<void(T)> visitar) {
        if (nodo == nullptr) return;
        inordenRec(nodo->izquierda, visitar);
        visitar(nodo->dato);
        inordenRec(nodo->derecha, visitar);
    }

    //Recorrido indorden Inverso: Derecha, Raiz, Izquierda(Sale ordenado de mayo a menor)
    void inordenInversoRec(NodoAVL* nodo, function<void(T)> visitar) {
        if (nodo == nullptr) return;
        inordenInversoRec(nodo->derecha, visitar); // Primero visitamos los mayores
        visitar(nodo->dato);                       // Procesamos el nodo actual
        inordenInversoRec(nodo->izquierda, visitar); // Al final los menores
    }

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

    bool buscarPiso(T limite, T& resultado) {
        NodoAVL* actual = raiz;
        bool encontrado = false;
        while (actual != nullptr) {
            if (limite < actual->dato) {
                actual = actual->izquierda;
            }
            else {
                resultado = actual->dato;
                encontrado = true;
                actual = actual->derecha;
            }
        }
        return encontrado;
    }

    bool getMinimo(T& resultado) {
        if (raiz == nullptr) return false;
        NodoAVL* actual = raiz;
        while (actual->izquierda != nullptr) actual = actual->izquierda;
        resultado = actual->dato;
        return true;
    }

    bool getMaximo(T& resultado) {
        if (raiz == nullptr) return false;
        NodoAVL* actual = raiz;
        while (actual->derecha != nullptr) actual = actual->derecha;
        resultado = actual->dato;
        return true;
    }

    void recorrerInorden(function<void(T)> visitar) {
        inordenRec(raiz, visitar);
    }

    void recorrerInordenInverso(function<void(T)> visitar) {
        inordenInversoRec(raiz, visitar);
    }

    int getCantidad() {
        return cantidad;
    }

    int getAltura() {
        return alturaDe(raiz);
    }

    bool estaVacio() {
        return raiz == nullptr;
    }
};
