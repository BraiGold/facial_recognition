#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include "../../inputOutput.h"
#include "../../operacionesMatriciales.h"
#include "../../pca.h"
#include "../../knn.h"
#include <vector>
#include <stdlib.h>
using namespace std;

//main para experimentar con el metodo de la potencia y su acuracy en funcion del tamaño de la matriz
// para compilar:
//g++ ../../inputOutput.cpp mainExpAutoval.cpp ../../pca.cpp ../../metodoPotencia.cpp ../../knn.cpp ../../operacionesMatriciales.cpp -o exp1_generar

typedef unsigned char uchar;


int main(int argc, char** argv) {
  
 
 // creo matriz random
   srand(0);
   int s= rand();
 
	vector < vector < double > > M;
	for (int i = 0; i < atoi(argv[1]); i++)
	{
		 
		vector<double> vecTemp;
		for (int j = 0; j < atoi(argv[2]); j++)
		{
			 
			srand(s);
			int parteEntera = rand() % 10000;
			s = rand();
			srand(s);
			s = rand();
			double f = parteEntera + (double)rand() / RAND_MAX;
			vecTemp.push_back(f);
			
		}
		 M.push_back(vecTemp);
		
	}
	
//imprimo matriz random en txt
	 ofstream archivoMatriz;
	 archivoMatriz.open("../../archivosGenerados/Exp1_MatrizAutoval.txt");
	 
	 if(archivoMatriz.is_open()){
		 archivoMatriz << fixed;
		 archivoMatriz << setprecision(15);
			archivoMatriz << M.size() <<" "<< M[0].size() << endl;
			for (int i = 0; i < M.size() ; i++){
				for (int j = 0; j < M[0].size() ; j++)
				{
					archivoMatriz << M[i][j] << " ";//atento a este espacio al final
				}
				archivoMatriz << endl;
				
			}		
	}
	archivoMatriz.close();

	
	//aplico Pca
    srand(time(0));
	pca pca(M, atoi(argv[3]), atoi(argv[4]));

	 //imprimo autoval encontrados en archivo 
	 ofstream AutoValsExperimentacion;
	 AutoValsExperimentacion.open("../../archivosGenerados/Exp1_autovalores.txt");
	 
	 if(AutoValsExperimentacion.is_open()){
		 AutoValsExperimentacion << fixed;
		 AutoValsExperimentacion << setprecision(15);
			AutoValsExperimentacion << pca.pcaAutovalores().size() << endl;
			for (int i = 0; i < pca.pcaAutovalores().size() ; i++){
				AutoValsExperimentacion << pca.pcaAutovalores()[i] << " ";//atento a este espacio al final
			}		
	}
	AutoValsExperimentacion.close();
	  


  return 0;
}
