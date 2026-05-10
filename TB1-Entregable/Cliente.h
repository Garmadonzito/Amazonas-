#pragma once

#include "Usuario.h"
#include "Inventario.h"
#include "MetodoPago.h"
#include "Registro.h"
#include "Pila.h"
#include "Direccion.h"
#include "GestorEscenas.h" 
#include <conio.h>
#include <fstream>

class Cliente : public Usuario {
private:
    ListaEnlazada<int>* carrito;
    Pila<int> historialNavegacion;
    Direccion direccionEnvio;

    const int MENU_PRINCIPAL = 1;
    const int MENU_BUSQUEDA = 2;
    const int MENU_CARRITO = 3;

    int comprasRealizadas = 0;

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
        gestorEscenas grafica;
        grafica.setEscena(gestorEscenas::CATALOGO);

        limpiarPantalla();
        grafica.dibujarFondoSinLogo();

        irA(4, PANEL_COL); cout << "            \033[96m========================================================\033[0m";
        irA(5, PANEL_COL); cout << "            \033[96m                    LOGIN DE CLIENTE                    \033[0m";
        irA(6, PANEL_COL); cout << "            \033[96m========================================================\033[0m";

        irA(14, PANEL_COL); cout << "Nombre: "; cin.ignore(); getline(cin, nombre);

        auto validarCorreo = [](string email) -> bool {
            return email.find('@') != string::npos && email.find(".com") != string::npos;
            };

        bool correoValido = false;
        do {
            irA(16, PANEL_COL); cout << string(50, ' ');
            irA(16, PANEL_COL); cout << "Correo: ";
            getline(cin, correo);
            if (validarCorreo(correo)) correoValido = true;
            else { irA(22, PANEL_COL); cout << "\033[91m>> Error: Formato de correo invalido.\033[0m"; }
        } while (!correoValido);

        if (correoValido) { irA(22, PANEL_COL); cout << string(60, ' '); }

        auto validarDNI = [](string d) -> bool {
            return d.length() == 8 && d.find_first_not_of("0123456789") == string::npos;
            };

        bool dniValido = false;
        do {
            irA(18, PANEL_COL); cout << string(50, ' ');
            irA(18, PANEL_COL); cout << "DNI: ";
            getline(cin, dni);
            if (validarDNI(dni)) dniValido = true;
            else { irA(22, PANEL_COL); cout << "\033[91m>> Error: DNI invalido.\033[0m"; }
        } while (!dniValido);

        irA(24, PANEL_COL); cout << "\033[92m>> Bienvenido " << nombre << "!\033[0m";
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

    double CalcularTotal(double& total, float precio) {
        auto lam = [](double& t, float p) {return t += p;};
        return lam(total, precio);
    }

    double CalcularTotalOferta(double& total) {
        auto lam = [](double& t) {return t = t * 0.85;};
        return lam(total);
    }

    void verCarrito(Inventario& inv) {
        linea(4, "            \033[93m========================================================\033[0m");
        linea(5, "            \033[93m                   TU CARRITO ACTUAL                    \033[0m");
        linea(6, "            \033[93m========================================================\033[0m");

        int fila = 12;
        if (carrito->getCabeza() == nullptr) {
            linea(fila, "  El carrito esta vacio.");
            return;
        }

        auto calcularIGV = [](double total) -> double {
            return total * 0.18;
            };

        double total = 0;
        int i = 1;
        Nodo<int>* actual = carrito->getCabeza();
        while (actual != nullptr) {
            Producto* p = inv.obtenerProducto(actual->dato);
            if (p != nullptr) {
                linea(fila++, "  " + to_string(i++) + ". " + p->nombre + " - S/. " + to_string((int)p->precio));
                CalcularTotal(total, p->precio);
            }
            actual = actual->siguiente;
        }
        if (comprasRealizadas == 0 && total > 0) {
            CalcularTotalOferta(total);
            linea(fila++, "\033[93m  Descuento de bienvenida (15%) aplicado!\033[0m");
        }

        fila++;
        if (direccionEnvio.esValida()) {
            linea(fila++, "\033[96mDATOS DE ENTREGA:\033[0m");
            direccionEnvio.mostrar(fila);
            fila += 4;
        }
        else {
            linea(fila++, "\033[91m[!] Direccion no registrada. Se usara recojo en tienda.\033[0m");
        }

        linea(fila++, "--------------------------------------------------------");
        double montoIGV = calcularIGV(total);
        linea(fila++, "  Subtotal (sin IGV): S/. " + to_string((int)(total - montoIGV)));
        linea(fila++, "  IGV (18%):          S/. " + to_string((int)montoIGV));
        linea(fila, "  \033[92mTOTAL A PAGAR:      S/. " + to_string((int)total) + "\033[0m");
    }

