#pragma once
#include <iostream>
#include <string>

struct Pedido {
    int idPedido;
    std::string cliente;
    float total;

    Pedido() {
        idPedido = 0;
        cliente = "";
        total = 0.0f;
    }

    Pedido(int _idPedido, std::string _cliente, float _total) {
        idPedido = _idPedido;
        cliente = _cliente;
        total = _total;
    }

    void mostrar() {
        std::cout << "[Pedido #" << idPedido << "] Cliente: " << cliente
            << " | Total: S/. " << total << "\n";
    }
};
