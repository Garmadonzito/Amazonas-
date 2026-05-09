#pragma once
#include "Usuario.h"
#include "Inventario.h"
#include "MetodoPago.h"
#include <conio.h>

extern void mostrarFondo2();

#define IZQ 12  // columna izquierda de la zona negra

class Cliente : public Usuario {
private:
    ListaEnlazada<int>* carrito;

    void linea(int fila, const string& texto) {
        irA(fila, IZQ); cout << "\033[0m" << texto;
    }

public:
    Cliente() {
        carrito = new ListaEnlazada<int>();
    }

    ~Cliente() {
        delete carrito;
    }

    void login() {
        cout << "\n--- LOGIN CLIENTE ---\n";
        cout << "Nombre: "; cin.ignore(); getline(cin, nombre);
        cout << "Correo: "; getline(cin, correo);

        auto validarDNI = [](string d) -> bool {
            if (d.length() != 8) return false;
            for (char c : d) {
                if (c < '0' || c > '9') return false;
            }
            return true;
        };

        bool dniValido = false;
        do {
            cout << "DNI: ";
            getline(cin, dni);
            if (validarDNI(dni)) {
                dniValido = true;
            } else {
                cout << ">> Error: El DNI debe tener exactamente 8 numeros.\n";
            }
        } while (!dniValido);

        cout << "\n>> Bienvenido " << nombre << "!\n";
        pausa();
    }

    void agregarAlCarrito(Inventario& inv) {
        limpiarZonaVerde();
        irA(10, IZQ);
        inv.listarTodo();

        irA(32, IZQ); cout << "\033[0m>> Ingrese ID y ENTER para agregar. ESC para terminar.";

        int filaMsg = 34;
        while (true) {
            irA(filaMsg, IZQ);
            cout << "\033[0mID: ";
            string buf;
            bool salir = false;

            while (true) {
                int c = _getch();
                if (c == 27) { salir = true; cout << "\n"; break; }
                if (c == '\r') { cout << "\n"; break; }
                if (c == 8 && !buf.empty()) { buf.pop_back(); cout << "\b \b"; }
                else if (c >= '0' && c <= '9') { buf += (char)c; cout << (char)c; }
            }

            if (salir) break;
            if (buf.empty()) continue;

            int id = stoi(buf);
            Producto* p = inv.obtenerProducto(id);
            irA(filaMsg + 1, IZQ);
            cout << "\033[0m";
            if (p == nullptr) {
                cout << ">> ID no encontrado.                          ";
            } else if (p->stock <= 0) {
                cout << ">> Sin stock disponible.                      ";
            } else {
                carrito->agregar(id);
                p->stock--;
                cout << ">> '" << p->nombre << "' agregado al carrito! ";
            }
        }
    }

    void verCarrito(Inventario& inv) {
        int fila = 10;
        linea(fila++, "--- TU CARRITO ---");
        fila++;

        if (carrito->getCabeza() == nullptr) {
            linea(fila, "Carrito vacio.");
            return;
        }

        double total = 0;
        int i = 1;
        Nodo<int>* actual = carrito->getCabeza();
        while (actual != nullptr) {
            Producto* p = inv.obtenerProducto(actual->dato);
            if (p != nullptr) {
                string item = to_string(i) + ". " + p->nombre +
                              "  -  S/. " + to_string((int)p->precio);
                linea(fila++, item);
                total += p->precio;
            }
            actual = actual->siguiente;
            i++;
        }
        fila++;
        linea(fila, "TOTAL: S/. " + to_string((int)total));
    }

    void seleccionarMetodoPago(double total) {
        limpiarZonaVerde();
        linea(10, "==============================");
        linea(11, "     METODO DE PAGO           ");
        linea(12, "==============================");
        linea(14, "1. Tarjeta");
        linea(15, "2. Yape / Plin");
        linea(17, "Opcion: ");
        int op; cin >> op;

        if (op == 1) {
            limpiarZonaVerde();
            irA(13, IZQ);
            Tarjeta t;
            t.pagar(total);
        } else if (op == 2) {
            limpiarPantalla();
            YapePlin yp;
            yp.pagar(total);
            mostrarFondo2();
        } else {
            linea(19, "Opcion invalida. Pago cancelado.");
        }
    }

