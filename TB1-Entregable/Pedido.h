#pragma once
#include "ListaEnlazada.h"
#include "Usuario.h"
#include <string>
#include <iostream>

struct Pedido {
    int id;
    std::string dniCliente;
    std::string nombreCliente;
    std::string nombreProducto;
    float total;
    std::string estado; // Pendiente, Enviado, Entregado

    Pedido() : id(0), dniCliente(""), nombreCliente(""), nombreProducto(""), total(0), estado("Pendiente") {}
    Pedido(int _id, std::string _dni, std::string _cliente, std::string _producto, float _total)
        : id(_id), dniCliente(_dni), nombreCliente(_cliente), nombreProducto(_producto),
          total(_total), estado("Pendiente") {}
};

class GestorPedidos {
private:
    ListaEnlazada<Pedido>* listaPedidos;
    int contadorId;

public:
    GestorPedidos() {
        listaPedidos = new ListaEnlazada<Pedido>();
        contadorId = 1;
    }

    ~GestorPedidos() { delete listaPedidos; }

    void registrarPedido(std::string dni, std::string cliente, std::string producto, float total) {
        listaPedidos->agregar(Pedido(contadorId++, dni, cliente, producto, total));
    }

    void avanzarEstado(int id) {
        Nodo<Pedido>* actual = listaPedidos->getCabeza();
        while (actual != nullptr) {
            if (actual->dato.id == id) {
                if (actual->dato.estado == "Pendiente")  actual->dato.estado = "Enviado";
                else if (actual->dato.estado == "Enviado") actual->dato.estado = "Entregado";
                return;
            }
            actual = actual->siguiente;
        }
    }

    void listarPedidos() {
        int fila = 10;
        imprimirEnPanel(fila++, "========================================================", 96);
        imprimirEnPanel(fila++, "                  GESTION DE PEDIDOS                    ", 96);
        imprimirEnPanel(fila++, "========================================================", 96);
        fila++;
        if (listaPedidos->getCabeza() == nullptr) {
            imprimirEnPanel(fila, "  No hay pedidos registrados aun.", 91);
            return;
        }
        Nodo<Pedido>* actual = listaPedidos->getCabeza();
        while (actual != nullptr && fila < 38) {
            int color = (actual->dato.estado == "Pendiente") ? 93 :
                        (actual->dato.estado == "Enviado")   ? 96 : 92;
            std::string item = "  [#" + std::to_string(actual->dato.id) + "] " +
                actual->dato.nombreCliente + " | " + actual->dato.nombreProducto +
                " | S/. " + std::to_string((int)actual->dato.total) +
                " | " + actual->dato.estado;
            imprimirEnPanel(fila++, item, color);
            actual = actual->siguiente;
        }
    }
};
