#pragma once
#include "Matriz.h"
#include <vector>

class gestorEscenas {
public:
	enum Escena { LOGIN, MENU_PRINCIPAL, CATALOGO, CARRITO, PAGO };

	struct Elemento {
		Matriz* obj;
		int x, y;
	};
private:

	Matriz fondo;
	vector<Elemento> listaObjetos;
	Escena estado;

public:

	gestorEscenas();
	~gestorEscenas();

	void setEscena(Escena nueva);
	void dibujarEscena();
	void dibujarFondoSinLogo();
	void restaurarFondo(int x, int y, int ancho, int alto);
	void limpiar_listaObj();

	Escena getEstado() { return estado; }
	Matriz& getFondoActual() { return fondo; }

private:
	void configObjEscena();
};

void gestorEscenas::limpiar_listaObj() {
	for (int i = 0; i < listaObjetos.size(); i++) {
		delete listaObjetos[i].obj;
	}
	listaObjetos.clear();
}

void gestorEscenas::configObjEscena() {
	limpiar_listaObj();

	switch (estado)
	{
	case gestorEscenas::LOGIN: {
		Matriz* Anaconda = new Matriz();
		Anaconda->inicializar(anaconda);
		listaObjetos.push_back({ Anaconda, 52, 21 });
		break;
	}
	case gestorEscenas::MENU_PRINCIPAL:
		break;
	case gestorEscenas::CATALOGO:
		break;
	case gestorEscenas::CARRITO:
		break;
	case gestorEscenas::PAGO:
		break;
	default:
		break;
	}
}

void gestorEscenas::dibujarEscena() {
	fondo.dibujarMatriz(0, 0);

	for (int i = 0; i < listaObjetos.size(); i++) {
		listaObjetos[i].obj->dibujarMatriz(listaObjetos[i].x, listaObjetos[i].y);
	}
}

void gestorEscenas::dibujarFondoSinLogo() {
	fondo.dibujarMatriz(0, 0);
}

void gestorEscenas::restaurarFondo(int x, int y, int ancho, int alto) {
	auto aplicarColorYForma = [this](int fila, int columna) {
		if (fila < 0 || fila >= 45 || columna < 0 || columna >= 130)return;
		int valor = this->fondo.getDatos(fila, columna);

		fondo.colores(valor);
		irA(columna, fila);
		cout << (char)219;
		};

	for (int i = 0; i < alto; i++) {
		for (int j = 0; j < ancho; j++) {
			aplicarColorYForma(y + i, x + j);
		}
	}
}

void gestorEscenas::setEscena(Escena nueva) {
	this->estado = nueva;

	if (estado == LOGIN) fondo.inicializar(fondo1);
	else if (estado == CATALOGO) fondo.inicializar(fondo2);

	configObjEscena();
}

gestorEscenas::gestorEscenas() {
	this->estado = LOGIN;
	fondo.inicializar(fondo1);
	configObjEscena();
}

gestorEscenas::~gestorEscenas() {
	limpiar_listaObj();
}