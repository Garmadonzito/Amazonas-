#pragma once

#include "ListaEnlazada.h"
#include "Cola.h"
#include "Producto.h"
#include "Usuario.h"
#include "Registro.h"
#include "Venta.h"
#include "Cupon.h"
#include "Resena.h"
#include "Soporte.h"
#include "TablaHash.h" 
#include "ArbolAVL.h"  
#include "HeapSort.h"  
#include "QuickSort.h" 
#include "MergeSort.h" 
#include "Grafo.h"       // <--- INCLUSION DEL MOTOR DE RECOMENDACIONES
#include <string>
#include <iostream>
#include <vector>
#include <functional>
#include <fstream>
#include <ctime>
#include <conio.h>     

struct ClienteFrecuente {
    string dni;
    string nombre;
    string correo;
    int cantidadCompras = 0;

    bool operator<(const ClienteFrecuente& otro) const {
        return this->cantidadCompras < otro.cantidadCompras;
    }
};

struct ProductoRanking {
    Producto prod;
    float promedio = 0.0f;

    bool operator>(const ProductoRanking& otro) const {
        return this->promedio > otro.promedio;
    }
};

class Inventario {
private:
    ListaEnlazada<Producto>* listaProductos;
    Cola<Venta>* registroVentas;
    GestorCupones* cupones;
    GestorResenas* resenas;
    GestorSoporte* soporte;

    TablaHash<RegistroCliente>* tablaClientesLogin;

    Nodo<Producto>* buscarRecursivo(Nodo<Producto>* actual, int idBuscado) {
        if (actual == nullptr) return nullptr;
        if (actual->dato.id == idBuscado) return actual;
        return buscarRecursivo(actual->siguiente, idBuscado);
    }

    int contarVentasRecursivo(Nodo<Venta>* actual) {
        if (actual == nullptr) return 0;
        return 1 + contarVentasRecursivo(actual->siguiente);
    }

    int contarStockTotalRecursivo(Nodo<Producto>* actual) {
        if (actual == nullptr) return 0;
        return actual->dato.stock + contarStockTotalRecursivo(actual->siguiente);
    }

    // MEGA OPTIMIZACIÓN: Busca nombres al instante en RAM, evita leer el disco duro.
    std::string buscarNombreCliente(std::string dniBuscado) {
        RegistroCliente* clienteHash = buscarClienteHash(dniBuscado);
        if (clienteHash != nullptr) {
            return std::string(clienteHash->nombre);
        }
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

    void ordenarHistorialPorMonto(std::vector<Venta>& historial) {
        int n = (int)historial.size();
        for (int i = 1; i < n; i++) {
            Venta llave = historial[i];
            int j = i - 1;
            while (j >= 0 && historial[j].precio < llave.precio) {
                historial[j + 1] = historial[j];
                j = j - 1;
            }
            historial[j + 1] = llave;
        }
    }

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
        cupones = new GestorCupones();
        resenas = new GestorResenas();
        soporte = new GestorSoporte();

        tablaClientesLogin = new TablaHash<RegistroCliente>(150);
        cargarClientesEnTablaHash();
    }

    ~Inventario() {
        delete listaProductos;
        delete registroVentas;
        delete cupones;
        delete resenas;
        delete soporte;
        delete tablaClientesLogin;
    }

    GestorCupones* getCupones() { return cupones; }
    GestorResenas* getResenas() { return resenas; }
    GestorSoporte* getSoporte() { return soporte; }

    // METODO NUEVO PARA EL GENERADOR: Retorna el pool real de clientes de la Hash
    std::vector<RegistroCliente> obtenerListaClientes() {
        std::vector<RegistroCliente> lista;
        auto recolector = [&lista](std::string clave, RegistroCliente& cliente) {
            lista.push_back(cliente);
            };
        tablaClientesLogin->recorrerEstructura(recolector);
        return lista;
    }

    void cargarClientesEnTablaHash() {
        std::ifstream archivo("clientes.dat", std::ios::binary);
        if (!archivo) return;

        RegistroCliente reg;
        while (archivo.read((char*)&reg, sizeof(RegistroCliente))) {
            tablaClientesLogin->insertar(std::string(reg.dni), reg);
        }
        archivo.close();
    }

