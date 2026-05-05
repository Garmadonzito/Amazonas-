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
        cout << "\n   Escanea el QR con Yape o Plin:\n\n";
        cout << "   ##############   #######\n";
        cout << "   ##          ##   ##   ##\n";
        cout << "   ##  ######  ##   ##   ##\n";
        cout << "   ##  ######  ##   #######\n";
        cout << "   ##  ######  ##          \n";
        cout << "   ##          ##   #######\n";
        cout << "   ##############   ##   ##\n";
        cout << "         ##         ##   ##\n";
        cout << "   #######   ###############\n";
        cout << "   ##   ##   ##            ##\n";
        cout << "   ##   ##   ##  ########  ##\n";
        cout << "   #######   ##  ########  ##\n";
        cout << "             ##  ########  ##\n";
        cout << "   ## ## ##  ##            ##\n";
        cout << "   ## ## ##  ##############\n\n";
        cout << "   Numero: 987-654-321\n";
    }

    void pagar(double total) override {
        cout << "\n--- PAGO CON YAPE / PLIN ---\n";
        mostrarQR();
        cout << ">> Monto a pagar: S/. " << total << "\n";
        cout << "\nPresione ENTER cuando haya realizado el pago...";
        cin.ignore(); cin.get();
        cout << ">> Pago confirmado. Gracias!\n";
    }
};
