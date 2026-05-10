#pragma once
#include <iostream>
#include <string>
#include <conio.h>

using namespace std;

class Tarjeta {
public:
    void pagar(double monto) {
        int col = 24; 

        irA(4, col); cout << "            \033[95m========================================================\033[0m";
        irA(5, col); cout << "            \033[95m               --- PAGO CON TARJETA ---                 \033[0m";
        irA(6, col); cout << "            \033[95m========================================================\033[0m";

        string num, fecha, cvv;
        irA(12, col); cout << "Ingrese numero de tarjeta: "; cin >> num;
        irA(14, col); cout << "Fecha de expiracion (MM/AA): "; cin >> fecha;
        irA(16, col); cout << "CVV: "; cin >> cvv;

        irA(20, col); cout << "\033[92m>> Procesando pago de S/. " << monto << "...\033[0m";
        irA(22, col); cout << "Presione ENTER para confirmar...";
        while (true) {
            if (_getch() == 13) break; 
        }
    }
};

class YapePlin {
public:
    void pagar(double monto) {
        int col = 24; 

        irA(4, col); cout << "            \033[95m========================================================\033[0m";
        irA(5, col); cout << "            \033[95m               --- PAGO CON YAPE / PLIN ---             \033[0m";
        irA(6, col); cout << "            \033[95m========================================================\033[0m";

        irA(10, col); cout << "  Escanea con Yape o Plin:";

        
        string qr[15] = {
            "                                    ",
            "   [XXX]  X  XX  X X  [XXX]   X     ",
            "   [ X ]  X  X   X X  [ X ]  XXX    ",
            "   [XXX]  XX XX  X X  [XXX]   X     ",
            "          X  XX  XXX                ",
            "   XXXXX    XX X X X  XXXXX  XXX    ",
            "   X X X  X X XX   X  X   X   X     ",
            "   X X X  XX     X X  XXXXX   X     ",
            "          X  X X X X                ",
            "   [XXX]  XX XX  XXX  X X X  XXX    ",
            "   [ X ]  X      X    X   X   X     ",
            "   [XXX]  XXXXXX X X  XXXXX   X     ",
            "   X   X      XX   X      X         ",
            "    XXX   XX  XX XXX   XXX    X     ",
            "                                    "
        };

        int filaActual = 12;

        
        for (int i = 0; i < 15; i++) {
            irA(filaActual++, col + 4);
          
            cout << "\033[47m\033[30m" << qr[i] << "\033[0m";
        }

        irA(filaActual + 2, col); cout << "  Numero corporativo: 987-654-321";
        irA(filaActual + 4, col); cout << "\033[92m  >> Monto a pagar: S/. " << monto << "\033[0m";
        irA(filaActual + 6, col); cout << "  Presione ENTER cuando haya realizado el pago...";

        while (true) {
            if (_getch() == 13) break; 
        }
    }
};