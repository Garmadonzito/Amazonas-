#pragma once
#include "ListaDoble.h"
#include <string>
#include <iostream>
#include <functional>

// Estructura Producto - Base de los datos del Marketplace
struct Producto {
    int id;
    std::string nombre;
    float precio;
    int stock;
};

class Inventario {
private:
    
    ListaDoble<Producto>* listaProductos;

    // ---------------------------------------------------------
    // MÉTODO RECURSIVO  - Rafael
    // ---------------------------------------------------------
    // Busca un producto por ID navegando por los punteros de los nodos
    Nodo<Producto>* buscarRecursivo(Nodo<Producto>* actual, int idBuscado) {

        // Caso base 1: No se encontró el producto (final de lista)
        if (actual == nullptr) return nullptr;

        // Caso base 2: Producto encontrado
        if (actual->dato.id == idBuscado) return actual;

        // Paso recursivo: Se llama a sí misma con el siguiente nodo
        return buscarRecursivo(actual->siguiente, idBuscado);
    }

public:
    Inventario() {
        // Inicialización de la estructura dinámica en el heap
        listaProductos = new ListaDoble<Producto>();
    }

    ~Inventario() {
        // El destructor de ListaDoble se encarga de liberar los nodos
        delete listaProductos;
    }

    // Agrega un nuevo producto creando un nodo dinámico
    void agregarProducto(int id, std::string nombre, float precio, int stock) {
        Producto nuevo = { id, nombre, precio, stock };
        listaProductos->agregar(nuevo);
    }

    // Verifica existencia de un producto usando la función recursiva
    bool existeProducto(int id) {
        Nodo<Producto>* encontrado = buscarRecursivo(listaProductos->getCabeza(), id);
        return encontrado != nullptr;
    }

    // Modifica los datos de un producto existente
    void modificarProducto(int id, float nuevoPrecio, int nuevoStock) {
        Nodo<Producto>* actual = buscarRecursivo(listaProductos->getCabeza(), id);
        if (actual != nullptr) {
            actual->dato.precio = nuevoPrecio;
            actual->dato.stock = nuevoStock;
            std::cout << ">> Producto actualizado correctamente.\n";
        }
    }

    // Muestra la información específica de un solo artículo
    void mostrarDetalle(int id) {
        Nodo<Producto>* actual = buscarRecursivo(listaProductos->getCabeza(), id);
        if (actual != nullptr) {
            std::cout << "\n--- DETALLE DEL PRODUCTO ---\n";
            std::cout << "ID: " << actual->dato.id << "\n";
            std::cout << "Nombre: " << actual->dato.nombre << "\n";
            std::cout << "Precio: S/. " << actual->dato.precio << "\n";
            std::cout << "Stock disponible: " << actual->dato.stock << "\n";
        }
    }

    // Elimina un producto gestionando los enlaces de la lista doble
    bool eliminarProducto(int id) {
        Nodo<Producto>* actual = buscarRecursivo(listaProductos->getCabeza(), id);
        if (actual == nullptr) return false; // El ID no existe

       
        if (actual->anterior != nullptr) {
            actual->anterior->siguiente = actual->siguiente;
        }
        if (actual->siguiente != nullptr) {
            actual->siguiente->anterior = actual->anterior;
        }

        // Liberamos la memoria del nodo para evitar memory leaks
        delete actual;
        return true;
    }

    // ---------------------------------------------------------
    // USO DE FUNCIÓN LAMBDA -  Rafael
    // ---------------------------------------------------------
    // Filtra y muestra productos con stock 
    void mostrarStockBajo(int limite) {
        std::cout << "\n========== ALERTAS DE STOCK (Limite: " << limite << ") ==========\n";

        // Definición de la Lambda: Captura 'limite' por valor
        auto esCritico = [limite](Producto p) -> bool {
            return p.stock < limite;
            };

        Nodo<Producto>* actual = listaProductos->getCabeza();
        bool huboAlertas = false;

        while (actual != nullptr) {
            // Aplicación de la función lambda
            if (esCritico(actual->dato)) {
                std::cout << "[ID: " << actual->dato.id << "] "
                    << actual->dato.nombre << " - Unidades: "
                    << actual->dato.stock << "\n";
                huboAlertas = true;
            }
            actual = actual->siguiente;
        }

        if (!huboAlertas) {
            std::cout << "Todo el inventario cuenta con stock suficiente.\n";
        }
        std::cout << "========================================================\n";
    }

    // Método iterativo para mostrar todo el catálogo
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
};