#include "knn.h"
#include <algorithm>

double distancia(vector < double > vecTest, vector < double > vecTrain) {
  double suma = 0;
  for (int i = 0; i < vecTest.size(); i++) {
    suma += (vecTest[i] - vecTrain[i])*(vecTest[i] - vecTrain[i]);
  }
  suma = sqrt(suma);
  return suma;
}

bool pairCompare(const pair<int, double>& firstElem, const pair<int, double>& secondElem) {
  return firstElem.second < secondElem.second;
}

int knn(int k, vector < double > TcImgTest, vector < vector < double > > TcImgsTraining, vector < int > labels) {
  vector < pair < int, double > > distancias(TcImgsTraining.size(), pair < int, double >());
  for (int i = 0; i < TcImgsTraining.size(); i++) {
    double dist = distancia(TcImgTest, TcImgsTraining[i]);
    distancias[i] = make_pair(labels[i], dist);
  }


  
  sort(distancias.begin(), distancias.end(), pairCompare);
  

  
  vector < int > votaciones(41, 0); //porque 41 sujetos
  
  int d = distancias.size();
  k = std::min(k,d);
  
  for (int j = 0; j < k; j++) {
    votaciones[distancias[j].first-1]++;
  }


  int max = 0;
  int sujetoMasVotado = 0;
  for (int p = 0; p < 41; p++) {
    if (votaciones[p] > max) {
      max = votaciones[p];
      sujetoMasVotado = p;
    }
  }

  return sujetoMasVotado+1;
}
