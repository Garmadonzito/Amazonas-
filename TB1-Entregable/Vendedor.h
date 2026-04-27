#pragma once

#pragma once
#include "Usuario.h"
#include "Inventario.h"

class Vendedor : public Usuario {
private:
    const string CODIGO_SECRETO = "VEND123";

public:
    bool login() {
        cout << "\n--- LOGIN VENDEDOR ---\n";
        cout << "Codigo de vendedor: ";
        string codigo; cin >> codigo;
        if (codigo != CODIGO_SECRETO) {
            cout << ">> Codigo incorrecto.\n";
            pausa();
            return false;
        }
        cout << ">> Acceso concedido.\n";
        pausa();
        return true;
    }

    void anadirProducto(Inventario& inv) {
        Producto p;
        cout << "Nombre: "; cin.ignore(); getline(cin, p.nombre);
        cout << "Categoria: "; getline(cin, p.categoria);
        cout << "Precio: "; cin >> p.precio;
        cout << "Stock: "; cin >> p.stock;
        cout << "En oferta? (1=si, 0=no): "; cin >> p.enOferta;

        if (inv.agregarProducto(p)) {
            cout << ">> Producto agregado exitosamente.\n";
        }
        else {
            cout << ">> Error: Limite de productos alcanzado.\n";
        }
    }

    void editarProducto(Inventario& inv) {
        cout << "Ingrese ID a editar: "; int id; cin >> id;
        int idx = inv.buscarProductoPorId(id);
        if (idx == -1) { cout << "No existe.\n"; return; }

        Producto* p = inv.getProducto(idx);
        cout << "Nuevo precio (actual " << p->precio << "): "; cin >> p->precio;
        cout << "Nuevo stock (actual " << p->stock << "): "; cin >> p->stock;
        cout << ">> Actualizado.\n";
    }

    void eliminarProducto(Inventario& inv) {
        cout << "Ingrese ID a eliminar: "; int id; cin >> id;
        if (inv.eliminarProducto(id)) {
            cout << ">> Producto eliminado.\n";
        }
        else {
            cout << ">> Error: Producto no encontrado.\n";
        }
    }

    void buscarProducto(Inventario& inv) {
        cout << "Ingrese ID: "; int id; cin >> id;
        int idx = inv.buscarProductoPorId(id);
        if (idx == -1) { cout << "No existe.\n"; return; }
        cout << "\n--- DETALLE ---\n";
        inv.mostrarProducto(*inv.getProducto(idx));
    }

    void menuInventario(Inventario& inv) {
        int op;
        do {
            limpiarPantalla();
            cout << "\n--- INVENTARIO ---\n";
            cout << "1. Buscar producto\n2. Anadir producto\n";
            cout << "3. Editar producto\n4. Eliminar producto\n5. Volver\n";
            cout << "Opcion: "; cin >> op;
            limpiarPantalla();

            switch (op) {
            case 1: buscarProducto(inv); break;
            case 2: anadirProducto(inv); break;
            case 3: editarProducto(inv); break;
            case 4: eliminarProducto(inv); break;
            }
            if (op != 5) pausa();
        } while (op != 5);
    }

    void menu(Inventario& inv) {
        int op;
        do {
            limpiarPantalla();
            cout << "\n=== MENU VENDEDOR ===\n";
            cout << "1. Inventario\n2. Generar reporte general\n3. Cerrar sesion\n";
            cout << "Opcion: "; cin >> op;

            switch (op) {
            case 1: menuInventario(inv); break;
            case 2:
                limpiarPantalla();
                inv.generarReporteVenta();
                pausa();
                break;
            }
        } while (op != 3);
    }
};