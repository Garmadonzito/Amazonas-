#pragma once
#include "ListaEnlazada.h"
#include "Usuario.h"
#include "Registro.h" // Asegúrate de tenerlo para la persistencia
#include <string>
#include <iostream>
#include <fstream>

// Estructura para guardar en el archivo .dat
struct RegistroResena {
    int idProducto;
    char dniCliente[9];
    char comentario[100];
    int puntuacion;
};

struct Resena {
    int idProducto;
    std::string nombreProducto;
    std::string dniCliente;
    std::string comentario;
    int puntuacion;

    Resena() : idProducto(0), nombreProducto(""), dniCliente(""), comentario(""), puntuacion(0) {}
    Resena(int _id, std::string _prod, std::string _dni, std::string _comentario, int _punt)
        : idProducto(_id), nombreProducto(_prod), dniCliente(_dni),
        comentario(_comentario), puntuacion(_punt) {
    }
};

class GestorResenas {
private:
    ListaEnlazada<Resena>* listaResenas;

    // --- RECURSIVIDAD: Calcular suma de puntos de un producto ---
    int sumarPuntosRecursivo(Nodo<Resena>* actual, int idProd) {
        if (actual == nullptr) return 0;
        int puntosActual = (actual->dato.idProducto == idProd) ? actual->dato.puntuacion : 0;
        return puntosActual + sumarPuntosRecursivo(actual->siguiente, idProd);
    }

    // --- RECURSIVIDAD: Contar cuántas reseñas tiene un producto ---
    int contarResenasRecursivo(Nodo<Resena>* actual, int idProd) {
        if (actual == nullptr) return 0;
        int cuenta = (actual->dato.idProducto == idProd) ? 1 : 0;
        return cuenta + contarResenasRecursivo(actual->siguiente, idProd);
    }

public:
    GestorResenas() {
        listaResenas = new ListaEnlazada<Resena>();
        cargarDesdeArchivo();
    }
    ~GestorResenas() { delete listaResenas; }

    // Calcula el promedio usando las funciones recursivas anteriores
    float obtenerPromedioProducto(int idProd) {
        int totalPuntos = sumarPuntosRecursivo(listaResenas->getCabeza(), idProd);
        int totalResenas = contarResenasRecursivo(listaResenas->getCabeza(), idProd);
        if (totalResenas == 0) return 0.0f;
        return (float)totalPuntos / totalResenas;
    }

    void agregarResena(int idProducto, std::string producto, std::string dni, std::string comentario, int puntuacion) {
        if (puntuacion < 1) puntuacion = 1;
        if (puntuacion > 5) puntuacion = 5;
        listaResenas->agregar(Resena(idProducto, producto, dni, comentario, puntuacion));
        guardarEnArchivo();
    }

    void guardarEnArchivo() {
        std::ofstream archivo("resenas.dat", std::ios::binary | std::ios::trunc);
        Nodo<Resena>* actual = listaResenas->getCabeza();
        while (actual != nullptr) {
            RegistroResena reg;
            reg.idProducto = actual->dato.idProducto;
            strncpy_s(reg.dniCliente, actual->dato.dniCliente.c_str(), _TRUNCATE);
            strncpy_s(reg.comentario, actual->dato.comentario.c_str(), _TRUNCATE);
            reg.puntuacion = actual->dato.puntuacion;
            archivo.write((char*)&reg, sizeof(RegistroResena));
            actual = actual->siguiente;
        }
        archivo.close();
    }

    void cargarDesdeArchivo() {
        std::ifstream archivo("resenas.dat", std::ios::binary);
        if (!archivo) return;
        RegistroResena reg;
        while (archivo.read((char*)&reg, sizeof(RegistroResena))) {
            listaResenas->agregar(Resena(reg.idProducto, "Producto", reg.dniCliente, reg.comentario, reg.puntuacion));
        }
        archivo.close();
    }

    void listarResenas() {
        imprimirEnPanel(4, "            \033[96m========================================================\033[0m");
        imprimirEnPanel(5, "            \033[96m                 RESENAS DE CLIENTES                    \033[0m");
        imprimirEnPanel(6, "            \033[96m========================================================\033[0m");

        int fila = 12;
        if (listaResenas->getCabeza() == nullptr) {
            imprimirEnPanel(fila, "  No hay resenas registradas aun.", 91);
            return;
        }
        Nodo<Resena>* actual = listaResenas->getCabeza();
        while (actual != nullptr && fila < 38) {
            std::string estrellas = "";
            for (int i = 0; i < actual->dato.puntuacion; i++) estrellas += "*";

            // Armamos la linea de texto
            std::string item = " [" + estrellas + "] " + actual->dato.dniCliente + ": " + actual->dato.comentario;

            // --- SOLUCION MARGEN: Si la linea es muy larga, la cortamos y ponemos "..." ---
            if (item.length() > 54) {
                item = item.substr(0, 51) + "...";
            }

            imprimirEnPanel(fila++, item);
            actual = actual->siguiente;
        }
    }
};