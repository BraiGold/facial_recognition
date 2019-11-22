#include "metodoPotencia.h"

void metodoPotencia(matrix &Mchica, int niter, int alfa, vector < vector < double > > &autoVectores, vector < double > &autoValores) {
  for (int i = 0; i < alfa; i++) {
	
    vector < double > v = randVector(Mchica.numcolumnas());
    vector < double > vCopia(v);
    normalizar(v);
  
    for (int j = 0; j < niter; j++) {
      v = Mchica*v;
      normalizar(v);
    }
    if( eq(prodInterno(vCopia, v), 0.0) ){
		cout << "ELEGISTE UN VECTOR ALEATORIO NO BUENO" << endl;
	}
    
    vector< double > autovect(v);//creo una copia
    autoVectores.push_back(autovect);
    double autovalor = prodInterno(Mchica*v, v) / prodInterno(v,v);
  
    autoValores.push_back(autovalor);
    deflacion(Mchica, v, autovalor);
    
  }
}

void deflacion(matrix &mchica, vector < double > autovec, double autoval) {

  mchica - (prodVecColxVecFila(autovec, autovec) * autoval);
}

void normalizar(vector < double > &vec) {
  /*double suma = 0;
  for (int j = 0; j < vec.size(); j++) {
    suma += vec[j]*vec[j];
  }
  suma = sqrt(suma);
*/
  vector< double> v(vec);
  for (int i = 0; i < vec.size(); i++) {
    vec[i] = v[i] / sqrt(prodInterno(v,v));
  }
  
}

vector < double > randVector(int size) {
  vector < double > v(size, 0.0);
	srand(time(0));
	for (unsigned int i = 0; i < size; i++) {
		v[i] = (rand() % 100); //cambio esta linea por un vector (1,1,...1) para debugear
		//v[i] = 1;
 	}
	return v;
}
