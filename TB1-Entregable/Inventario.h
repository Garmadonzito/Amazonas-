#pragma once
#include "ListaEnlazada.h"
#include <string>
#include <iostream>
#include <functional>

struct Producto {
    int id;
    std::string nombre;
    float precio;
    int stock;
};

class Inventario {
private:
    // Estructura dinámica usando Lista Enlazada
    ListaEnlazada<Producto>* listaProductos;

    // MÉTODO RECURSIVO - Rafael
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

    void agregarProducto(int id, std::string nombre, float precio, int stock) {
        Producto nuevo = { id, nombre, precio, stock };
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

    // FUNCIÓN LAMBDA - Rafael
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
        Nodo<Producto>* actual = listaProductos->getCabeza();
        if (actual == nullptr) {
            std::cout << "Inventario vacio.\n";
            return;
        }
        std::cout << "\n--- CATALOGO COMPLETO ---\n";
        while (actual != nullptr) {
            std::cout << "ID: " << actual->dato.id
                << " | " << actual->dato.nombre
                << " | Precio: S/. " << actual->dato.precio
                << " | Stock: " << actual->dato.stock << "\n";
            actual = actual->siguiente;
        }
    }

    void buscarPorNombre(std::string nom) {
        Nodo<Producto>* actual = listaProductos->getCabeza();
        bool encontrado = false;
        std::cout << "\n--- RESULTADOS ---\n";
        while (actual != nullptr) {
            if (actual->dato.nombre.find(nom) != std::string::npos) {
                std::cout << "ID: " << actual->dato.id << " | " << actual->dato.nombre << " | S/. " << actual->dato.precio << "\n";
                encontrado = true;
            }
            actual = actual->siguiente;
        }
        if (!encontrado) std::cout << "No se encontraron productos.\n";
    }
};