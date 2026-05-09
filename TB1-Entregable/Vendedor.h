#pragma once
#include "Usuario.h"
#include "Inventario.h"
#include <iostream>
#include <string>
#include <conio.h>

using namespace std;

class Vendedor : public Usuario {
private:
    const string CODIGO_ACCESO = "UPC2026";

    void linea(int fila, const string& texto) {
        irA(fila, PANEL_COL); cout << "\033[0m" << texto;
    }

public:
    bool login() {
        limpiarPantalla();

        irA(15, PANEL_COL); cout << "========================================================";
        irA(17, PANEL_COL); cout << "                ACCESO DE ADMINISTRADOR                 ";
        irA(19, PANEL_COL); cout << "========================================================";

        irA(23, PANEL_COL); cout << "Ingrese codigo de seguridad: ";
        string pass; cin >> pass;

        if (pass == CODIGO_ACCESO) {
            irA(26, PANEL_COL); cout << "\033[92m>> Acceso concedido. Cargando sistema...\033[0m";
            pausa();
            return true;
        }

        irA(26, PANEL_COL); cout << "\033[91m>> Codigo incorrecto. Acceso denegado.\033[0m";
        pausa();
        return false;
    }

    void gestionarProductos(Inventario& inv) {
        limpiarZonaVerde();
        int id, stock;
        string nombre, categoria;
        float precio;

        linea(12, "--- REGISTRO DE NUEVO PRODUCTO ---");
        irA(15, PANEL_COL); cout << "ID: "; cin >> id;
        if (inv.existeProducto(id)) {
            linea(17, "\033[91m>> Error: El ID ya existe en el sistema.\033[0m");
            return;
        }
        irA(17, PANEL_COL); cout << "Nombre: "; cin.ignore(); getline(cin, nombre);
        irA(19, PANEL_COL); cout << "Categoria: "; getline(cin, categoria);
        irA(21, PANEL_COL); cout << "Precio: S/. "; cin >> precio;
        irA(23, PANEL_COL); cout << "Stock Inicial: "; cin >> stock;

        inv.agregarProducto(id, nombre, categoria, precio, stock);
        linea(26, "\033[92m>> Producto guardado correctamente en la lista enlazada.\033[0m");
    }

    void editarProducto(Inventario& inv) {
        limpiarZonaVerde();
        int id;
        linea(12, "--- MODIFICACION DE STOCK Y PRECIO ---");
        irA(15, PANEL_COL); cout << "ID del producto a editar: "; cin >> id;

        if (!inv.existeProducto(id)) {
            linea(17, "\033[91m>> Producto no encontrado.\033[0m");
            return;
        }

        float nPrecio;
        int nStock;
        irA(18, PANEL_COL); cout << "Nuevo Precio: S/. "; cin >> nPrecio;
        irA(20, PANEL_COL); cout << "Nuevo Stock Unidades: "; cin >> nStock;
        inv.modificarProducto(id, nPrecio, nStock);
        linea(23, "\033[92m>> Datos actualizados en el archivo binario.\033[0m");
    }

    void eliminarProducto(Inventario& inv) {
        limpiarZonaVerde();
        int id;
        linea(12, "--- ELIMINAR PRODUCTO DEL SISTEMA ---");
        irA(15, PANEL_COL); cout << "ID del producto a eliminar: "; cin >> id;

        if (inv.eliminarProducto(id)) {
            linea(18, "\033[92m>> Nodo eliminado y memoria liberada correctamente.\033[0m");
        }
        else {
            linea(18, "\033[91m>> No se pudo eliminar: ID inexistente.\033[0m");
        }
    }

    void menu(Inventario& inv) {
        int op;
        do {
            limpiarZonaVerde();
            linea(10, "========================================================");
            linea(11, "               PANEL DE CONTROL - LOGISTICA             ");
            linea(12, "========================================================");

            linea(15, "    1. Listar Catalogo Completo");
            linea(17, "    2. Registrar Nuevo Producto");
            linea(19, "    3. Editar Precio / Stock");
            linea(21, "    4. Eliminar Producto");
            linea(23, "    5. Alertas de Stock Critico (Lambdas)");
            linea(25, "    6. Ver Historial de Ventas (Ordenado Shell)");
            linea(27, "    7. Reporte Visual de Almacen");
            linea(29, "    8. Ordenar Inventario por Precio (Burbuja)");
            linea(33, "    9. Cerrar Sesion de Administrador");

            linea(36, "    Seleccione una opcion: "); cin >> op;

            switch (op) {
            case 1: limpiarZonaVerde(); inv.listarTodo(); break;
            case 2: gestionarProductos(inv); break;
            case 3: editarProducto(inv); break;
            case 4: eliminarProducto(inv); break;
            case 5: limpiarZonaVerde(); inv.mostrarStockBajo(10); break;
            case 6: limpiarZonaVerde(); inv.mostrarRegistroVentas(); break;
            case 7: limpiarZonaVerde(); inv.verStockGeneral(); break;
            case 8: limpiarZonaVerde(); inv.ordenarPorPrecio(); inv.listarTodo(); break;
            }

            if (op != 9) pausa();

        } while (op != 9);
    }
};
