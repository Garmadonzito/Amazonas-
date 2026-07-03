#pragma once
#include "Matriz.h"
#include <vector>

class gestorEscenas {
public:
	enum Escena { PAGINA_PRINCIPAL,LOGIN, MENU_PRINCIPAL_VENDEDOR, CATALOGO, CARRITO, PAGO };

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

	void setEscena(Escena nueva); //cambia el estado de la escena que se encuentra ahora
	void dibujarEscena();//dibuja la escena completa, el fondo y objetos
	void dibujarFondoSinLogo();
	void restaurarFondo(int x, int y, int ancho, int alto);
	void limpiar_listaObj();

	Escena getEstado() { return estado; }
	Matriz& getFondoActual() { return fondo; }

private:
	void configObjEscena();//creara la lista de objetos para cada escena
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
	case gestorEscenas::PAGINA_PRINCIPAL: {

		Matriz* nubebig = new Matriz();
		nubebig->inicializar(nubeBig);
		listaObjetos.push_back({ nubebig ,35,15 });//Objeto, Ubicacion en X, Ubicacion en Y

		Matriz* cuadroTxt = new Matriz();
		cuadroTxt->inicializar(cuadroText);
		listaObjetos.push_back({ cuadroTxt,50,35 });//Objeto, Ubicacion en X, Ubicacion en Y

		Matriz* Anaconda = new Matriz();
		Anaconda->inicializar(anaconda);
		listaObjetos.push_back({ Anaconda,47,6 });

		//Mascota decorativa del menu principal
		Matriz* pollito = new Matriz();
		pollito->inicializar(mascotaPollitoCara);
		listaObjetos.push_back({ pollito,12,20 });
		break;

	}
	case gestorEscenas::LOGIN: {
		Matriz* nubebig = new Matriz();
		nubebig->inicializar(nubeBig);
		listaObjetos.push_back({ nubebig ,35,15 });//Objeto, Ubicacion en X, Ubicacion en Y

		//Mascotas decorativas a los costados (no chocan con el texto del menu)
		Matriz* pollito = new Matriz();
		pollito->inicializar(mascotaPollitoCara);
		listaObjetos.push_back({ pollito,12,20 });

		Matriz* tucan = new Matriz();
		tucan->inicializar(mascotaTucan);
		listaObjetos.push_back({ tucan,95,20 });
		break;
	}
	case gestorEscenas::MENU_PRINCIPAL_VENDEDOR: {
		Matriz* cuadroTxt = new Matriz();
		cuadroTxt->inicializar(cuadroText);
		listaObjetos.push_back({ cuadroTxt,50,35 });//Objeto, Ubicacion en X, Ubicacion en Y

		Matriz* pizarrabig = new Matriz();
		pizarrabig->inicializar(pizarraBig);
		listaObjetos.push_back({ pizarrabig,30,10 });
		break;

	}

	case gestorEscenas::CATALOGO: {
		break;
	}
	case gestorEscenas::CARRITO: {
		break;
	}
	case gestorEscenas::PAGO: {
		break;
	}
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

void gestorEscenas::restaurarFondo(int x, int y, int ancho, int alto) { //...
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