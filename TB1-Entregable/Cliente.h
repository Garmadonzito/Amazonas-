#pragma once
#include "Usuario.h"
#include "Inventario.h"
#include "MetodoPago.h"
#include "Registro.h" 
#include "Pila.h" 
#include <conio.h>
#include <fstream>

extern void mostrarFondo2();

class Cliente : public Usuario {
private:
    ListaEnlazada<int>* carrito;
    Pila<int> historialNavegacion;

    const int MENU_PRINCIPAL = 1;
    const int MENU_BUSQUEDA = 2;
    const int MENU_CARRITO = 3;

    void linea(int fila, const string& texto) {
        irA(fila, PANEL_COL); cout << "\033[0m" << texto;
    }

    void pausaRetroceder(int fila = 37) {
        irA(fila, PANEL_COL);
        cout << "\033[93m>> Presione ESC para volver atras...";
        while (true) {
            if (_kbhit()) {
                int c = _getch();
                if (c == 27) break;
            }
        }
    }

public:
    Cliente() {
        carrito = new ListaEnlazada<int>();
    }

    ~Cliente() {
        delete carrito;
    }

    void login() {
        cout << "\n--- LOGIN CLIENTE ---\n";
        cout << "Nombre: "; cin.ignore(); getline(cin, nombre);
        cout << "Correo: "; getline(cin, correo);

        auto validarDNI = [](string d) -> bool {
            return d.length() == 8 && d.find_first_not_of("0123456789") == string::npos;
            };

        bool dniValido = false;
        do {
            cout << "DNI: ";
            getline(cin, dni);
            if (validarDNI(dni)) dniValido = true;
            else cout << ">> Error: DNI invalido.\n";
        } while (!dniValido);

        cout << "\n>> Bienvenido " << nombre << "!\n";
        pausa();
    }

    // Lógica reutilizable para agregar productos por ID desde cualquier pantalla
    void bucleAgregarRapido(Inventario& inv, int filaInicio) {
        irA(filaInicio, PANEL_COL);
        cout << "\033[0m>> Ingrese ID para agregar. [ESC] para dejar de añadir.";

        int filaMsg = filaInicio + 2;
        while (true) {
            irA(filaMsg, PANEL_COL);
            cout << "\033[0mID: ";
            string buf;
            bool terminar = false;

            while (true) {
                int c = _getch();
                if (c == 27) { terminar = true; break; }
                if (c == '\r') { cout << "\n"; break; }
                if (c == 8 && !buf.empty()) { buf.pop_back(); cout << "\b \b"; }
                else if (c >= '0' && c <= '9') { buf += (char)c; cout << (char)c; }
            }

            if (terminar) break;
            if (buf.empty()) continue;

            int id = stoi(buf);
            Producto* p = inv.obtenerProducto(id);
            irA(filaMsg + 1, PANEL_COL);
            if (p != nullptr && p->stock > 0) {
                carrito->agregar(id);
                cout << "\033[92m>> '" << p->nombre << "' agregado! Stock: " << p->stock << "          ";
            }
            else {
                cout << "\033[91m>> ID invalido o sin stock.                         ";
            }
        }
    }

    void verCarrito(Inventario& inv) {
        int fila = 10;
        linea(fila++, "--- TU CARRITO ---");
        fila++;

        if (carrito->getCabeza() == nullptr) {
            linea(fila, "Carrito vacio.");
            return;
        }

        double total = 0;
        int i = 1;
        Nodo<int>* actual = carrito->getCabeza();
        while (actual != nullptr) {
            Producto* p = inv.obtenerProducto(actual->dato);
            if (p != nullptr) {
                linea(fila++, to_string(i++) + ". " + p->nombre + " - S/. " + to_string((int)p->precio));
                total += p->precio;
            }
            actual = actual->siguiente;
        }
        linea(fila + 1, "TOTAL: S/. " + to_string((int)total));
    }

    void seleccionarMetodoPago(double total) {
        limpiarZonaVerde();
        linea(10, "==============================");
        linea(11, "     METODO DE PAGO           ");
        linea(12, "==============================");
        linea(14, "1. Tarjeta");
        linea(15, "2. Yape / Plin");
        linea(17, "Opcion: ");
        int op; cin >> op;

        if (op == 1) {
            limpiarZonaVerde();
            Tarjeta t; t.pagar(total);
        }
        else if (op == 2) {
            limpiarPantalla();
            YapePlin yp; yp.pagar(total);
            mostrarFondo2();
        }
    }

