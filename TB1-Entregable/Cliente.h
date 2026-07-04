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
#include <Windows.h>

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
        cout << "\033[93m>> Presione ESC para volver atras...\033[0;42;97m";
        while (true) {
            if (_kbhit()) {
                int c = _getch();
                if (c == 27) break;
            }
        }
        irA(fila, PANEL_COL);
        cout << string(60, ' ');
    }

    void aplicarColorBasico(int color) {
        HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hCon, color);
    }

public:
    Cliente() { carrito = new ListaEnlazada<int>(); }
    ~Cliente() { delete carrito; }

    void login(Inventario& inv) {
        gestorEscenas grafica;
        grafica.setEscena(gestorEscenas::CATALOGO);

        limpiarPantalla();
        grafica.dibujarFondoSinLogo();

        irA(4, PANEL_COL); cout << "\033[0;42;97m            \033[96m========================================================\033[0;42;97m";
        irA(5, PANEL_COL); cout << "\033[0;42;97m            \033[96m                    LOGIN DE CLIENTE                    \033[0;42;97m";
        irA(6, PANEL_COL); cout << "\033[0;42;97m            \033[96m========================================================\033[0;42;97m";

        auto validarDNI = [](string d) -> bool {
            return d.length() == 8 && d.find_first_not_of("0123456789") == string::npos;
            };

        // El cin >> opcion del menu principal deja un Enter pendiente en el buffer;
        // si no se limpia, getline lo lee como DNI vacio y sale "DNI invalido" sin escribir nada
        cin.ignore(10000, '\n');

        bool primeraVez = true;
        bool dniValido = false;
        do {
            if (!primeraVez) {
                irA(22, PANEL_COL); cout << "\033[91m>> Error: DNI invalido. Debe tener exactamente 8 digitos numericos.\033[0;42;97m";
            }
            primeraVez = false;

            irA(14, PANEL_COL); cout << string(60, ' ');
            irA(14, PANEL_COL); cout << "\033[0;42;97mDNI: ";
            getline(cin, dni);
            if (validarDNI(dni)) dniValido = true;
        } while (!dniValido);

        irA(22, PANEL_COL); cout << string(70, ' ');

        RegistroCliente* clienteEncontrado = inv.buscarClienteHash(dni);

        if (clienteEncontrado != nullptr) {
            this->nombre = std::string(clienteEncontrado->nombre);
            this->correo = std::string(clienteEncontrado->correo);

            irA(16, PANEL_COL); cout << "\033[0;42;97mNombre: " << this->nombre << " (Cargado en memoria)";
            irA(18, PANEL_COL); cout << "\033[0;42;97mCorreo: " << this->correo << " (Cargado en memoria)";
            irA(24, PANEL_COL); cout << "\033[92m>> Bienvenido nuevamente, " << this->nombre << "! (Acceso instantaneo)\033[0;42;97m";
        }
        else {
            irA(16, PANEL_COL); cout << "\033[0;42;97mNombre nuevo: "; getline(cin, nombre);

            auto validarCorreo = [](string email) -> bool {
                return email.find('@') != string::npos && email.find(".com") != string::npos;
                };

            bool primeraVezCorreo = true;
            bool correoValido = false;
            do {
                if (!primeraVezCorreo) {
                    irA(22, PANEL_COL); cout << "\033[91m>> Error: Formato de correo invalido.\033[0;42;97m";
                }
                primeraVezCorreo = false;

                irA(18, PANEL_COL); cout << string(60, ' ');
                irA(18, PANEL_COL); cout << "\033[0;42;97mCorreo: ";
                getline(cin, correo);
                if (validarCorreo(correo)) correoValido = true;
            } while (!correoValido);

            irA(22, PANEL_COL); cout << string(60, ' ');

            inv.registrarNuevoCliente(this->nombre, this->correo, this->dni);
            irA(24, PANEL_COL); cout << "\033[92m>> Registro masivo exitoso. Bienvenido " << this->nombre << "!\033[0;42;97m";
        }
        pausa();
    }

    void bucleAgregarRapido(Inventario& inv, int filaInicio) {
        irA(filaInicio, PANEL_COL);
        cout << "\033[0;42;97m>> Ingrese ID para agregar. [\033[93mESC\033[0;42;97m] para terminar.";

        int filaMsg = filaInicio + 2;
        while (true) {
            irA(filaMsg, PANEL_COL); cout << string(50, ' ');
            irA(filaMsg, PANEL_COL); cout << "\033[0;42;97mID: ";
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
                cout << "\033[92m>> '" << p->nombre << "' agregado! Stock: " << p->stock << "          \033[0;42;97m";
            }
            else {
                cout << "\033[91m>> ID invalido o sin stock.                         \033[0;42;97m";
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
        irA(4, PANEL_COL); cout << "\033[0;42;97m            \033[93m========================================================\033[0;42;97m";
        irA(5, PANEL_COL); cout << "\033[0;42;97m            \033[93m                   TU CARRITO ACTUAL                    \033[0;42;97m";
        irA(6, PANEL_COL); cout << "\033[0;42;97m            \033[93m========================================================\033[0;42;97m";

        int fila = 12;
        if (carrito->getCabeza() == nullptr) {
            irA(fila, PANEL_COL); cout << "\033[0;42;97m  El carrito esta vacio.";
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
                irA(fila++, PANEL_COL); cout << "\033[0;42;97m  " << i++ << ". " << p->nombre << " - S/. " << (int)p->precio;
                CalcularTotal(total, p->precio);
            }
            actual = actual->siguiente;
        }
        if (comprasRealizadas == 0 && total > 0) {
            CalcularTotalOferta(total);
            irA(fila++, PANEL_COL); cout << "\033[93m  Descuento de bienvenida (15%) aplicado!\033[0;42;97m";
        }

        fila++;
        if (direccionEnvio.esValida()) {
            irA(fila++, PANEL_COL); cout << "\033[96mDATOS DE ENTREGA:\033[0;42;97m";
            direccionEnvio.mostrar(fila);
            fila += 4;
        }
        else {
            irA(fila++, PANEL_COL); cout << "\033[91m[!] Direccion no registrada. Se usara recojo en tienda.\033[0;42;97m";
        }

        irA(fila++, PANEL_COL); cout << "\033[0;42;97m--------------------------------------------------------";
        double montoIGV = calcularIGV(total);
        irA(fila++, PANEL_COL); cout << "\033[0;42;97m  Subtotal (sin IGV): S/. " << (int)(total - montoIGV);
        irA(fila++, PANEL_COL); cout << "\033[0;42;97m  IGV (18%):          S/. " << (int)montoIGV;
        irA(fila, PANEL_COL); cout << "\033[92m  TOTAL A PAGAR:      S/. " << (int)total << "\033[0;42;97m";
    }

    void seleccionarMetodoPago(double total) {
        gestorEscenas grafica; grafica.setEscena(gestorEscenas::CATALOGO);
        limpiarZonaVerde();
        grafica.dibujarFondoSinLogo();

        irA(4, PANEL_COL); cout << "\033[0;42;97m            \033[95m========================================================\033[0;42;97m";
        irA(5, PANEL_COL); cout << "\033[0;42;97m            \033[95m                SELECCIONA METODO DE PAGO               \033[0;42;97m";
        irA(6, PANEL_COL); cout << "\033[0;42;97m            \033[95m========================================================\033[0;42;97m";

        irA(14, PANEL_COL); cout << "\033[0;42;97m    1. Tarjeta de Credito / Debito";
        irA(16, PANEL_COL); cout << "\033[0;42;97m    2. Yape / Plin (Codigo QR)";
        irA(20, PANEL_COL); cout << "\033[0;42;97m    Opcion: ";
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
            irA(18, PANEL_COL); cout << "\033[0;42;97m  El carrito esta vacio. Agrega productos primero.";
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
        irA(20, PANEL_COL); cout << "  \033[92m>> Compra procesada con exito. Gracias por preferirnos!\033[0;42;97m";
        pausaRetroceder(24);
    }

    void menuBuscarProducto(Inventario& inv) {
        gestorEscenas grafica;
        grafica.setEscena(gestorEscenas::CATALOGO);

        while (true) {
            limpiarZonaVerde();
            grafica.dibujarFondoSinLogo();

            irA(2, PANEL_COL); cout << "\033[93m   ______ \033[92m   ___      __  ___    ___     ____   ___     _  __     ___     ____ \033[0;42;97m";
            irA(3, PANEL_COL); cout << "\033[93m  /     /|\033[92m  /   |    /  |/  /   /   |   /_  /  / _ \\   / |/ /    /   |   / __/ \033[0;42;97m";
            irA(4, PANEL_COL); cout << "\033[93m /_____/ |\033[92m / /| |   / /|_/ /   / /| |    / /  / / / / /    /    / /| |  \\__ \\ \033[0;42;97m";
            irA(5, PANEL_COL); cout << "\033[93m |     | |\033[92m / ___ |  / /  / /   / ___ |   / /_/ /_/ / / /|  /   / ___ | ___/ /  \033[0;42;97m";
            irA(6, PANEL_COL); cout << "\033[93m |_____|/ \033[92m /_/  |_| /_/  /_/   /_/  |_|  /___/ \\____/ /_/ |_/   /_/  |_|/____/  \033[0;42;97m";

            irA(12, PANEL_COL); cout << "\033[0;42;97m    1. Ver Catalogo Completo (Agregar por ID)";
            irA(14, PANEL_COL); cout << "\033[0;42;97m    2. Buscar producto por nombre especifico";
            irA(16, PANEL_COL); cout << "\033[0;42;97m    3. Ver Catalogo ordenado por Precio";
            irA(18, PANEL_COL); cout << "\033[0;42;97m    4. Registrar Direccion de Envio";
            irA(20, PANEL_COL); cout << "\033[0;42;97m    5. Dejar Resena de un Producto";
            irA(22, PANEL_COL); cout << "\033[0;42;97m    6. Soporte - Reportar problema con pedido";

            // NUEVO: Opción 7 con resaltado interactivo
            irA(24, PANEL_COL); cout << "    \033[0;42;97m7. ";
            aplicarColorBasico(14 + (2 * 16)); cout << "Deseas que te recomiende un producto?"; aplicarColorBasico(0 + (2 * 16));

            irA(28, PANEL_COL); cout << "    ";
            aplicarColorBasico(14 + (2 * 16)); cout << "[ESC] Volver al Menu Principal"; aplicarColorBasico(0 + (2 * 16));

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
                irA(14, PANEL_COL); cout << "\033[0;42;97m  Ingrese nombre del producto: ";
                string nom; cin.ignore(); getline(cin, nom);

                limpiarZonaVerde();
                grafica.dibujarFondoSinLogo();
                inv.buscarPorNombre(nom);

                irA(32, PANEL_COL); cout << "\033[0;42;97m  >> Ingrese el ID del producto que desea: ";
                string idStr; cin >> idStr;

                if (idStr.find_first_not_of("0123456789") == string::npos) {
                    int idBuscado = stoi(idStr);
                    Producto* p = inv.obtenerProducto(idBuscado);

                    if (p != nullptr && p->stock > 0) {
                        irA(34, PANEL_COL); cout << "\033[0;42;97m  Desea agregar '" << p->nombre << "' al carrito? (S/N): ";
                        char confirma = _getch();
                        if (confirma == 's' || confirma == 'S') {
                            carrito->agregar(idBuscado);
                            irA(36, PANEL_COL); cout << "  \033[92m>> Agregado con exito!\033[0;42;97m";
                        }
                        else {
                            irA(36, PANEL_COL); cout << "  \033[91m>> Operacion cancelada.\033[0;42;97m";
                        }
                    }
                    else {
                        irA(34, PANEL_COL); cout << "  \033[91m>> Producto no encontrado o sin stock.\033[0;42;97m";
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
                    irA(24, PANEL_COL); cout << "  \033[92m>> Direccion registrada correctamente.\033[0;42;97m";
                }
                pausaRetroceder(26);
            }
            else if (c == '5') {
                limpiarZonaVerde();
                grafica.dibujarFondoSinLogo();
                irA(12, PANEL_COL); cout << "\033[0;42;97m  ID del producto a resenar: ";
                string idStr; cin.ignore(); cin >> idStr;
                if (idStr.find_first_not_of("0123456789") == string::npos) {
                    int idProd = stoi(idStr);
                    Producto* p = inv.obtenerProducto(idProd);
                    if (p != nullptr) {
                        limpiarZonaVerde();
                        grafica.dibujarFondoSinLogo();
                        irA(12, PANEL_COL); cout << "\033[0;42;97m  Producto: " << p->nombre;
                        irA(14, PANEL_COL); cout << "\033[0;42;97m  Puntuacion (1-5): ";
                        int punt; irA(14, PANEL_COL + 20); cin >> punt;
                        irA(16, PANEL_COL); cout << "\033[0;42;97m  Comentario: ";
                        string comentario; cin.ignore(); irA(16, PANEL_COL + 14);
                        getline(cin, comentario);
                        inv.getResenas()->agregarResena(idProd, p->nombre, dni, comentario, punt);
                        irA(19, PANEL_COL); cout << "  \033[92m>> Resena registrada. Gracias!\033[0;42;97m";
                    }
                    else {
                        irA(14, PANEL_COL); cout << "  \033[91m>> Producto no encontrado.\033[0;42;97m";
                    }
                }
                pausaRetroceder(22);
            }
            else if (c == '6') {
                limpiarZonaVerde();
                grafica.dibujarFondoSinLogo();

                irA(4, PANEL_COL); cout << "\033[0;42;97m            \033[91m========================================================\033[0;42;97m";
                irA(5, PANEL_COL); cout << "\033[0;42;97m            \033[91m                  SOPORTE AL CLIENTE                    \033[0;42;97m";
                irA(6, PANEL_COL); cout << "\033[0;42;97m            \033[91m========================================================\033[0;42;97m";

                irA(12, PANEL_COL); cout << "\033[0;42;97m    1. Reportar un problema";
                irA(14, PANEL_COL); cout << "\033[0;42;97m    2. Solicitar devolucion de ultima compra";
                irA(16, PANEL_COL); cout << "\033[0;42;97m    3. Ver mis tickets";
                irA(20, PANEL_COL); cout << "\033[0;42;97m    Opcion: ";
                char op2 = _getch();

                if (op2 == '1') {
                    limpiarZonaVerde();
                    grafica.dibujarFondoSinLogo();
                    irA(12, PANEL_COL); cout << "\033[0;42;97m  Asunto: ";
                    string asunto; cin.ignore(); irA(12, PANEL_COL + 10); getline(cin, asunto);
                    irA(14, PANEL_COL); cout << "\033[0;42;97m  Descripcion del problema: ";
                    string desc; irA(14, PANEL_COL + 28); getline(cin, desc);
                    inv.getSoporte()->abrirTicket(dni, nombre, asunto, desc);
                    irA(17, PANEL_COL); cout << "  \033[92m>> Ticket abierto. Un agente lo revisara pronto.\033[0;42;97m";
                }
                else if (op2 == '2') {
                    limpiarZonaVerde();
                    grafica.dibujarFondoSinLogo();

                    auto validarIdentidad = [&](string nomIngresado, string correoIngresado, string dniIngresado) -> bool {
                        return dniIngresado == dni &&
                            nomIngresado == nombre &&
                            correoIngresado == correo;
                        };

                    irA(4, PANEL_COL); cout << "\033[0;42;97m            \033[93m========================================================\033[0;42;97m";
                    irA(5, PANEL_COL); cout << "\033[0;42;97m            \033[93m          VERIFICACION DE IDENTIDAD - REEMBOLSO         \033[0;42;97m";
                    irA(6, PANEL_COL); cout << "\033[0;42;97m            \033[93m========================================================\033[0;42;97m";

                    irA(12, PANEL_COL); cout << "\033[0;42;97m  Confirme sus datos para continuar:";
                    irA(14, PANEL_COL); cout << "\033[0;42;97m  Nombre : "; string nomV; cin.ignore(); irA(14, PANEL_COL + 11); getline(cin, nomV);
                    irA(15, PANEL_COL); cout << "\033[0;42;97m  Correo : "; string correoV;           irA(15, PANEL_COL + 11); getline(cin, correoV);
                    irA(16, PANEL_COL); cout << "\033[0;42;97m  DNI    : "; string dniV;              irA(16, PANEL_COL + 11); getline(cin, dniV);

                    if (!validarIdentidad(nomV, correoV, dniV)) {
                        limpiarZonaVerde();
                        grafica.dibujarFondoSinLogo();
                        irA(16, PANEL_COL); cout << "  \033[91m>> Datos incorrectos. No se puede procesar el reembolso.\033[0;42;97m";
                        pausaRetroceder(19);
                    }
                    else {
                        limpiarZonaVerde();
                        grafica.dibujarFondoSinLogo();
                        vector<Venta> misCompras = inv.obtenerVentasPorCliente(dni);
                        if (misCompras.empty()) {
                            irA(14, PANEL_COL); cout << "  \033[91m>> No tienes compras registradas aun.\033[0;42;97m";
                        }
                        else {
                            irA(4, PANEL_COL); cout << "\033[0;42;97m            \033[96m========================================================\033[0;42;97m";
                            irA(5, PANEL_COL); cout << "\033[0;42;97m            \033[96m               TUS COMPRAS REGISTRADAS                  \033[0;42;97m";
                            irA(6, PANEL_COL); cout << "\033[0;42;97m            \033[96m========================================================\033[0;42;97m";

                            int fila = 12;
                            for (int i = 0; i < (int)misCompras.size() && fila < 35; i++) {
                                string item = "  [" + to_string(i + 1) + "] " +
                                    misCompras[i].producto + " - S/. " +
                                    to_string((int)misCompras[i].precio) +
                                    " | " + misCompras[i].fechaTexto;
                                imprimirEnPanel(fila++, item);
                            }
                            irA(36, PANEL_COL); cout << "\033[0;42;97m  Ingrese numeros de compras a devolver (ej: 1 2 3), 0 para cancelar: ";
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
                                irA(14, PANEL_COL); cout << "  \033[92m>> Reembolso solicitado correctamente.\033[0;42;97m";
                                irA(15, PANEL_COL); cout << "  \033[92m>> Un agente revisara y confirmara cada devolucion.\033[0;42;97m";
                            }
                            else {
                                irA(14, PANEL_COL); cout << "  \033[91m>> No se selecciono ninguna compra.\033[0;42;97m";
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
            // =========================================================
            // NUEVO: OPCION 7 - MOTOR DE RECOMENDACION CON GRAFO
            // =========================================================
            else if (c == '7') {
                limpiarZonaVerde();
                grafica.dibujarFondoSinLogo();

                irA(12, PANEL_COL); aplicarColorBasico(14 + (2 * 16)); cout << "  ========================================================";
                irA(13, PANEL_COL); aplicarColorBasico(14 + (2 * 16)); cout << "                 RECOMENDACION DEL SISTEMA                ";
                irA(14, PANEL_COL); aplicarColorBasico(14 + (2 * 16)); cout << "  ========================================================";
                aplicarColorBasico(0 + (2 * 16)); // Restaurar negro sobre verde

                int cantidadVentas = 0;
                Producto* p = inv.obtenerRecomendacionDelGrafo(cantidadVentas);

                if (p != nullptr) {
                    irA(17, PANEL_COL); cout << "  Basado en las tendencias de compra de otros clientes,";
                    irA(18, PANEL_COL); cout << "  te recomendamos este producto estrella:";

                    aplicarColorBasico(10 + (2 * 16)); // Verde Brillante
                    irA(20, PANEL_COL); cout << "  >> " << p->nombre << " - S/. " << p->precio;
                    aplicarColorBasico(0 + (2 * 16));

                    irA(21, PANEL_COL); cout << "  (Ha sido comprado " << cantidadVentas << " veces por nuestra comunidad!)";

                    if (p->stock > 0) {
                        irA(24, PANEL_COL); cout << "  Deseas agregar '" << p->nombre << "' al carrito? (S/N): ";
                        char confirma = _getch();
                        if (confirma == 's' || confirma == 'S') {
                            carrito->agregar(p->id);
                            aplicarColorBasico(10 + (2 * 16));
                            irA(26, PANEL_COL); cout << "  >> Producto agregado con exito al carrito!";
                            aplicarColorBasico(0 + (2 * 16));
                        }
                        else {
                            aplicarColorBasico(12 + (2 * 16)); // Rojo Brillante
                            irA(26, PANEL_COL); cout << "  >> Operacion cancelada.";
                            aplicarColorBasico(0 + (2 * 16));
                        }
                    }
                    else {
                        aplicarColorBasico(12 + (2 * 16));
                        irA(24, PANEL_COL); cout << "  >> Lamentablemente este producto esta agotado por su alta demanda.";
                        aplicarColorBasico(0 + (2 * 16));
                    }
                }
                else {
                    irA(17, PANEL_COL); cout << "  Aun no hay suficientes datos de ventas para recomendarte algo.";
                }
                pausaRetroceder(29);
            }
        }
    }

    void menuCarrito(Inventario& inv) {
        gestorEscenas grafica;
        grafica.setEscena(gestorEscenas::CATALOGO);

        while (true) {
            limpiarZonaVerde();
            grafica.dibujarFondoSinLogo();

            irA(4, PANEL_COL); cout << "\033[0;42;97m            \033[93m========================================================\033[0;42;97m";
            irA(5, PANEL_COL); cout << "\033[0;42;97m            \033[93m               --- MI CARRITO DE COMPRAS ---            \033[0;42;97m";
            irA(6, PANEL_COL); cout << "\033[0;42;97m            \033[93m========================================================\033[0;42;97m";

            irA(12, PANEL_COL); cout << "\033[0;42;97m    1. Ver lista de productos anadidos";
            irA(14, PANEL_COL); cout << "\033[0;42;97m    2. Proceder al pago final";
            irA(16, PANEL_COL); cout << "\033[0;42;97m    3. Historial de pedidos - Orden por Monto";
            irA(20, PANEL_COL); cout << "    \033[93m[ESC] Volver al Menu Principal\033[0;42;97m";

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

        bool ojoCerrado = false; // Estado del easter egg de la mascota

        while (!historialNavegacion.estaVacia()) {
            int pantallaActual = historialNavegacion.obtenerTope();
            limpiarZonaVerde();
            grafica.dibujarFondoSinLogo();

            if (pantallaActual == MENU_PRINCIPAL) {
                irA(4, PANEL_COL); cout << "\033[0;42;97m            \033[94m========================================================\033[0;42;97m";

                string saludo = "             BIENVENIDO AL MARKETPLACE, " + nombre;
                while (saludo.length() < 56) saludo += " ";
                irA(5, PANEL_COL); cout << "\033[0;42;97m            \033[94m" << saludo << "\033[0;42;97m";
                irA(6, PANEL_COL); cout << "\033[0;42;97m            \033[94m========================================================\033[0;42;97m";

                irA(12, PANEL_COL); cout << "\033[0;42;97m    1. Entrar a la Tienda (Catalogo y Busqueda)";
                irA(14, PANEL_COL); cout << "\033[0;42;97m    2. Gestionar mi Carrito de Compras";
                irA(16, PANEL_COL); cout << "\033[0;42;97m    3. Mi Presupuesto y Filtros Inteligentes";
                irA(20, PANEL_COL); cout << "\033[0;42;97m    \033[93m[ESC] Salir del Sistema de Cliente\033[0;42;97m";

                // EASTER EGG: Interaccion con el ojito del fondo
                if (_kbhit()) {
                    int teclaEspecial = _getch();
                    if (teclaEspecial == 224) {
                        _getch();
                        HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
                        ojoCerrado = !ojoCerrado;
                        SetConsoleTextAttribute(hCon, ojoCerrado ? (14 + (2 * 16)) : (0 + (2 * 16)));
                        gotoxy(88 + 6, 12 + 1); cout << (char)219;
                        gotoxy(88 + 6, 12 + 2); cout << (char)219;
                        SetConsoleTextAttribute(hCon, 0 + (2 * 16)); // Restaurar
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
                limpiarZonaVerde();
                grafica.dibujarFondoSinLogo();
                irA(10, PANEL_COL); cout << "\033[0;42;97m  Cuanto dinero tienes? S/. "; cin >> presupuesto;
                if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); presupuesto = 0; }

                // Aquí podrías listar cosas que alcancen con ese presupuesto...
                irA(12, PANEL_COL); cout << "\033[92m  >> Procesando opciones para S/." << presupuesto << "...\033[0;42;97m";
                pausaRetroceder(15);
                historialNavegacion.desapilar();
            }
        }
    }
};