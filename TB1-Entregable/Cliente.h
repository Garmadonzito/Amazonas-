#pragma once
#include "Usuario.h"
#include "Inventario.h"

class Cliente : public Usuario {
private:
    // El carrito ahora es dinámico, reutilizando la Lista Enlazada
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

        // ---------------------------------------------------------
        // FUNCIÓN LAMBDA - Rafael
        // ---------------------------------------------------------
        auto validarDNI = [](string d) -> bool {
            // 1. Verifica que tenga exactamente 8 caracteres
            if (d.length() != 8) return false;

            // 2. Verifica manualmente que cada carácter sea un número
            for (char c : d) {
                if (c < '0' || c > '9') {
                    return false; 
                }
            }
            return true;
            };

        bool dniValido = false;
        do {
            cout << "DNI: ";
            getline(cin, dni);

            
            if (validarDNI(dni)) {
                dniValido = true;
            }
            else {
                cout << ">> Error: El DNI es invalido. Debe contener exactamente 8 numeros.\n";
            }
        } while (!dniValido);

        cout << "\n>> Bienvenido " << nombre << "!\n";
        pausa();
    }

    void agregarAlCarrito(Inventario& inv) {
        cout << "Ingrese ID del producto a agregar: ";
        int id; cin >> id;

        Producto* p = inv.obtenerProducto(id);
        if (p == nullptr) { cout << "Producto no existe.\n"; return; }
        if (p->stock <= 0) { cout << "Sin stock.\n"; return; }

        carrito->agregar(id);
        p->stock--;
        cout << ">> " << p->nombre << " agregado al carrito.\n";
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

    void comprarCarrito(Inventario& inv) {
        if (carrito->getCabeza() == nullptr) { cout << "Carrito vacio.\n"; return; }

        double total = 0;
        Nodo<int>* actual = carrito->getCabeza();
        while (actual != nullptr) {
            Producto* p = inv.obtenerProducto(actual->dato);
            if (p != nullptr) {
                total += p->precio;
            }
            actual = actual->siguiente;
        }
        cout << "\n>> Compra realizada por S/. " << total << endl;
        cout << ">> Gracias " << nombre << "!\n";

      
        delete carrito;
        carrito = new ListaEnlazada<int>();
    }

    void menuBuscarProducto(Inventario& inv) {
        int op;
        do {
            limpiarPantalla();
            cout << "\n--- BUSCAR PRODUCTO ---\n";
            cout << "1. Ver todo el catalogo\n2. Buscar por nombre\n";
            cout << "3. Agregar al carrito\n4. Volver\n";
            cout << "Opcion: "; cin >> op;
            limpiarPantalla();

            switch (op) {
            case 1: inv.listarTodo(); break;
            case 2: {
                cout << "Ingrese nombre: ";
                string nom; cin.ignore(); getline(cin, nom);
                inv.buscarPorNombre(nom);
                break;
            }
            case 3: agregarAlCarrito(inv); break;
            }
            if (op != 4) pausa();
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
            case 1: verCarrito(inv); break;
            case 2: comprarCarrito(inv); pausa(); return;
            }
            if (op != 3 && op != 2) pausa();
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