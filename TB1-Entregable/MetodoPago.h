#pragma once
#include "Usuario.h"

class MetodoPago {
public:
    virtual void pagar(double total) = 0;
    virtual ~MetodoPago() {}
};

class Tarjeta : public MetodoPago {
private:
    string titular;
    string numero;
    string vencimiento;

public:
    void pagar(double total) override {
        cout << "\n--- PAGO CON TARJETA ---\n";
        cin.ignore();
        cout << "Titular: "; getline(cin, titular);
        cout << "Numero de tarjeta: "; getline(cin, numero);
        cout << "Vencimiento (MM/AA): "; getline(cin, vencimiento);
        cout << "\n>> Procesando pago de S/. " << total << "...\n";
        cout << ">> Pago aprobado. Gracias, " << titular << "!\n";
    }
};

class YapePlin : public MetodoPago {
public:
    void mostrarQR() {
        // char 220 = lower half block (▄)
        // Foreground = lower half color, Background = upper half color
        // Procesamos 2 filas del QR por cada linea de terminal -> aspecto cuadrado

        const int QR_SIZE = 41;
        const char* QR_DATA[] = {
            "11111110001101100001111110100101001111111",
            "10000010100011110110110011011010101000001",
            "10111010000011000010110010100000001011101",
            "10111010111010110000111011011110101011101",
            "10111010001110010001010110100001001011101",
            "10000010100101111110000010011000001000001",
            "11111110101010101010101010101010101111111",
            "00000000011110011010010100011000100000000",
            "11111011111001011110011101011110110101010",
            "11101001101110100001101000100001000110001",
            "00111110101100011010001011011010100000010",
            "00010101100010110011110110111000010010001",
            "11100111000110011110101011010110000101110",
            "10001101001000100000110100101001111011001",
            "10001101001000100000110100101001111011001",
            "01000111001110111100010001010110010011110",
            "11110101000100101010010010110011110010010",
            "00001011000101010110000101001110100101110",
            "10101001001111101001110110100001010010011",
            "10111111111001111100111000011100010010100",
            "01101001111011011011011010010001011010011",
            "11011110101000101110001011001110000101100",
            "11101001110010000001011100100001111010101",
            "10101011110001110010110011111010100111010",
            "10001001101010001011010000000011000011010",
            "10000110011011010111111101011110110100110",
            "10101100111100101001111110100001001010011",
            "10000011011010011110011000010100000010000",
            "10000001000110110001110010100010011000011",
            "10111110011010001111001111011100100001100",
            "11100100110000100001111100100011011111001",
            "10110110101110011010011010011000010101010",
            "10110000000100000011010010010011010010000",
            "10000110100101010110001001011110111111101",
            "00000000100111101001111110100001100010011",
            "11111110111001110010101000010001101011000",
            "10000010011111011010011000110010100010010",
            "10111010110000100110101001001110111111110",
            "10111010100010011001110000100001010100001",
            "10111010111000010000100010110000001011100",
            "10000010100010011011110100011010100111010",
        };

        unsigned char halfBlock = 220; // ▄  lower half block

        // Negro = \033[40m   Blanco brillante = \033[107m
        // char 220: fg = mitad inferior, bg = mitad superior
        //   top=0 bot=0 -> blanco/blanco   -> \033[107;107m \033[0m
        //   top=0 bot=1 -> fg negro bg bco -> \033[30;107m▄\033[0m
        //   top=1 bot=0 -> fg bco bg negro -> \033[97;40m▄\033[0m
        //   top=1 bot=1 -> negro/negro     -> \033[40;40m \033[0m

        cout << "\n   Escanea con Yape o Plin:\n\n";

        // Quiet zone superior (2 lineas = 4 filas blancas)
        int lineWidth = QR_SIZE + 8; // 4 quiet zone cada lado
        for (int i = 0; i < 2; i++) {
            cout << "  \033[107m";
            for (int x = 0; x < lineWidth; x++) cout << ' ';
            cout << "\033[0m\n";
        }

        // Renderizar QR: 2 filas por linea
        for (int row = 0; row < QR_SIZE; row += 2) {
            cout << "  \033[107m    \033[0m"; // quiet zone izquierdo
            for (int col = 0; col < QR_SIZE; col++) {
                bool top    = QR_DATA[row][col]          == '1';
                bool bottom = (row + 1 < QR_SIZE) ?
                              QR_DATA[row + 1][col] == '1' : false;

                if (!top && !bottom) {
                    // ambos blancos
                    cout << "\033[107;107m " << "\033[0m";
                } else if (!top && bottom) {
                    // superior blanco, inferior negro
                    cout << "\033[30;107m" << halfBlock << "\033[0m";
                } else if (top && !bottom) {
                    // superior negro, inferior blanco
                    cout << "\033[97;40m" << halfBlock << "\033[0m";
                } else {
                    // ambos negros
                    cout << "\033[40;40m " << "\033[0m";
                }
            }
            cout << "\033[107m    \033[0m\n"; // quiet zone derecho
        }

        // Quiet zone inferior
        for (int i = 0; i < 2; i++) {
            cout << "  \033[107m";
            for (int x = 0; x < lineWidth; x++) cout << ' ';
            cout << "\033[0m\n";
        }

        cout << "\n   Numero: 987-654-321\n";
    }

    void pagar(double total) override {
        cout << "\n--- PAGO CON YAPE / PLIN ---\n";
        mostrarQR();
        cout << "\n>> Monto a pagar: S/. " << total << "\n";
        cout << "\nPresione ENTER cuando haya realizado el pago...";
        cin.ignore(); cin.get();
        cout << ">> Pago confirmado. Gracias!\n";
    }
};
