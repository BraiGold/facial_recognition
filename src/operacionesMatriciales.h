#ifndef operacionesMatriciales_H
#define operacionesMatriciales_H

#include <vector>
#include <map>
#include <math.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

const double eps = 0.001;

class matrix {

	public:
		matrix();
		matrix(int rows, int columns);
		matrix(const matrix& mat);
		int numfilas() const;
		int numcolumnas() const;
		matrix& operator+(const matrix& mat);
		matrix& operator-(const matrix& mat);
		matrix operator*(const matrix& mat);
		matrix& operator*(double num);
		vector < double > operator*(const vector<double>& v);
		matrix& operator=(const matrix& mat);
		matrix transponer();
		void eliminacionGaussiana(vector < double > &terminoInd);
		vector < double > sustitucionParaAtras(const vector < double > &b); //para resolver sistema
		vector<double>& operator[] (int i);
		const vector<double>& operator[] (int i) const;
		void print_matrix();

	private:
		vector < vector < double > > _matrix;
};

bool eq(double a, double b);
void print_vector(const vector<double>& vec);
matrix prodVecColxVecFila(const vector<double>& v, const vector<double>& w);
double prodInterno(const vector<double>& v, const vector<double>& w);
void divisorVectorNum(vector < double > &vec, const double d);
bool esBaseOrtonormal(const vector < vector <double> > &vectores); 

#endif
