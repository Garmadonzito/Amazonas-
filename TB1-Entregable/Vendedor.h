#pragma once
#include "Usuario.h"
#include "Inventario.h"
#include <iostream>
#include <string>

using namespace std;


void pausa();
void limpiarPantalla();

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
        int id, stock;
        string nombre;
        float precio;

        cout << "ID del Producto: "; cin >> id;

        // Usamos la funcion recursiva para evitar duplicados
        if (inv.existeProducto(id)) {
            cout << ">> Error: Ya existe un producto con el ID " << id << ".\n";
            return;
        }

        cout << "Nombre: "; cin.ignore(); getline(cin, nombre);
        cout << "Precio: "; cin >> precio;
        cout << "Stock: "; cin >> stock;

        // Ahora no hay limite de espacio gracias a la memoria dinamica
        inv.agregarProducto(id, nombre, precio, stock);
        cout << ">> Producto agregado exitosamente al catalogo dinamico.\n";
    }

    void editarProducto(Inventario& inv) {
        cout << "Ingrese ID a editar: "; int id; cin >> id;
        if (!inv.existeProducto(id)) {
            cout << ">> Error: El producto no existe.\n";
            return;
        }

        float nuevoPrecio;
        int nuevoStock;
        cout << "Nuevo precio: "; cin >> nuevoPrecio;
        cout << "Nuevo stock: "; cin >> nuevoStock;

        inv.modificarProducto(id, nuevoPrecio, nuevoStock);
    }

    void eliminarProducto(Inventario& inv) {
        cout << "Ingrese ID a eliminar: "; int id; cin >> id;
        if (inv.eliminarProducto(id)) {
            cout << ">> Producto eliminado exitosamente de la lista.\n";
        }
        else {
            cout << ">> Error: Producto no encontrado.\n";
        }
    }

    void buscarProducto(Inventario& inv) {
        cout << "Ingrese ID a buscar: "; int id; cin >> id;
        if (!inv.existeProducto(id)) {
            cout << ">> No existe en el inventario.\n";
            return;
        }
        cout << "\n--- DETALLE DEL PRODUCTO ---\n";
        inv.mostrarDetalle(id);
    }

    void menuInventario(Inventario& inv) {
        int op;
        do {
            limpiarPantalla();
            cout << "\n--- GESTION DE INVENTARIO ---\n";
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
            cout << "\n=== PANEL DE VENDEDOR ===\n";
            cout << "1. Gestionar Inventario\n";
            cout << "2. Generar Reporte General (Todo el catalogo)\n";
            cout << "3. Generar Alerta de Stock (Lambda)\n";
            cout << "4. Cerrar sesion\n";
            cout << "Opcion: "; cin >> op;

            switch (op) {
            case 1: menuInventario(inv); break;
            case 2:
                limpiarPantalla();
                inv.listarTodo(); // Lista toda la estructura dinamica
                pausa();
                break;
            case 3:
                limpiarPantalla();
                inv.mostrarStockBajo(10); // Llama a la funcion lambda
                pausa();
                break;
            }
        } while (op != 4);
    }
};