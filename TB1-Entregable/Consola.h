
#include <iostream>
#include <vector>
#include <string>
#include <windows.h>

#include "Inventario.h"
#include "Cliente.h"
#include "Vendedor.h"
#include"GestorEscenas.h"

using namespace std;

#define ANCHO_TERMINAL 130
#define ALTO_TERMINAL 45

class Consola
{
private:
    
public:
	Consola();
	~Consola();

	void configurarConsola();
    void establecerColor(int letra, int fondo);
};

void Consola::configurarConsola() {
    HANDLE manejadorConsola = GetStdHandle(STD_OUTPUT_HANDLE);
    if (manejadorConsola == INVALID_HANDLE_VALUE) return;

    // Habilita los caracteres ASCII extendidos (necesario para imprimir el bloque 219)
    SetConsoleOutputCP(437);

    // Habilita el procesamiento ANSI (colores) y evita el salto automático de línea
    DWORD modoConsola = 0;
    if (GetConsoleMode(manejadorConsola, &modoConsola)) {
        SetConsoleMode(manejadorConsola, (modoConsola | ENABLE_VIRTUAL_TERMINAL_PROCESSING) & ~ENABLE_WRAP_AT_EOL_OUTPUT);
    }

    // Fuerza la fuente a 8x16 para asegurar que la matriz de 130x45 quepa en pantallas estándar
    CONSOLE_FONT_INFOEX infoFuente;
    infoFuente.cbSize = sizeof(infoFuente);
    infoFuente.nFont = 0;
    infoFuente.dwFontSize.X = 8;
    infoFuente.dwFontSize.Y = 16;
    infoFuente.FontFamily = FF_DONTCARE;
    infoFuente.FontWeight = FW_NORMAL;
    lstrcpyW(infoFuente.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(manejadorConsola, FALSE, &infoFuente);

    // Redimensionamiento forzado moderno compatible con Windows Terminal
    cout << "\033[8;" << ALTO_TERMINAL << ";" << ANCHO_TERMINAL << "t";
    cout.flush();
}

void Consola::establecerColor(int letra, int fondo) {
    // Obtenemos el identificador de la consola
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);

    // La fórmula mágica: (fondo * 16) + letra
    // Esto combina ambos colores en un solo atributo de Windows
    SetConsoleTextAttribute(hCon, (fondo * 16) + letra);
}




Consola::Consola()
{
}

Consola::~Consola()
{
}