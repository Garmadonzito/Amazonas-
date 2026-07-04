#pragma once
#include "Usuario.h"
#include "Inventario.h"
#include "GestorEscenas.h"
#include "GeneradorDatos.h" // Generador de Data Aleatoria Masiva
#include "Consola.h"

using namespace std;

class Vendedor : public Usuario {
private:
    const string CODIGO_ACCESO = "UPC2026";

    // Pinta un rectangulo con el verde del fondo (para borrar la mascota
    // en cada paso de la animacion)
    void borrarZonaMascota(int x, int y, int ancho, int alto) {
        HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hCon, 2); // 2 = verde del fondo
        for (int i = 0; i < alto; i++) {
            gotoxy(x, y + i);
            for (int j = 0; j < ancho; j++) cout << (char)219;
        }
    }

    // Animacion: la llama entra caminando al panel de izquierda a derecha.
    // El "rebote" se logra alternando la fila entre 18 y 17 segun el paso.
    void animarLlamaCaminando(Matriz& llama) {
        for (int x = 1; x <= 13; x++) {
            int filaPaso = 18 - (x % 2); // un paso arriba, un paso abajo
            llama.dibujarMatriz(x, filaPaso);
            Sleep(70);
            borrarZonaMascota(x, 17, 1, 19); // borra la columna que deja atras
            if (x % 2 != 0) borrarZonaMascota(x, 17, 17, 1); // limpia la fila del rebote
        }
        llama.dibujarMatriz(13, 18); // posicion final
    }

