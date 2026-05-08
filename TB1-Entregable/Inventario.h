#pragma once
#include "ListaEnlazada.h"
#include "Cola.h"
#include "Producto.h"
#include "Usuario.h" 
#include "Registro.h" 
#include "Venta.h"
#include <string>
#include <iostream>
#include <vector>
#include <functional>
#include <fstream>
#include <ctime>

class Inventario {
private:
    ListaEnlazada<Producto>* listaProductos;
    Cola<Venta>* registroVentas;

    // Métodos privados de soporte y recursividad
    Nodo<Producto>* buscarRecursivo(Nodo<Producto>* actual, int idBuscado) {
        if (actual == nullptr) return nullptr;
        if (actual->dato.id == idBuscado) return actual;
        return buscarRecursivo(actual->siguiente, idBuscado);
    }

    int contarVentasRecursivo(Nodo<Venta>* actual) {
        if (actual == nullptr) return 0;
        return 1 + contarVentasRecursivo(actual->siguiente);
    }

    std::string buscarNombreCliente(std::string dniBuscado) {
        std::ifstream archivo("clientes.dat", std::ios::binary);
        RegistroCliente reg;
        while (archivo.read((char*)&reg, sizeof(RegistroCliente))) {
            if (dniBuscado == reg.dni) {
                archivo.close();
                return reg.nombre;
            }
        }
        archivo.close();
        return "Cliente no encontrado";
    }

    std::string obtenerFechaTexto(long long fechaHora) {
        if (fechaHora == 0) return "Sin fecha";
        time_t tiempo = (time_t)fechaHora;
        tm fechaLocal;
        localtime_s(&fechaLocal, &tiempo);
        char texto[20];
        strftime(texto, sizeof(texto), "%d/%m/%Y %H:%M", &fechaLocal);
        return texto;
    }

    // ---------------------------------------------------------
    // ALGORITMO AVANZADO: ORDENAMIENTO SHELL
    // ---------------------------------------------------------
    void ordenarVentasShell(std::vector<Venta>& ventas) {
        int n = (int)ventas.size();
        if (n <= 1) return;
        int i, j, k, intervalo = n / 2;
        Venta temp;
        while (intervalo > 0) {
            for (i = intervalo; i < n; i++) {
                j = i - intervalo;
                while (j >= 0) {
                    k = j + intervalo;
                    if (ventas[j].fechaHora <= ventas[k].fechaHora) {
                        j = -1;
                    }
                    else {
                        temp = ventas[j];
                        ventas[j] = ventas[k];
                        ventas[k] = temp;
                        j -= intervalo;
                    }
                }
            }
            intervalo = intervalo / 2;
        }
    }

public:
    Inventario() {
        listaProductos = new ListaEnlazada<Producto>();
        registroVentas = new Cola<Venta>();
    }

    ~Inventario() {
        delete listaProductos;
        delete registroVentas;
    }

    // NUEVO MÉTODO: REPORTE DE STOCK GENERAL
    void verStockGeneral() {
        if (listaProductos->getCabeza() == nullptr) {
            imprimirEnPanel(10, "Inventario vacio.");
            return;
        }
        int fila = 10;
        imprimirEnPanel(fila++, "========================================================", 96);
        imprimirEnPanel(fila++, "                REPORTE DE STOCK GENERAL                ", 96);
        imprimirEnPanel(fila++, "========================================================", 96);
        fila++;
        Nodo<Producto>* actual = listaProductos->getCabeza();
        while (actual != nullptr) {
            std::string item = "Producto: " + actual->dato.nombre;
            while (item.length() < 45) item += " "; // Alineación manual
            item += "| Stock: " + std::to_string(actual->dato.stock);
            imprimirEnPanel(fila++, item, (actual->dato.stock < 10 ? 91 : 0)); // Rojo si es bajo
            actual = actual->siguiente;
        }
        imprimirEnPanel(fila, "========================================================", 96);
    }

