#include "Consola.h"

int main() {
    Consola consola ;
    consola.configurarConsola();

    Inventario* miTienda = new Inventario();
    miTienda->cargarDesdeArchivo();

    Vendedor* administrador = new Vendedor();
    Cliente* usuario = new Cliente();
    gestorEscenas grafica;
    grafica.dibujarEscena();
    consola.establecerColor(0, 15);
    int opcion;
    do {

        cout << "========================================\n";
        cout << "        AMAZONAS - INICIO\n";
        cout << "========================================\n";
        cout << "Que tipo de usuario eres?\n";
        cout << "1. Cliente\n";
        cout << "2. Vendedor (Empieza por aqui para anadir stock)\n";
        cout << "3. Salir\n";
        cout << "Opcion: ";
        cin >> opcion;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            opcion = -1;
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

    cout << "Hasta pronto!\n";
    return 0;
}