public:
    bool login() {
        gestorEscenas grafica;
        grafica.setEscena(gestorEscenas::LOGIN);

        limpiarPantalla();
        grafica.dibujarEscena();

        dibujarMarco(4, PANEL_COL, 56, 3, "96");
        cout << "\033[96m";
        imprimirLento(5, PANEL_COL + 16, "ACCESO DE ADMINISTRADOR", 20);
        cout << "\033[0;1;42;97m";

        // Texto centrado DENTRO de la nube (la nube ocupa columnas 35-91, filas 16-36)
        // y con fondo blanco (47) para que se funda con ella
        irA(24, 45); cout << "\033[30;47mIngrese codigo de seguridad: ";
        string pass; cin >> pass;
        cout << "\033[0;1;42;97m";

        if (pass == CODIGO_ACCESO) {
            irA(28, 44); cout << "\033[32;47m>> Acceso concedido. Cargando sistema...\033[0;1;42;97m";
            pausa();
            return true;
        }

        irA(28, 44); cout << "\033[31;47m>> Codigo incorrecto. Acceso denegado.\033[0;1;42;97m";
        pausa();
        return false;
    }

    void gestionarProductos(Inventario& inv) {
        limpiarZonaVerde();
        int id, stock;
        string nombre, categoria;
        float precio;

        irA(4, PANEL_COL); cout << "\033[0;1;42;97m\033[96m========================================================\033[0;1;42;97m";
        irA(5, PANEL_COL); cout << "\033[0;1;42;97m\033[96m           --- REGISTRO DE NUEVO PRODUCTO ---           \033[0;1;42;97m";
        irA(6, PANEL_COL); cout << "\033[0;1;42;97m\033[96m========================================================\033[0;1;42;97m";

        irA(12, PANEL_COL); cout << "\033[0;1;42;97mID: "; cin >> id;
        if (inv.existeProducto(id)) {
            irA(14, PANEL_COL); cout << "\033[0;1;42;97m\033[91m>> Error: El ID ya existe en el sistema.\033[0;1;42;97m";
            return;
        }
        irA(14, PANEL_COL); cout << "\033[0;1;42;97mNombre: "; cin.ignore(); getline(cin, nombre);
        irA(16, PANEL_COL); cout << "\033[0;1;42;97mCategoria: "; getline(cin, categoria);
        irA(18, PANEL_COL); cout << "\033[0;1;42;97mPrecio: S/. "; cin >> precio;
        irA(20, PANEL_COL); cout << "\033[0;1;42;97mStock Inicial: "; cin >> stock;

        inv.agregarProducto(id, nombre, categoria, precio, stock);
        irA(24, PANEL_COL); cout << "\033[0;1;42;97m\033[92m>> Producto guardado correctamente en la lista enlazada.\033[0;1;42;97m";
    }

    void editarProducto(Inventario& inv) {
        limpiarZonaVerde();
        int id;
        irA(4, PANEL_COL); cout << "\033[0;1;42;97m            \033[96m========================================================\033[0;1;42;97m";
        irA(5, PANEL_COL); cout << "\033[0;1;42;97m            \033[96m         --- MODIFICACION DE STOCK Y PRECIO ---         \033[0;1;42;97m";
        irA(6, PANEL_COL); cout << "\033[0;1;42;97m            \033[96m========================================================\033[0;1;42;97m";

        irA(12, PANEL_COL); cout << "\033[0;1;42;97mID del producto a editar: "; cin >> id;

        if (!inv.existeProducto(id)) {
            irA(14, PANEL_COL); cout << "\033[0;1;42;97m\033[91m>> Producto no encontrado.\033[0;1;42;97m";
            return;
        }

        float nPrecio;
        int nStock;
        irA(14, PANEL_COL); cout << "\033[0;1;42;97mNuevo Precio: S/. "; cin >> nPrecio;
        irA(16, PANEL_COL); cout << "\033[0;1;42;97mNuevo Stock Unidades: "; cin >> nStock;
        inv.modificarProducto(id, nPrecio, nStock);
        irA(20, PANEL_COL); cout << "\033[0;1;42;97m\033[92m>> Datos actualizados en el archivo binario.\033[0;1;42;97m";
    }

    void eliminarProducto(Inventario& inv) {
        limpiarZonaVerde();
        int id;
        irA(4, PANEL_COL); cout << "\033[0;1;42;97m            \033[96m========================================================\033[0;1;42;97m";
        irA(5, PANEL_COL); cout << "\033[0;1;42;97m            \033[96m          --- ELIMINAR PRODUCTO DEL SISTEMA ---         \033[0;1;42;97m";
        irA(6, PANEL_COL); cout << "\033[0;1;42;97m            \033[96m========================================================\033[0;1;42;97m";

        irA(12, PANEL_COL); cout << "\033[0;1;42;97mID del producto a eliminar: "; cin >> id;

        if (inv.eliminarProducto(id)) {
            irA(15, PANEL_COL); cout << "\033[0;1;42;97m\033[92m>> Nodo eliminado y memoria liberada correctamente.\033[0;1;42;97m";
        }
        else {
            irA(15, PANEL_COL); cout << "\033[0;1;42;97m\033[91m>> No se pudo eliminar: ID inexistente.\033[0;1;42;97m";
        }
    }

    void menu(Inventario& inv) {
        int op;
        gestorEscenas grafica; // ¡Correccion E0020 aplicada!
        grafica.setEscena(gestorEscenas::CATALOGO);

        // Mascota decorativa del panel del vendedor
        Matriz llama;
        llama.inicializar(mascotaLlama);
        bool llamaYaEntro = false; // la animacion solo se ve al entrar al panel
        transicionCortina();       // entrada con efecto de barrido

        do {
            limpiarZonaVerde();
            grafica.setEscena(gestorEscenas::MENU_PRINCIPAL_VENDEDOR);
            grafica.dibujarEscena();

            if (!llamaYaEntro) {
                animarLlamaCaminando(llama);
                llamaYaEntro = true;
            }
            else {
                llama.dibujarMatriz(13, 18);
            }

            dibujarMarco(4, PANEL_COL, 56, 3, "96");
            irA(5, PANEL_COL + 14); cout << "\033[96mPANEL DE CONTROL - LOGISTICA\033[0;1;42;97m";

            irA(13, TEXT_COL); cout << "\033[0;1;42;97m1. Listar Catalogo          8. Ordenar por Precio";
            irA(15, TEXT_COL); cout << "\033[0;1;42;97m2. Registrar Producto       9. Filtrar por Rango";
            irA(17, TEXT_COL); cout << "\033[0;1;42;97m3. Editar Precio/Stock     10. Valor Total Inventario";
            irA(19, TEXT_COL); cout << "\033[0;1;42;97m4. Eliminar Producto       11. Gestionar Cupones";
            irA(21, TEXT_COL); cout << "\033[0;1;42;97m5. Alertas de Stock        12. Soporte al Cliente";
            irA(23, TEXT_COL); cout << "\033[0;1;42;97m6. Historial de Ventas     13. Ver Resenas";
            irA(25, TEXT_COL); cout << "\033[0;1;42;97m7. Reporte de Almacen      14. Ver Clientes (Tabla Hash)";
            irA(27, TEXT_COL); cout << "\033[0;1;42;97m17. Ordenar por Precio (Quick Sort)";
            irA(29, TEXT_COL); cout << "\033[0;1;42;97m18. Mostrar Top Clientes   19. Generar Dataset (100 Datos)";
            irA(31, TEXT_COL); cout << "\033[0;1;42;97m\033[93m15. Cerrar Sesion de Administrador\033[0;1;42;97m";
            irA(38, 52); cout << "\033[0;1;42;97mSeleccione una opcion: "; cin >> op;

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
                irA(12, PANEL_COL); cout << "\033[0;1;42;97m  Precio minimo: "; irA(12, PANEL_COL + 18); cin >> minP;
                irA(14, PANEL_COL); cout << "\033[0;1;42;97m  Precio maximo: "; irA(14, PANEL_COL + 18); cin >> maxP;
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
                irA(30, PANEL_COL); cout << "\033[0;1;42;97m  Agregar cupon? (S/N): ";
                char r; cin >> r;
                if (r == 'S' || r == 's') {
                    string cod; float desc;
                    irA(32, PANEL_COL); cout << "\033[0;1;42;97m  Codigo: "; irA(32, PANEL_COL + 10); cin >> cod;
                    irA(33, PANEL_COL); cout << "\033[0;1;42;97m  Descuento %: "; irA(33, PANEL_COL + 14); cin >> desc;
                    inv.getCupones()->agregarCupon(cod, desc);
                }
                break;
            }
            case 12: {
                limpiarZonaVerde();
                grafica.dibujarFondoSinLogo();
                inv.getSoporte()->listarTodos();
                irA(38, PANEL_COL); cout << "\033[0;1;42;97m  Avanzar estado de ticket? Ingrese ID (0 para no): ";
                int idT; irA(38, PANEL_COL + 50); cin >> idT;

                if (idT > 0) inv.getSoporte()->avanzarEstado(idT);
                break;
            }
            case 13: limpiarZonaVerde(); grafica.dibujarFondoSinLogo(); inv.getResenas()->listarResenas(); break;
            case 14: limpiarZonaVerde(); grafica.dibujarFondoSinLogo(); inv.listarClientesRegistrados(); break;
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

                irA(12, PANEL_COL); cout << "\033[0;1;42;97m  \033[93m>> Generando " << cant << " clientes aleatorios...\033[0;1;42;97m";
                GeneradorDatos::generarClientesAleatorios(cant, inv);

                irA(14, PANEL_COL); cout << "\033[0;1;42;97m  \033[93m>> Simulando " << cant << " ventas masivas...\033[0;1;42;97m";
                GeneradorDatos::generarVentasAleatorias(cant, inv);

                irA(16, PANEL_COL); cout << "\033[0;1;42;97m  \033[92m>> Dataset creado e insertado en O(1).\033[0;1;42;97m";
                irA(18, PANEL_COL); cout << "\033[0;1;42;97m  \033[96m>> Tip: Usa la opcion 14 o 18 para probar la velocidad.\033[0;1;42;97m";
                break;
            }
            }

            if (op != 15) {
                pausa();
            }

        } while (op != 15);
    }
};