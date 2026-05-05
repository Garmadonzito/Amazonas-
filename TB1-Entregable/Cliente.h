#pragma once
#include "Usuario.h"
#include "Inventario.h"
#include "MetodoPago.h"
#include <conio.h>

extern void mostrarFondo2();

class Cliente : public Usuario {
private:
    ListaEnlazada<int>* carrito;

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

        // Mostrar catalogo en la zona verde
        int fila = 10;
        irA(fila++, 12); cout << "\033[0m";
        inv.listarTodo();

        irA(32, 12);
        cout << "\033[0m>> Ingrese ID y ENTER para agregar. ESC para terminar.\n";

        int filaMsg = 34;
        while (true) {
            irA(filaMsg, 12);
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
            irA(filaMsg + 1, 12);
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
        int fila = 14;
        imprimirCentrado(fila++, "--- TU CARRITO ---");
        fila++;

        if (carrito->getCabeza() == nullptr) {
            imprimirCentrado(fila, "Carrito vacio.");
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
                imprimirCentrado(fila++, item);
                total += p->precio;
            }
            actual = actual->siguiente;
            i++;
        }
        fila++;
        imprimirCentrado(fila, "TOTAL: S/. " + to_string((int)total));
    }

    void seleccionarMetodoPago(double total) {
        limpiarZonaVerde();
        imprimirCentrado(14, "==============================");
        imprimirCentrado(15, "     METODO DE PAGO           ");
        imprimirCentrado(16, "==============================");
        imprimirCentrado(18, "1. Tarjeta");
        imprimirCentrado(19, "2. Yape / Plin");
        imprimirCentrado(21, "Opcion: ");
        irA(21, 63);
        int op; cin >> op;

        if (op == 1) {
            limpiarZonaVerde();
            irA(13, 12);
            Tarjeta t;
            t.pagar(total);
        } else if (op == 2) {
            limpiarPantalla();
            YapePlin yp;
            yp.pagar(total);
            mostrarFondo2();
        } else {
            imprimirCentrado(23, "Opcion invalida. Pago cancelado.");
        }
    }

    void comprarCarrito(Inventario& inv) {
        limpiarZonaVerde();
        if (carrito->getCabeza() == nullptr) {
            imprimirCentrado(20, "El carrito esta vacio.");
            irA(23, 12); pausa();
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
        irA(36, 12); pausa();

        seleccionarMetodoPago(total);

        limpiarZonaVerde();
        imprimirCentrado(20, ">> Compra realizada. Gracias " + nombre + "!");
        irA(23, 12); pausa();

        delete carrito;
        carrito = new ListaEnlazada<int>();
    }

    void menuBuscarProducto(Inventario& inv) {
        int op;
        do {
            limpiarZonaVerde();
            imprimirCentrado(14, "==============================");
            imprimirCentrado(15, "      BUSCAR PRODUCTO         ");
            imprimirCentrado(16, "==============================");
            imprimirCentrado(18, "1. Ver todo el catalogo");
            imprimirCentrado(19, "2. Buscar por nombre");
            imprimirCentrado(20, "3. Agregar al carrito");
            imprimirCentrado(21, "4. Volver");
            imprimirCentrado(23, "Opcion: ");
            irA(23, 63);
            cin >> op;

            switch (op) {
            case 1:
                limpiarZonaVerde();
                irA(10, 12);
                inv.listarTodo();
                irA(37, 12); pausa();
                break;
            case 2: {
                limpiarZonaVerde();
                imprimirCentrado(18, "Ingrese nombre a buscar: ");
                irA(18, 76);
                string nom; cin.ignore(); getline(cin, nom);
                limpiarZonaVerde();
                irA(11, 12);
                inv.buscarPorNombre(nom);
                irA(35, 12); pausa();
                break;
            }
            case 3:
                agregarAlCarrito(inv);
                break;
            }
        } while (op != 4);
    }

    void menuCarrito(Inventario& inv) {
        int op;
        do {
            limpiarZonaVerde();
            imprimirCentrado(14, "==============================");
            imprimirCentrado(15, "         MI CARRITO           ");
            imprimirCentrado(16, "==============================");
            imprimirCentrado(18, "1. Ver carrito");
            imprimirCentrado(19, "2. Comprar");
            imprimirCentrado(20, "3. Volver");
            imprimirCentrado(22, "Opcion: ");
            irA(22, 63);
            cin >> op;

            switch (op) {
            case 1:
                limpiarZonaVerde();
                verCarrito(inv);
                irA(36, 12); pausa();
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
            imprimirCentrado(13, "========================================");
            imprimirCentrado(14, "   BIENVENIDO, " + nombre);
            imprimirCentrado(15, "========================================");
            imprimirCentrado(17, "1. Ver productos / Buscar");
            imprimirCentrado(18, "2. Mi carrito");
            imprimirCentrado(19, "3. Cerrar sesion");
            imprimirCentrado(21, "Opcion: ");
            irA(21, 63);
            cin >> op;

            switch (op) {
            case 1: menuBuscarProducto(inv); break;
            case 2: menuCarrito(inv); break;
            }
        } while (op != 3);
    }
};
