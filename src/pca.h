#ifndef pca_H
#define pca_H

#include "operacionesMatriciales.h"
#include "metodoPotencia.h"
#include <fstream>

class pca {
  public:
    pca(vector < vector < double > > imagenes, int numeroIteraciones, int alfa);
    vector < vector < double > > pcaAutovectores();
    vector < double > pcaAutovalores();
  private:
    vector < vector < double > > autoVectores;
    vector < double > autoValores;
};

vector < double > sacarMu(vector < vector < double > > images);
vector < double > restaVectores(vector < double > a, vector < double > b);
vector < double > sumaVectores(vector < double > a, vector < double > b);
matrix sacarX(vector < vector < double > > images, vector < double > media);

#endif