    // Persistencia y Gestión
    void guardarEnArchivo() {
        std::ofstream archivo("productos.dat", std::ios::binary | std::ios::trunc);
        if (!archivo) return;
        Nodo<Producto>* actual = listaProductos->getCabeza();
        while (actual != nullptr) {
            RegistroProducto reg;
            reg.id = actual->dato.id;
            reg.precio = actual->dato.precio;
            reg.stock = actual->dato.stock;
            strncpy_s(reg.nombre, sizeof(reg.nombre), actual->dato.nombre.c_str(), _TRUNCATE);
            strncpy_s(reg.categoria, sizeof(reg.categoria), actual->dato.categoria.c_str(), _TRUNCATE);
            archivo.write((char*)&reg, sizeof(RegistroProducto));
            actual = actual->siguiente;
        }
        archivo.close();
    }

    void cargarDesdeArchivo() {
        std::ifstream archivo("productos.dat", std::ios::binary);
        if (!archivo) {
            cargarProductosIniciales();
            guardarEnArchivo();
            return;
        }
        listaProductos->vaciar();
        RegistroProducto reg;
        while (archivo.read((char*)&reg, sizeof(RegistroProducto))) {
            agregarProducto(reg.id, reg.nombre, reg.categoria, reg.precio, reg.stock, false);
        }
        archivo.close();
    }

    void cargarProductosIniciales() {
        agregarProducto(101, "Laptop Gamer ASUS ROG", "Computadoras", 4500.00f, 15, false);
        agregarProducto(104, "Monitor LG 144Hz 24\"", "Computadoras", 800.00f, 10, false);
        agregarProducto(102, "Smartphone Samsung S23", "Telefonia", 3200.00f, 20, false);
        agregarProducto(103, "Teclado Mecanico Redragon", "Accesorios", 150.00f, 50, false);
        agregarProducto(105, "Mouse Logitech G502", "Accesorios", 200.00f, 5, false);
        agregarProducto(201, "Amazon Echo Dot (Alexa)", "Dispositivos Amazon", 180.00f, 30, false);
        agregarProducto(202, "Kindle Paperwhite", "Dispositivos Amazon", 550.00f, 12, false);
        agregarProducto(203, "Fire TV Stick 4K", "Dispositivos Amazon", 220.00f, 25, false);
    }

    void agregarProducto(int id, std::string nombre, std::string categoria, float precio, int stock, bool guardar = true) {
        Producto nuevo(id, nombre, categoria, precio, stock);
        listaProductos->agregar(nuevo);
        if (guardar) guardarEnArchivo();
    }

    bool existeProducto(int id) {
        return buscarRecursivo(listaProductos->getCabeza(), id) != nullptr;
    }

    Producto* obtenerProducto(int id) {
        Nodo<Producto>* encontrado = buscarRecursivo(listaProductos->getCabeza(), id);
        return (encontrado != nullptr) ? &(encontrado->dato) : nullptr;
    }

    void modificarProducto(int id, float nuevoPrecio, int nuevoStock) {
        Producto* p = obtenerProducto(id);
        if (p != nullptr) {
            p->precio = nuevoPrecio;
            p->stock = nuevoStock;
            guardarEnArchivo();
        }
    }

    bool eliminarProducto(int id) {
        bool eliminado = listaProductos->eliminarSi([id](Producto p) -> bool { return p.id == id; });
        if (eliminado) guardarEnArchivo();
        return eliminado;
    }

