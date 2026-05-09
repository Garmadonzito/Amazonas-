#pragma once
#include "ListaEnlazada.h"
#include "Producto.h"
#include <string>
#include <iostream>
#include <vector>
#include <functional>

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
            std::cout << "Inventario vacio.\n";
            return;
        }

        std::cout << "\n\033[1;36m========================================================\033[0m\n";
        std::cout << "\033[1;36m                  CATALOGO AMAZONAS                     \033[0m\n";
        std::cout << "\033[1;36m========================================================\033[0m\n";

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
            std::cout << "\n\033[1;33m>>> " << cat << " <<<\033[0m\n";
            actual = listaProductos->getCabeza();
            while (actual != nullptr) {
                if (actual->dato.categoria == cat) {
                    std::cout << "  [ID: " << actual->dato.id << "] "
                        << actual->dato.nombre
                        << " | Precio: S/. " << actual->dato.precio
                        << " | Stock: " << actual->dato.stock << "\n";
                }
                actual = actual->siguiente;
            }
        }
        std::cout << "\033[1;36m========================================================\033[0m\n";
    }

    void ordenarPorPrecio(bool ascendente = true) {
        auto comparador = [ascendente](Producto a, Producto b) -> bool {
            return ascendente ? (a.precio > b.precio) : (a.precio < b.precio);
        };
        listaProductos->ordenar(comparador);
        std::cout << ">> Inventario ordenado por precio "
                  << (ascendente ? "(menor a mayor)" : "(mayor a menor)") << ".\n";
    }

    void ordenarAlfabetico() {
        auto comparador = [](Producto a, Producto b) -> bool {
            return a.nombre > b.nombre;
        };
        listaProductos->ordenar(comparador);
        std::cout << ">> Inventario ordenado alfabeticamente.\n";
    }

    void buscarPorNombre(std::string nom) {
        Nodo<Producto>* actual = listaProductos->getCabeza();
        bool encontrado = false;
        std::cout << "\n--- RESULTADOS ---\n";
        while (actual != nullptr) {
            if (actual->dato.nombre.find(nom) != std::string::npos) {
                std::cout << "[ID: " << actual->dato.id << "] (" << actual->dato.categoria << ") "
                    << actual->dato.nombre << " | S/. " << actual->dato.precio << "\n";
                encontrado = true;
            }
            actual = actual->siguiente;
        }
        if (!encontrado) std::cout << "No se encontraron productos.\n";
    }
};
