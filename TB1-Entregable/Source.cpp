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
            system("cls");
            // Estoy pasando miTienda por referencia para utilizar la Tabla Hash en memoria 
            // lo dejo como anotacion si quieren referenciar tambien los arboles binarios xd
            usuario->login(*miTienda);
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