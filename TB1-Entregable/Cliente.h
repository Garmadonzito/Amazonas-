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
#include "Consola.h"

class Cliente : public Usuario {
private:
    ListaEnlazada<int>* carrito;
    Pila<int> historialNavegacion;
    Direccion direccionEnvio;

    const int MENU_PRINCIPAL = 1;
    const int MENU_BUSQUEDA = 2;
    const int MENU_CARRITO = 3;
    const int MENU_PRESUPUESTO = 4;

    int comprasRealizadas = 0;

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

    void login(Inventario& inv, Consola& consola) {
        gestorEscenas grafica;
        grafica.setEscena(gestorEscenas::CATALOGO);
        consola.establecerColor(0, 2);

        limpiarPantalla();
        grafica.dibujarFondoSinLogo();
        consola.establecerColor(0, 2);

        dibujarMarco(4, PANEL_COL + 12, 56, 3, "96");
        cout << "\033[96m";
        imprimirLento(5, PANEL_COL + 12 + 20, "LOGIN DE CLIENTE", 20);
        cout << "\033[0m";
        consola.establecerColor(0, 2);

        auto validarDNI = [](string d) -> bool {
            return d.length() == 8 && d.find_first_not_of("0123456789") == string::npos;
        };

        bool dniValido = false;
        do {
            irA(14, PANEL_COL); cout << string(50, ' ');
            consola.establecerColor(0, 2);
            irA(14, PANEL_COL); cout << "DNI: ";
            getline(cin, dni);
            consola.establecerColor(0, 2);
            if (validarDNI(dni)) dniValido = true;
            else { irA(22, PANEL_COL); cout << "\033[91m>> Error: DNI invalido.\033[0m"; }
        } while (!dniValido);
        irA(22, PANEL_COL); cout << string(60, ' ');

        RegistroCliente* clienteEncontrado = inv.buscarClienteHash(dni);

        if (clienteEncontrado != nullptr) {
            this->nombre = std::string(clienteEncontrado->nombre);
            this->correo = std::string(clienteEncontrado->correo);
            consola.establecerColor(0, 2);
            irA(16, PANEL_COL); cout << "Nombre: " << this->nombre << " (Cargado en memoria)";
            consola.establecerColor(0, 2);
            irA(18, PANEL_COL); cout << "Correo: " << this->correo << " (Cargado en memoria)";
            consola.establecerColor(0, 2);
            irA(24, PANEL_COL); cout << "\033[92m>> Bienvenido nuevamente, " << this->nombre << "! (Acceso instantaneo)\033[0m";
        }
        else {
            irA(16, PANEL_COL); cout << "Nombre nuevo: "; getline(cin, nombre);

            auto validarCorreo = [](string email) -> bool {
                return email.find('@') != string::npos && email.find(".com") != string::npos;
                };

            bool correoValido = false;
            do {
                irA(18, PANEL_COL); cout << string(50, ' ');
                irA(18, PANEL_COL); cout << "Correo: ";
                getline(cin, correo);
                if (validarCorreo(correo)) correoValido = true;
                else { irA(22, PANEL_COL); cout << "\033[91m>> Error: Formato de correo invalido.\033[0m"; }
            } while (!correoValido);

            irA(22, PANEL_COL); cout << string(60, ' ');

            inv.registrarNuevoCliente(this->nombre, this->correo, this->dni);
            irA(24, PANEL_COL); cout << "\033[92m>> Registro exitoso. Bienvenido " << this->nombre << "!\033[0m";
        }
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
        irA(4, PANEL_COL); cout << "\033[0m            \033[93m========================================================\033[0m";
        irA(5, PANEL_COL); cout << "\033[0m            \033[93m                   TU CARRITO ACTUAL                    \033[0m";
        irA(6, PANEL_COL); cout << "\033[0m            \033[93m========================================================\033[0m";

        int fila = 12;
        if (carrito->getCabeza() == nullptr) {
            irA(fila, PANEL_COL); cout << "\033[0m  El carrito esta vacio.";
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
                irA(fila++, PANEL_COL); cout << "\033[0m  " << i++ << ". " << p->nombre << " - S/. " << (int)p->precio;
                CalcularTotal(total, p->precio);
            }
            actual = actual->siguiente;
        }
        if (comprasRealizadas == 0 && total > 0) {
            CalcularTotalOferta(total);
            irA(fila++, PANEL_COL); cout << "\033[0m\033[93m  Descuento de bienvenida (15%) aplicado!\033[0m";
        }

