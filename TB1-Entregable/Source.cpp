#include <iostream>
#include "Inventario.h"
#include "Cliente.h"
#include "Vendedor.h"

using namespace std;

int main() {
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