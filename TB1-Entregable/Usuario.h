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

// Limpia solo la zona de contenido para no pisar los bordes
inline void limpiarZonaVerde() {
    cout << "\033[0m";
    for (int i = 0; i < PANEL_ALTO; i++) {
        irA(PANEL_FILA + i, PANEL_COL);
        cout << string(PANEL_ANCHO, ' ');
    }
    cout.flush();
}

// Movido desde Inventario.h y optimizado para reutilizar irA()
inline void imprimirEnPanel(int fila, const string& texto, int color = 0) {
    irA(fila, PANEL_COL);
    if (color != 0) cout << "\033[" << color << "m";
    else cout << "\033[0m";
    cout << texto << "\033[0m";
}

class Usuario {
protected:
    string nombre;
    string correo;
    string dni;

public:
    Usuario() : nombre(""), correo(""), dni("") {}
    // getNombre() eliminado por ser código muerto (garbage code)
};