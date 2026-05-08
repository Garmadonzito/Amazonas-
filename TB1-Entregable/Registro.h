#pragma once

#include <cstring> 

// Registro para el archivo binario de Stock (productos.dat)
struct RegistroProducto {
    int id;
    char nombre[50];
    char categoria[30];
    float precio;
    int stock;
};

// Registro para el archivo binario de Clientes (clientes.dat)
struct RegistroCliente {
    char nombre[50];
    char correo[50];
    char dni[9];
};

// Registro para el archivo binario de Ventas/Historial (ventas.dat)
struct RegistroVenta {
    int idProducto;
    char nombreProducto[50];
    float monto;
    char dniCliente[9];
};