#pragma once
#include <iostream>
#include <string>

struct Venta {
    int idProducto;
    std::string dniCliente;
    std::string cliente;
    std::string producto;
    float precio;
    int cantidadRestada;
    int stockRestante;
    bool stockDisminuido;
    long long fechaHora;
    std::string fechaTexto;

    Venta() {
        idProducto = 0;
        dniCliente = "";
        cliente = "";
        producto = "";
        precio = 0.0f;
        cantidadRestada = 0;
        stockRestante = 0;
        stockDisminuido = false;
        fechaHora = 0;
        fechaTexto = "Sin fecha";
    }

    Venta(int _idProducto, std::string _dniCliente, std::string _cliente, std::string _producto, float _precio, int _cantidadRestada, int _stockRestante, bool _stockDisminuido, long long _fechaHora, std::string _fechaTexto) {
        idProducto = _idProducto;
        dniCliente = _dniCliente;
        cliente = _cliente;
        producto = _producto;
        precio = _precio;
        cantidadRestada = _cantidadRestada;
        stockRestante = _stockRestante;
        stockDisminuido = _stockDisminuido;
        fechaHora = _fechaHora;
        fechaTexto = _fechaTexto;
    }

    void mostrar() {
        std::cout << "Fecha/Hora: " << fechaTexto
            << " | Cliente: " << cliente
            << " | DNI: " << dniCliente
            << " | Producto: " << producto
            << " | Precio: S/. " << precio
            << " | Cantidad restada: " << cantidadRestada
            << " | Stock restante: " << stockRestante
            << " | Stock disminuyo: " << (stockDisminuido ? "Si" : "No") << "\n";
    }
};
