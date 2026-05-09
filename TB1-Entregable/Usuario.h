#pragma once

#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;


const int PANEL_FILA = 8;
const int PANEL_COL = 25;   
const int PANEL_ANCHO = 80;
const int PANEL_ALTO = 35;   

inline void limpiarPantalla() {
    system("cls||clear");
}

inline void irA(int fila, int col) {
    cout << "\033[" << fila << ";" << col << "H";
}


inline void pausa() {
    irA(43, PANEL_COL);
    cout << "\033[93m>> Presione ENTER para continuar...\033[0m";
    cin.clear();
    cin.ignore(10000, '\n');
    cin.get();

   
    irA(43, PANEL_COL);
    cout << string(50, ' ');
}

inline void limpiarZonaVerde() {
    cout << "\033[0m";
    for (int i = 0; i < PANEL_ALTO; i++) {
        irA(PANEL_FILA + i, PANEL_COL);
        cout << string(PANEL_ANCHO, ' ');
    }
    cout.flush();
}

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
};