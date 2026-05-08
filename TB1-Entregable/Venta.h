#pragma once
#include <iostream>
#include <string>

struct Venta {
    std::string cliente;
    std::string producto;
    float precio;
    int stockRestante;
    bool stockDisminuido;

    Venta() {
        cliente = "";
        producto = "";
        precio = 0.0f;
        stockRestante = 0;
        stockDisminuido = false;
    }

    Venta(std::string _cliente, std::string _producto, float _precio, int _stockRestante, bool _stockDisminuido) {
        cliente = _cliente;
        producto = _producto;
        precio = _precio;
        stockRestante = _stockRestante;
        stockDisminuido = _stockDisminuido;
    }

    void mostrar() {
        std::cout << "Cliente: " << cliente
            << " | Producto: " << producto
            << " | Precio: S/. " << precio
            << " | Stock restante: " << stockRestante
            << " | Stock disminuyo: " << (stockDisminuido ? "Si" : "No") << "\n";
    }
};