    void seleccionarMetodoPago(double total) {
        gestorEscenas grafica; grafica.setEscena(gestorEscenas::CATALOGO);
        limpiarZonaVerde();
        grafica.dibujarFondoSinLogo();

        linea(4, "            \033[95m========================================================\033[0m");
        linea(5, "            \033[95m                SELECCIONA METODO DE PAGO               \033[0m");
        linea(6, "            \033[95m========================================================\033[0m");

        linea(14, "    1. Tarjeta de Credito / Debito");
        linea(16, "    2. Yape / Plin (Codigo QR)");
        linea(20, "    Opcion: ");
        int op; cin >> op;

        if (op == 1) {
            limpiarZonaVerde();
            grafica.dibujarFondoSinLogo();
            Tarjeta t; t.pagar(total);
        }
        else if (op == 2) {
            limpiarPantalla();
            grafica.dibujarFondoSinLogo();
            YapePlin yp; yp.pagar(total);
        }
    }

    void comprarCarrito(Inventario& inv) {
        gestorEscenas grafica; grafica.setEscena(gestorEscenas::CATALOGO);

        limpiarZonaVerde();
        grafica.dibujarFondoSinLogo();

        if (carrito->getCabeza() == nullptr) {
            linea(18, "  El carrito esta vacio. Agrega productos primero.");
            pausaRetroceder();
            return;
        }

        double total = 0;
        Nodo<int>* actual = carrito->getCabeza();
        while (actual != nullptr) {
            Producto* p = inv.obtenerProducto(actual->dato);
            if (p != nullptr) CalcularTotal(total, p->precio);
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
        grafica.dibujarFondoSinLogo();
        comprasRealizadas++;
        linea(20, "  \033[92m>> Compra procesada con exito. Gracias por preferirnos!\033[0m");
        pausaRetroceder(24);
    }

    void menuBuscarProducto(Inventario& inv) {
        gestorEscenas grafica;
        grafica.setEscena(gestorEscenas::CATALOGO);

        while (true) {
            limpiarZonaVerde();
            grafica.dibujarFondoSinLogo();

            linea(2, "\033[93m   ______ \033[92m   ___      __  ___    ___     ____   ___     _  __     ___     ____ \033[0m");
            linea(3, "\033[93m  /     /|\033[92m  /   |    /  |/  /   /   |   /_  /  / _ \\   / |/ /    /   |   / __/ \033[0m");
            linea(4, "\033[93m /_____/ |\033[92m / /| |   / /|_/ /   / /| |    / /  / / / / /    /    / /| |  \\__ \\ \033[0m");
            linea(5, "\033[93m |     | |\033[92m / ___ |  / /  / /   / ___ |   / /_/ /_/ / / /|  /   / ___ | ___/ /  \033[0m");
            linea(6, "\033[93m |_____|/ \033[92m /_/  |_| /_/  /_/   /_/  |_|  /___/ \\____/ /_/ |_/   /_/  |_|/____/  \033[0m");

            linea(12, "    1. Ver Catalogo Completo (Agregar por ID)");
            linea(14, "    2. Buscar producto por nombre especifico");
            linea(16, "    3. Ver Catalogo ordenado por Precio");
            linea(18, "    4. Registrar Direccion de Envio");
            linea(20, "    5. Dejar Resena de un Producto");
            linea(22, "    6. Soporte - Reportar problema con pedido");
            linea(26, "    \033[93m[ESC] Volver al Menu Principal\033[0m");

            int c = _getch();
            if (c == 27) break;

            if (c == '1') {
                limpiarZonaVerde();
                grafica.dibujarFondoSinLogo();
                inv.listarTodo();
                bucleAgregarRapido(inv, 38);
            }
            else if (c == '2') {
                limpiarZonaVerde();
                grafica.dibujarFondoSinLogo();
                linea(14, "  Ingrese nombre del producto: ");
                string nom; cin.ignore(); getline(cin, nom);

                limpiarZonaVerde();
                grafica.dibujarFondoSinLogo();
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
            else if (c == '3') {
                limpiarZonaVerde();
                grafica.dibujarFondoSinLogo();
                inv.ordenarPorPrecio();
                inv.listarTodo();
                bucleAgregarRapido(inv, 38);
            }
            else if (c == '4') {
                limpiarZonaVerde();
                grafica.dibujarFondoSinLogo();
                direccionEnvio.registrar(12);
                if (direccionEnvio.esValida())
                    linea(24, "  \033[92m>> Direccion registrada correctamente.\033[0m");
                pausaRetroceder(26);
            }
            else if (c == '5') {
                limpiarZonaVerde();
                grafica.dibujarFondoSinLogo();
                linea(12, "  ID del producto a resenar: ");
                string idStr; cin.ignore(); cin >> idStr;
                if (idStr.find_first_not_of("0123456789") == string::npos) {
                    int idProd = stoi(idStr);
                    Producto* p = inv.obtenerProducto(idProd);
                    if (p != nullptr) {
                        limpiarZonaVerde();
                        grafica.dibujarFondoSinLogo();
                        linea(12, "  Producto: " + p->nombre);
                        linea(14, "  Puntuacion (1-5): ");
                        int punt; irA(14, PANEL_COL + 20); cin >> punt;
                        linea(16, "  Comentario: ");
                        string comentario; cin.ignore(); irA(16, PANEL_COL + 14);
                        getline(cin, comentario);
                        inv.getResenas()->agregarResena(idProd, p->nombre, dni, comentario, punt);
                        linea(19, "  \033[92m>> Resena registrada. Gracias!\033[0m");
                    }
                    else {
                        linea(14, "  \033[91m>> Producto no encontrado.\033[0m");
                    }
                }
                pausaRetroceder(22);
            }
            else if (c == '6') {
                limpiarZonaVerde();
                grafica.dibujarFondoSinLogo();

                linea(4, "            \033[91m========================================================\033[0m");
                linea(5, "            \033[91m                  SOPORTE AL CLIENTE                    \033[0m");
                linea(6, "            \033[91m========================================================\033[0m");

                linea(12, "    1. Reportar un problema");
                linea(14, "    2. Solicitar devolucion de ultima compra");
                linea(16, "    3. Ver mis tickets");
                linea(20, "    Opcion: ");
                char op2 = _getch();

                if (op2 == '1') {
                    limpiarZonaVerde();
                    grafica.dibujarFondoSinLogo();
                    linea(12, "  Asunto: ");
                    string asunto; cin.ignore(); irA(12, PANEL_COL + 10); getline(cin, asunto);
                    linea(14, "  Descripcion del problema: ");
                    string desc; irA(14, PANEL_COL + 28); getline(cin, desc);
                    inv.getSoporte()->abrirTicket(dni, nombre, asunto, desc);
                    linea(17, "  \033[92m>> Ticket abierto. Un agente lo revisara pronto.\033[0m");
                }
                else if (op2 == '2') {
                    limpiarZonaVerde();
                    grafica.dibujarFondoSinLogo();

                    auto validarIdentidad = [&](string nomIngresado, string correoIngresado, string dniIngresado) -> bool {
                        return dniIngresado == dni &&
                            nomIngresado == nombre &&
                            correoIngresado == correo;
                        };

                    linea(4, "            \033[93m========================================================\033[0m");
                    linea(5, "            \033[93m          VERIFICACION DE IDENTIDAD - REEMBOLSO         \033[0m");
                    linea(6, "            \033[93m========================================================\033[0m");

                    linea(12, "  Confirme sus datos para continuar:");
                    linea(14, "  Nombre : "); string nomV; cin.ignore(); irA(14, PANEL_COL + 11); getline(cin, nomV);
                    linea(15, "  Correo : "); string correoV;           irA(15, PANEL_COL + 11); getline(cin, correoV);
                    linea(16, "  DNI    : "); string dniV;              irA(16, PANEL_COL + 11); getline(cin, dniV);

                    if (!validarIdentidad(nomV, correoV, dniV)) {
                        limpiarZonaVerde();
                        grafica.dibujarFondoSinLogo();
                        linea(16, "  \033[91m>> Datos incorrectos. No se puede procesar el reembolso.\033[0m");
                        pausaRetroceder(19);
                    }
                    else {
                        limpiarZonaVerde();
                        grafica.dibujarFondoSinLogo();
                        vector<Venta> misCompras = inv.obtenerVentasPorCliente(dni);
                        if (misCompras.empty()) {
                            linea(14, "  \033[91m>> No tienes compras registradas aun.\033[0m");
                        }
                        else {
                            linea(4, "            \033[96m========================================================\033[0m");
                            linea(5, "            \033[96m               TUS COMPRAS REGISTRADAS                  \033[0m");
                            linea(6, "            \033[96m========================================================\033[0m");

                            int fila = 12;
                            for (int i = 0; i < (int)misCompras.size() && fila < 35; i++) {
                                string item = "  [" + to_string(i + 1) + "] " +
                                    misCompras[i].producto + " - S/. " +
                                    to_string((int)misCompras[i].precio) +
                                    " | " + misCompras[i].fechaTexto;
                                imprimirEnPanel(fila++, item);
                            }
                            linea(36, "  Ingrese numeros de compras a devolver (ej: 1 2 3), 0 para cancelar: ");
                            irA(37, PANEL_COL);
                            int sel;
                            bool huboDevolucion = false;
                            while (cin >> sel && sel != 0) {
                                if (sel >= 1 && sel <= (int)misCompras.size()) {
                                    Venta& v = misCompras[sel - 1];
                                    inv.getSoporte()->solicitarDevolucion(dni, nombre, v.producto, v.precio);
                                    huboDevolucion = true;
                                }
                            }
                            limpiarZonaVerde();
                            grafica.dibujarFondoSinLogo();
                            if (huboDevolucion) {
                                linea(14, "  \033[92m>> Reembolso solicitado correctamente.\033[0m");
                                linea(15, "  \033[92m>> Un agente revisara y confirmara cada devolucion.\033[0m");
                            }
                            else {
                                linea(14, "  \033[91m>> No se selecciono ninguna compra.\033[0m");
                            }
                        }
                    }
                }
                else if (op2 == '3') {
                    limpiarZonaVerde();
                    grafica.dibujarFondoSinLogo();
                    inv.getSoporte()->listarMisTickets(dni);
                }
                pausaRetroceder(38);
            }
        }
    }

    void menuCarrito(Inventario& inv) {
        gestorEscenas grafica;
        grafica.setEscena(gestorEscenas::CATALOGO);

        while (true) {
            limpiarZonaVerde();
            grafica.dibujarFondoSinLogo();

            linea(4, "            \033[93m========================================================\033[0m");
            linea(5, "            \033[93m               --- MI CARRITO DE COMPRAS ---            \033[0m");
            linea(6, "            \033[93m========================================================\033[0m");

            linea(12, "    1. Ver lista de productos anadidos");
            linea(14, "    2. Proceder al pago final");
            linea(16, "    3. Historial de pedidos - Orden por Monto");
            linea(20, "    \033[93m[ESC] Volver al Menu Principal\033[0m");

            int c = _getch();
            if (c == 27) break;

            if (c == '1') {
                limpiarZonaVerde();
                grafica.dibujarFondoSinLogo();
                verCarrito(inv);
                pausaRetroceder(38);
            }
            else if (c == '2') {
                comprarCarrito(inv);
                break;
            }
            else if (c == '3') {
                limpiarZonaVerde();
                grafica.dibujarFondoSinLogo();
                inv.mostrarHistorialClientePersonalizado(dni);
                pausaRetroceder(38);
            }
        }
    }

    void menu(Inventario& inv) {
        historialNavegacion.apilar(MENU_PRINCIPAL);

        gestorEscenas grafica;
        grafica.setEscena(gestorEscenas::CATALOGO);

        while (!historialNavegacion.estaVacia()) {
            int pantallaActual = historialNavegacion.obtenerTope();
            limpiarZonaVerde();
            grafica.dibujarFondoSinLogo();

            if (pantallaActual == MENU_PRINCIPAL) {
                linea(4, "            \033[94m========================================================\033[0m");

                string saludo = "             BIENVENIDO AL MARKETPLACE, " + nombre;
                while (saludo.length() < 56) saludo += " ";
                linea(5, "            \033[94m" + saludo + "\033[0m");
                linea(6, "            \033[94m========================================================\033[0m");

                linea(12, "    1. Entrar a la Tienda (Catalogo y Busqueda)");
                linea(14, "    2. Gestionar mi Carrito de Compras");
                linea(20, "    \033[93m[ESC] Salir del Sistema de Cliente\033[0m");

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