    void mostrarStockBajo(int limite) {
        std::cout << "\n========== ALERTAS DE STOCK (Limite: " << limite << ") ==========\n";
        auto esCritico = [limite](Producto p) -> bool { return p.stock < limite; };
        Nodo<Producto>* actual = listaProductos->getCabeza();
        bool huboAlertas = false;
        while (actual != nullptr) {
            if (esCritico(actual->dato)) {
                std::cout << "[ID: " << actual->dato.id << "] " << actual->dato.nombre << " - Unidades: " << actual->dato.stock << "\n";
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
            bool existe = false;
            for (const std::string& cat : categorias) if (cat == actual->dato.categoria) existe = true;
            if (!existe) categorias.push_back(actual->dato.categoria);
            actual = actual->siguiente;
        }
        for (const std::string& cat : categorias) {
            imprimirEnPanel(fila++, ">>> " + cat + " <<<", 93);
            actual = listaProductos->getCabeza();
            while (actual != nullptr) {
                if (actual->dato.categoria == cat) {
                    std::string item = "  [ID: " + std::to_string(actual->dato.id) + "] " + actual->dato.nombre +
                        " | Precio: S/. " + std::to_string((int)actual->dato.precio) + " | Stock: " + std::to_string(actual->dato.stock);
                    imprimirEnPanel(fila++, item);
                }
                actual = actual->siguiente;
            }
            fila++;
        }
    }

    void buscarPorNombre(std::string nom) {
        Nodo<Producto>* actual = listaProductos->getCabeza();
        int fila = 10;
        bool encontrado = false;
        imprimirEnPanel(fila++, "--- RESULTADOS ---");
        while (actual != nullptr) {
            if (actual->dato.nombre.find(nom) != std::string::npos) {
                std::string item = "[ID: " + std::to_string(actual->dato.id) + "] (" + actual->dato.categoria + ") " + actual->dato.nombre;
                imprimirEnPanel(fila++, item);
                encontrado = true;
            }
            actual = actual->siguiente;
        }
        if (!encontrado) imprimirEnPanel(fila, "No se encontraron productos.");
    }

    void registrarVenta(std::string dniCliente, std::string cliente, Producto* producto) {
        if (producto == nullptr || producto->precio <= 0 || cliente == "") return;
        RegistroVenta reg;
        reg.idProducto = producto->id;
        reg.monto = producto->precio;
        reg.cantidadRestada = 1;
        reg.stockRestante = producto->stock;
        reg.fechaHora = (long long)time(0);
        std::string fechaT = obtenerFechaTexto(reg.fechaHora);
        strncpy_s(reg.fechaTexto, sizeof(reg.fechaTexto), fechaT.c_str(), _TRUNCATE);
        strncpy_s(reg.nombreProducto, sizeof(reg.nombreProducto), producto->nombre.c_str(), _TRUNCATE);
        strncpy_s(reg.dniCliente, sizeof(reg.dniCliente), dniCliente.c_str(), _TRUNCATE);
        std::ofstream archivo("ventas.dat", std::ios::binary | std::ios::app);
        if (archivo) {
            archivo.write((char*)&reg, sizeof(RegistroVenta));
            archivo.close();
        }
        Venta nueva(producto->id, dniCliente, cliente, producto->nombre, producto->precio, 1, producto->stock, true, reg.fechaHora, fechaT);
        registroVentas->encolar(nueva);
    }

    void cargarVentasDesdeArchivo() {
        delete registroVentas;
        registroVentas = new Cola<Venta>();
        std::ifstream archivo("ventas.dat", std::ios::binary);
        if (!archivo) return;
        std::vector<Venta> ventasOrdenadas;
        RegistroVenta reg;
        while (archivo.read((char*)&reg, sizeof(RegistroVenta))) {
            std::string cliente = buscarNombreCliente(reg.dniCliente);
            Venta v(reg.idProducto, reg.dniCliente, cliente, reg.nombreProducto, reg.monto, reg.cantidadRestada, reg.stockRestante, true, reg.fechaHora, reg.fechaTexto);
            ventasOrdenadas.push_back(v);
        }
        archivo.close();
        ordenarVentasShell(ventasOrdenadas);
        for (Venta v : ventasOrdenadas) registroVentas->encolar(v);
    }

    void mostrarRegistroVentas() {
        cargarVentasDesdeArchivo();
        std::cout << "\n========== REGISTRO DE VENTAS ==========\n";
        if (registroVentas->estaVacia()) {
            std::cout << "Sin ventas registradas.\n";
            return;
        }
        Nodo<Venta>* actual = registroVentas->getFrente();
        while (actual != nullptr) {
            actual->dato.mostrar();
            actual = actual->siguiente;
        }
        std::cout << "Total de ventas: " << contarVentasRecursivo(registroVentas->getFrente()) << "\n";
    }
};