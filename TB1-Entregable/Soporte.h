#pragma once
#include "ListaEnlazada.h"
#include "Usuario.h"
#include <string>
#include <iostream>

struct Ticket {
    int id;
    std::string dniCliente;
    std::string nombreCliente;
    std::string tipo;           // PROBLEMA, DEVOLUCION
    std::string asunto;
    std::string descripcion;
    std::string nombreProducto; // solo para DEVOLUCION
    float monto;                // solo para DEVOLUCION
    std::string estado;         // Abierto, En proceso, Resuelto

    Ticket() : id(0), dniCliente(""), nombreCliente(""), tipo("PROBLEMA"),
               asunto(""), descripcion(""), nombreProducto(""), monto(0), estado("Abierto") {}

    Ticket(int _id, std::string _dni, std::string _nombre, std::string _tipo,
           std::string _asunto, std::string _descripcion,
           std::string _producto = "", float _monto = 0)
        : id(_id), dniCliente(_dni), nombreCliente(_nombre), tipo(_tipo),
          asunto(_asunto), descripcion(_descripcion),
          nombreProducto(_producto), monto(_monto), estado("Abierto") {}
};

class GestorSoporte {
private:
    ListaEnlazada<Ticket>* listaTickets;
    int contadorId;

public:
    GestorSoporte() {
        listaTickets = new ListaEnlazada<Ticket>();
        contadorId = 1;
    }

    ~GestorSoporte() { delete listaTickets; }

    void abrirTicket(std::string dni, std::string nombre,
                     std::string asunto, std::string descripcion) {
        listaTickets->agregar(Ticket(contadorId++, dni, nombre,
                                    "PROBLEMA", asunto, descripcion));
    }

    void solicitarDevolucion(std::string dni, std::string nombre,
                             std::string producto, float monto) {
        std::string asunto = "Devolucion de: " + producto;
        std::string desc   = "Cliente solicita devolucion. Monto a reembolsar: S/. " +
                             std::to_string((int)monto);
        listaTickets->agregar(Ticket(contadorId++, dni, nombre,
                                    "DEVOLUCION", asunto, desc, producto, monto));
    }

    void avanzarEstado(int id) {
        Nodo<Ticket>* actual = listaTickets->getCabeza();
        while (actual != nullptr) {
            if (actual->dato.id == id) {
                if      (actual->dato.estado == "Abierto")    actual->dato.estado = "En proceso";
                else if (actual->dato.estado == "En proceso") actual->dato.estado = "Resuelto";
                return;
            }
            actual = actual->siguiente;
        }
    }

    // Vendedor: ve todos los tickets
    void listarTodos() {
        int fila = 10;
        imprimirEnPanel(fila++, "========================================================", 96);
        imprimirEnPanel(fila++, "               PANEL DE SOPORTE AL CLIENTE              ", 96);
        imprimirEnPanel(fila++, "========================================================", 96);
        fila++;
        if (listaTickets->getCabeza() == nullptr) {
            imprimirEnPanel(fila, "  No hay tickets de soporte.", 92);
            return;
        }
        Nodo<Ticket>* actual = listaTickets->getCabeza();
        while (actual != nullptr && fila < 36) {
            int color = (actual->dato.estado == "Abierto")    ? 91 :
                        (actual->dato.estado == "En proceso") ? 93 : 92;
            std::string item = "  [#" + std::to_string(actual->dato.id) + "] [" +
                actual->dato.tipo + "] " + actual->dato.nombreCliente +
                " | " + actual->dato.asunto + " [" + actual->dato.estado + "]";
            imprimirEnPanel(fila++, item, color);
            if (actual->dato.tipo == "DEVOLUCION") {
                imprimirEnPanel(fila++, "    >> Reembolso: S/. " +
                    std::to_string((int)actual->dato.monto) + " por " + actual->dato.nombreProducto, 93);
            }
            actual = actual->siguiente;
        }
    }

    // Cliente: ve solo sus tickets
    void listarMisTickets(std::string dni) {
        int fila = 10;
        imprimirEnPanel(fila++, "========================================================", 96);
        imprimirEnPanel(fila++, "                  MIS TICKETS DE SOPORTE                ", 96);
        imprimirEnPanel(fila++, "========================================================", 96);
        fila++;
        Nodo<Ticket>* actual = listaTickets->getCabeza();
        bool encontrado = false;
        while (actual != nullptr && fila < 36) {
            if (actual->dato.dniCliente == dni) {
                int color = (actual->dato.estado == "Abierto")    ? 91 :
                            (actual->dato.estado == "En proceso") ? 93 : 92;
                std::string item = "  [#" + std::to_string(actual->dato.id) + "] [" +
                    actual->dato.tipo + "] " + actual->dato.asunto +
                    " | Estado: " + actual->dato.estado;
                imprimirEnPanel(fila++, item, color);
                if (actual->dato.tipo == "DEVOLUCION")
                    imprimirEnPanel(fila++, "    >> Reembolso pendiente: S/. " +
                        std::to_string((int)actual->dato.monto), 93);
                encontrado = true;
            }
            actual = actual->siguiente;
        }
        if (!encontrado) imprimirEnPanel(fila, "  No tienes tickets abiertos.", 92);
    }

    int getCantidad() { return listaTickets->getCantidad(); }
};
