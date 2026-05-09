#pragma once
#include "ListaEnlazada.h"
#include "Usuario.h"
#include <string>
#include <iostream>

struct Ticket {
    int id;
    std::string dniCliente;
    std::string nombreCliente;
    int idPedido;
    std::string asunto;
    std::string descripcion;
    std::string estado; // Abierto, En proceso, Resuelto

    Ticket() : id(0), dniCliente(""), nombreCliente(""), idPedido(0),
               asunto(""), descripcion(""), estado("Abierto") {}

    Ticket(int _id, std::string _dni, std::string _nombre, int _idPedido,
           std::string _asunto, std::string _descripcion)
        : id(_id), dniCliente(_dni), nombreCliente(_nombre), idPedido(_idPedido),
          asunto(_asunto), descripcion(_descripcion), estado("Abierto") {}
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

    void abrirTicket(std::string dni, std::string nombre, int idPedido,
                     std::string asunto, std::string descripcion) {
        listaTickets->agregar(Ticket(contadorId++, dni, nombre, idPedido, asunto, descripcion));
    }

    void avanzarEstado(int id) {
        Nodo<Ticket>* actual = listaTickets->getCabeza();
        while (actual != nullptr) {
            if (actual->dato.id == id) {
                if (actual->dato.estado == "Abierto")     actual->dato.estado = "En proceso";
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
            imprimirEnPanel(fila, "  No hay tickets de soporte abiertos.", 92);
            return;
        }
        Nodo<Ticket>* actual = listaTickets->getCabeza();
        while (actual != nullptr && fila < 36) {
            int color = (actual->dato.estado == "Abierto")     ? 91 :
                        (actual->dato.estado == "En proceso")  ? 93 : 92;
            std::string item = "  [#" + std::to_string(actual->dato.id) + "] " +
                actual->dato.nombreCliente + " | Pedido #" +
                std::to_string(actual->dato.idPedido) + " | " +
                actual->dato.asunto + " [" + actual->dato.estado + "]";
            imprimirEnPanel(fila++, item, color);
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
                std::string item = "  [#" + std::to_string(actual->dato.id) + "] " +
                    actual->dato.asunto + " | Estado: " + actual->dato.estado;
                imprimirEnPanel(fila++, item, color);
                std::string detalle = "    >> " + actual->dato.descripcion;
                imprimirEnPanel(fila++, detalle);
                encontrado = true;
            }
            actual = actual->siguiente;
        }
        if (!encontrado) imprimirEnPanel(fila, "  No tienes tickets de soporte abiertos.", 92);
    }

    int getCantidad() { return listaTickets->getCantidad(); }
};
