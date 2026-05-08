#pragma once
#include <Windows.h>
#include <iostream>
#include <conio.h>
#include "Plantillas.h"

using namespace std;


void gotoxy(int x, int y) { //me ayude con IA me cambie el gotoxy que puse con namespace System a uno que use Windows.h
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hCon, dwPos);
}

class Matriz
{
private:
    int** datos; //Matriz dinamica que se ajustará al tamaño de diferentes matrices
    int FILAS, COLUMNAS;
    int X, Y;
    void reservarMemoria();

public:
    Matriz();
    ~Matriz(); //Destructor

    //Funciones Principales
    void colores(int c); //Aplica los colores a las matrices
    void inicializar(Plantilla matriz);
    void dibujarMatriz(int x, int y);
    void liberarMemoria();

    //Getters
    int getX() { return X; }
    int getY() { return Y; }
    int getFilas() { return FILAS; }
    int getColumnas() { return COLUMNAS; }
    int getDatos(int i, int j) { if (i <= FILAS && i >= 0 && j <= COLUMNAS && j >= 0)return datos[i][j];else return 0; }
};

Matriz::Matriz() {
    this->FILAS = 0;
    this->COLUMNAS = 0;
    this->X = 0;
    this->Y = 0;
    this->datos = nullptr;
}

void Matriz::liberarMemoria()
{
    for (int i = 0;i < FILAS;i++) delete[] datos[i];
    delete[] datos;
}

Matriz::~Matriz()
{
    liberarMemoria();
}

void Matriz::reservarMemoria() {
    datos = new int* [FILAS];
    for (int i = 0;i < FILAS;i++) {
        datos[i] = new int[COLUMNAS] {0};//el "0" asegura que todos los espacios de memoria que he pedido se llenen con ceros inmediatamente, sino esos espacios contendran basura
    }
}

void Matriz::inicializar(Plantilla matriz) {
    //0. Verificamos que no se haya utilizado "datos" antes
    if (datos != nullptr) {
        liberarMemoria();
        datos = nullptr;
    }

    //1. Se configuran las dimensiones en base a la plantilla de matriz recibida
    FILAS = matriz.filas;
    COLUMNAS = matriz.columnas;

    //2. Se reserva la memoria en el heap
    reservarMemoria();

    //3. Se cargan los datos en base al tipo y matriz elegido
    for (int i = 0;i < FILAS;i++) {
        for (int j = 0;j < COLUMNAS;j++) {
            datos[i][j] = matriz.matriz[i * COLUMNAS + j];//no se puede hacer matriz[i][j], ya que es estatica, por ende la recibimos como una larga lista de numero y esta operacion hace que se convierta como una "matriz".
        }
    }
}

void Matriz::dibujarMatriz(int x, int y) {
    X = x;
    Y = y;
    for (int i = 0;i < FILAS;i++) {
        for (int j = 0;j < COLUMNAS;j++) {
            if (datos[i][j] != -1) { // Solo si NO es transparente
                gotoxy(x + j, y + i); // Posición exacta: origen + columna, origen + fila
                colores(datos[i][j]);
                cout << (char)219;
            }
        }
    }
}

void Matriz::colores(int c) { //me ayude con IA me cambie colores() que puse con namespace System a uno que use Windows.h
    // Obtenemos el control de la consola
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);

    // Aplicamos el color directamente usando el número 'c'
    // Si tus dibujos ya tienen los números del 0 al 15, funcionará igual
    SetConsoleTextAttribute(hCon, c);
}
/*Tabla de colores

0, Negro 
1, Azul Oscuro
2, Verde Oscuro
3, Cian Oscuro
4, Rojo Oscuro
5, Púrpura
6, Amarillo Oscuro
7, Gris Claro
8, Gris Oscuro
9, Azul Brillante
10, Verde Brillante
11, Cian Brillante
12, Rojo Brillante
13, Magenta Brillante
14, Amarillo Brillante
15, Blanco Brillante*/