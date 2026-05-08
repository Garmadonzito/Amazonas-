#pragma once
#include <iostream>
#include <string>

struct Venta {
    int idProducto;
    std::string dniCliente;
    std::string cliente;
    std::string producto;
    float precio;
    int stockRestante;
    bool stockDisminuido;

    Venta() {
        idProducto = 0;
        dniCliente = "";
        cliente = "";
        producto = "";
        precio = 0.0f;
        stockRestante = 0;
        stockDisminuido = false;
    }

    Venta(int _idProducto, std::string _dniCliente, std::string _cliente, std::string _producto, float _precio, int _stockRestante, bool _stockDisminuido) {
        idProducto = _idProducto;
        dniCliente = _dniCliente;
        cliente = _cliente;
        producto = _producto;
        precio = _precio;
        stockRestante = _stockRestante;
        stockDisminuido = _stockDisminuido;
    }

    void mostrar() {
        std::cout << "Cliente: " << cliente
            << " | DNI: " << dniCliente
            << " | Producto: " << producto
            << " | Precio: S/. " << precio
            << " | Stock restante: " << stockRestante
            << " | Stock disminuyo: " << (stockDisminuido ? "Si" : "No") << "\n";
    }
};
