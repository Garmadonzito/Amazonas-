#pragma once
#include "Usuario.h"
#include "Inventario.h"
#include "MetodoPago.h"

class Cliente : public Usuario {
private:
    static const int MAX_CARRITO = 50;
    int carrito[MAX_CARRITO];
    int totalCarrito = 0;

public:
    void login() {
        cout << "\n--- LOGIN CLIENTE ---\n";
        cout << "Nombre: "; cin.ignore(); getline(cin, nombre);
        cout << "Correo: "; getline(cin, correo);
        cout << "DNI: "; getline(cin, dni);
        cout << "\n>> Bienvenido " << nombre << "!\n";
        pausa();
    }

    void agregarAlCarrito(Inventario& inv) {
        cout << "Ingrese ID del producto a agregar: ";
        int id; cin >> id;
        int idx = inv.buscarProductoPorId(id);
        if (idx == -1) { cout << "Producto no existe.\n"; return; }

        Producto* p = inv.getProducto(idx);
        if (p->stock <= 0) { cout << "Sin stock.\n"; return; }
        if (totalCarrito >= MAX_CARRITO) { cout << "Carrito lleno.\n"; return; }

        carrito[totalCarrito++] = id;
        p->stock--;
        cout << ">> " << p->nombre << " agregado al carrito.\n";
    }

    void verCarrito(Inventario& inv) {
        cout << "\n--- TU CARRITO ---\n";
        if (totalCarrito == 0) { cout << "Vacio.\n"; return; }
        double total = 0;
        for (int i = 0; i < totalCarrito; i++) {
            int idx = inv.buscarProductoPorId(carrito[i]);
            if (idx != -1) {
                Producto* p = inv.getProducto(idx);
                cout << i + 1 << ". " << p->nombre << " - S/. " << p->precio << endl;
                total += p->precio;
            }
        }
        cout << "TOTAL: S/. " << total << endl;
    }

    void eliminarDelCarrito(Inventario& inv) {
        verCarrito(inv);
        if (totalCarrito == 0) return;
        cout << "Numero de item a eliminar: ";
        int n; cin >> n;
        if (n < 1 || n > totalCarrito) { cout << "Invalido.\n"; return; }

        int idx = inv.buscarProductoPorId(carrito[n - 1]);
        if (idx != -1) {
            Producto* p = inv.getProducto(idx);
            p->stock++;
        }
        for (int i = n - 1; i < totalCarrito - 1; i++) {
            carrito[i] = carrito[i + 1];
        }
        totalCarrito--;
        cout << ">> Item eliminado.\n";
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
        }
        else if (op == 2) {
            YapePlin yp;
            yp.pagar(total);
        }
        else {
            cout << "Opcion invalida. Pago cancelado.\n";
        }
    }

    void comprarCarrito(Inventario& inv) {
        if (totalCarrito == 0) { cout << "Carrito vacio.\n"; return; }
        double total = 0;
        for (int i = 0; i < totalCarrito; i++) {
            int idx = inv.buscarProductoPorId(carrito[i]);
            if (idx != -1) {
                total += inv.getProducto(idx)->precio;
            }
        }
        verCarrito(inv);
        seleccionarMetodoPago(total);
        cout << ">> Gracias " << nombre << "!\n";
        totalCarrito = 0;
    }

    void menuBuscarProducto(Inventario& inv) {
        int op;
        do {
            limpiarPantalla();
            cout << "\n--- BUSCAR PRODUCTO ---\n";
            cout << "1. Por categoria\n2. Productos en oferta\n3. Buscar por nombre\n";
            cout << "4. Agregar al carrito\n5. Volver\n";
            cout << "Opcion: "; cin >> op;
            limpiarPantalla();

            switch (op) {
            case 1: inv.verCategorias(); break;
            case 2: inv.verOfertas(); break;
            case 3: inv.buscarPorNombre(); break;
            case 4: agregarAlCarrito(inv); break;
            }
            if (op != 5) pausa();
        } while (op != 5);
    }

    void menuCarrito(Inventario& inv) {
        int op;
        do {
            limpiarPantalla();
            cout << "\n--- MI CARRITO ---\n";
            cout << "1. Ver carrito\n2. Eliminar del carrito\n3. Comprar\n4. Volver\n";
            cout << "Opcion: "; cin >> op;
            limpiarPantalla();

            switch (op) {
            case 1: verCarrito(inv); break;
            case 2: eliminarDelCarrito(inv); break;
            case 3: comprarCarrito(inv); pausa(); return;
            }
            if (op != 4 && op != 3) pausa();
        } while (op != 4);
    }

    void menu(Inventario& inv) {
        int op;
        do {
            limpiarPantalla();
            cout << "\n=== MENU CLIENTE - " << nombre << " ===\n";
            cout << "1. Ver productos / Buscar\n";
            cout << "2. Mi carrito\n";
            cout << "3. Historial de compras\n";
            cout << "4. Atencion al cliente (devoluciones)\n";
            cout << "5. Cerrar sesion\n";
            cout << "Opcion: "; cin >> op;

            switch (op) {
            case 1: menuBuscarProducto(inv); break;
            case 2: menuCarrito(inv); break;
            case 3: limpiarPantalla(); cout << "\n>> No hay compras registradas aun.\n"; pausa(); break;
            case 4: limpiarPantalla(); cout << "\n>> Devolucion registrada. Sera atendida en 48h.\n"; pausa(); break;
            }
        } while (op != 5);
    }
};