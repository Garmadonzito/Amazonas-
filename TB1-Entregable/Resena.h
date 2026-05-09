#pragma once
#include "ListaEnlazada.h"
#include "Usuario.h"
#include <string>
#include <iostream>

struct Resena {
    int idProducto;
    std::string nombreProducto;
    std::string dniCliente;
    std::string comentario;
    int puntuacion; // 1 a 5

    Resena() : idProducto(0), nombreProducto(""), dniCliente(""), comentario(""), puntuacion(0) {}
    Resena(int _id, std::string _prod, std::string _dni, std::string _comentario, int _punt)
        : idProducto(_id), nombreProducto(_prod), dniCliente(_dni),
          comentario(_comentario), puntuacion(_punt) {}
};

class GestorResenas {
private:
    ListaEnlazada<Resena>* listaResenas;

    std::string estrellas(int n) {
        std::string s = "";
        for (int i = 0; i < n; i++) s += "*";
        for (int i = n; i < 5; i++) s += "-";
        return "[" + s + "]";
    }

public:
    GestorResenas() { listaResenas = new ListaEnlazada<Resena>(); }
    ~GestorResenas() { delete listaResenas; }

    void agregarResena(int idProducto, std::string producto, std::string dni,
                       std::string comentario, int puntuacion) {
        if (puntuacion < 1) puntuacion = 1;
        if (puntuacion > 5) puntuacion = 5;
        listaResenas->agregar(Resena(idProducto, producto, dni, comentario, puntuacion));
    }

    void listarResenas() {
        int fila = 10;
        imprimirEnPanel(fila++, "========================================================", 96);
        imprimirEnPanel(fila++, "                 RESENAS DE CLIENTES                    ", 96);
        imprimirEnPanel(fila++, "========================================================", 96);
        fila++;
        if (listaResenas->getCabeza() == nullptr) {
            imprimirEnPanel(fila, "  No hay resenas registradas aun.", 91);
            return;
        }
        Nodo<Resena>* actual = listaResenas->getCabeza();
        while (actual != nullptr && fila < 38) {
            std::string item = "  " + estrellas(actual->dato.puntuacion) +
                " " + actual->dato.nombreProducto + " - \"" + actual->dato.comentario + "\"";
            imprimirEnPanel(fila++, item, 93);
            actual = actual->siguiente;
        }
    }
};
