#pragma once
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

inline void limpiarPantalla() {
    system("cls||clear");
}

inline void pausa() {
    cout << "\033[0m\nPresione ENTER para continuar...";
    cin.ignore();
    cin.get();
}

// Mueve el cursor a fila/col (1-indexado ANSI)
inline void irA(int fila, int col) {
    cout << "\033[" << fila << ";" << col << "H";
}

// Zona verde: terminal cols 12-114 (103 chars), filas 10-38
// Limpia solo esa zona para no pisar los bordes grises
inline void limpiarZonaVerde() {
    cout << "\033[0m";
    for (int i = 0; i < 29; i++) {
        irA(10 + i, 12);
        cout << string(103, ' ');
    }
    cout.flush();
}

// Imprime texto centrado en la zona verde (103 cols) en la fila indicada
inline void imprimirCentrado(int fila, const string& texto) {
    int padding = (103 - (int)texto.length()) / 2;
    if (padding < 0) padding = 0;
    irA(fila, 12 + padding);
    cout << "\033[0m" << texto;
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
