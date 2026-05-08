#pragma once

#include "Usuario.h"
#include "Inventario.h"
#include "MetodoPago.h"
#include "Registro.h" 
#include "Pila.h" 
#include <conio.h>
#include <fstream>

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

   
    void pausaRetroceder(int fila = 40) {
        irA(fila, PANEL_COL);
        cout << "\033[93m>> Presione ESC para volver atras...\033[0m";
        while (true) {
            if (_kbhit()) {
                int c = _getch();
                if (c == 27) break;
            }
        }
        irA(fila, PANEL_COL);
        cout << string(60, ' '); 
    }

public:
    Cliente() { carrito = new ListaEnlazada<int>(); }
    ~Cliente() { delete carrito; }

    void login() {
        limpiarPantalla();
        
        irA(12, PANEL_COL); cout << "========================================================";
        irA(14, PANEL_COL); cout << "                    LOGIN DE CLIENTE                    ";
        irA(16, PANEL_COL); cout << "========================================================";

        irA(20, PANEL_COL); cout << "Nombre: "; cin.ignore(); getline(cin, nombre);
        irA(22, PANEL_COL); cout << "Correo: "; getline(cin, correo);

        auto validarDNI = [](string d) -> bool {
            return d.length() == 8 && d.find_first_not_of("0123456789") == string::npos;
            };

        bool dniValido = false;
        do {
            irA(24, PANEL_COL); cout << string(40, ' ');
            irA(24, PANEL_COL); cout << "DNI: ";
            getline(cin, dni);
            if (validarDNI(dni)) dniValido = true;
            else { irA(26, PANEL_COL); cout << "\033[91m>> Error: DNI invalido.\033[0m"; }
        } while (!dniValido);

        irA(28, PANEL_COL); cout << "\033[92m>> Bienvenido " << nombre << "!\033[0m";
        pausa();
    }

    void bucleAgregarRapido(Inventario& inv, int filaInicio) {
        irA(filaInicio, PANEL_COL);
        cout << "\033[0m>> Ingrese ID para agregar. [\033[93mESC\033[0m] para terminar.";

        int filaMsg = filaInicio + 2;
        while (true) {
            irA(filaMsg, PANEL_COL); cout << string(50, ' ');
            irA(filaMsg, PANEL_COL); cout << "\033[0mID: ";
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
        int fila = 12;
        linea(fila++, "========================================================");
        linea(fila++, "                   TU CARRITO ACTUAL                    ");
        linea(fila++, "========================================================");
        fila++;

        if (carrito->getCabeza() == nullptr) {
            linea(fila, "El carrito esta vacio.");
            return;
        }

        double total = 0;
        int i = 1;
        Nodo<int>* actual = carrito->getCabeza();
        while (actual != nullptr) {
            Producto* p = inv.obtenerProducto(actual->dato);
            if (p != nullptr) {
                linea(fila++, "  " + to_string(i++) + ". " + p->nombre + " - S/. " + to_string((int)p->precio));
                total += p->precio;
            }
            actual = actual->siguiente;
        }
        fila++;
        linea(fila++, "--------------------------------------------------------");
        linea(fila, "  \033[92mTOTAL A PAGAR: S/. " + to_string((int)total) + "\033[0m");
    }

    void seleccionarMetodoPago(double total) {
        limpiarZonaVerde();
        linea(12, "========================================================");
        linea(14, "                SELECCIONA METODO DE PAGO               ");
        linea(16, "========================================================");
        linea(20, "    1. Tarjeta de Credito / Debito");
        linea(22, "    2. Yape / Plin (Codigo QR)");
        linea(26, "    Opcion: ");
        int op; cin >> op;

        if (op == 1) {
            limpiarZonaVerde();
            Tarjeta t; t.pagar(total);
        }
        else if (op == 2) {
            limpiarPantalla();
            YapePlin yp; yp.pagar(total);
        }
    }

    void comprarCarrito(Inventario& inv) {
        limpiarZonaVerde();
        if (carrito->getCabeza() == nullptr) {
            linea(18, "  El carrito esta vacio. Agrega productos primero.");
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
        pausaRetroceder(38);

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
        linea(20, "  \033[92m>> Compra procesada con exito. Gracias por preferirnos!\033[0m");
        pausaRetroceder(24);
    }

    void menuBuscarProducto(Inventario& inv) {
        while (true) {
            limpiarZonaVerde();
          
            linea(12, "========================================================");
            linea(14, "                 --- TIENDA AMAZONAS ---                ");
            linea(16, "========================================================");
            linea(20, "    1. Ver Catalogo Completo (Agregar por ID)");
            linea(22, "    2. Buscar producto por nombre especifico");
            linea(28, "    \033[93m[ESC] Volver al Menu Principal\033[0m");

            int c = _getch();
            if (c == 27) break;

            if (c == '1') {
                limpiarZonaVerde();
                inv.listarTodo();
                bucleAgregarRapido(inv, 38);
            }
            else if (c == '2') {
                limpiarZonaVerde();
                linea(14, "  Ingrese nombre del producto: ");
                string nom; cin.ignore(); getline(cin, nom);

                limpiarZonaVerde();
                inv.buscarPorNombre(nom);

                linea(32, "  >> Ingrese el ID del producto que desea: ");
                string idStr; cin >> idStr;

                if (idStr.find_first_not_of("0123456789") == string::npos) {
                    int idBuscado = stoi(idStr);
                    Producto* p = inv.obtenerProducto(idBuscado);

                    if (p != nullptr && p->stock > 0) {
                        linea(34, "  Desea agregar '" + p->nombre + "' al carrito? (S/N): ");
                        char confirma = _getch();
                        if (confirma == 's' || confirma == 'S') {
                            carrito->agregar(idBuscado);
                            linea(36, "  \033[92m>> Agregado con exito!\033[0m");
                        }
                        else {
                            linea(36, "  \033[91m>> Operacion cancelada.\033[0m");
                        }
                    }
                    else {
                        linea(34, "  \033[91m>> Producto no encontrado o sin stock.\033[0m");
                    }
                }
                pausaRetroceder(39);
            }
        }
    }

    void menuCarrito(Inventario& inv) {
        while (true) {
            limpiarZonaVerde();
           
            linea(12, "========================================================");
            linea(14, "               --- MI CARRITO DE COMPRAS ---            ");
            linea(16, "========================================================");
            linea(20, "    1. Ver lista de productos añadidos");
            linea(22, "    2. Proceder al pago final");
            linea(28, "    \033[93m[ESC] Volver al Menu Principal\033[0m");

            int c = _getch();
            if (c == 27) break;

            if (c == '1') {
                limpiarZonaVerde();
                verCarrito(inv);
                pausaRetroceder(38);
            }
            else if (c == '2') {
                comprarCarrito(inv);
                break;
            }
        }
    }

    void menu(Inventario& inv) {
        historialNavegacion.apilar(MENU_PRINCIPAL);

        while (!historialNavegacion.estaVacia()) {
            int pantallaActual = historialNavegacion.obtenerTope();
            limpiarZonaVerde();

            if (pantallaActual == MENU_PRINCIPAL) {
                
                linea(12, "========================================================");
                linea(14, "             BIENVENIDO AL MARKETPLACE, " + nombre);
                linea(16, "========================================================");
                linea(20, "    1. Entrar a la Tienda (Catalogo y Busqueda)");
                linea(22, "    2. Gestionar mi Carrito de Compras");
                linea(28, "    \033[93m[ESC] Salir del Sistema de Cliente\033[0m");

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