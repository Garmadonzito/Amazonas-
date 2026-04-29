#include <iostream>
#include <vector>
#include <string>
#include <windows.h> 

#include "Inventario.h"
#include "Cliente.h"
#include "Vendedor.h"

using namespace std;


void mostrarLogo() {
    // Mapa de la matriz 
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

    for (const string& fila : mapa) {
        cout << "    ";
        for (char c : fila) {
            if (c == 'R') cout << colorRojo << '#';
            else if (c == 'G') cout << colorVerde << '*';
            else if (c == 'B') cout << colorAzul << '#';
            else if (c == 'Y') cout << colorAmarillo << '@';
            else if (c == 'W') cout << colorBlanco << '#';
            else if (c == ' ') cout << ' ';
            else cout << colorBlanco << c;
        }
        cout << reset << endl;
    }

    cout << "\n    ==========================================================" << endl;
    cout << "    Presione ENTER para ingresar al Marketplace...";
    cin.get();
}

int main() {

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    // ============================================================

    limpiarPantalla(); 
    mostrarLogo();

   
    Inventario* miTienda = new Inventario();
    Vendedor* admin = new Vendedor();
    Cliente* user = new Cliente();

    int op;
    do {
        limpiarPantalla();
        cout << "========================================\n";
        cout << "        AMAZONAS - INICIO\n";
        cout << "========================================\n";
        cout << "Que tipo de usuario eres?\n";
        cout << "1. Cliente\n";
        cout << "2. Vendedor (Empieza por aqui para anadir stock)\n";
        cout << "3. Salir\n";
        cout << "Opcion: "; cin >> op;

        switch (op) {
        case 1: {
            limpiarPantalla();
            user->login();
            user->menu(*miTienda);
            break;
        }
        case 2: {
            limpiarPantalla();
            if (admin->login()) {
                admin->menu(*miTienda);
            }
            break;
        }
        }
    } while (op != 3);

  
    delete miTienda;
    delete admin;
    delete user;

    limpiarPantalla();
    cout << "\nHasta pronto!\n";
    return 0;
}