#pragma once
#include "Usuario.h"
#include <string>
#include <iostream>

class Direccion {
private:
    std::string calle;
    std::string distrito;
    std::string ciudad;
    std::string referencia;

public:
    Direccion() : calle(""), distrito(""), ciudad(""), referencia("") {}

    void registrar(int filaInicio) {
        imprimirEnPanel(filaInicio,     "========================================================", 96);
        imprimirEnPanel(filaInicio + 1, "              DIRECCION DE ENVIO                        ", 96);
        imprimirEnPanel(filaInicio + 2, "========================================================", 96);
        irA(filaInicio + 4, PANEL_COL); std::cout << "Calle y numero : "; std::cin.ignore(); std::getline(std::cin, calle);
        irA(filaInicio + 5, PANEL_COL); std::cout << "Distrito       : "; std::getline(std::cin, distrito);
        irA(filaInicio + 6, PANEL_COL); std::cout << "Ciudad         : "; std::getline(std::cin, ciudad);
        irA(filaInicio + 7, PANEL_COL); std::cout << "Referencia     : "; std::getline(std::cin, referencia);
    }

    bool esValida() {
        return !calle.empty() && !distrito.empty() && !ciudad.empty();
    }

    void mostrar(int filaInicio) {
        imprimirEnPanel(filaInicio,     "  Calle     : " + calle);
        imprimirEnPanel(filaInicio + 1, "  Distrito  : " + distrito);
        imprimirEnPanel(filaInicio + 2, "  Ciudad    : " + ciudad);
        imprimirEnPanel(filaInicio + 3, "  Referencia: " + referencia);
    }

    std::string getCalle()     { return calle; }
    std::string getDistrito()  { return distrito; }
    std::string getCiudad()    { return ciudad; }
    std::string getReferencia(){ return referencia; }
};
