#pragma once
#include "Usuario.h"
#include "Inventario.h"
#include "GestorEscenas.h"
#include "GeneradorDatos.h" // Generador de Data Aleatoria Masiva

using namespace std;

class Vendedor : public Usuario {
private:
    const string CODIGO_ACCESO = "UPC2026";

public:
    bool login() {
        gestorEscenas grafica;
        grafica.setEscena(gestorEscenas::LOGIN);

        limpiarPantalla();
        grafica.dibujarEscena();

        irA(4, PANEL_COL); cout << "\033[0m\033[96m========================================================\033[0m";
        irA(5, PANEL_COL); cout << "\033[0m\033[96m                ACCESO DE ADMINISTRADOR                 \033[0m";
        irA(6, PANEL_COL); cout << "\033[0m\033[96m========================================================\033[0m";

        irA(TEXT_FILA, TEXT_COL); cout << "\033[0mIngrese codigo de seguridad: ";
        string pass; cin >> pass;

        if (pass == CODIGO_ACCESO) {
            irA(22, TEXT_COL); cout << "\033[0m\033[92m>> Acceso concedido. Cargando sistema...\033[0m";
            pausa();
            return true;
        }

        irA(22, TEXT_COL); cout << "\033[0m\033[91m>> Codigo incorrecto. Acceso denegado.\033[0m";
        pausa();
        return false;
    }

    void gestionarProductos(Inventario& inv) {
        limpiarZonaVerde();
        int id, stock;
        string nombre, categoria;
        float precio;

        irA(4, PANEL_COL); cout << "\033[0m\033[96m========================================================\033[0m";
        irA(5, PANEL_COL); cout << "\033[0m\033[96m           --- REGISTRO DE NUEVO PRODUCTO ---           \033[0m";
        irA(6, PANEL_COL); cout << "\033[0m\033[96m========================================================\033[0m";

        irA(12, PANEL_COL); cout << "\033[0mID: "; cin >> id;
        if (inv.existeProducto(id)) {
            irA(14, PANEL_COL); cout << "\033[0m\033[91m>> Error: El ID ya existe en el sistema.\033[0m";
            return;
        }
        irA(14, PANEL_COL); cout << "\033[0mNombre: "; cin.ignore(); getline(cin, nombre);
        irA(16, PANEL_COL); cout << "\033[0mCategoria: "; getline(cin, categoria);
        irA(18, PANEL_COL); cout << "\033[0mPrecio: S/. "; cin >> precio;
        irA(20, PANEL_COL); cout << "\033[0mStock Inicial: "; cin >> stock;

        inv.agregarProducto(id, nombre, categoria, precio, stock);
        irA(24, PANEL_COL); cout << "\033[0m\033[92m>> Producto guardado correctamente en la lista enlazada.\033[0m";
    }

    void editarProducto(Inventario& inv) {
        limpiarZonaVerde();
        int id;
        irA(4, PANEL_COL); cout << "\033[0m            \033[96m========================================================\033[0m";
        irA(5, PANEL_COL); cout << "\033[0m            \033[96m         --- MODIFICACION DE STOCK Y PRECIO ---         \033[0m";
        irA(6, PANEL_COL); cout << "\033[0m            \033[96m========================================================\033[0m";

        irA(12, PANEL_COL); cout << "\033[0mID del producto a editar: "; cin >> id;

        if (!inv.existeProducto(id)) {
            irA(14, PANEL_COL); cout << "\033[0m\033[91m>> Producto no encontrado.\033[0m";
            return;
        }

        float nPrecio;
        int nStock;
        irA(14, PANEL_COL); cout << "\033[0mNuevo Precio: S/. "; cin >> nPrecio;
        irA(16, PANEL_COL); cout << "\033[0mNuevo Stock Unidades: "; cin >> nStock;
        inv.modificarProducto(id, nPrecio, nStock);
        irA(20, PANEL_COL); cout << "\033[0m\033[92m>> Datos actualizados en el archivo binario.\033[0m";
    }

    void eliminarProducto(Inventario& inv) {
        limpiarZonaVerde();
        int id;
        irA(4, PANEL_COL); cout << "\033[0m            \033[96m========================================================\033[0m";
        irA(5, PANEL_COL); cout << "\033[0m            \033[96m          --- ELIMINAR PRODUCTO DEL SISTEMA ---         \033[0m";
        irA(6, PANEL_COL); cout << "\033[0m            \033[96m========================================================\033[0m";

        irA(12, PANEL_COL); cout << "\033[0mID del producto a eliminar: "; cin >> id;

        if (inv.eliminarProducto(id)) {
            irA(15, PANEL_COL); cout << "\033[0m\033[92m>> Nodo eliminado y memoria liberada correctamente.\033[0m";
        }
        else {
            irA(15, PANEL_COL); cout << "\033[0m\033[91m>> No se pudo eliminar: ID inexistente.\033[0m";
        }
    }