    void comprarCarrito(Inventario& inv) {
        limpiarZonaVerde();
        if (carrito->getCabeza() == nullptr) {
            linea(16, "El carrito esta vacio.");
            irA(19, IZQ); pausa();
            return;
        }

        double total = 0;
        Nodo<int>* actual = carrito->getCabeza();
        while (actual != nullptr) {
            Producto* p = inv.obtenerProducto(actual->dato);
            if (p != nullptr) total += p->precio;
            actual = actual->siguiente;
        }

        verCarrito(inv);
        irA(36, IZQ); pausa();

        seleccionarMetodoPago(total);

        limpiarZonaVerde();
        linea(16, ">> Compra realizada. Gracias " + nombre + "!");
        irA(19, IZQ); pausa();

        delete carrito;
        carrito = new ListaEnlazada<int>();
    }

    void menuBuscarProducto(Inventario& inv) {
        int op;
        do {
            limpiarZonaVerde();
            linea(10, "--- BUSCAR PRODUCTO ---");
            linea(12, "1. Ver todo el catalogo");
            linea(13, "2. Buscar por nombre");
            linea(14, "3. Agregar al carrito");
            linea(15, "4. Ver catalogo por precio (menor a mayor)");
            linea(16, "5. Ver catalogo alfabeticamente");
            linea(17, "6. Volver");
            linea(19, "Opcion: ");
            cin >> op;

            switch (op) {
            case 1:
                limpiarZonaVerde();
                irA(10, IZQ);
                inv.listarTodo();
                irA(37, IZQ); pausa();
                break;
            case 2: {
                limpiarZonaVerde();
                linea(10, "Nombre a buscar: ");
                string nom; cin.ignore(); getline(cin, nom);
                limpiarZonaVerde();
                irA(10, IZQ);
                inv.buscarPorNombre(nom);
                irA(35, IZQ); pausa();
                break;
            }
            case 3:
                agregarAlCarrito(inv);
                break;
            case 4:
                limpiarZonaVerde();
                irA(10, IZQ);
                inv.ordenarPorPrecio(true);
                inv.listarTodo();
                irA(37, IZQ); pausa();
                break;
            case 5:
                limpiarZonaVerde();
                irA(10, IZQ);
                inv.ordenarAlfabetico();
                inv.listarTodo();
                irA(37, IZQ); pausa();
                break;
            }
        } while (op != 6);
    }

    void menuCarrito(Inventario& inv) {
        int op;
        do {
            limpiarZonaVerde();
            linea(10, "--- MI CARRITO ---");
            linea(12, "1. Ver carrito");
            linea(13, "2. Comprar");
            linea(14, "3. Volver");
            linea(16, "Opcion: ");
            cin >> op;

            switch (op) {
            case 1:
                limpiarZonaVerde();
                verCarrito(inv);
                irA(36, IZQ); pausa();
                break;
            case 2:
                comprarCarrito(inv);
                return;
            }
        } while (op != 3);
    }

    void menu(Inventario& inv) {
        mostrarFondo2();
        int op;
        do {
            limpiarZonaVerde();
            linea(10, "========================================");
            linea(11, "   BIENVENIDO, " + nombre);
            linea(12, "========================================");
            linea(14, "1. Ver productos / Buscar");
            linea(15, "2. Mi carrito");
            linea(16, "3. Cerrar sesion");
            linea(18, "Opcion: ");
            cin >> op;

            switch (op) {
            case 1: menuBuscarProducto(inv); break;
            case 2: menuCarrito(inv); break;
            }
        } while (op != 3);
    }
};
