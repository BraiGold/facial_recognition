#ifndef knn_H
#define knn_H

#include <math.h>
#include <utility>      // std::pair, std::make_pair
#include <vector>
#include <iostream>

using namespace std;

int knn(int k, vector < double > TcImgTest, vector < vector < double > > TcImgsTraining, vector < int > labels);
bool pairCompare(const pair<int, double>& firstElem, const pair<int, double>& secondElem);
double distancia(vector < double > vecTest, vector < double > vecTrain);


#endif
