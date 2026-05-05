#pragma once
#include <string>

struct Producto {
    int id;
    std::string nombre;
    std::string categoria;
    float precio;
    int stock;

    Producto() {
        id = 0;
        nombre = "";
        categoria = "Sin Categoria";
        precio = 0.0f;
        stock = 0;
    }

    Producto(int _id, std::string _nombre, std::string _categoria, float _precio, int _stock) {
        id = _id;
        nombre = _nombre;
        categoria = _categoria;
        precio = _precio;
        stock = _stock;
    }
};
