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

class Usuario {
protected:
    string nombre;
    string correo;
    string dni;

public:
    Usuario() : nombre(""), correo(""), dni("") {}
    string getNombre() { return nombre; }
};