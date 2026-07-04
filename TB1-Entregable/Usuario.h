#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
#include <Windows.h> // para Sleep en los efectos visuales

using namespace std;

//TITULOS
const int PANEL_FILA = 8;
const int PANEL_COL = 35;   
const int PANEL_ANCHO = 80;
const int PANEL_ALTO = 35;

//Cuadro de texto
const int TEXT_FILA = 20;
const int TEXT_COL = 33;
const int TEXT_ANCHO = 80;
const int TEXT_ALTO = 35;
inline void limpiarPantalla() {
    system("cls||clear");
}

inline void irA(int fila, int col) {
    cout << "\033[" << fila << ";" << col << "H";
}


inline void pausa() {
    irA(43, PANEL_COL);
    cout << "\033[93m>> Presione ENTER para continuar...\033[0;42;97m";
    cin.clear();
    cin.ignore(10000, '\n');
    cin.get();

   
    irA(43, PANEL_COL);
    cout << string(50, ' ');
}

inline void limpiarZonaVerde() {
    cout << "\033[0;42;97m";
    for (int i = 0; i < PANEL_ALTO; i++) {
        irA(PANEL_FILA + i, PANEL_COL);
        cout << string(PANEL_ANCHO, ' ');
    }
    cout.flush();
}

inline void imprimirEnPanel(int fila, const string& texto, int color = 0) {
    irA(fila, PANEL_COL);
    if (color != 0) cout << "\033[" << color << "m";
    else cout << "\033[0;42;97m";
    cout << texto << "\033[0;42;97m";
}

// Efecto maquina de escribir: imprime el texto letra por letra
inline void imprimirLento(int fila, int col, const string& texto, int retardo = 15) {
    irA(fila, col);
    for (char letra : texto) {
        cout << letra;
        cout.flush();
        Sleep(retardo);
    }
}

// Dibuja un marco con caracteres de caja (201=esquina sup izq, 205=linea
// horizontal, 187=sup der, 186=linea vertical, 200=inf izq, 188=inf der)
inline void dibujarMarco(int fila, int col, int ancho, int alto, const string& colorAnsi = "96") {
    cout << "\033[" << colorAnsi << "m";
    irA(fila, col);
    cout << (char)201;
    for (int j = 0; j < ancho - 2; j++) cout << (char)205;
    cout << (char)187;
    for (int i = 1; i < alto - 1; i++) {
        irA(fila + i, col); cout << (char)186;
        irA(fila + i, col + ancho - 1); cout << (char)186;
    }
    irA(fila + alto - 1, col);
    cout << (char)200;
    for (int j = 0; j < ancho - 2; j++) cout << (char)205;
    cout << (char)188;
    cout << "\033[0;42;97m";
}

// Transicion tipo cortina: barre la pantalla con columnas verdes
inline void transicionCortina() {
    cout << "\033[42m"; // fondo verde
    for (int col = 1; col <= 124; col += 3) {
        for (int fila = 1; fila <= 44; fila++) {
            irA(fila, col);
            cout << "   ";
        }
        Sleep(4);
    }
    cout << "\033[0;42;97m";
}

class Usuario {
protected:
    string nombre;
    string correo;
    string dni;

public:
    Usuario() : nombre(""), correo(""), dni("") {}
};