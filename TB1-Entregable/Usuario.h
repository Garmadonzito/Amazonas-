#pragma once
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

inline void limpiarPantalla() {
    system("cls||clear");
}

inline void pausa() {
    cout << "\nPresione ENTER para continuar...";
    cin.ignore();
    cin.get();
}

// Posiciona el cursor en fila/col (1-indexado, compatible con ANSI)
inline void irA(int fila, int col) {
    cout << "\033[" << fila << ";" << col << "H";
}

// Limpia el area del menu (zona verde central de la segunda matriz)
inline void limpiarMenuArea() {
    for (int i = 0; i < 28; i++) {
        irA(11 + i, 14);
        cout << string(97, ' ');
    }
    irA(11, 14);
    cout.flush();
}

class Usuario {
protected:
    string nombre;
    string correo;
    string dni;

public:
    Usuario() : nombre(""), correo(""), dni("") {}
    string getNombre() { return nombre; }
};
