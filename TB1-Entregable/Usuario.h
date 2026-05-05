#pragma once
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

const int PANEL_FILA = 10;
const int PANEL_COL = 12;
const int PANEL_ANCHO = 98;
const int PANEL_ALTO = 29;

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

// Panel de contenido: terminal cols 12-109, filas 10-38
// Limpia solo esa zona para no pisar los bordes grises
inline void limpiarZonaVerde() {
    cout << "\033[0m";
    for (int i = 0; i < PANEL_ALTO; i++) {
        irA(PANEL_FILA + i, PANEL_COL);
        cout << string(PANEL_ANCHO, ' ');
    }
    cout.flush();
}

// Imprime texto centrado en el panel negro en la fila indicada
inline void imprimirCentrado(int fila, const string& texto) {
    int padding = (PANEL_ANCHO - (int)texto.length()) / 2;
    if (padding < 0) padding = 0;
    irA(fila, PANEL_COL + padding);
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
