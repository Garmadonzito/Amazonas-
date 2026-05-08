#pragma once
#include "Usuario.h"
#include "Inventario.h"
#include <iostream>
#include <string>

using namespace std;

class Vendedor : public Usuario {
private:
    const string CODIGO_ACCESO = "UPC2026";

public:
    bool login() {
        string pass;
        cout << "\n--- ACCESO VENDEDOR ---\n";
        cout << "Ingrese codigo de seguridad: ";
        cin >> pass;
        if (pass == CODIGO_ACCESO) {
            cout << ">> Acceso concedido.\n";
            pausa();
            return true;
        }
        cout << ">> Codigo incorrecto.\n";
        pausa();
        return false;
    }

    void gestionarProductos(Inventario& inv) {
        int id, stock;
        string nombre, categoria;
        float precio;

        cout << "--- NUEVO PRODUCTO ---\n";
        cout << "ID: "; cin >> id;
        if (inv.existeProducto(id)) {
            cout << ">> El ID ya existe en el sistema.\n";
            return;
        }
        cout << "Nombre: "; cin.ignore(); getline(cin, nombre);
        cout << "Categoria: "; getline(cin, categoria);
        cout << "Precio: "; cin >> precio;
        cout << "Stock: "; cin >> stock;

        inv.agregarProducto(id, nombre, categoria, precio, stock);
        cout << ">> Producto guardado en la lista enlazada.\n";
    }

    void editarProducto(Inventario& inv) {
        int id;
        cout << "ID del producto a editar: "; cin >> id;
        if (!inv.existeProducto(id)) {
            cout << ">> Producto no encontrado.\n";
            return;
        }

        float nPrecio;
        int nStock;
        cout << "Nuevo Precio: "; cin >> nPrecio;
        cout << "Nuevo Stock: "; cin >> nStock;
        inv.modificarProducto(id, nPrecio, nStock);
    }

    void eliminarProducto(Inventario& inv) {
        int id;
        cout << "ID del producto a eliminar: "; cin >> id;
        if (inv.eliminarProducto(id)) {
            cout << ">> Nodo eliminado correctamente.\n";
        }
        else {
            cout << ">> No se pudo eliminar el producto.\n";
        }
    }

    void menu(Inventario& inv) {
        int op;
        do {
            limpiarPantalla();
            cout << "\n=== PANEL DE CONTROL VENDEDOR ===\n";
            cout << "1. Listar Catalogo Completo\n";
            cout << "2. Agregar Producto\n";
            cout << "3. Editar Producto\n";
            cout << "4. Eliminar Producto\n";
            cout << "5. Alertas de Stock (Lambda)\n";
            cout << "6. Cerrar Sesion\n";
            cout << "Opcion: "; cin >> op;

            switch (op) {
            case 1: inv.listarTodo(); break;
            case 2: gestionarProductos(inv); break;
            case 3: editarProducto(inv); break;
            case 4: eliminarProducto(inv); break;
            case 5: inv.mostrarStockBajo(10); break;
            }
            if (op != 6) pausa();
        } while (op != 6);
    }
};