    RegistroCliente* buscarClienteHash(std::string dni) {
        return tablaClientesLogin->buscar(dni);
    }

    void registrarNuevoCliente(std::string nombre, std::string correo, std::string dni) {
        RegistroCliente nuevoReg;
        strncpy_s(nuevoReg.nombre, sizeof(nuevoReg.nombre), nombre.c_str(), _TRUNCATE);
        strncpy_s(nuevoReg.correo, sizeof(nuevoReg.correo), correo.c_str(), _TRUNCATE);
        strncpy_s(nuevoReg.dni, sizeof(nuevoReg.dni), dni.c_str(), _TRUNCATE);

        std::ofstream archivo("clientes.dat", std::ios::binary | std::ios::app);
        if (archivo) {
            archivo.write((char*)&nuevoReg, sizeof(RegistroCliente));
            archivo.close();
        }

        tablaClientesLogin->insertar(dni, nuevoReg);
    }

    void registrarClientesMasivo(const std::vector<RegistroCliente>& lote) {
        std::ofstream archivo("clientes.dat", std::ios::binary | std::ios::app);
        if (archivo) {
            for (const auto& reg : lote) {
                archivo.write((char*)&reg, sizeof(RegistroCliente));
                tablaClientesLogin->insertar(std::string(reg.dni), reg);
            }
            archivo.close();
        }
    }

    void registrarVentasMasivas(const std::vector<RegistroVenta>& loteReg, const std::vector<Venta>& loteMemoria) {
        std::ofstream archivo("ventas.dat", std::ios::binary | std::ios::app);
        if (archivo) {
            for (const auto& reg : loteReg) {
                archivo.write((char*)&reg, sizeof(RegistroVenta));
            }
            archivo.close();
        }
        for (const auto& venta : loteMemoria) {
            registroVentas->encolar(venta);
        }
    }

    void listarClientesRegistrados() {
        limpiarZonaVerde();
        imprimirEnPanel(4, "            \033[95m========================================================\033[0;42m");
        imprimirEnPanel(5, "            \033[95m              LISTADO DE CLIENTES REGISTRADOS           \033[0;42m");
        imprimirEnPanel(6, "            \033[95m========================================================\033[0;42m");

        std::vector<RegistroCliente> clientesTemp;
        auto recolector = [&clientesTemp](std::string clave, RegistroCliente& cliente) {
            clientesTemp.push_back(cliente);
            };
        tablaClientesLogin->recorrerEstructura(recolector);

        int fila = 12;
        if (clientesTemp.empty()) {
            imprimirEnPanel(fila, "  No hay clientes registrados en el sistema.", 91);
            return;
        }

        for (const auto& c : clientesTemp) {
            if (fila >= 35) break;
            std::string item = "  [DNI: " + std::string(c.dni) + "] " + std::string(c.nombre) + " | " + std::string(c.correo);
            imprimirEnPanel(fila++, item);
        }

        fila++;
        imprimirEnPanel(fila++, "--------------------------------------------------------");
        imprimirEnPanel(fila++, "  TOTAL DE CLIENTES EN LA TABLA HASH: " + std::to_string(clientesTemp.size()), 92);

        irA(fila + 1, PANEL_COL); cout << "\033[0;42m  \033[93m>> Presione 'C' para ordenar alfabeticamente (Merge Sort)\033[0;42m";
        irA(fila + 2, PANEL_COL); cout << "\033[0;42m  \033[93m>> Presione cualquier otra tecla para volver al menu...\033[0;42m";

        char tecla = _getch();
        if (tecla == 'c' || tecla == 'C') {
            auto comparadorAlfabetico = [](RegistroCliente a, RegistroCliente b) -> bool {
                return std::string(a.nombre) < std::string(b.nombre);
                };

            mergeSort<RegistroCliente>(clientesTemp, comparadorAlfabetico);

            limpiarZonaVerde();
            imprimirEnPanel(4, "            \033[95m========================================================\033[0;42m");
            imprimirEnPanel(5, "            \033[95m       DIRECTORIO DE CLIENTES (A-Z) - MERGE SORT        \033[0;42m");
            imprimirEnPanel(6, "            \033[95m========================================================\033[0;42m");

            int filaNueva = 12;
            for (const auto& c : clientesTemp) {
                if (filaNueva >= 38) break;
                std::string item = "  [DNI: " + std::string(c.dni) + "] " + std::string(c.nombre);
                while (item.length() < 45) item += " ";
                item += "| " + std::string(c.correo);
                imprimirEnPanel(filaNueva++, item);
            }
            filaNueva++;
            imprimirEnPanel(filaNueva++, "--------------------------------------------------------");
            imprimirEnPanel(filaNueva, "  TOTAL: " + std::to_string(clientesTemp.size()) + " clientes ordenados exitosamente.", 92);

            irA(filaNueva + 2, PANEL_COL); cout << "\033[93m>> Presione cualquier tecla para continuar...\033[0;42m";
            (void)_getch();
        }
    }


