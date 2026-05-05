#pragma once
#include "Usuario.h"
#include "Inventario.h"
#include "MetodoPago.h"
#include <conio.h>

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
        limpiarPantalla();
        inv.listarTodo();
        cout << "\n>> Ingrese ID del producto y ENTER para agregar. Presione ESC para terminar.\n";

        while (true) {
            cout << "ID: ";
            string buf;
            bool salir = false;

            while (true) {
                int c = _getch();
                if (c == 27) { salir = true; cout << "\n"; break; }
                if (c == '\r') { cout << "\n"; break; }
                if (c == 8 && !buf.empty()) {
                    buf.pop_back();
                    cout << "\b \b";
                } else if (c >= '0' && c <= '9') {
                    buf += (char)c;
                    cout << (char)c;
                }
            }

            if (salir) break;
            if (buf.empty()) continue;

            int id = stoi(buf);
            Producto* p = inv.obtenerProducto(id);
            if (p == nullptr) {
                cout << ">> Producto no existe.\n";
            } else if (p->stock <= 0) {
                cout << ">> Sin stock disponible.\n";
            } else {
                carrito->agregar(id);
                p->stock--;
                cout << ">> " << p->nombre << " agregado al carrito!\n";
            }
        }
    }

    void verCarrito(Inventario& inv) {
        cout << "\n--- TU CARRITO ---\n";
        if (carrito->getCabeza() == nullptr) { cout << "Vacio.\n"; return; }

        double total = 0;
        int i = 1;
        Nodo<int>* actual = carrito->getCabeza();

        while (actual != nullptr) {
            Producto* p = inv.obtenerProducto(actual->dato);
            if (p != nullptr) {
                cout << i << ". " << p->nombre << " - S/. " << p->precio << endl;
                total += p->precio;
            }
            actual = actual->siguiente;
            i++;
        }
        cout << "TOTAL: S/. " << total << endl;
    }

    void seleccionarMetodoPago(double total) {
        int op;
        cout << "\n--- METODO DE PAGO ---\n";
        cout << "1. Tarjeta\n";
        cout << "2. Yape / Plin\n";
        cout << "Opcion: "; cin >> op;

        if (op == 1) {
            Tarjeta t;
            t.pagar(total);
        } else if (op == 2) {
            YapePlin yp;
            yp.pagar(total);
        } else {
            cout << "Opcion invalida. Pago cancelado.\n";
        }
    }

    void comprarCarrito(Inventario& inv) {
        if (carrito->getCabeza() == nullptr) { cout << "Carrito vacio.\n"; return; }

        double total = 0;
        Nodo<int>* actual = carrito->getCabeza();
        while (actual != nullptr) {
            Producto* p = inv.obtenerProducto(actual->dato);
            if (p != nullptr) total += p->precio;
            actual = actual->siguiente;
        }
        verCarrito(inv);
        seleccionarMetodoPago(total);
        cout << ">> Gracias " << nombre << "!\n";

        delete carrito;
        carrito = new ListaEnlazada<int>();
    }

    void menuBuscarProducto(Inventario& inv) {
        int op;
        do {
            limpiarPantalla();
            cout << "\n--- BUSCAR PRODUCTO ---\n";
            cout << "1. Ver todo el catalogo\n";
            cout << "2. Buscar por nombre\n";
            cout << "3. Agregar al carrito\n";
            cout << "4. Volver\n";
            cout << "Opcion: "; cin >> op;
            limpiarPantalla();

            switch (op) {
            case 1: inv.listarTodo(); pausa(); break;
            case 2: {
                cout << "Ingrese nombre: ";
                string nom; cin.ignore(); getline(cin, nom);
                inv.buscarPorNombre(nom);
                pausa();
                break;
            }
            case 3: agregarAlCarrito(inv); break;
            }
        } while (op != 4);
    }

    void menuCarrito(Inventario& inv) {
        int op;
        do {
            limpiarPantalla();
            cout << "\n--- MI CARRITO ---\n";
            cout << "1. Ver carrito\n2. Comprar\n3. Volver\n";
            cout << "Opcion: "; cin >> op;
            limpiarPantalla();

            switch (op) {
            case 1: verCarrito(inv); pausa(); break;
            case 2: comprarCarrito(inv); pausa(); return;
            }
        } while (op != 3);
    }

    void menu(Inventario& inv) {
        int op;
        do {
            limpiarPantalla();
            cout << "\n=== MENU CLIENTE - " << nombre << " ===\n";
            cout << "1. Ver productos / Buscar\n";
            cout << "2. Mi carrito\n";
            cout << "3. Cerrar sesion\n";
            cout << "Opcion: "; cin >> op;

            switch (op) {
            case 1: menuBuscarProducto(inv); break;
            case 2: menuCarrito(inv); break;
            }
        } while (op != 3);
    }
};