    void menu(Inventario& inv) {
        int op;
        gestorEscenas grafica; // ¡Correccion E0020 aplicada!
        grafica.setEscena(gestorEscenas::CATALOGO);

        do {
            limpiarZonaVerde();
            grafica.dibujarFondoSinLogo();

            irA(4, PANEL_COL); cout << "\033[0m            \033[96m========================================================\033[0m";
            irA(5, PANEL_COL); cout << "\033[0m            \033[96m               PANEL DE CONTROL - LOGISTICA             \033[0m";
            irA(6, PANEL_COL); cout << "\033[0m            \033[96m========================================================\033[0m";

            irA(12, TEXT_COL); cout << "\033[0m    1. Listar Catalogo         8. Ordenar por Precio";
            irA(14, TEXT_COL); cout << "\033[0m    2. Registrar Producto      9. Filtrar por Rango";
            irA(16, TEXT_COL); cout << "\033[0m    3. Editar Precio/Stock    10. Valor Total Inventario";
            irA(18, TEXT_COL); cout << "\033[0m    4. Eliminar Producto      11. Gestionar Cupones";
            irA(20, TEXT_COL); cout << "\033[0m    5. Alertas de Stock       12. Soporte al Cliente";
            irA(22, TEXT_COL); cout << "\033[0m    6. Historial de Ventas    13. Ver Resenas";
            irA(24, TEXT_COL); cout << "\033[0m    7. Reporte de Almacen     14. Ver Clientes (Tabla Hash)";
            irA(26, TEXT_COL); cout << "\033[0m   16. Buscar por ID (Arbol)  17. Ordenar por Precio (Quick Sort)";
            irA(28, TEXT_COL); cout << "\033[0m   18. Mostrar Top Clientes   19. Generar Dataset (100 Datos)";
            irA(30, TEXT_COL); cout << "\033[0m   \033[93m15. Cerrar Sesion de Administrador\033[0m";
            irA(33, TEXT_COL); cout << "\033[0m    Seleccione una opcion: "; cin >> op;

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
                irA(12, PANEL_COL); cout << "\033[0m  Precio minimo: "; irA(12, PANEL_COL + 18); cin >> minP;
                irA(14, PANEL_COL); cout << "\033[0m  Precio maximo: "; irA(14, PANEL_COL + 18); cin >> maxP;
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
                irA(30, PANEL_COL); cout << "\033[0m  Agregar cupon? (S/N): ";
                char r; cin >> r;
                if (r == 'S' || r == 's') {
                    string cod; float desc;
                    irA(32, PANEL_COL); cout << "\033[0m  Codigo: "; irA(32, PANEL_COL + 10); cin >> cod;
                    irA(33, PANEL_COL); cout << "\033[0m  Descuento %: "; irA(33, PANEL_COL + 14); cin >> desc;
                    inv.getCupones()->agregarCupon(cod, desc);
                }
                break;
            }
            case 12: {
                limpiarZonaVerde();
                grafica.dibujarFondoSinLogo();
                inv.getSoporte()->listarTodos();
                irA(38, PANEL_COL); cout << "\033[0m  Avanzar estado de ticket? Ingrese ID (0 para no): ";
                int idT; irA(38, PANEL_COL + 50); cin >> idT;

                if (idT > 0) inv.getSoporte()->avanzarEstado(idT);
                break;
            }
            case 13: limpiarZonaVerde(); grafica.dibujarFondoSinLogo(); inv.getResenas()->listarResenas(); break;
            case 14: limpiarZonaVerde(); grafica.dibujarFondoSinLogo(); inv.listarClientesRegistrados(); break;
            case 16: {
                limpiarZonaVerde();
                grafica.dibujarFondoSinLogo();
                int idBuscar;
                irA(10, PANEL_COL); cout << "\033[0m  ID de producto a buscar: "; irA(10, PANEL_COL + 27); cin >> idBuscar;
                limpiarZonaVerde();
                grafica.dibujarFondoSinLogo();
                inv.buscarConArbolAVL(idBuscar);
                break;
            }
            case 17: limpiarZonaVerde(); grafica.dibujarFondoSinLogo(); inv.ordenarConQuickSort(); break;
            case 18: {
                limpiarZonaVerde();
                grafica.dibujarFondoSinLogo();
                inv.mostrarTopClientesAVL();
                break;
            }
            case 19: { // OPCION UNIFICADA PARA EL DATASET
                limpiarZonaVerde();
                grafica.dibujarFondoSinLogo();
                int cant = 100;

                irA(12, PANEL_COL); cout << "\033[0m  \033[93m>> Generando " << cant << " clientes aleatorios...\033[0m";
                GeneradorDatos::generarClientesAleatorios(cant, inv);

                irA(14, PANEL_COL); cout << "\033[0m  \033[93m>> Simulando " << cant << " ventas masivas...\033[0m";
                GeneradorDatos::generarVentasAleatorias(cant, inv);

                irA(16, PANEL_COL); cout << "\033[0m  \033[92m>> Dataset creado e insertado en O(1).\033[0m";
                irA(18, PANEL_COL); cout << "\033[0m  \033[96m>> Tip: Usa la opcion 14 o 18 para probar la velocidad.\033[0m";
                break;
            }
            }

            if (op != 15) {
                pausa();
            }

        } while (op != 15);
    }
};