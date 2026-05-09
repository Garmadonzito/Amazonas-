#pragma once
#include "ListaEnlazada.h"
#include "Usuario.h"
#include <string>
#include <iostream>
#include <ctime>

struct Notificacion {
    std::string mensaje;
    std::string tipo; // INFO, ALERTA, VENTA
    std::string fecha;

    Notificacion() : mensaje(""), tipo("INFO"), fecha("") {}
    Notificacion(std::string _mensaje, std::string _tipo, std::string _fecha)
        : mensaje(_mensaje), tipo(_tipo), fecha(_fecha) {}
};

class GestorNotificaciones {
private:
    ListaEnlazada<Notificacion>* listaNotificaciones;

    std::string obtenerFecha() {
        time_t tiempo = time(0);
        tm fechaLocal;
        localtime_s(&fechaLocal, &tiempo);
        char texto[20];
        strftime(texto, sizeof(texto), "%d/%m %H:%M", &fechaLocal);
        return texto;
    }

public:
    GestorNotificaciones() { listaNotificaciones = new ListaEnlazada<Notificacion>(); }
    ~GestorNotificaciones() { delete listaNotificaciones; }

    void agregar(std::string mensaje, std::string tipo = "INFO") {
        listaNotificaciones->agregar(Notificacion(mensaje, tipo, obtenerFecha()));
    }

    void mostrarTodas() {
        int fila = 10;
        imprimirEnPanel(fila++, "========================================================", 96);
        imprimirEnPanel(fila++, "                   NOTIFICACIONES                       ", 96);
        imprimirEnPanel(fila++, "========================================================", 96);
        fila++;
        if (listaNotificaciones->getCabeza() == nullptr) {
            imprimirEnPanel(fila, "  No hay notificaciones.", 91);
            return;
        }
        Nodo<Notificacion>* actual = listaNotificaciones->getCabeza();
        while (actual != nullptr && fila < 38) {
            int color = (actual->dato.tipo == "ALERTA") ? 91 :
                        (actual->dato.tipo == "VENTA")  ? 92 : 96;
            std::string item = "  [" + actual->dato.tipo + "] " +
                actual->dato.fecha + " - " + actual->dato.mensaje;
            imprimirEnPanel(fila++, item, color);
            actual = actual->siguiente;
        }
    }

    void limpiar() {
        delete listaNotificaciones;
        listaNotificaciones = new ListaEnlazada<Notificacion>();
    }

    int getCantidad() { return listaNotificaciones->getCantidad(); }
};
