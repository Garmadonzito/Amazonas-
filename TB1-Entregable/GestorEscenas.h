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

	Matriz fondo; //marco o diseño
	vector<Elemento> listaObjetos;
	Escena estado; //que estamos viendo ahora

public:

	gestorEscenas();
	~gestorEscenas();

	//Funciones Principales
	void setEscena(Escena nueva);//cambia el estado de la escena que se encuentra ahora
	void dibujarEscena();//dibuja la escena completa, el fondo y objetos
	void restaurarFondo(); //restaura una parte del fondo
	void limpiar_listaObj();

	//Getter
	Escena getEstado() { return estado; }//devuelve la escena actual
	Matriz& getFondoActual() { return fondo; }//devuelve el fondo


private:
	void configObjEscena();//creara la lista de objetos para cada escena
};

void gestorEscenas::limpiar_listaObj() {
	for (int i = 0;i < listaObjetos.size();i++) {
		delete listaObjetos[i].obj;
	}
	listaObjetos.clear();
}

void gestorEscenas::configObjEscena() {
	//1. limpiamos la lista
	limpiar_listaObj();

	//2. Configuracion de objetos segun "estado"/escena
	switch (estado)
	{
	case gestorEscenas::LOGIN: {
		Matriz* nubebig = new Matriz();
		nubebig->inicializar(nubeBig);
		listaObjetos.push_back({ nubebig ,35,15 });//Objeto, Ubicacion en X, Ubicacion en Y

		Matriz* cuadroTxt = new Matriz();
		cuadroTxt->inicializar(cuadroText);
		listaObjetos.push_back({ cuadroTxt,50,35 });//Objeto, Ubicacion en X, Ubicacion en Y

		Matriz* Anaconda = new Matriz();
		Anaconda->inicializar(anaconda);
		// 🔴 SOLUCIÓN DE COORDENADAS: 
		// La nube blanca (nubebig) empieza en X=35, Y=15.
		// Para centrar la anaconda azul dentro de ella, la movemos a X=52, Y=21.
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
	fondo.dibujarMatriz(0, 0);//solo dibuja el fondo base

	//recorre la listaObjetos y dibuja lo que hay adentro
	for (int i = 0; i < listaObjetos.size();i++) {
		listaObjetos[i].obj->dibujarMatriz(listaObjetos[i].x, listaObjetos[i].y);
	}
}

void gestorEscenas::setEscena(Escena nueva) {
	//0. estado cambio de escena
	this->estado = nueva;

	//1. Se limpia y actuliza el fondo segun escena nueva
	if (estado == LOGIN) fondo.inicializar(fondo1);
	else if (estado == CATALOGO) fondo.inicializar(fondo2);

	//2. Se limpia actualizan los objetos segun escena
	configObjEscena();
}


gestorEscenas::gestorEscenas() {
	//1. Se establece el estado inicial, osea pantalla login
	this->estado = LOGIN;

	//2.Cargamos los datos de la matriz login
	fondo.inicializar(fondo1);

	//3.Configuramos los objetos segun "estado"
	configObjEscena();
}

gestorEscenas::~gestorEscenas() {
	limpiar_listaObj();
}