    std::vector<Venta> obtenerVentasPorCliente(std::string dni) {
        std::vector<Venta> resultado;
        Nodo<Venta>* actual = registroVentas->getFrente();
        while (actual != nullptr) {
            if (actual->dato.dniCliente == dni)
                resultado.push_back(actual->dato);
            actual = actual->siguiente;
        }
        return resultado;
    }

    void verStockGeneral() {
        if (listaProductos->getCabeza() == nullptr) {
            imprimirEnPanel(12, "Inventario vacio.");
            return;
        }

        imprimirEnPanel(4, "            \033[96m========================================================\033[0;42m");
        imprimirEnPanel(5, "            \033[96m                REPORTE DE STOCK GENERAL                \033[0;42m");
        imprimirEnPanel(6, "            \033[96m========================================================\033[0;42m");

        int fila = 12;
        Nodo<Producto>* actual = listaProductos->getCabeza();
        while (actual != nullptr && fila < 35) {
            std::string item = "Producto: " + actual->dato.nombre;
            while (item.length() < 45) item += " ";
            item += "| Stock: " + std::to_string(actual->dato.stock);
            imprimirEnPanel(fila++, item, (actual->dato.stock < 10 ? 91 : 0));
            actual = actual->siguiente;
        }

        fila++;
        int totalUnidades = contarStockTotalRecursivo(listaProductos->getCabeza());
        imprimirEnPanel(fila++, "--------------------------------------------------------");
        imprimirEnPanel(fila, "  TOTAL DE UNIDADES FISICAS EN ALMACEN: " + std::to_string(totalUnidades), 92);
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
        limpiarZonaVerde();
        imprimirEnPanel(4, "            \033[91m========== ALERTAS DE STOCK CRITICO ==========\033[0;42m");
        int fila = 12;

        auto esCritico = [limite](Producto p) -> bool { return p.stock < limite; };
        Nodo<Producto>* actual = listaProductos->getCabeza();
        bool huboAlertas = false;
        while (actual != nullptr) {
            if (esCritico(actual->dato)) {
                std::string msg = "[ID: " + std::to_string(actual->dato.id) + "] " + actual->dato.nombre + " - Quedan: " + std::to_string(actual->dato.stock);
                imprimirEnPanel(fila++, msg, 93);
                huboAlertas = true;
            }
            actual = actual->siguiente;
        }
        if (!huboAlertas) imprimirEnPanel(fila++, "Todo el inventario cuenta con stock suficiente.", 92);
    }