    void comprarCarrito(Inventario& inv) {
        limpiarZonaVerde();
        if (carrito->getCabeza() == nullptr) {
            linea(16, "El carrito esta vacio.");
            pausaRetroceder();
            return;
        }

        double total = 0;
        Nodo<int>* actual = carrito->getCabeza();
        while (actual != nullptr) {
            Producto* p = inv.obtenerProducto(actual->dato);
            if (p != nullptr) total += p->precio;
            actual = actual->siguiente;
        }

        verCarrito(inv);
        pausaRetroceder(36);

        seleccionarMetodoPago(total);

        actual = carrito->getCabeza();
        while (actual != nullptr) {
            Producto* p = inv.obtenerProducto(actual->dato);
            if (p != nullptr && p->stock > 0) {
                p->stock--;
                inv.registrarVenta(dni, nombre, p);
            }
            actual = actual->siguiente;
        }
        inv.guardarEnArchivo();
        carrito->vaciar();

        limpiarZonaVerde();
        linea(16, ">> Compra realizada con exito.");
        pausaRetroceder(19);
    }

    void menuBuscarProducto(Inventario& inv) {
        while (true) {
            limpiarZonaVerde();
            linea(10, "--- TIENDA AMAZONAS ---");
            linea(12, "1. Ver Catalogo Completo (Agregar por ID)");
            linea(13, "2. Buscar por nombre (Confirmacion S/N)");
            linea(15, "\033[93m[ESC] Volver al Menu Principal");

            int c = _getch();
            if (c == 27) break;

            if (c == '1') {
                limpiarZonaVerde();
                inv.listarTodo();
                bucleAgregarRapido(inv, 32); // Permite agregar directamente
            }
            else if (c == '2') {
                limpiarZonaVerde();
                linea(10, "Ingrese nombre del producto: ");
                string nom; cin.ignore(); getline(cin, nom);

                limpiarZonaVerde();
                inv.buscarPorNombre(nom);

                linea(30, ">> Ingrese el ID del producto que desea: ");
                string idStr; cin >> idStr;

                if (idStr.find_first_not_of("0123456789") == string::npos) {
                    int idBuscado = stoi(idStr);
                    Producto* p = inv.obtenerProducto(idBuscado);

                    if (p != nullptr && p->stock > 0) {
                        linea(32, "Desea agregar '" + p->nombre + "' al carrito? (S/N): ");
                        char confirma = _getch();
                        if (confirma == 's' || confirma == 'S') {
                            carrito->agregar(idBuscado);
                            linea(34, "\033[92m>> Agregado con exito!");
                        }
                        else {
                            linea(34, "\033[91m>> Operacion cancelada.");
                        }
                    }
                    else {
                        linea(32, "\033[91m>> Producto no encontrado o sin stock.");
                    }
                }
                pausaRetroceder(36);
            }
        }
    }

    void menuCarrito(Inventario& inv) {
        while (true) {
            limpiarZonaVerde();
            linea(10, "--- MI CARRITO DE COMPRAS ---");
            linea(12, "1. Revisar lista de productos");
            linea(13, "2. Proceder al pago");
            linea(15, "\033[93m[ESC] Volver al Menu Principal");

            int c = _getch();
            if (c == 27) break;

            if (c == '1') {
                limpiarZonaVerde();
                verCarrito(inv);
                pausaRetroceder(36);
            }
            else if (c == '2') {
                comprarCarrito(inv);
                break;
            }
        }
    }

    void menu(Inventario& inv) {
        mostrarFondo2();
        historialNavegacion.apilar(MENU_PRINCIPAL);

        while (!historialNavegacion.estaVacia()) {
            int pantallaActual = historialNavegacion.obtenerTope();
            limpiarZonaVerde();

            if (pantallaActual == MENU_PRINCIPAL) {
                linea(10, "========================================");
                linea(11, "   BIENVENIDO, " + nombre);
                linea(12, "========================================");
                linea(14, "1. Entrar a la Tienda (Catalogo/Busqueda)");
                linea(15, "2. Ver mi Carrito");
                linea(17, "\033[93m[ESC] Salir del Sistema");

                int c = _getch();
                if (c == '1') historialNavegacion.apilar(MENU_BUSQUEDA);
                else if (c == '2') historialNavegacion.apilar(MENU_CARRITO);
                else if (c == 27) historialNavegacion.desapilar();
            }
            else if (pantallaActual == MENU_BUSQUEDA) {
                menuBuscarProducto(inv);
                historialNavegacion.desapilar();
            }
            else if (pantallaActual == MENU_CARRITO) {
                menuCarrito(inv);
                historialNavegacion.desapilar();
            }
        }
    }
};