        fila++;
        if (direccionEnvio.esValida()) {
            irA(fila++, PANEL_COL); cout << "\033[0m\033[96mDATOS DE ENTREGA:\033[0m";
            direccionEnvio.mostrar(fila);
            fila += 4;
        }
        else {
            irA(fila++, PANEL_COL); cout << "\033[0m\033[91m[!] Direccion no registrada. Se usara recojo en tienda.\033[0m";
        }

        irA(fila++, PANEL_COL); cout << "\033[0m--------------------------------------------------------";
        double montoIGV = calcularIGV(total);
        irA(fila++, PANEL_COL); cout << "\033[0m  Subtotal (sin IGV): S/. " << (int)(total - montoIGV);
        irA(fila++, PANEL_COL); cout << "\033[0m  IGV (18%):          S/. " << (int)montoIGV;
        irA(fila, PANEL_COL); cout << "\033[0m  \033[92mTOTAL A PAGAR:      S/. " << (int)total << "\033[0m";
    }

    void seleccionarMetodoPago(double total) {
        gestorEscenas grafica; grafica.setEscena(gestorEscenas::CATALOGO);
        limpiarZonaVerde();
        grafica.dibujarFondoSinLogo();

        irA(4, PANEL_COL); cout << "\033[0m            \033[95m========================================================\033[0m";
        irA(5, PANEL_COL); cout << "\033[0m            \033[95m                SELECCIONA METODO DE PAGO               \033[0m";
        irA(6, PANEL_COL); cout << "\033[0m            \033[95m========================================================\033[0m";

        irA(14, PANEL_COL); cout << "\033[0m    1. Tarjeta de Credito / Debito";
        irA(16, PANEL_COL); cout << "\033[0m    2. Yape / Plin (Codigo QR)";
        irA(20, PANEL_COL); cout << "\033[0m    Opcion: ";
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
            irA(18, PANEL_COL); cout << "\033[0m  El carrito esta vacio. Agrega productos primero.";
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

        // Check gigante dibujado con bloques + mensaje parpadeante
        cout << "\033[92m";
        irA(14, PANEL_COL + 34); cout << (char)219 << (char)219;
        irA(15, PANEL_COL + 32); cout << (char)219 << (char)219;
        irA(16, PANEL_COL + 22); cout << (char)219 << (char)219; irA(16, PANEL_COL + 30); cout << (char)219 << (char)219;
        irA(17, PANEL_COL + 24); cout << (char)219 << (char)219 << (char)219 << (char)219 << (char)219 << (char)219;
        irA(18, PANEL_COL + 26); cout << (char)219 << (char)219;
        cout << "\033[0m";

        for (int i = 0; i < 3; i++) {
            irA(21, PANEL_COL + 10); cout << "\033[92m>> Compra procesada con exito. Gracias por preferirnos!\033[0m";
            Sleep(250);
            irA(21, PANEL_COL + 10); cout << string(56, ' ');
            Sleep(150);
        }
        irA(21, PANEL_COL + 10); cout << "\033[92m>> Compra procesada con exito. Gracias por preferirnos!\033[0m";
        pausaRetroceder(24);
    }

    void menuBuscarProducto(Inventario& inv) {
        gestorEscenas grafica;
        grafica.setEscena(gestorEscenas::CATALOGO);

        while (true) {
            limpiarZonaVerde();
            grafica.dibujarFondoSinLogo();

            irA(2, PANEL_COL); cout << "\033[0m\033[93m   ______ \033[92m   ___      __  ___    ___     ____   ___     _  __     ___     ____ \033[0m";
            irA(3, PANEL_COL); cout << "\033[0m\033[93m  /     /|\033[92m  /   |    /  |/  /   /   |   /_  /  / _ \\   / |/ /    /   |   / __/ \033[0m";
            irA(4, PANEL_COL); cout << "\033[0m\033[93m /_____/ |\033[92m / /| |   / /|_/ /   / /| |    / /  / / / / /    /    / /| |  \\__ \\ \033[0m";
            irA(5, PANEL_COL); cout << "\033[0m\033[93m |     | |\033[92m / ___ |  / /  / /   / ___ |   / /_/ /_/ / / /|  /   / ___ | ___/ /  \033[0m";
            irA(6, PANEL_COL); cout << "\033[0m\033[93m |_____|/ \033[92m /_/  |_| /_/  /_/   /_/  |_|  /___/ \\____/ /_/ |_/   /_/  |_|/____/  \033[0m";

            irA(12, PANEL_COL); cout << "\033[0m    1. Ver Catalogo Completo (Agregar por ID)";
            irA(14, PANEL_COL); cout << "\033[0m    2. Buscar producto por nombre especifico";
            irA(16, PANEL_COL); cout << "\033[0m    3. Ver Catalogo ordenado por Precio";
            irA(18, PANEL_COL); cout << "\033[0m    4. Registrar Direccion de Envio";
            irA(20, PANEL_COL); cout << "\033[0m    5. Dejar Resena de un Producto";
            irA(22, PANEL_COL); cout << "\033[0m    6. Soporte - Reportar problema con pedido";
            irA(26, PANEL_COL); cout << "\033[0m    \033[93m[ESC] Volver al Menu Principal\033[0m";

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
                irA(14, PANEL_COL); cout << "\033[0m  Ingrese nombre del producto: ";
                string nom; cin.ignore(); getline(cin, nom);

                limpiarZonaVerde();
                grafica.dibujarFondoSinLogo();
                inv.buscarPorNombre(nom);

                irA(32, PANEL_COL); cout << "\033[0m  >> Ingrese el ID del producto que desea: ";
                string idStr; cin >> idStr;

                if (idStr.find_first_not_of("0123456789") == string::npos) {
                    int idBuscado = stoi(idStr);
                    Producto* p = inv.obtenerProducto(idBuscado);

                    if (p != nullptr && p->stock > 0) {
                        irA(34, PANEL_COL); cout << "\033[0m  Desea agregar '" << p->nombre << "' al carrito? (S/N): ";
                        char confirma = _getch();
                        if (confirma == 's' || confirma == 'S') {
                            carrito->agregar(idBuscado);
                            irA(36, PANEL_COL); cout << "\033[0m  \033[92m>> Agregado con exito!\033[0m";
                        }
                        else {
                            irA(36, PANEL_COL); cout << "\033[0m  \033[91m>> Operacion cancelada.\033[0m";
                        }
                    }
                    else {
                        irA(34, PANEL_COL); cout << "\033[0m  \033[91m>> Producto no encontrado o sin stock.\033[0m";
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
                if (direccionEnvio.esValida()) {
                    irA(24, PANEL_COL); cout << "\033[0m  \033[92m>> Direccion registrada correctamente.\033[0m";
                }
                pausaRetroceder(26);
            }
            else if (c == '5') {
                limpiarZonaVerde();
                grafica.dibujarFondoSinLogo();
                irA(12, PANEL_COL); cout << "\033[0m  ID del producto a resenar: ";
                string idStr; cin.ignore(); cin >> idStr;
                if (idStr.find_first_not_of("0123456789") == string::npos) {
                    int idProd = stoi(idStr);
                    Producto* p = inv.obtenerProducto(idProd);
                    if (p != nullptr) {
                        limpiarZonaVerde();
                        grafica.dibujarFondoSinLogo();
                        irA(12, PANEL_COL); cout << "\033[0m  Producto: " << p->nombre;
                        irA(14, PANEL_COL); cout << "\033[0m  Puntuacion (1-5): ";
                        int punt; irA(14, PANEL_COL + 20); cin >> punt;
                        irA(16, PANEL_COL); cout << "\033[0m  Comentario: ";
                        string comentario; cin.ignore(); irA(16, PANEL_COL + 14);
                        getline(cin, comentario);
                        inv.getResenas()->agregarResena(idProd, p->nombre, dni, comentario, punt);
                        irA(19, PANEL_COL); cout << "\033[0m  \033[92m>> Resena registrada. Gracias!\033[0m";
                    }
                    else {
                        irA(14, PANEL_COL); cout << "\033[0m  \033[91m>> Producto no encontrado.\033[0m";
                    }
                }
                pausaRetroceder(22);
            }
            else if (c == '6') {
                limpiarZonaVerde();
                grafica.dibujarFondoSinLogo();

                irA(4, PANEL_COL); cout << "\033[0m            \033[91m========================================================\033[0m";
                irA(5, PANEL_COL); cout << "\033[0m            \033[91m                  SOPORTE AL CLIENTE                    \033[0m";
                irA(6, PANEL_COL); cout << "\033[0m            \033[91m========================================================\033[0m";

                irA(12, PANEL_COL); cout << "\033[0m    1. Reportar un problema";
                irA(14, PANEL_COL); cout << "\033[0m    2. Solicitar devolucion de ultima compra";
                irA(16, PANEL_COL); cout << "\033[0m    3. Ver mis tickets";
                irA(20, PANEL_COL); cout << "\033[0m    Opcion: ";
                char op2 = _getch();

                if (op2 == '1') {
                    limpiarZonaVerde();
                    grafica.dibujarFondoSinLogo();
                    irA(12, PANEL_COL); cout << "\033[0m  Asunto: ";
                    string asunto; cin.ignore(); irA(12, PANEL_COL + 10); getline(cin, asunto);
                    irA(14, PANEL_COL); cout << "\033[0m  Descripcion del problema: ";
                    string desc; irA(14, PANEL_COL + 28); getline(cin, desc);
                    inv.getSoporte()->abrirTicket(dni, nombre, asunto, desc);
                    irA(17, PANEL_COL); cout << "\033[0m  \033[92m>> Ticket abierto. Un agente lo revisara pronto.\033[0m";
                }
                else if (op2 == '2') {
                    limpiarZonaVerde();
                    grafica.dibujarFondoSinLogo();

                    auto validarIdentidad = [&](string nomIngresado, string correoIngresado, string dniIngresado) -> bool {
                        return dniIngresado == dni &&
                            nomIngresado == nombre &&
                            correoIngresado == correo;
                        };

                    irA(4, PANEL_COL); cout << "\033[0m            \033[93m========================================================\033[0m";
                    irA(5, PANEL_COL); cout << "\033[0m            \033[93m          VERIFICACION DE IDENTIDAD - REEMBOLSO         \033[0m";
                    irA(6, PANEL_COL); cout << "\033[0m            \033[93m========================================================\033[0m";

                    irA(12, PANEL_COL); cout << "\033[0m  Confirme sus datos para continuar:";
                    irA(14, PANEL_COL); cout << "\033[0m  Nombre : "; string nomV; cin.ignore(); irA(14, PANEL_COL + 11); getline(cin, nomV);
                    irA(15, PANEL_COL); cout << "\033[0m  Correo : "; string correoV;           irA(15, PANEL_COL + 11); getline(cin, correoV);
                    irA(16, PANEL_COL); cout << "\033[0m  DNI    : "; string dniV;              irA(16, PANEL_COL + 11); getline(cin, dniV);

                    if (!validarIdentidad(nomV, correoV, dniV)) {
                        limpiarZonaVerde();
                        grafica.dibujarFondoSinLogo();
                        irA(16, PANEL_COL); cout << "\033[0m  \033[91m>> Datos incorrectos. No se puede procesar el reembolso.\033[0m";
                        pausaRetroceder(19);
                    }
                    else {
                        limpiarZonaVerde();
                        grafica.dibujarFondoSinLogo();
                        vector<Venta> misCompras = inv.obtenerVentasPorCliente(dni);
                        if (misCompras.empty()) {
                            irA(14, PANEL_COL); cout << "\033[0m  \033[91m>> No tienes compras registradas aun.\033[0m";
                        }
                        else {
                            irA(4, PANEL_COL); cout << "\033[0m            \033[96m========================================================\033[0m";
                            irA(5, PANEL_COL); cout << "\033[0m            \033[96m               TUS COMPRAS REGISTRADAS                  \033[0m";
                            irA(6, PANEL_COL); cout << "\033[0m            \033[96m========================================================\033[0m";

                            int fila = 12;
                            for (int i = 0; i < (int)misCompras.size() && fila < 35; i++) {
                                string item = "  [" + to_string(i + 1) + "] " +
                                    misCompras[i].producto + " - S/. " +
                                    to_string((int)misCompras[i].precio) +
                                    " | " + misCompras[i].fechaTexto;
                                imprimirEnPanel(fila++, item);
                            }
                            irA(36, PANEL_COL); cout << "\033[0m  Ingrese numeros de compras a devolver (ej: 1 2 3), 0 para cancelar: ";
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
                                irA(14, PANEL_COL); cout << "\033[0m  \033[92m>> Reembolso solicitado correctamente.\033[0m";
                                irA(15, PANEL_COL); cout << "\033[0m  \033[92m>> Un agente revisara y confirmara cada devolucion.\033[0m";
                            }
                            else {
                                irA(14, PANEL_COL); cout << "\033[0m  \033[91m>> No se selecciono ninguna compra.\033[0m";
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

            irA(4, PANEL_COL); cout << "\033[0m            \033[93m========================================================\033[0m";
            irA(5, PANEL_COL); cout << "\033[0m            \033[93m               --- MI CARRITO DE COMPRAS ---            \033[0m";
            irA(6, PANEL_COL); cout << "\033[0m            \033[93m========================================================\033[0m";

            irA(12, PANEL_COL); cout << "\033[0m    1. Ver lista de productos anadidos";
            irA(14, PANEL_COL); cout << "\033[0m    2. Proceder al pago final";
            irA(16, PANEL_COL); cout << "\033[0m    3. Historial de pedidos - Orden por Monto";
            irA(20, PANEL_COL); cout << "\033[0m    \033[93m[ESC] Volver al Menu Principal\033[0m";

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
        transicionCortina(); // entrada con efecto de barrido

        // Mascota decorativa del menu del cliente
        Matriz mascota;
        mascota.inicializar(mascotaPollitoCuerpo);

        while (!historialNavegacion.estaVacia()) {
            int pantallaActual = historialNavegacion.obtenerTope();
            limpiarZonaVerde();
            grafica.dibujarFondoSinLogo();

            if (pantallaActual == MENU_PRINCIPAL) {
                mascota.dibujarMatriz(88, 12);
                dibujarMarco(4, PANEL_COL + 12, 56, 3, "94");
                string saludo = "BIENVENIDO AL MARKETPLACE, " + nombre;
                irA(5, PANEL_COL + 12 + 2); cout << "\033[94m" << saludo << "\033[0m";

                irA(12, PANEL_COL); cout << "\033[0m    1. Entrar a la Tienda (Catalogo y Busqueda)";
                irA(14, PANEL_COL); cout << "\033[0m    2. Gestionar mi Carrito de Compras";
                irA(16, PANEL_COL); cout << "\033[0m    3. Que me alcanza? (Asistente de Presupuesto)";
                irA(20, PANEL_COL); cout << "\033[0m    \033[93m[ESC] Salir del Sistema de Cliente\033[0m";

                // El pollito parpadea mientras esperamos que el usuario elija.
                // Su ojo es la celda de la columna 6, filas 1 y 2 de la matriz
                // (dibujada en la posicion 88,12 de la pantalla).
                int tiempo = 0;
                bool ojoCerrado = false;
                HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
                while (!_kbhit()) {
                    Sleep(50);
                    tiempo += 50;
                    if (tiempo >= 700) {
                        tiempo = 0;
                        ojoCerrado = !ojoCerrado;
                        SetConsoleTextAttribute(hCon, ojoCerrado ? 14 : 0);
                        gotoxy(88 + 6, 12 + 1); cout << (char)219;
                        gotoxy(88 + 6, 12 + 2); cout << (char)219;
                    }
                }
                int c = _getch();
                if (c == '1') historialNavegacion.apilar(MENU_BUSQUEDA);
                else if (c == '2') historialNavegacion.apilar(MENU_CARRITO);
                else if (c == '3') historialNavegacion.apilar(MENU_PRESUPUESTO);
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
            else if (pantallaActual == MENU_PRESUPUESTO) {
                float presupuesto;
                irA(10, PANEL_COL); cout << "\033[0m  Cuanto dinero tienes? S/. "; cin >> presupuesto;
                if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); presupuesto = 0; }
                limpiarZonaVerde();
                grafica.dibujarFondoSinLogo();
                inv.asistentePresupuesto(presupuesto);
                pausaRetroceder(38);
                historialNavegacion.desapilar();
            }
        }
    }
};