    void listarTodo() {
        if (listaProductos->getCabeza() == nullptr) {
            imprimirEnPanel(12, "Inventario vacio.");
            return;
        }

        imprimirEnPanel(4, "            \033[96m========================================================\033[0;42m");
        imprimirEnPanel(5, "            \033[96m                  CATALOGO AMAZONAS                     \033[0;42m");
        imprimirEnPanel(6, "            \033[96m========================================================\033[0;42m");

        int fila = 12;
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
            while (actual != nullptr && fila < 38) {
                if (actual->dato.categoria == cat) {
                    float promedio = resenas->obtenerPromedioProducto(actual->dato.id);
                    std::string estrellas = (promedio > 0) ? "[" + std::to_string(promedio).substr(0, 3) + " *]" : "[Sin resenas]";

                    std::string nom = actual->dato.nombre;
                    if (nom.length() > 28) nom = nom.substr(0, 28);
                    while (nom.length() < 28) nom += " ";
                    std::string precioTxt = std::to_string((int)actual->dato.precio);
                    while (precioTxt.length() < 6) precioTxt = " " + precioTxt;

                    std::string item = "  [" + std::to_string(actual->dato.id) + "] " + nom + " S/." + precioTxt + "  " + estrellas;
                    imprimirEnPanel(fila++, item);
                }
                actual = actual->siguiente;
            }
            fila++;
        }
    }

    void ordenarPorPrecio() {
        auto comparador = [](Producto a, Producto b) -> bool {
            return a.precio > b.precio;
            };
        listaProductos->ordenar(comparador);
    }

    void asistentePresupuesto(float presupuesto) {
        ArbolAVL<float> arbolPrecios;
        Nodo<Producto>* actual = listaProductos->getCabeza();
        while (actual != nullptr) {
            arbolPrecios.insertar(actual->dato.precio);
            actual = actual->siguiente;
        }

        imprimirEnPanel(4, "            \033[96m========================================================\033[0;42m");
        imprimirEnPanel(5, "            \033[96m          ASISTENTE DE PRESUPUESTO (ARBOL AVL)          \033[0;42m");
        imprimirEnPanel(6, "            \033[96m========================================================\033[0;42m");

        if (arbolPrecios.estaVacio()) {
            imprimirEnPanel(12, "  Inventario vacio.", 91);
            return;
        }

        imprimirEnPanel(10, "  Presupuesto ingresado: S/. " + std::to_string((int)presupuesto));
        imprimirEnPanel(11, "  Altura del arbol: " + std::to_string(arbolPrecios.getAltura()) +
            "   |   Precios distintos: " + std::to_string(arbolPrecios.getCantidad()));

        float pisoPrecio;
        if (arbolPrecios.buscarPiso(presupuesto, pisoPrecio)) {
            actual = listaProductos->getCabeza();
            while (actual != nullptr) {
                if (actual->dato.precio == pisoPrecio) break;
                actual = actual->siguiente;
            }
            if (actual != nullptr) {
                imprimirEnPanel(13, "  >> Lo maximo que te alcanza:", 92);
                imprimirEnPanel(14, "     [ID: " + std::to_string(actual->dato.id) + "] " + actual->dato.nombre +
                    " | S/. " + std::to_string((int)actual->dato.precio), 92);
            }
        }
        else {
            imprimirEnPanel(13, "  >> Ningun producto entra en tu presupuesto.", 91);
        }

        float masBarato, masCaro;
        arbolPrecios.getMinimo(masBarato);
        arbolPrecios.getMaximo(masCaro);
        imprimirEnPanel(16, "  Mas barato del catalogo: S/. " + std::to_string((int)masBarato));
        imprimirEnPanel(17, "  Mas caro del catalogo:   S/. " + std::to_string((int)masCaro));
    }

    void ordenarConQuickSort() {
        std::vector<Producto> copia;
        Nodo<Producto>* actual = listaProductos->getCabeza();
        while (actual != nullptr) {
            copia.push_back(actual->dato);
            actual = actual->siguiente;
        }

        quickSort<Producto>(copia, [](Producto a, Producto b) { return a.precio < b.precio; });

        imprimirEnPanel(4, "            \033[96m========================================================\033[0;42m");
        imprimirEnPanel(5, "            \033[96m       PRODUCTOS ORDENADOS POR PRECIO (QUICK SORT)      \033[0;42m");
        imprimirEnPanel(6, "            \033[96m========================================================\033[0;42m");

        if (copia.empty()) {
            imprimirEnPanel(12, "  Inventario vacio.", 91);
            return;
        }

        int fila = 12;
        for (const Producto& p : copia) {
            if (fila >= 38) break;
            std::string nom = p.nombre;
            if (nom.length() > 28) nom = nom.substr(0, 28);
            while (nom.length() < 28) nom += " ";
            std::string precioTxt = std::to_string((int)p.precio);
            while (precioTxt.length() < 6) precioTxt = " " + precioTxt;

            std::string item = "  [" + std::to_string(p.id) + "] " + nom + " S/." + precioTxt;
            imprimirEnPanel(fila++, item);
        }
    }

    void filtrarPorRangoPrecio(float minPrecio, float maxPrecio) {
        auto dentroDelRango = [minPrecio, maxPrecio](Producto p) -> bool {
            return p.precio >= minPrecio && p.precio <= maxPrecio;
            };

        imprimirEnPanel(4, "            \033[96m========================================================\033[0;42m");
        imprimirEnPanel(5, "            \033[96m          PRODUCTOS FILTRADOS POR RANGO DE PRECIO       \033[0;42m");
        imprimirEnPanel(6, "            \033[96m========================================================\033[0;42m");

        int fila = 12;
        Nodo<Producto>* actual = listaProductos->getCabeza();
        bool encontrado = false;
        while (actual != nullptr) {
            if (dentroDelRango(actual->dato)) {
                std::string item = "  [ID: " + std::to_string(actual->dato.id) + "] " +
                    actual->dato.nombre + " | S/. " + std::to_string((int)actual->dato.precio);
                imprimirEnPanel(fila++, item);
                encontrado = true;
            }
            actual = actual->siguiente;
        }
        if (!encontrado) imprimirEnPanel(fila, "  No hay productos en ese rango.", 91);
    }

    void calcularTotalInventario() {
        auto sumarValor = [](float acumulado, Producto p) -> float {
            return acumulado + (p.precio * p.stock);
            };
        float total = 0;
        Nodo<Producto>* actual = listaProductos->getCabeza();
        while (actual != nullptr) {
            total = sumarValor(total, actual->dato);
            actual = actual->siguiente;
        }

        imprimirEnPanel(4, "            \033[96m========================================================\033[0;42m");
        imprimirEnPanel(5, "            \033[96m            VALOR TOTAL DEL INVENTARIO                  \033[0;42m");
        imprimirEnPanel(6, "            \033[96m========================================================\033[0;42m");

        imprimirEnPanel(12, "  Valor total en stock: S/. " + std::to_string((int)total), 92);
    }

    void buscarPorNombre(std::string nom) {
        Nodo<Producto>* actual = listaProductos->getCabeza();
        imprimirEnPanel(4, "            \033[96m========================================================\033[0;42m");
        imprimirEnPanel(5, "            \033[96m                 --- RESULTADOS ---                     \033[0;42m");
        imprimirEnPanel(6, "            \033[96m========================================================\033[0;42m");

        int fila = 12;
        bool encontrado = false;

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
        limpiarZonaVerde();
        cargarVentasDesdeArchivo();

        imprimirEnPanel(4, "    \033[96m=======================================================================\033[0;42m");
        imprimirEnPanel(5, "    \033[96m                 HISTORIAL DE VENTAS (LOGISTICA)                       \033[0;42m");
        imprimirEnPanel(6, "    \033[96m=======================================================================\033[0;42m");

        int fila = 12;
        if (registroVentas->estaVacia()) {
            imprimirEnPanel(fila, "  No hay ventas registradas in el sistema.", 91);
            return;
        }

        imprimirEnPanel(fila++, " FECHA/HORA       | CLIENTE (DNI)     | PRODUCTO             | S/. ", 93);
        imprimirEnPanel(fila++, "-----------------------------------------------------------------------");

        Nodo<Venta>* actual = registroVentas->getFrente();
        int totalVentas = contarVentasRecursivo(registroVentas->getFrente());

        int omitir = totalVentas > 20 ? totalVentas - 20 : 0;
        for (int i = 0; i < omitir; i++) {
            actual = actual->siguiente;
        }

        while (actual != nullptr && fila < 38) {
            Venta v = actual->dato;

            std::string fecha = v.fechaTexto;
            while (fecha.length() < 16) fecha += " ";

            std::string clienteDNI = v.cliente + " (" + v.dniCliente + ")";
            if (clienteDNI.length() > 17) clienteDNI = clienteDNI.substr(0, 17);
            while (clienteDNI.length() < 17) clienteDNI += " ";

            std::string prod = v.producto;
            if (prod.length() > 20) prod = prod.substr(0, 20);
            while (prod.length() < 20) prod += " ";

            std::string precioStr = std::to_string((int)v.precio);
            std::string lineaVenta = " " + fecha + " | " + clienteDNI + " | " + prod + " | " + precioStr;

            imprimirEnPanel(fila++, lineaVenta);
            actual = actual->siguiente;
        }

        fila++;
        imprimirEnPanel(fila++, "-----------------------------------------------------------------------");
        imprimirEnPanel(fila, "  TOTAL DE TRANSACCIONES HISTORICAS: " + std::to_string(totalVentas), 92);
    }

    void mostrarHistorialClientePersonalizado(std::string dni) {
        std::vector<Venta> misVentas = obtenerVentasPorCliente(dni);

        if (misVentas.empty()) {
            imprimirEnPanel(12, "Aun no has realizado compras.");
            return;
        }

        ordenarHistorialPorMonto(misVentas);

        limpiarZonaVerde();
        imprimirEnPanel(4, "            \033[93m========================================================\033[0;42m");
        imprimirEnPanel(5, "            \033[93m        MIS COMPRAS (ORDENADAS POR MONTO)               \033[0;42m");
        imprimirEnPanel(6, "            \033[93m========================================================\033[0;42m");

        int fila = 12;
        for (const auto& v : misVentas) {
            if (fila >= 38) break;
            std::string linea = "Fecha: " + v.fechaTexto + " | " + v.producto + " | S/. " + std::to_string((int)v.precio);
            imprimirEnPanel(fila++, linea);
        }
    }
    void mostrarTopClientesAVL() {
        limpiarZonaVerde();

        irA(4, PANEL_COL); cout << "            \033[95m========================================================\033[0;42m";
        irA(5, PANEL_COL); cout << "            \033[95m          TOP CLIENTES FRECUENTES (ARBOL AVL)           \033[0;42m";
        irA(6, PANEL_COL); cout << "            \033[95m========================================================\033[0;42m";

        if (registroVentas == nullptr || registroVentas->estaVacia()) {
            irA(12, PANEL_COL); cout << "  \033[91m>> No hay ventas registradas en el sistema.\033[0;42m";
            return;
        }

        vector<ClienteFrecuente> listaTemporal;
        auto* actualVenta = registroVentas->getFrente();

        while (actualVenta != nullptr) {
            string dniActual = actualVenta->dato.dniCliente;
            string nombreActual = actualVenta->dato.cliente;
            int cantidadActual = actualVenta->dato.cantidadRestada;

            bool encontrado = false;

            for (size_t i = 0; i < listaTemporal.size(); i++) {
                if (listaTemporal[i].dni == dniActual) {
                    listaTemporal[i].cantidadCompras += cantidadActual;
                    encontrado = true;
                    break;
                }
            }

            if (!encontrado) {
                ClienteFrecuente nuevoCliente;
                nuevoCliente.dni = dniActual;
                nuevoCliente.nombre = nombreActual;
                nuevoCliente.cantidadCompras = cantidadActual;

                auto* datosHash = buscarClienteHash(dniActual);
                if (datosHash != nullptr) {
                    nuevoCliente.correo = datosHash->correo;
                }
                else {
                    nuevoCliente.correo = "Sin correo";
                }

                listaTemporal.push_back(nuevoCliente);
            }
            actualVenta = actualVenta->siguiente;
        }

        ArbolAVL<ClienteFrecuente> arbolReporte;

        for (size_t i = 0; i < listaTemporal.size(); i++) {
            arbolReporte.insertar(listaTemporal[i]);
        }

        int filaDibujo = 12;

        arbolReporte.recorrerInordenInverso([&filaDibujo](ClienteFrecuente cf) {
            if (filaDibujo < 36) {
                irA(filaDibujo, PANEL_COL);
                cout << "  \033[92m[" << cf.cantidadCompras << " unds]\033[0;42m "
                    << cf.nombre << " | Correo: " << cf.correo;
                filaDibujo += 2;
            }
            });

        irA(filaDibujo + 2, PANEL_COL);
        cout << "\033[96m>> Reporte generado exitosamente mediante Arbol AVL balanceado.\033[0;42m";
    }

    void mostrarInvResenasHeap() {
        if (listaProductos->getCabeza() == nullptr) {
            imprimirEnPanel(12, "Inventario vacio. No hay productos para calificar.");
            return;
        }

        limpiarZonaVerde();
        imprimirEnPanel(4, "            \033[96m========================================================\033[0;42m");
        imprimirEnPanel(5, "            \033[96m         TOP PRODUCTOS POR REPUTACION (HEAP SORT)       \033[0;42m");
        imprimirEnPanel(6, "            \033[96m========================================================\033[0;42m");

        vector<ProductoRanking> copiaRanking;
        Nodo<Producto>* actual = listaProductos->getCabeza();

        while (actual != nullptr) {
            ProductoRanking pr;
            pr.prod = actual->dato;
            pr.promedio = resenas->obtenerPromedioProducto(actual->dato.id);

            copiaRanking.push_back(pr);
            actual = actual->siguiente;
        }

        int n = (int)copiaRanking.size();

        heapsort(copiaRanking.data(), (int)copiaRanking.size());

        int fila = 12;
        for (int i = n - 1; i >= 0; i--) {
            if (fila >= 38) break;

            float prom = copiaRanking[i].promedio;

            string estrellas = (prom > 0) ? " [" + std::to_string(prom).substr(0, 3) + " \033[93m*\033[0;42m]" : " [Sin resenas]";

            string item = "  [ID: " + std::to_string(copiaRanking[i].prod.id) + "] " +
                copiaRanking[i].prod.nombre + estrellas +
                " | S/. " + std::to_string((int)copiaRanking[i].prod.precio);

            imprimirEnPanel(fila++, item);
        }

        fila++;
        imprimirEnPanel(fila, "\033[92m>> Catalogo de reputacion ordenado mediante Max-Heap de manera exitosa.\033[0;42m");
    }

    // =========================================================
    // NUEVA FUNCIONALIDAD: RECOMENDACION INTELIGENTE CON GRAFO
    // =========================================================
    Producto* obtenerRecomendacionDelGrafo(int& maxVentas) {
        // Creamos el grafo (T = int para guardar el ID del producto)
        CGrafo<int>* grafo = new CGrafo<int>();

        // 1. Añadimos el Vértice 0: El "Nodo Central" (El Motor de la Tienda)
        grafo->adicionarVertice(0);

        vector<int> idProductos;
        vector<int> ventasProductos;

        // 2. Extraemos todos los productos como Vértices
        Nodo<Producto>* actual = listaProductos->getCabeza();
        while (actual != nullptr) {
            idProductos.push_back(actual->dato.id);
            ventasProductos.push_back(0); // Iniciamos contador en 0

            // Añadir el producto como vértice (los índices serán 1, 2, 3...)
            grafo->adicionarVertice(actual->dato.id);
            actual = actual->siguiente;
        }

        if (idProductos.empty()) {
            delete grafo;
            return nullptr; // No hay catálogo para recomendar
        }

        // 3. Recorremos el historial binario de ventas para pesar las conexiones
        ifstream archivo("ventas.dat", ios::binary);
        if (archivo.is_open()) {
            RegistroVenta reg;
            while (archivo.read((char*)&reg, sizeof(RegistroVenta))) {
                for (size_t j = 0; j < idProductos.size(); j++) {
                    if (reg.idProducto == idProductos[j]) {
                        ventasProductos[j]++; // Sumamos 1 venta como peso
                        break;
                    }
                }
            }
            archivo.close();
        }

        // 4. Creamos los ARCOS desde el Nodo Central hacia cada Producto
        for (size_t j = 0; j < idProductos.size(); j++) {
            grafo->adicionarArco(0, j + 1);
            // El peso del arco es la cantidad de ventas históricas
            grafo->modificarArco(0, j, ventasProductos[j]);
        }

        // 5. Analizamos los Arcos del Nodo Central para buscar el de mayor peso
        int maxV = -1;
        int mejorIndiceVertice = -1;

        for (int j = 0; j < grafo->cantidadArcos(0); j++) {
            int ventas = grafo->obtenerArco(0, j);
            if (ventas > maxV) {
                maxV = ventas;
                mejorIndiceVertice = grafo->obtenerVerticeLlegada(0, j);
            }
        }

        Producto* recomendado = nullptr;
        // Si el mejor producto tiene al menos 1 venta, lo extraemos
        if (mejorIndiceVertice != -1 && maxV > 0) {
            int idRecomendado = grafo->obtenerVertice(mejorIndiceVertice);
            recomendado = obtenerProducto(idRecomendado);
            maxVentas = maxV;
        }

        delete grafo; // Liberamos memoria de la RAM
        return recomendado;
    }
};