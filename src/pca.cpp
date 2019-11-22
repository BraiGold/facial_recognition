#include "pca.h"

pca::pca(vector < vector < double > > imagenesTrain, int numeroIteraciones, int alfa) {
  vector < double > media = sacarMu(imagenesTrain);
  matrix matrixX = sacarX(imagenesTrain, media);
  matrix matrixXtraspuesta = matrixX.transponer();
  matrix Mchica = matrixX*matrixXtraspuesta;
  matrix Mchica2(Mchica);
  ofstream myOut("./archivosGenerados/mchica.txt");
  myOut.precision(10);
  for(int x=0; x<Mchica.numfilas(); x++){
  	for(int y=0; y<Mchica.numcolumnas(); y++){
  		myOut << fixed << Mchica[x][y] << " "; 
  	}
  	myOut << endl;
  }
  	
 // cout << "antes de metodo de la potencia" << endl;
  metodoPotencia(Mchica, numeroIteraciones, alfa, this->autoVectores, this->autoValores);
 // cout << "despues del metodo de la potencia se tienen: " << (this->autoVectores).size() << " autovec" << endl;
  //consigo autovec de la grande
  for (int i = 0; i < (this->autoVectores).size(); i++) {
    vector < double > pepe = matrixXtraspuesta*((this->autoVectores)[i]);
    divisorVectorNum(pepe, sqrt(this->autoValores[i]));
    (this->autoVectores)[i] = pepe;
  }
}

vector < double > sacarMu(vector < vector < double > > images) {
   vector < double > res(images[0].size(), 0);
   /*for (int i = 0; i < images[0].size(); i++) {
     double media = 0;*/
     for (int j = 0; j < images.size(); j++) {
       res = sumaVectores(res, images[j]);
     }
     divisorVectorNum(res, images.size());
   //}
   return res;
  
}

vector < vector < double > > pca::pcaAutovectores() {
  return this->autoVectores;
}

vector < double > pca::pcaAutovalores() {
  return this->autoValores;
}

matrix sacarX(vector < vector < double > > images, vector < double > media) {
   matrix res(images.size(), images[0].size());
   for (int i = 0; i < images.size(); i++) {
     res[i] = restaVectores(images[i], media);
     for (int j = 0; j < images[0].size(); j++) {
       res[i][j] = res[i][j] / sqrt(images.size() - 1);
     }
   }
   return res;
}

//ambos vectores son del mismo tamaño
vector < double > restaVectores(vector < double > a, vector < double > b) {
  vector < double > res(a.size(), 0);
  for (int i = 0; i < a.size(); i++) {
    res[i] = a[i] - b[i];
  }
  return res;
}

vector < double > sumaVectores(vector < double > a, vector < double > b) {
  vector < double > res(a.size(), 0);
  for (int i = 0; i < a.size(); i++) {
    res[i] = a[i] + b[i];
  }
  return res;
}
