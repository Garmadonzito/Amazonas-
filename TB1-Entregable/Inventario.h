#pragma once
#include "ListaEnlazada.h"
#include "Producto.h"
#include <string>
#include <iostream>
#include <vector>
#include <functional>

inline void imprimirEnPanel(int fila, const std::string& texto, int color = 0) {
    const int panelCol = 12;
    std::cout << "\033[" << fila << ";" << panelCol << "H";
    if (color != 0) std::cout << "\033[" << color << "m";
    else std::cout << "\033[0m";
    std::cout << texto << "\033[0m";
}

class Inventario {
private:
    ListaEnlazada<Producto>* listaProductos;

    Nodo<Producto>* buscarRecursivo(Nodo<Producto>* actual, int idBuscado) {
        if (actual == nullptr) return nullptr;
        if (actual->dato.id == idBuscado) return actual;
        return buscarRecursivo(actual->siguiente, idBuscado);
    }

public:
    Inventario() {
        listaProductos = new ListaEnlazada<Producto>();
    }

    ~Inventario() {
        delete listaProductos;
    }

    void cargarProductosIniciales() {
        agregarProducto(101, "Laptop Gamer ASUS ROG", "Computadoras", 4500.00f, 15);
        agregarProducto(104, "Monitor LG 144Hz 24\"", "Computadoras", 800.00f, 10);

        agregarProducto(102, "Smartphone Samsung S23", "Telefonia", 3200.00f, 20);

        agregarProducto(103, "Teclado Mecanico Redragon", "Accesorios", 150.00f, 50);
        agregarProducto(105, "Mouse Logitech G502", "Accesorios", 200.00f, 5);

        agregarProducto(201, "Amazon Echo Dot (Alexa)", "Dispositivos Amazon", 180.00f, 30);
        agregarProducto(202, "Kindle Paperwhite", "Dispositivos Amazon", 550.00f, 12);
        agregarProducto(203, "Fire TV Stick 4K", "Dispositivos Amazon", 220.00f, 25);
    }

    void agregarProducto(int id, std::string nombre, std::string categoria, float precio, int stock) {
        Producto nuevo(id, nombre, categoria, precio, stock);
        listaProductos->agregar(nuevo);
    }

    bool existeProducto(int id) {
        return buscarRecursivo(listaProductos->getCabeza(), id) != nullptr;
    }

    Producto* obtenerProducto(int id) {
        Nodo<Producto>* encontrado = buscarRecursivo(listaProductos->getCabeza(), id);
        if (encontrado != nullptr) {
            return &(encontrado->dato);
        }
        return nullptr;
    }

    void modificarProducto(int id, float nuevoPrecio, int nuevoStock) {
        Producto* p = obtenerProducto(id);
        if (p != nullptr) {
            p->precio = nuevoPrecio;
            p->stock = nuevoStock;
            std::cout << ">> Producto actualizado correctamente.\n";
        }
    }

    void mostrarDetalle(int id) {
        Producto* p = obtenerProducto(id);
        if (p != nullptr) {
            std::cout << "\n--- DETALLE DEL PRODUCTO ---\n";
            std::cout << "ID: " << p->id << "\n";
            std::cout << "Categoria: " << p->categoria << "\n";
            std::cout << "Nombre: " << p->nombre << "\n";
            std::cout << "Precio: S/. " << p->precio << "\n";
            std::cout << "Stock disponible: " << p->stock << "\n";
        }
    }

    bool eliminarProducto(int id) {
        return listaProductos->eliminarSi([id](Producto p) -> bool {
            return p.id == id;
            });
    }

    void mostrarStockBajo(int limite) {
        std::cout << "\n========== ALERTAS DE STOCK (Limite: " << limite << ") ==========\n";
        auto esCritico = [limite](Producto p) -> bool {
            return p.stock < limite;
            };
        Nodo<Producto>* actual = listaProductos->getCabeza();
        bool huboAlertas = false;
        while (actual != nullptr) {
            if (esCritico(actual->dato)) {
                std::cout << "[ID: " << actual->dato.id << "] "
                    << actual->dato.nombre << " - Unidades: "
                    << actual->dato.stock << "\n";
                huboAlertas = true;
            }
            actual = actual->siguiente;
        }
        if (!huboAlertas) std::cout << "Todo el inventario cuenta con stock suficiente.\n";
        std::cout << "========================================================\n";
    }

    void listarTodo() {
        if (listaProductos->getCabeza() == nullptr) {
            imprimirEnPanel(10, "Inventario vacio.");
            return;
        }

        int fila = 10;
        imprimirEnPanel(fila++, "========================================================", 96);
        imprimirEnPanel(fila++, "                  CATALOGO AMAZONAS                     ", 96);
        imprimirEnPanel(fila++, "========================================================", 96);
        fila++;

        std::vector<std::string> categorias;
        Nodo<Producto>* actual = listaProductos->getCabeza();

        while (actual != nullptr) {
            bool categoriaExiste = false;
            for (const std::string& cat : categorias) {
                if (cat == actual->dato.categoria) {
                    categoriaExiste = true;
                    break;
                }
            }
            if (!categoriaExiste) {
                categorias.push_back(actual->dato.categoria);
            }
            actual = actual->siguiente;
        }

        for (const std::string& cat : categorias) {
            imprimirEnPanel(fila++, ">>> " + cat + " <<<", 93);
            actual = listaProductos->getCabeza();
            while (actual != nullptr) {
                if (actual->dato.categoria == cat) {
                    std::string item = "  [ID: " + std::to_string(actual->dato.id) + "] " +
                        actual->dato.nombre +
                        " | Precio: S/. " + std::to_string((int)actual->dato.precio) +
                        " | Stock: " + std::to_string(actual->dato.stock);
                    imprimirEnPanel(fila++, item);
                }
                actual = actual->siguiente;
            }
            fila++;
        }
        imprimirEnPanel(fila, "========================================================", 96);
    }

    void buscarPorNombre(std::string nom) {
        Nodo<Producto>* actual = listaProductos->getCabeza();
        bool encontrado = false;
        int fila = 10;
        imprimirEnPanel(fila++, "--- RESULTADOS ---");
        fila++;
        while (actual != nullptr) {
            if (actual->dato.nombre.find(nom) != std::string::npos) {
                std::string item = "[ID: " + std::to_string(actual->dato.id) + "] (" +
                    actual->dato.categoria + ") " + actual->dato.nombre +
                    " | S/. " + std::to_string((int)actual->dato.precio);
                imprimirEnPanel(fila++, item);
                encontrado = true;
            }
            actual = actual->siguiente;
        }
        if (!encontrado) imprimirEnPanel(fila, "No se encontraron productos.");
    }
};
