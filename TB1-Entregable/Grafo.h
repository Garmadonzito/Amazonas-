#pragma once

#include <vector>

using namespace std;

template<class T, T vacio = -1>
class CGrafo {
private:
    
    class CArco {
    public:
        T info;
        int v; // indice del vertice de llegada
        CArco(int vLlegada) { 
            info = vacio;
            v = vLlegada; 
        }
    };

    
    class CVertice {
    public:
        T info; 
        vector<CArco*>* ady; 
        CVertice() {
            info = vacio;
            ady = new vector<CArco*>();
        }
        ~CVertice() { 
            for (CArco* arco : *ady) delete arco;
            delete ady;
        }
    };

    
    vector<CVertice*>* vertices; 

public:
    CGrafo() {
        vertices = new vector<CVertice*>(); 
    }
    ~CGrafo() {
        for (CVertice* vertice : *vertices) delete vertice;
        delete vertices;
    }

    
    void adicionarVertice(T info) {
        CVertice* vert = new CVertice();
        vert->info = info;
        vertices->push_back(vert);
    }

    int cantidadVertices() {
        return vertices->size();
    }

    T obtenerVertice(int v) {
        return (vertices->at(v))->info;
    }

    void modificarVertice(int v, T info) {
        (vertices->at(v))->info = info;
    }

   
    void adicionarArco(int v, int vLlegada) {
        CVertice* ver = vertices->at(v);
        CArco* arc = new CArco(vLlegada);
        ver->ady->push_back(arc);
    }

    int cantidadArcos(int v) {
        return (vertices->at(v))->ady->size();
    }

    T obtenerArco(int v, int apos) {
        CVertice* ver = vertices->at(v);
        return (ver->ady->at(apos))->info;
    }

    void modificarArco(int v, int apos, T info) {
        CVertice* ver = vertices->at(v);
        (ver->ady->at(apos))->info = info;
    }

    int obtenerVerticeLlegada(int v, int apos) {
        CVertice* ver = vertices->at(v);
        return (ver->ady->at(apos))->v;
    }
};
