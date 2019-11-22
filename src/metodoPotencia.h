#ifndef metodoPotencia_H
#define metodoPotencia_H

#include <math.h>
#include <assert.h>
#include "operacionesMatriciales.h"

using namespace std;

void metodoPotencia(matrix &Mchica, int niter, int alfa, vector < vector < double > > &autoVectores, vector < double > &autoValores);
void deflacion(matrix &mchica, vector < double > autovec, double autoval);
void normalizar(vector < double > &vec);
vector < double > randVector(int size);

#endif
