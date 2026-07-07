#pragma once

#include "Inventario.h"
#include "Registro.h"
#include "Venta.h"
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

class GeneradorDatos {
public:
    static void generarClientesAleatorios(int cantidad, Inventario& inv) {
        srand((unsigned)time(0));

        vector<string> nombres = { "Rafael", "Romina", "Consuelo", "Damaris", "Alfonso", "Juan", "Maria", "Carlos", "Lucia" };
        vector<string> apellidos = { "Rivas", "Vega", "Araujo", "Contreras", "Perez", "Gomez", "Silva", "Flores" };
        vector<string> dominios = { "gmail.com", "upc.edu.pe", "hotmail.com", "outlook.com" };

        vector<RegistroCliente> loteClientes;

        for (int i = 0; i < cantidad; i++) {
            string nombreGen = nombres[rand() % nombres.size()] + " " + apellidos[rand() % apellidos.size()];
            string dniGen = to_string(10000000 + rand() % 90000000);
            string primerNombre = nombreGen.substr(0, nombreGen.find(' '));
            string correoGen = primerNombre + to_string(100 + rand() % 900) + "@" + dominios[rand() % dominios.size()];

            RegistroCliente nuevoReg;
            strncpy_s(nuevoReg.nombre, sizeof(nuevoReg.nombre), nombreGen.c_str(), _TRUNCATE);
            strncpy_s(nuevoReg.correo, sizeof(nuevoReg.correo), correoGen.c_str(), _TRUNCATE);
            strncpy_s(nuevoReg.dni, sizeof(nuevoReg.dni), dniGen.c_str(), _TRUNCATE);
            loteClientes.push_back(nuevoReg);
        }
        inv.registrarClientesMasivo(loteClientes);
    }

    static void generarVentasAleatorias(int cantidad, Inventario& inv) {
        srand((unsigned)time(0));

        vector<int> idsProductos = { 101, 102, 103, 104, 105, 201, 202, 203 };

        
        vector<RegistroCliente> clientesReales = inv.obtenerListaClientes();

        
        if (clientesReales.empty()) {
            generarClientesAleatorios(20, inv);
            clientesReales = inv.obtenerListaClientes();
        }

        vector<RegistroVenta> loteRegistros;
        vector<Venta> loteVentasEnMemoria;

        long long tiempoActual = (long long)time(0);

        for (int i = 0; i < cantidad; i++) {
            
            int idxCliente = rand() % clientesReales.size();
            RegistroCliente clienteElegido = clientesReales[idxCliente];

            int idProdAleatorio = idsProductos[rand() % idsProductos.size()];
            Producto* p = inv.obtenerProducto(idProdAleatorio);
            if (p == nullptr) continue;

            RegistroVenta reg;
            reg.idProducto = p->id;
            reg.monto = p->precio;
            reg.cantidadRestada = 1;
            reg.stockRestante = p->stock;
            reg.fechaHora = tiempoActual - (rand() % 864000);

            time_t tiempo = (time_t)reg.fechaHora;
            tm fechaLocal; localtime_s(&fechaLocal, &tiempo);
            char fechaT[20]; strftime(fechaT, sizeof(fechaT), "%d/%m/%Y %H:%M", &fechaLocal);

            strncpy_s(reg.fechaTexto, sizeof(reg.fechaTexto), fechaT, _TRUNCATE);
            strncpy_s(reg.nombreProducto, sizeof(reg.nombreProducto), p->nombre.c_str(), _TRUNCATE);
            
            strncpy_s(reg.dniCliente, sizeof(reg.dniCliente), clienteElegido.dni, _TRUNCATE);

            
            Venta nuevaVenta(p->id, clienteElegido.dni, clienteElegido.nombre, p->nombre, p->precio, 1, p->stock, false, reg.fechaHora, fechaT);

            loteRegistros.push_back(reg);
            loteVentasEnMemoria.push_back(nuevaVenta);
        }
        inv.registrarVentasMasivas(loteRegistros, loteVentasEnMemoria);
    }
};