#include <iostream>
#include <vector>
#include <string>
#include <windows.h> 

#include "Inventario.h"
#include "Cliente.h"
#include "Vendedor.h"

using namespace std;

void mostrarLogo() {
    // Dimensiones corregidas y exactas
    const int FILAS = 23;
    const int COLS = 80;

    // Motor de Pixel Art - Matriz Numérica Estricta
    // 0: Negro/Fondo, 1: Blanco (Texto/Salpicadura), 2: Rojo (Bandera), 3: Magenta Oscuro (Cielo)
    // 4: Purpura (Horizonte/Scanlines), 5: Amarillo (Sol alto/Flecha), 6: Naranja (Sol bajo)
    // 7: Verde (Selva), 8: Cian (Rio base), 9: Azul (Rio olas), 10: Rosa (Delfines)
    int matriz_c[FILAS][COLS] = {
        // [0-2] Cielo Superior Magenta y Banderas de Peru
        {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
        {3,3,3,3, 2,2,1,1,1,2,2, 3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3, 2,2,1,1,1,2,2, 3,3,3,3},
        {3,3,3,3, 2,2,1,1,1,2,2, 3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3, 5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5, 3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3, 2,2,1,1,1,2,2, 3,3,3,3},

        // [3-5] Sol Amarillo Central y Palmeras
        {3,3,3,3,3,3,3,3,3,3,3,3,3, 7,7,7,7, 3,3,3,3,3,3,3,3,3,3,3,3,3, 5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5, 3,3,3,3,3,3,3,3,3,3,3,3,3, 7,7,7,7, 3,3,3,3,3,3,3,3,3,3,3,3,3},
        {3,3,3,3,3,3,3,3,3,3,3,3, 7,7,7,7,7,7, 3,3,3,3,3,3,3,3,3,3,3, 5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5, 3,3,3,3,3,3,3,3,3,3,3, 7,7,7,7,7,7, 3,3,3,3,3,3,3,3,3,3,3,3},
        {3,3,3,3,3,3,3,3,3,3,3,3,3, 7,7,7,7, 3,3,3,3,3,3,3,3,3,3,3, 5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5, 3,3,3,3,3,3,3,3,3,3,3, 7,7,7,7, 3,3,3,3,3,3,3,3,3,3,3,3,3},

        // [6-9] Scanlines (Lineas de Escaneo) y Sol Naranja Inferior
        {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4},
        {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4, 6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6, 4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4},
        {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4},
        {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4, 6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6, 4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4},

        // [10] Separador Purpura Horizonte
        {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4},

        // [11-14] Texto AMAZONAS
        {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4, 1,1,1,4,1,4,1,4,1,1,1,4,1,1,1,4,1,1,1,4,1,1,1,4,1,1,1,4,1,1,1, 4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4},
        {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4, 1,4,1,4,1,1,1,4,1,4,1,4,4,4,1,4,1,4,1,4,1,1,4,4,1,4,1,4,1,4,4, 4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4},
        {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4, 1,1,1,4,1,4,1,4,1,1,1,4,4,1,4,4,1,4,1,4,1,4,1,4,1,1,1,4,4,1,1, 4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4},
        {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4, 1,4,1,4,1,4,1,4,1,4,1,4,1,1,1,4,1,1,1,4,1,4,1,4,1,4,1,4,1,1,1, 4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4},

        // [15-17] Sonrisa Amazon y Delfines Rosados saltando (10)
        {4,4,4,4,4,4,4,4, 4,4,4,4,4,10,10,4,4,4, 4,4,4,4,4,4,4,4, 5, 4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4, 5, 4,4,4,4,4,4,4,4, 4,4,4,10,10,4,4,4,4,4, 4,4,4,4,4,4,4,4},
        {4,4,4,4,4,4,4,4, 4,4,4,10,10,10,10,10,4,4, 4,4,4,4,4,4,4,4,4, 5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5, 4,4,4,4,4,4,4,4,4, 4,4,10,10,10,10,10,4,4,4, 4,4,4,4,4,4,4,4},
        {0,0,0,0,0,0,0,0, 0,0,10,10,0,0,0,10,10,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,10,10,0,0,0,10,10,0,0, 0,0,0,0,0,0,0,0},

        // [18] Base verde de la selva y cola del delfín entrando al agua
        {7,7,7,7,7,7,7,7, 7,10,7,7,7,7,7,7,10,7, 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7, 7,10,7,7,7,7,7,7,10,7, 7,7,7,7,7,7,7,7},

        // [19-22] Lago / Rio Amazonas Profundo y Salpicadura Blanca (1)
        {8,8,8,8,8,8,8,8, 8,1,8,8,8,8,8,8,1,8, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 8,1,8,8,8,8,8,8,1,8, 8,8,8,8,8,8,8,8},
        {8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8},
        {8,8,9,9,8,8,9,9,8,8,9,9,8,8,9,9,8,8,9,9,8,8,9,9,8,8,9,9,8,8,9,9,8,8,9,9,8,8,9,9,8,8,9,9,8,8,9,9,8,8,9,9,8,8,9,9,8,8,9,9,8,8,9,9,8,8,9,9,8,8,9,9,8,8,9,9,8,8,9,9},
        {8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8}
    };

    unsigned char bloque = 219; // Bloque ASCII sólido para efecto Retro

    cout << "\nCargando sistema Amazonas (Synthwave & Dolphins Edition)...\n\n";

    // Recorrido oficial de la matriz 2D
    for (int i = 0; i < FILAS; i++) {
        cout << "  "; // Margen izquierdo ligero
        for (int j = 0; j < COLS; j++) {
            if (matriz_c[i][j] == 0) {
                cout << " ";
            }
            else {
                // Renderizado condicional de colores ANSI
                switch (matriz_c[i][j]) {
                case 1: cout << "\033[97m" << bloque; break; // Blanco Brillante (Texto/Bandera/Salpicadura)
                case 2: cout << "\033[31m" << bloque; break; // Rojo (Bandera)
                case 3: cout << "\033[35m" << bloque; break; // Magenta (Cielo superior)
                case 4: cout << "\033[95m" << bloque; break; // Púrpura (Horizonte)
                case 5: cout << "\033[93m" << bloque; break; // Amarillo (Sol superior/Flecha)
                case 6: cout << "\033[33m" << bloque; break; // Naranja (Sol inferior)
                case 7: cout << "\033[32m" << bloque; break; // Verde (Selva)
                case 8: cout << "\033[36m" << bloque; break; // Cian (Rio agua clara)
                case 9: cout << "\033[34m" << bloque; break; // Azul (Rio olas profundas)
                case 10: cout << "\033[38;5;213m" << bloque; break; // Rosa Vivo (Delfines Inia geoffrensis)
                default: cout << " "; break;
                }
            }
        }
        cout << "\033[0m\n"; // Resetea color al final de cada fila
    }

    cout << "\033[0m"; // Reset final por seguridad
    cout << "\n  ================================================================================" << endl;
    cout << "  Presione ENTER para ingresar al Marketplace...";
    cin.get();
}

int main() {
    // Configuracion de la consola de Windows para soportar colores ANSI
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