#include "Consola.h"

int main() {
    Consola consola;
    consola.configurarConsola();

    Inventario* miTienda = new Inventario();
    miTienda->cargarDesdeArchivo();

    Vendedor* administrador = new Vendedor();
    Cliente* usuario = new Cliente();
    gestorEscenas grafica;

    int opcion;
    do {
        system("cls");
        grafica.dibujarEscena(); 
        consola.establecerColor(15, 0);

       
        irA(2, 24);  cout << "--------------------------------------------------------------------------------";
        irA(3, 24);  cout << "\033[93m  ________ \033[92m     ___      __  ___    ___     ____   ___       _____     ___     ____ \033[0m";
        irA(4, 24);  cout << "\033[93m /       /|\033[92m    /   |    /  |/  /   /   |   /_  /  / _ \\     / |/ /    /   |   / __/ \033[0m";
        irA(5, 24);  cout << "\033[93m/_______/ |\033[92m   / /| |   / /|_/ /   / /| |    / /  / / / /   /    /    / /| |  \\__ \\ \033[0m";
        irA(6, 24);  cout << "\033[93m|       | |\033[92m  / ___ |  / /  / /   / ___ |   / /_ / /_/ /   / /|  /   / ___ | ___/ /  \033[0m";
        irA(7, 24);  cout << "\033[93m|_______|/ \033[92m /_/  |_| /_/  /_/   /_/  |_|  /___/ \\____/   /_/ |_/   /_/  |_|/____/  \033[0m";
        irA(9, 24);  cout << "--------------------------------------------------------------------------------";

        irA(13, 24); cout << "Que tipo de usuario eres?";
        irA(14, 24); cout << "1. Cliente";
        irA(15, 24); cout << "2. Vendedor (Empieza por aqui para anadir stock)";
        irA(16, 24); cout << "3. Salir";
        irA(17, 24); cout << "Opcion: ";
        cin >> opcion;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            opcion = -1;
            continue;
        }

        switch (opcion) {
        case 1: {
            system("cls");
            usuario->login();
            usuario->menu(*miTienda);
            break;
        }
        case 2: {
            system("cls");
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

    irA(15, 12); cout << "Hasta pronto!\n";
    return 0;
}