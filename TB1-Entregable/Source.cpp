#include "Consola.h"
#include "Cliente.h"
#include "Vendedor.h"

void borrarZona(int x, int y, int ancho, int alto) {
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hCon, 2);
    for (int i = 0; i < alto; i++) {
        gotoxy(x, y + i);
        for (int j = 0; j < ancho; j++) cout << (char)219;
    }
}

void barraDeCarga() {
    limpiarPantalla();
    irA(20, 45); cout << "\033[93mCargando Amazonas tu sitio de compras...\033[0;1;42;97m";
    irA(22, 45); cout << "\033[0;1;42;97m[";
    irA(22, 76); cout << "]";
    for (int i = 0; i < 30; i++) {
        irA(22, 46 + i); cout << "\033[92m" << (char)219 << "\033[0;1;42;97m";
        Sleep(35);
    }
    irA(24, 45); cout << "\033[92mListo!\033[0;1;42;97m";
    Sleep(400);
}

void despedida(gestorEscenas& grafica) {
    limpiarPantalla();
    grafica.dibujarFondoSinLogo();

    Matriz m1, m2, m3, m4;
    m1.inicializar(mascotaPollitoCara);
    m2.inicializar(mascotaTucan);
    m3.inicializar(mascotaLlama);
    m4.inicializar(mascotaPollitoCuerpo);

    m1.dibujarMatriz(20, 12);
    m2.dibujarMatriz(43, 12);
    m3.dibujarMatriz(66, 12);
    m4.dibujarMatriz(89, 12);

    cout << "\033[93m";
    imprimirLento(33, 46, "GRACIAS POR TU VISITA. HASTA PRONTO!", 30);
    cout << "\033[0;1;42;97m";
    Sleep(2200);
}

void animarIntroTucan(gestorEscenas& grafica) {
    limpiarPantalla();
    grafica.dibujarFondoSinLogo();

    Matriz tucan;
    tucan.inicializar(mascotaTucan);

    for (int x = 14; x <= 92; x += 2) {
        tucan.dibujarMatriz(x, 15);
        Sleep(25);
        borrarZona(x, 15, 2, 17);
    }
    borrarZona(92, 15, 17, 17);
}

int main() {
    Consola consola;
    consola.configurarConsola();

    Inventario* miTienda = new Inventario();
    miTienda->cargarDesdeArchivo();

    Vendedor* administrador = new Vendedor();
    Cliente* usuario = new Cliente();
    gestorEscenas grafica;
    
    /*
    //BORRAR SOLO PARA PRUEBAS
    administrador->menu(*miTienda);
    //BORRAR SOLO PARA PRUEBAS
    */

    barraDeCarga();
    animarIntroTucan(grafica);

    int opcion;
    do {
        limpiarPantalla();
        grafica.setEscena(gestorEscenas::PAGINA_PRINCIPAL);
        grafica.dibujarEscena();
        consola.establecerColor(0, 2);

        irA(2, 22);  cout << "  ________      ___      __  ___    ___     ____   ___       _____     ___     ____";
        irA(3, 22);  cout << " /       /|    /   |    /  |/  /   /   |   /_  /  / _ \\     / |/ /    /   |   / __/";
        irA(4, 22);  cout << "/_______/ |   / /| |   / /|_/ /   / /| |    / /  / / / /   /    /    / /| |  \\ __ \\ ";
        irA(5, 22);  cout << "|       | |  / ___ |  / /  / /   / ___ |   / /_ / /_/ /   / /|  /   / ___ | ___/ / ";
        irA(6, 22);  cout << "|_______|/  /_/  |_| /_/  /_/   /_/  |_|  /___/ \\____/   /_/ |_/   /_/  |_|/____/  ";

        consola.establecerColor(0, 15);

        irA(20, 40); cout << "Que tipo de usuario eres?";
        irA(21, 40); cout << "1. Cliente";
        irA(22, 40); cout << "2. Vendedor (Empieza por aqui para anadir stock)";
        irA(23, 40); cout << "3. Salir";
        consola.establecerColor(15, 0);
        irA(38, 53); cout << "Opcion: ";
        cin >> opcion;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            opcion = -1;
            continue;
        }

        switch (opcion) {
        case 1: {
            limpiarPantalla();
            // Estoy pasando miTienda por referencia para utilizar la Tabla Hash en memoria 
            // lo dejo como anotacion si quieren referenciar tambien los arboles binarios xd
            usuario->login(*miTienda);
            usuario->menu(*miTienda);
            break;
        }
        case 2: {
            limpiarPantalla();
            if (administrador->login()) {
                administrador->menu(*miTienda);
            }
            break;
        }
        }
    } while (opcion != 3);
    
    delete miTienda;
    delete administrador;
    delete usuario;

    despedida(grafica);
    return 0;
}