#pragma once

#pragma once
#include <iostream>
#include <string>

using namespace std;

struct Producto {
    int id;
    string nombre;
    string categoria;
    double precio;
    int stock;
    bool enOferta;
};

class Inventario {
private:
    static const int MAX_PRODUCTOS = 100;
    Producto productos[MAX_PRODUCTOS];
    int totalProductos = 0;

public:
    Inventario() {
        totalProductos = 0;
    }

    void cargarProductosIniciales() {
        productos[0] = { 1, "Laptop HP", "Tecnologia", 2500.00, 10, false };
        productos[1] = { 2, "Mouse Logitech", "Tecnologia", 80.00, 25, true };
        productos[2] = { 3, "Teclado Redragon", "Tecnologia", 150.00, 15, false };
        productos[3] = { 4, "Olla Arrocera", "Cocina", 120.00, 8, true };
        productos[4] = { 5, "Sarten Teflon", "Cocina", 90.00, 12, false };
        productos[5] = { 6, "Polo Algodon", "Ropa", 45.00, 30, false };
        productos[6] = { 7, "Zapatillas Nike", "Ropa", 280.00, 20, true };
        totalProductos = 7;
    }

    int buscarProductoPorId(int id) {
        for (int i = 0; i < totalProductos; i++) {
            if (productos[i].id == id) return i;
        }
        return -1;
    }

    Producto* getProducto(int index) {
        if (index >= 0 && index < totalProductos) return &productos[index];
        return nullptr;
    }

    void mostrarProducto(Producto p) {
        cout << "  ID: " << p.id << " | " << p.nombre
            << " | Cat: " << p.categoria
            << " | S/. " << p.precio
            << " | Stock: " << p.stock;
        if (p.enOferta) cout << " [OFERTA]";
        cout << endl;
    }

    void verCategorias() {
        cout << "\n--- CATEGORIAS DISPONIBLES ---\n";
        cout << "1. Tecnologia\n2. Cocina\n3. Ropa\n";
        cout << "Elija categoria: ";
        int op; cin >> op;
        string cat;
        if (op == 1) cat = "Tecnologia";
        else if (op == 2) cat = "Cocina";
        else if (op == 3) cat = "Ropa";
        else { cout << "Opcion invalida\n"; return; }

        cout << "\n--- Productos en " << cat << " ---\n";
        for (int i = 0; i < totalProductos; i++) {
            if (productos[i].categoria == cat) mostrarProducto(productos[i]);
        }
    }

    void verOfertas() {
        cout << "\n--- PRODUCTOS EN OFERTA ---\n";
        for (int i = 0; i < totalProductos; i++) {
            if (productos[i].enOferta) mostrarProducto(productos[i]);
        }
    }

    void buscarPorNombre() {
        cout << "Ingrese nombre a buscar: ";
        string nom; cin.ignore(); getline(cin, nom);
        cout << "\n--- RESULTADOS ---\n";
        bool encontrado = false;
        for (int i = 0; i < totalProductos; i++) {
            if (productos[i].nombre.find(nom) != string::npos) {
                mostrarProducto(productos[i]);
                encontrado = true;
            }
        }
        if (!encontrado) cout << "No se encontraron productos.\n";
    }

    void generarReporteVenta() {
        cout << "\n--- REPORTE DE INVENTARIO ---\n";
        cout << "Total de productos: " << totalProductos << endl;
        double valorTotal = 0;
        for (int i = 0; i < totalProductos; i++) {
            valorTotal += productos[i].precio * productos[i].stock;
            mostrarProducto(productos[i]);
        }
        cout << "\nValor total inventario: S/. " << valorTotal << endl;
    }

    bool agregarProducto(Producto p) {
        if (totalProductos >= MAX_PRODUCTOS) return false;
        p.id = productos[totalProductos - 1].id + 1;
        productos[totalProductos++] = p;
        return true;
    }

    bool eliminarProducto(int id) {
        int idx = buscarProductoPorId(id);
        if (idx == -1) return false;
        for (int i = idx; i < totalProductos - 1; i++) {
            productos[i] = productos[i + 1];
        }
        totalProductos--;
        return true;
    }
};