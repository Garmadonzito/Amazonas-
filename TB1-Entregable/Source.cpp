#include <iostream>
#include <vector>
#include <string>
#include <windows.h> // Necesario para habilitar colores ANSI en la consola de Windows

#include "Inventario.h"
#include "Cliente.h"
#include "Vendedor.h"

using namespace std;

// Función que dibuja el logo en pantalla usando el mapa compacto
void mostrarLogo() {
    // Mapa de la matriz compacta (16 filas x 65 columnas)
    // R=Rojo, G=Verde, B=Azul, Y=Amarillo, W=Blanco
    vector<string> mapa = {
        "                                                               ",
        "                                 GGG       GGG                 ",
        "         R                      GGGGG     GGGGG          G     ",
        "        RRR        A           GGGGGGG   GGGGGGG        GGG    ",
        "       RWRR       A A   M   M  A   A ZZZZZ OOO  N   N A   A SSSS",
        "       RRRR      A   A  MM MM  A   A    Z  O O  NN  N A   A S   ",
        "        BB       AAAAA  M M M  AAAAA   Z   O O  N N N AAAAA  SS ",
        "        BB       A   A  M   M  A   A  Z    O O  N  NN A   A    S",
        "                 A   A  M   M  A   A ZZZZZ OOO  N   N A   A SSSS",
        "        YY                                                     ",
        "         YY                                                 Y  ",
        "           YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY   ",
        "                                                               ",
        "  R W R                                                        ",
        "  R W R    Bienvenido a nuestra tienda Amazonas                ",
        "  R W R                                                        "
    };

    // Códigos ANSI para los colores
    string colorRojo = "\033[1;31m";
    string colorVerde = "\033[1;32m";
    string colorAmarillo = "\033[1;33m";
    string colorAzul = "\033[1;34m";
    string colorBlanco = "\033[1;37m";
    string reset = "\033[0m";

    cout << "\nCargando sistema Amazonas...\n\n";

    // Recorremos el mapa y pintamos según la letra
    for (const string& fila : mapa) {
        cout << "    "; // Pequeño margen izquierdo para centrar
        for (char c : fila) {
            if (c == 'R') cout << colorRojo << '#';
            else if (c == 'G') cout << colorVerde << '*';
            else if (c == 'B') cout << colorAzul << '#';
            else if (c == 'Y') cout << colorAmarillo << '@';
            else if (c == 'W') cout << colorBlanco << '#';
            else if (c == ' ') cout << ' ';
            else cout << colorBlanco << c; // Para las letras del título
        }
        cout << reset << endl;
    }

    cout << "\n    ==========================================================" << endl;
    cout << "    Presione ENTER para ingresar al Marketplace...";
    cin.get();
}

int main() {
    // === CONFIGURACIÓN DE CONSOLA PARA HABILITAR COLORES ANSI ===
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    // ============================================================

    limpiarPantalla(); // Función definida en tu Usuario.h
    mostrarLogo();     // Muestra la matriz dibujada de forma compacta

    Inventario miInventario;
    miInventario.cargarProductosIniciales();

    int op;
    do {
        limpiarPantalla();
        cout << "========================================\n";
        cout << "        AMAZONAS - INICIO\n";
        cout << "========================================\n";
        cout << "Que tipo de usuario eres?\n";
        cout << "1. Cliente\n";
        cout << "2. Vendedor\n";
        cout << "3. Salir\n";
        cout << "Opcion: "; cin >> op;

        switch (op) {
        case 1: {
            limpiarPantalla();
            Cliente clienteActivo;
            clienteActivo.login();
            clienteActivo.menu(miInventario);
            break;
        }
        case 2: {
            limpiarPantalla();
            Vendedor vendedorActivo;
            if (vendedorActivo.login()) {
                vendedorActivo.menu(miInventario);
            }
            break;
        }
        }
    } while (op != 3);

    limpiarPantalla();
    cout << "\nHasta pronto!\n";
    return 0;
}