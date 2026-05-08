#pragma once
#include "ListaEnlazada.h"
#include "Cola.h"
#include "Producto.h"
#include "Usuario.h" 
#include "Registro.h" // Nombre corregido
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

    struct RegistroVentaAntiguo {
        int idProducto;
        char nombreProducto[50];
        float monto;
        char dniCliente[9];
    };

    struct RegistroVentaSinFecha {
        int idProducto;
        char nombreProducto[50];
        float monto;
        char dniCliente[9];
        int cantidadRestada;
        int stockRestante;
    };

    // Complejidad Tiempo O(n), Espacio O(n) por la pila de llamadas
    Nodo<Producto>* buscarRecursivo(Nodo<Producto>* actual, int idBuscado) {
        if (actual == nullptr) return nullptr;
        if (actual->dato.id == idBuscado) return actual;
        return buscarRecursivo(actual->siguiente, idBuscado);
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

    void ordenarVentasBurbuja(std::vector<Venta>& ventas) {
        int n = (int)ventas.size();
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (ventas[j].fechaHora > ventas[j + 1].fechaHora) {
                    Venta temp = ventas[j];
                    ventas[j] = ventas[j + 1];
                    ventas[j + 1] = temp;
                }
            }
        }
    }

    void convertirVentasAntiguas() {
        std::ifstream archivo("ventas.dat", std::ios::binary);
        if (!archivo) return;

        archivo.seekg(0, std::ios::end);
        long tamanoArchivo = (long)archivo.tellg();
        archivo.seekg(0, std::ios::beg);

        if (tamanoArchivo == 0 || tamanoArchivo % sizeof(RegistroVenta) == 0) {
            archivo.close();
            return;
        }

        std::vector<RegistroVenta> ventasConvertidas;

        if (tamanoArchivo % sizeof(RegistroVentaSinFecha) == 0) {
            RegistroVentaSinFecha sinFecha;
            while (archivo.read((char*)&sinFecha, sizeof(RegistroVentaSinFecha))) {
                RegistroVenta nuevo;
                nuevo.idProducto = sinFecha.idProducto;
                nuevo.monto = sinFecha.monto;
                nuevo.cantidadRestada = sinFecha.cantidadRestada;
                nuevo.stockRestante = sinFecha.stockRestante;
                nuevo.fechaHora = 0;
                strncpy_s(nuevo.fechaTexto, sizeof(nuevo.fechaTexto), "Sin fecha", _TRUNCATE);
                strncpy_s(nuevo.nombreProducto, sizeof(nuevo.nombreProducto), sinFecha.nombreProducto, _TRUNCATE);
                strncpy_s(nuevo.dniCliente, sizeof(nuevo.dniCliente), sinFecha.dniCliente, _TRUNCATE);
                ventasConvertidas.push_back(nuevo);
            }
            archivo.close();

            std::ofstream salida("ventas.dat", std::ios::binary | std::ios::trunc);
            for (RegistroVenta venta : ventasConvertidas) {
                salida.write((char*)&venta, sizeof(RegistroVenta));
            }
            salida.close();
            return;
        }

        if (tamanoArchivo % sizeof(RegistroVentaAntiguo) != 0) {
            archivo.close();
            return;
        }

        RegistroVentaAntiguo antiguo;
        while (archivo.read((char*)&antiguo, sizeof(RegistroVentaAntiguo))) {
            RegistroVenta nuevo;
            nuevo.idProducto = antiguo.idProducto;
            nuevo.monto = antiguo.monto;
            nuevo.cantidadRestada = 1;

            Producto* p = obtenerProducto(antiguo.idProducto);
            nuevo.stockRestante = (p != nullptr) ? p->stock : 0;
            nuevo.fechaHora = 0;
            strncpy_s(nuevo.fechaTexto, sizeof(nuevo.fechaTexto), "Sin fecha", _TRUNCATE);

            strncpy_s(nuevo.nombreProducto, sizeof(nuevo.nombreProducto), antiguo.nombreProducto, _TRUNCATE);
            strncpy_s(nuevo.dniCliente, sizeof(nuevo.dniCliente), antiguo.dniCliente, _TRUNCATE);
            ventasConvertidas.push_back(nuevo);
        }
        archivo.close();

        std::ofstream salida("ventas.dat", std::ios::binary | std::ios::trunc);
        for (RegistroVenta venta : ventasConvertidas) {
            salida.write((char*)&venta, sizeof(RegistroVenta));
        }
        salida.close();
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
            guardarEnArchivo(); 
        }
    }

    bool eliminarProducto(int id) {
        bool eliminado = listaProductos->eliminarSi([id](Producto p) -> bool {
            return p.id == id;
        });
        if (eliminado) guardarEnArchivo(); 
        return eliminado;
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
            if (!categoriaExiste) categorias.push_back(actual->dato.categoria);
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

    void registrarVenta(std::string dniCliente, std::string cliente, Producto* producto) {
        auto precioValido = [](float monto) -> bool {
            return monto > 0;
            };

        auto tieneCliente = [](std::string nombre) -> bool {
            return nombre != "";
            };

        if (producto == nullptr) return;
        if (!precioValido(producto->precio)) return;
        if (!tieneCliente(cliente)) return;

        RegistroVenta reg;
        reg.idProducto = producto->id;
        reg.monto = producto->precio;
        reg.cantidadRestada = 1;
        reg.stockRestante = producto->stock;
        reg.fechaHora = (long long)time(0);
        std::string fechaTexto = obtenerFechaTexto(reg.fechaHora);
        strncpy_s(reg.fechaTexto, sizeof(reg.fechaTexto), fechaTexto.c_str(), _TRUNCATE);
        strncpy_s(reg.nombreProducto, sizeof(reg.nombreProducto), producto->nombre.c_str(), _TRUNCATE);
        strncpy_s(reg.dniCliente, sizeof(reg.dniCliente), dniCliente.c_str(), _TRUNCATE);

        convertirVentasAntiguas();

        std::ofstream archivo("ventas.dat", std::ios::binary | std::ios::app);
        if (archivo) {
            archivo.write((char*)&reg, sizeof(RegistroVenta));
            archivo.close();
        }

        Venta nueva(producto->id, dniCliente, cliente, producto->nombre, producto->precio, 1, producto->stock, true, reg.fechaHora, reg.fechaTexto);
        registroVentas->encolar(nueva);
    }

    void cargarVentasDesdeArchivo() {
        delete registroVentas;
        registroVentas = new Cola<Venta>();

        std::ifstream archivo("ventas.dat", std::ios::binary);
        if (!archivo) return;

        archivo.seekg(0, std::ios::end);
        long tamanoArchivo = (long)archivo.tellg();
        archivo.seekg(0, std::ios::beg);

        if (tamanoArchivo % sizeof(RegistroVenta) != 0) {
            archivo.close();
            convertirVentasAntiguas();
            cargarVentasDesdeArchivo();
            return;
        }

        std::vector<Venta> ventasOrdenadas;
        RegistroVenta reg;
        while (archivo.read((char*)&reg, sizeof(RegistroVenta))) {
            Producto* p = obtenerProducto(reg.idProducto);
            int stockMostrar = reg.stockRestante;
            if (stockMostrar == 0 && p != nullptr) stockMostrar = p->stock;

            std::string dni = reg.dniCliente;
            std::string cliente = buscarNombreCliente(dni);
            std::string fechaTexto = reg.fechaTexto;
            if (fechaTexto == "") fechaTexto = obtenerFechaTexto(reg.fechaHora);
            Venta venta(reg.idProducto, dni, cliente, reg.nombreProducto, reg.monto, reg.cantidadRestada, stockMostrar, true, reg.fechaHora, fechaTexto);
            ventasOrdenadas.push_back(venta);
        }
        archivo.close();

        ordenarVentasBurbuja(ventasOrdenadas);
        for (Venta venta : ventasOrdenadas) {
            registroVentas->encolar(venta);
        }
    }

    void mostrarRegistroVentas() {
        cargarVentasDesdeArchivo();
        std::cout << "\n========== REGISTRO DE VENTAS ==========\n";

        if (registroVentas->estaVacia()) {
            std::cout << "Todavia no hay ventas registradas.\n";
            std::cout << "========================================\n";
            return;
        }

        auto imprimirVenta = [](Venta v) {
            v.mostrar();
            };

        auto stockBajoLuegoVenta = [](Venta v) -> bool {
            return v.stockRestante < 10;
            };

        NodoCola<Venta>* actual = registroVentas->getFrente();
        while (actual != nullptr) {
            imprimirVenta(actual->dato);
            if (stockBajoLuegoVenta(actual->dato)) {
                std::cout << "  >> Aviso: este producto quedo con stock bajo.\n";
            }
            actual = actual->siguiente;
        }

        std::cout << "Total de ventas registradas: " << registroVentas->getCantidad() << "\n";
        std::cout << "========================================\n";
    }
};
