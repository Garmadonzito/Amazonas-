#pragma once
#include "Usuario.h"
#include "Inventario.h"
#include "MetodoPago.h"
#include <conio.h>

extern void mostrarFondo2();

class Cliente : public Usuario {
private:
    ListaEnlazada<int>* carrito;

    // Imprime una linea en la posicion indicada dentro del fondo
    void linea(int fila, int col, const string& texto) {
        irA(fila, col);
        cout << "\033[0m" << texto;
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
        mostrarFondo2();
        irA(11, 14);
        inv.listarTodo();

        irA(30, 14);
        cout << "\033[0m>> Ingrese ID y ENTER para agregar. ESC para terminar.";

        int filaInput = 31;
        while (true) {
            irA(filaInput, 14);
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
            irA(filaInput + 1, 14);
            cout << "\033[0m";
            if (p == nullptr) {
                cout << ">> Producto no existe.                    ";
            } else if (p->stock <= 0) {
                cout << ">> Sin stock disponible.                  ";
            } else {
                carrito->agregar(id);
                p->stock--;
                cout << ">> " << p->nombre << " agregado!          ";
                filaInput += 2;
                if (filaInput > 36) filaInput = 31;
            }
        }
    }

    void verCarrito(Inventario& inv) {
        int fila = 13;
        linea(fila++, 14, "--- TU CARRITO ---");
        if (carrito->getCabeza() == nullptr) {
            linea(fila++, 14, "Vacio.            ");
            return;
        }

        double total = 0;
        int i = 1;
        Nodo<int>* actual = carrito->getCabeza();
        while (actual != nullptr) {
            Producto* p = inv.obtenerProducto(actual->dato);
            if (p != nullptr) {
                string item = to_string(i) + ". " + p->nombre + " - S/. " + to_string((int)p->precio);
                linea(fila++, 14, item);
                total += p->precio;
            }
            actual = actual->siguiente;
            i++;
        }
        linea(fila++, 14, "TOTAL: S/. " + to_string((int)total) + "      ");
    }

    void seleccionarMetodoPago(double total) {
        limpiarMenuArea();
        linea(11, 14, "--- METODO DE PAGO ---");
        linea(13, 14, "1. Tarjeta");
        linea(14, 14, "2. Yape / Plin");
        linea(16, 14, "Opcion: ");
        irA(16, 22);
        int op; cin >> op;

        if (op == 1) {
            Tarjeta t;
            limpiarMenuArea();
            irA(11, 14);
            t.pagar(total);
        } else if (op == 2) {
            limpiarPantalla();
            YapePlin yp;
            yp.pagar(total);
            mostrarFondo2();
        } else {
            linea(18, 14, "Opcion invalida. Pago cancelado.");
        }
    }

    void comprarCarrito(Inventario& inv) {
        if (carrito->getCabeza() == nullptr) {
            linea(13, 14, "Carrito vacio.          ");
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
        seleccionarMetodoPago(total);

        linea(26, 14, ">> Gracias " + nombre + "!          ");
        irA(28, 14);
        pausa();

        delete carrito;
        carrito = new ListaEnlazada<int>();
    }

    void menuBuscarProducto(Inventario& inv) {
        int op;
        do {
            mostrarFondo2();
            limpiarMenuArea();
            linea(11, 14, "--- BUSCAR PRODUCTO ---");
            linea(13, 14, "1. Ver todo el catalogo");
            linea(14, 14, "2. Buscar por nombre");
            linea(15, 14, "3. Agregar al carrito");
            linea(16, 14, "4. Volver");
            linea(18, 14, "Opcion: ");
            irA(18, 22);
            cin >> op;

            switch (op) {
            case 1:
                mostrarFondo2();
                irA(11, 14);
                inv.listarTodo();
                irA(37, 14);
                pausa();
                break;
            case 2: {
                limpiarMenuArea();
                linea(11, 14, "Ingrese nombre: ");
                irA(11, 30);
                string nom; cin.ignore(); getline(cin, nom);
                irA(13, 14);
                inv.buscarPorNombre(nom);
                irA(30, 14);
                pausa();
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
            mostrarFondo2();
            limpiarMenuArea();
            linea(11, 14, "--- MI CARRITO ---");
            linea(13, 14, "1. Ver carrito");
            linea(14, 14, "2. Comprar");
            linea(15, 14, "3. Volver");
            linea(17, 14, "Opcion: ");
            irA(17, 22);
            cin >> op;

            switch (op) {
            case 1:
                limpiarMenuArea();
                verCarrito(inv);
                irA(35, 14);
                pausa();
                break;
            case 2:
                limpiarMenuArea();
                comprarCarrito(inv);
                return;
            }
        } while (op != 3);
    }

    void menu(Inventario& inv) {
        mostrarFondo2();
        int op;
        do {
            limpiarMenuArea();
            linea(11, 14, "========================================");
            linea(12, 14, "   MENU CLIENTE - " + nombre);
            linea(13, 14, "========================================");
            linea(15, 14, "1. Ver productos / Buscar");
            linea(16, 14, "2. Mi carrito");
            linea(17, 14, "3. Cerrar sesion");
            linea(19, 14, "Opcion: ");
            irA(19, 22);
            cin >> op;

            switch (op) {
            case 1: menuBuscarProducto(inv); mostrarFondo2(); break;
            case 2: menuCarrito(inv); mostrarFondo2(); break;
            }
        } while (op != 3);
    }
};
