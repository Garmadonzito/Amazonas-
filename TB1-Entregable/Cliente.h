#pragma once
#include "Usuario.h"
#include "Inventario.h"
#include "MetodoPago.h"
#include "Registro.h" // Nombre corregido
#include <conio.h>
#include <fstream>

extern void mostrarFondo2();

class Cliente : public Usuario {
private:
    ListaEnlazada<int>* carrito;

    void linea(int fila, const string& texto) {
        irA(fila, PANEL_COL); cout << "\033[0m" << texto;
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
            if (d.length() != 8) return false;
            for (char c : d) {
                if (c < '0' || c > '9') return false;
            }
            return true;
            };

        bool dniValido = false;
        do {
            cout << "DNI: ";
            getline(cin, dni);
            if (validarDNI(dni)) {
                dniValido = true;
            }
            else {
                cout << ">> Error: El DNI debe tener exactamente 8 numeros.\n";
            }
        } while (!dniValido);

        cout << "\n>> Bienvenido " << nombre << "!\n";
        pausa();
    }

    void agregarAlCarrito(Inventario& inv) {
        limpiarZonaVerde();
        irA(10, PANEL_COL);
        inv.listarTodo();

        irA(32, PANEL_COL); cout << "\033[0m>> Ingrese ID y ENTER para agregar. ESC para terminar.";

        int filaMsg = 34;
        while (true) {
            irA(filaMsg, PANEL_COL);
            cout << "\033[0mID: ";
            string buf;
            bool salir = false;

            while (true) {
                int c = _getch();
                if (c == 27) { salir = true; cout << "\n"; break; }
                if (c == '\r') { cout << "\n"; break; }
                if (c == 8 && !buf.empty()) { buf.pop_back(); cout << "\b \b"; }
                else if (c >= '0' && c <= '9') { buf += (char)c; cout << (char)c; }
            }

            if (salir) break;
            if (buf.empty()) continue;

            int id = stoi(buf);
            Producto* p = inv.obtenerProducto(id);
            irA(filaMsg + 1, PANEL_COL);
            cout << "\033[0m";
            if (p == nullptr) {
                cout << ">> ID no encontrado.                          ";
            }
            else if (p->stock <= 0) {
                cout << ">> Sin stock disponible.                      ";
            }
            else {
                carrito->agregar(id);
                p->stock--;
                cout << ">> '" << p->nombre << "' agregado al carrito! ";
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
                string item = to_string(i) + ". " + p->nombre +
                    "  -  S/. " + to_string((int)p->precio);
                linea(fila++, item);
                total += p->precio;
            }
            actual = actual->siguiente;
            i++;
        }
        fila++;
        linea(fila, "TOTAL: S/. " + to_string((int)total));
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
            irA(13, PANEL_COL);
            Tarjeta t;
            t.pagar(total);
        }
        else if (op == 2) {
            limpiarPantalla();
            YapePlin yp;
            yp.pagar(total);
            mostrarFondo2();
        }
        else {
            linea(19, "Opcion invalida. Pago cancelado.");
        }
    }

    void comprarCarrito(Inventario& inv) {
        limpiarZonaVerde();
        if (carrito->getCabeza() == nullptr) {
            linea(16, "El carrito esta vacio.");
            irA(19, PANEL_COL); pausa();
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
        irA(36, PANEL_COL); pausa();

        seleccionarMetodoPago(total);

        inv.registrarPedido(nombre, (float)total);

        actual = carrito->getCabeza();
        while (actual != nullptr) {
            Producto* p = inv.obtenerProducto(actual->dato);
            if (p != nullptr) {
                inv.registrarVenta(nombre, p->nombre, p->precio, p->stock);
            }
            actual = actual->siguiente;
        }

        // 1. Guardar cliente en clientes.dat
        ofstream archivoClientes("clientes.dat", ios::binary | ios::app);
        if (archivoClientes) {
            RegistroCliente rc;
            strncpy_s(rc.nombre, sizeof(rc.nombre), nombre.c_str(), _TRUNCATE);
            strncpy_s(rc.correo, sizeof(rc.correo), correo.c_str(), _TRUNCATE);
            strncpy_s(rc.dni, sizeof(rc.dni), dni.c_str(), _TRUNCATE);
            archivoClientes.write((char*)&rc, sizeof(RegistroCliente));
            archivoClientes.close();
        }

        // 2. Guardar ventas individuales en ventas.dat
        ofstream archivoVentas("ventas.dat", ios::binary | ios::app);
        if (archivoVentas) {
            actual = carrito->getCabeza();
            while (actual != nullptr) {
                Producto* p = inv.obtenerProducto(actual->dato);
                if (p != nullptr) {
                    RegistroVenta rv;
                    rv.idProducto = p->id;
                    strncpy_s(rv.nombreProducto, sizeof(rv.nombreProducto), p->nombre.c_str(), _TRUNCATE);
                    rv.monto = p->precio;
                    strncpy_s(rv.dniCliente, sizeof(rv.dniCliente), dni.c_str(), _TRUNCATE);
                    archivoVentas.write((char*)&rv, sizeof(RegistroVenta));
                }
                actual = actual->siguiente;
            }
            archivoVentas.close();
        }

        // 3. Confirmar la resta de stock escribiendo en el disco
        inv.guardarEnArchivo();

        limpiarZonaVerde();
        linea(16, ">> Compra realizada. Gracias " + nombre + "!");
        irA(19, PANEL_COL); pausa();

        carrito->vaciar();
    }

    void menuBuscarProducto(Inventario& inv) {
        int op;
        do {
            limpiarZonaVerde();
            linea(10, "--- BUSCAR PRODUCTO ---");
            linea(12, "1. Ver todo el catalogo");
            linea(13, "2. Buscar por nombre");
            linea(14, "3. Agregar al carrito");
            linea(15, "4. Volver");
            linea(17, "Opcion: ");
            cin >> op;

            if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); op = -1; }

            switch (op) {
            case 1:
                limpiarZonaVerde();
                irA(10, PANEL_COL);
                inv.listarTodo();
                irA(37, PANEL_COL); pausa();
                break;
            case 2: {
                limpiarZonaVerde();
                linea(10, "Nombre a buscar: ");
                string nom; cin.ignore(); getline(cin, nom);
                limpiarZonaVerde();
                irA(10, PANEL_COL);
                inv.buscarPorNombre(nom);
                irA(35, PANEL_COL); pausa();
                break;
            }
            case 3:
                agregarAlCarrito(inv);
                break;
            }
        } while (op != 4);
    }

    void menuCarrito(Inventario& inv) {
        int op;
        do {
            limpiarZonaVerde();
            linea(10, "--- MI CARRITO ---");
            linea(12, "1. Ver carrito");
            linea(13, "2. Comprar");
            linea(14, "3. Volver");
            linea(16, "Opcion: ");
            cin >> op;

            if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); op = -1; }

            switch (op) {
            case 1:
                limpiarZonaVerde();
                verCarrito(inv);
                irA(36, PANEL_COL); pausa();
                break;
            case 2:
                comprarCarrito(inv);
                return;
            }
        } while (op != 3);
    }

    void menu(Inventario& inv) {
        mostrarFondo2();
        int op;
        do {
            limpiarZonaVerde();
            linea(10, "========================================");
            linea(11, "   BIENVENIDO, " + nombre);
            linea(12, "========================================");
            linea(14, "1. Ver productos / Buscar");
            linea(15, "2. Mi carrito");
            linea(16, "3. Cerrar sesion");
            linea(18, "Opcion: ");
            cin >> op;

            if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); op = -1; }

            switch (op) {
            case 1: menuBuscarProducto(inv); break;
            case 2: menuCarrito(inv); break;
            }
        } while (op != 3);
    }
};
