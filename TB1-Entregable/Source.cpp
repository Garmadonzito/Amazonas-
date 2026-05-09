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
        consola.establecerColor(15, 0); //Cambie el color blanco por negro, no se veia muy bien 

       
        irA(4, 12); cout << "========================================";
        irA(5, 12); cout << "        AMAZONAS - INICIO";
        irA(6, 12); cout << "========================================";
        irA(8, 12); cout << "Que tipo de usuario eres?";
        irA(9, 12); cout << "1. Cliente";
        irA(10, 12); cout << "2. Vendedor (Empieza por aqui para anadir stock)";
        irA(11, 12); cout << "3. Salir";
        irA(13, 12); cout << "Opcion: ";

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