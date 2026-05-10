#pragma once
#include "Usuario.h"
#include "Inventario.h"
#include "GestorEscenas.h"
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
        gestorEscenas grafica;
        grafica.setEscena(gestorEscenas::CATALOGO);

        limpiarPantalla();
        grafica.dibujarFondoSinLogo();

        irA(4, PANEL_COL); cout << "            \033[96m========================================================\033[0m";
        irA(5, PANEL_COL); cout << "            \033[96m                ACCESO DE ADMINISTRADOR                 \033[0m";
        irA(6, PANEL_COL); cout << "            \033[96m========================================================\033[0m";

        irA(14, PANEL_COL); cout << "Ingrese codigo de seguridad: ";
        string pass; cin >> pass;

        if (pass == CODIGO_ACCESO) {
            irA(17, PANEL_COL); cout << "\033[92m>> Acceso concedido. Cargando sistema...\033[0m";
            pausa();
            return true;
        }

        irA(17, PANEL_COL); cout << "\033[91m>> Codigo incorrecto. Acceso denegado.\033[0m";
        pausa();
        return false;
    }

    void gestionarProductos(Inventario& inv) {
        limpiarZonaVerde();
        int id, stock;
        string nombre, categoria;
        float precio;

        linea(4, "            \033[96m========================================================\033[0m");
        linea(5, "            \033[96m           --- REGISTRO DE NUEVO PRODUCTO ---           \033[0m");
        linea(6, "            \033[96m========================================================\033[0m");

        irA(12, PANEL_COL); cout << "ID: "; cin >> id;
        if (inv.existeProducto(id)) {
            linea(14, "\033[91m>> Error: El ID ya existe en el sistema.\033[0m");
            return;
        }
        irA(14, PANEL_COL); cout << "Nombre: "; cin.ignore(); getline(cin, nombre);
        irA(16, PANEL_COL); cout << "Categoria: "; getline(cin, categoria);
        irA(18, PANEL_COL); cout << "Precio: S/. "; cin >> precio;
        irA(20, PANEL_COL); cout << "Stock Inicial: "; cin >> stock;

        inv.agregarProducto(id, nombre, categoria, precio, stock);
        linea(24, "\033[92m>> Producto guardado correctamente en la lista enlazada.\033[0m");
    }

    void editarProducto(Inventario& inv) {
        limpiarZonaVerde();
        int id;
        linea(4, "            \033[96m========================================================\033[0m");
        linea(5, "            \033[96m         --- MODIFICACION DE STOCK Y PRECIO ---         \033[0m");
        linea(6, "            \033[96m========================================================\033[0m");

        irA(12, PANEL_COL); cout << "ID del producto a editar: "; cin >> id;

        if (!inv.existeProducto(id)) {
            linea(14, "\033[91m>> Producto no encontrado.\033[0m");
            return;
        }

        float nPrecio;
        int nStock;
        irA(14, PANEL_COL); cout << "Nuevo Precio: S/. "; cin >> nPrecio;
        irA(16, PANEL_COL); cout << "Nuevo Stock Unidades: "; cin >> nStock;
        inv.modificarProducto(id, nPrecio, nStock);
        linea(20, "\033[92m>> Datos actualizados en el archivo binario.\033[0m");
    }

    void eliminarProducto(Inventario& inv) {
        limpiarZonaVerde();
        int id;
        linea(4, "            \033[96m========================================================\033[0m");
        linea(5, "            \033[96m          --- ELIMINAR PRODUCTO DEL SISTEMA ---         \033[0m");
        linea(6, "            \033[96m========================================================\033[0m");

        irA(12, PANEL_COL); cout << "ID del producto a eliminar: "; cin >> id;

        if (inv.eliminarProducto(id)) {
            linea(15, "\033[92m>> Nodo eliminado y memoria liberada correctamente.\033[0m");
        }
        else {
            linea(15, "\033[91m>> No se pudo eliminar: ID inexistente.\033[0m");
        }
    }

    void menu(Inventario& inv) {
        int op;
        gestorEscenas grafica;
        grafica.setEscena(gestorEscenas::CATALOGO);

        do {
            limpiarZonaVerde();
            grafica.dibujarFondoSinLogo();

            linea(4, "            \033[96m========================================================\033[0m");
            linea(5, "            \033[96m               PANEL DE CONTROL - LOGISTICA             \033[0m");
            linea(6, "            \033[96m========================================================\033[0m");

            linea(12, "    1. Listar Catalogo         8. Ordenar por Precio");
            linea(14, "    2. Registrar Producto      9. Filtrar por Rango");
            linea(16, "    3. Editar Precio/Stock    10. Valor Total Inventario");
            linea(18, "    4. Eliminar Producto      11. Gestionar Cupones");
            linea(20, "    5. Alertas de Stock       12. Soporte al Cliente");
            linea(22, "    6. Historial de Ventas    13. Ver Resenas");
            linea(24, "    7. Reporte de Almacen     ");
            linea(28, "   \033[93m14. Cerrar Sesion de Administrador\033[0m");
            linea(32, "    Seleccione una opcion: "); cin >> op;

            switch (op) {
            case 1:  limpiarZonaVerde(); grafica.dibujarFondoSinLogo(); inv.listarTodo(); break;
            case 2:  grafica.dibujarFondoSinLogo(); gestionarProductos(inv); break;
            case 3:  grafica.dibujarFondoSinLogo(); editarProducto(inv); break;
            case 4:  grafica.dibujarFondoSinLogo(); eliminarProducto(inv); break;
            case 5:  limpiarZonaVerde(); grafica.dibujarFondoSinLogo(); inv.mostrarStockBajo(10); break;
            case 6:  limpiarZonaVerde(); grafica.dibujarFondoSinLogo(); inv.mostrarRegistroVentas(); break;
            case 7:  limpiarZonaVerde(); grafica.dibujarFondoSinLogo(); inv.verStockGeneral(); break;
            case 8:  limpiarZonaVerde(); grafica.dibujarFondoSinLogo(); inv.ordenarPorPrecio(); inv.listarTodo(); break;
            case 9: {
                limpiarZonaVerde();
                grafica.dibujarFondoSinLogo();
                float minP, maxP;
                linea(12, "  Precio minimo: "); irA(12, PANEL_COL + 18); cin >> minP;
                linea(14, "  Precio maximo: "); irA(14, PANEL_COL + 18); cin >> maxP;
                limpiarZonaVerde();
                grafica.dibujarFondoSinLogo();
                inv.filtrarPorRangoPrecio(minP, maxP);
                break;
            }
            case 10: limpiarZonaVerde(); grafica.dibujarFondoSinLogo(); inv.calcularTotalInventario(); break;
            case 11: {
                limpiarZonaVerde();
                grafica.dibujarFondoSinLogo();
                inv.getCupones()->listarCupones();
                linea(30, "  Agregar cupon? (S/N): ");
                char r; cin >> r;
                if (r == 'S' || r == 's') {
                    string cod; float desc;
                    linea(32, "  Codigo: "); irA(32, PANEL_COL + 10); cin >> cod;
                    linea(33, "  Descuento %: "); irA(33, PANEL_COL + 14); cin >> desc;
                    inv.getCupones()->agregarCupon(cod, desc);
                }
                break;
            }
            case 12: {
                limpiarZonaVerde();
                grafica.dibujarFondoSinLogo();
                inv.getSoporte()->listarTodos();
                linea(38, "  Avanzar estado de ticket? Ingrese ID (0 para no): ");
                int idT; irA(38, PANEL_COL + 50); cin >> idT;
                if (idT > 0) inv.getSoporte()->avanzarEstado(idT);
                break;
            }
            case 13: limpiarZonaVerde(); grafica.dibujarFondoSinLogo(); inv.getResenas()->listarResenas(); break;
            }

            if (op != 14) pausa();

        } while (op != 14);
    }
};