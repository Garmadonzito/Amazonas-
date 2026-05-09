#pragma once
#include "ListaEnlazada.h"
#include "Usuario.h"
#include <string>
#include <iostream>

struct Cupon {
    std::string codigo;
    float descuento;
    bool activo;

    Cupon() : codigo(""), descuento(0), activo(false) {}
    Cupon(std::string _codigo, float _descuento)
        : codigo(_codigo), descuento(_descuento), activo(true) {}
};

class GestorCupones {
private:
    ListaEnlazada<Cupon>* listaCupones;

public:
    GestorCupones() {
        listaCupones = new ListaEnlazada<Cupon>();
        listaCupones->agregar(Cupon("AMAZON10", 10.0f));
        listaCupones->agregar(Cupon("OFERTA20", 20.0f));
        listaCupones->agregar(Cupon("PROMO15", 15.0f));
    }

    ~GestorCupones() { delete listaCupones; }

    void agregarCupon(std::string codigo, float descuento) {
        listaCupones->agregar(Cupon(codigo, descuento));
    }

    // Retorna el porcentaje de descuento, 0 si no es valido
    float aplicarCupon(std::string codigo) {
        Nodo<Cupon>* actual = listaCupones->getCabeza();
        while (actual != nullptr) {
            if (actual->dato.codigo == codigo && actual->dato.activo)
                return actual->dato.descuento;
            actual = actual->siguiente;
        }
        return 0.0f;
    }

    void desactivarCupon(std::string codigo) {
        Nodo<Cupon>* actual = listaCupones->getCabeza();
        while (actual != nullptr) {
            if (actual->dato.codigo == codigo) {
                actual->dato.activo = false;
                return;
            }
            actual = actual->siguiente;
        }
    }

    void listarCupones() {
        int fila = 10;
        imprimirEnPanel(fila++, "========================================================", 96);
        imprimirEnPanel(fila++, "                 CUPONES DE DESCUENTO                   ", 96);
        imprimirEnPanel(fila++, "========================================================", 96);
        fila++;
        Nodo<Cupon>* actual = listaCupones->getCabeza();
        while (actual != nullptr) {
            std::string estado = actual->dato.activo ? "[ACTIVO]" : "[INACTIVO]";
            int color = actual->dato.activo ? 92 : 91;
            std::string item = "  " + estado + " " + actual->dato.codigo +
                " - " + std::to_string((int)actual->dato.descuento) + "% de descuento";
            imprimirEnPanel(fila++, item, color);
            actual = actual->siguiente;
        }
    }
};
