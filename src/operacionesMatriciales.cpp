#include "operacionesMatriciales.h"

matrix::matrix() {
	this->_matrix = (vector < vector < double > > ());
}

matrix::matrix(int rows, int columns) {
	vector < double > ancho(columns, 0);
	vector < vector < double > > informacion(rows, ancho);
	(this->_matrix) = informacion;
}

matrix::matrix(const matrix& mat) {
	vector < double > ancho(mat.numcolumnas(), 0);
	vector < vector < double > > informacion(mat.numfilas(), ancho);
	(this->_matrix) = informacion;
	for (int i = 0; i < mat.numfilas(); i++) {
		for (int j = 0; j < mat.numcolumnas(); j++) {
			((*this)._matrix)[i][j] = (mat._matrix)[i][j];
		}
	}
}

int matrix::numfilas() const {
	return (this->_matrix).size();
}

int matrix::numcolumnas() const {
	if(this->numfilas() == 0){
		return 0;
	}else{
		return (this->_matrix)[0].size();
	}
}

//asumo que son del mismo tamaño
matrix& matrix::operator+(const matrix& mat) {
	for (int i = 0; i < mat.numfilas(); i++) {
		for (int j = 0; j < mat.numcolumnas(); j++) {
			((*this)._matrix)[i][j] += (mat._matrix)[i][j];
		}
	}

	return (*this);
}

//asumo que son del mismo tamaño
matrix& matrix::operator-(const matrix& mat) {
	for (int i = 0; i < mat.numfilas(); i++) {
		for (int j = 0; j < mat.numcolumnas(); j++) {
			((*this)._matrix)[i][j] -= (mat._matrix)[i][j];
		}
	}

	return (*this);
}

matrix matrix::operator*(const matrix& mat) {
	matrix res(numfilas(), mat.numcolumnas());
	for(int i = 0; i < numfilas(); i++){
		for(int j = 0; j < mat.numcolumnas(); j++){
			for (int k = 0; k < numcolumnas(); k++) {
				(res._matrix)[i][j] += ((*this)._matrix)[i][k]*(mat._matrix)[k][j];
			}
		}
	}

	return res;
}

matrix& matrix::operator*(double num){
	for(int i = 0; i < numfilas(); i++){
		for (int j = 0; j < numcolumnas(); j++) {
			(*this)._matrix[i][j] = (*this)._matrix[i][j]*num;
		}
	}
	return (*this);
}

matrix& matrix::operator=(const matrix& mat) {
	//cout << "estoy usando la asignacion"<<endl;
	//cout << mat.numcolumnas() << " " << mat.numfilas() << " ";

	if (this != &mat) {
		vector < double > ancho(mat.numcolumnas(), 0);
		vector < vector < double > > informacion(mat.numfilas(), ancho);
		(this->_matrix) = informacion;

		for (int i = 0; i < mat.numfilas(); i++) {
			for (int j = 0; j < mat.numcolumnas(); j++) {
				((*this)._matrix)[i][j] = (mat._matrix)[i][j];
			}
		}
	}
	//cout << "termine la asignacion"<<endl;

	return (*this);
}

//asumo que la entrada no es la matriz nula
matrix matrix::transponer() {
	matrix res((this->_matrix)[0].size(), (this->_matrix).size());

	for (int i = 0; i < (this->_matrix).size(); i++) {
		for (int j = 0; j < this->numcolumnas(); j++) {
			res._matrix[j][i] = (this)->_matrix[i][j];
		}
	}

	return res;
}

void matrix::eliminacionGaussiana(vector<double> &terminoInd) {
	for (int i = 0; i < (*this).numfilas(); i++) {
		for (int j = i + 1; j < (*this).numfilas(); j++) {
			if (!eq((*this)._matrix[j][i], 0)) {
				double k = (*this)._matrix[j][i] / (*this)._matrix[i][i];
				//a la fila j le resto k por fila i
				for (int col = i; col < (*this).numcolumnas(); col++) {//resto filas
					(*this)._matrix[j][col] = (*this)._matrix[j][col] - k*(*this)._matrix[i][col];
				}
				terminoInd[j] = (terminoInd[j] - k*terminoInd[i]);//resto los numeros a la derecha del igual
			}
		}
	}
}

vector < double > matrix::sustitucionParaAtras(const vector < double > &b) {
	//supongo A es matriz triangular superior
	vector < double > res((*this).numcolumnas(), 0);
	double suma = 0;
	for (int i = (*this).numfilas() - 1; i >= 0; i--) {
		if (eq(this->_matrix[i][i], 0)) throw runtime_error("encontre un cero!! estas haciendo las cosas mal");
		suma = 0;
  	double ultimoDeFila = this->_matrix[i][this->numcolumnas() - 1];
  	int j = this->numcolumnas() - 1;
		while (eq(ultimoDeFila, 0) || j > i) {
			suma += this->_matrix[i][j] * res[j];
			j--;
			ultimoDeFila = this->_matrix[i][j];
		}
		res[i] = (b[i] - suma) / (this->_matrix)[i][i];
	}

	return res;
}

bool eq(double a, double b) {
	return fabs(a - b) < eps;
}


vector<double>& matrix::operator[] (int i){
	return this->_matrix[i];
}

const vector<double>& matrix::operator[] (int i) const{
	return this->_matrix[i];
}

void print_vector(const vector<double>& vec) {
	for (int i = 0; i < vec.size(); i++) {
		cout << vec[i] << " ";
	}
	cout << endl;
}

void matrix::print_matrix() {
	for (int i = 0; i < this->numfilas(); i++) {
		for (int j = 0; j < this->numcolumnas(); j++) {
			cout << this->_matrix[i][j] << "  ";
		}
		cout << endl;
	}
}

vector < double > matrix::operator*(const vector<double>& v){
	vector<double> res(numfilas(), 0);

	for(int i=0; i<numfilas(); i++){
		res[i]=prodInterno((this->_matrix)[i], v);
	}

	return res;
}

//asumo que se puede hacer el producto entre vectores de nx1 y 1xn
matrix prodVecColxVecFila(const vector<double>& v, const vector<double>& w){
	matrix V(v.size(), 1);
	matrix W(1, w.size());

	for(int i=0; i<v.size(); i++){//paso v a matriz
		V[i][0]=v[i]; 
	}
	

	for(int i=0; i<v.size(); i++){//paso w a matriz
		W[0][i]=w[i];
	}
	
	
	matrix res(V*W);

	return res;
}

void divisorVectorNum(vector < double > &vec, const double d) {
	for (int i = 0; i < vec.size(); i++) {
		vec[i] = vec[i] / d;
	}
}

//Asumo que se puede hacer el prod interno entre v y w
double prodInterno(const vector < double > & v, const vector < double > & w){
	double res=0.0;

	for(int i=0; i<v.size(); i++){
		res+=v[i]*w[i];
	}

	return res;
}

bool esBaseOrtonormal(const vector < vector <double> > &vectores){
	bool esBaseOrt=true;
	for(int k=0; k<vectores.size(); k++){
		if( !eq(prodInterno(vectores[k], vectores[k]), 1.0) ){
			esBaseOrt &= false;
			cout << "el prod inter de un vec por si mismo no es 1" << endl;
		}
	}
	
	for(int i=0; i<vectores.size(); i++){
		for(int j=i+1; j<vectores.size(); j++){
			if( !eq(prodInterno(vectores[i], vectores[j]), 0.0) ){
				esBaseOrt &= false;
				cout << "el prod inter de un vec por otro distinto no es 0" << endl;
			}
		}
	}
	return esBaseOrt;
}
