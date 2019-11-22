#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include "inputOutput.h"
#include "operacionesMatriciales.h"
#include "pca.h"
#include "knn.h"
#include <time.h>

using namespace std;

//PARA COMPILAR g++ inputOutput.cpp main.cpp pca.cpp metodoPotencia.cpp knn.cpp operacionesMatriciales.cpp -o NOMBRE_DE_EJECUTABLE
//PARA EJECUTAR ej: ./NOMBRE_DE_EJECUTABLE -m 1 -i train.csv -q test.csv -o outputResult.csv -k 3 -a 2 -n 30
//donde k es de knn, a es alfa de pca y n es num de iteraciones del Metodo Potencia
//LOS SUJETOS ESTAN EN DIRECTORIOS reduced/ y full/
//TRAIN Y TEST pueden estar en cualquier carpeta que tenga a src/main.cpp como direcctorio raiz

typedef unsigned char uchar;

void read_image(std::string filename, uchar** data, int* width, int* height){
  *data = NULL;
  *width = 0;
  *height = 0;
  PPM_LOADER_PIXEL_TYPE pt = PPM_LOADER_PIXEL_TYPE_INVALID;

  bool ret = LoadPPMFile(data, width, height, &pt, filename.c_str());
  cout << "tipo de img " << pt << endl;
  if (!ret || width == 0|| height == 0|| pt!=PPM_LOADER_PIXEL_TYPE_GRAY_8B/*PPM_LOADER_PIXEL_TYPE_RGB_8B*/){
    throw std::runtime_error("Fallo al leer la imagen.");
  }

}

int get_pixel_pgm(uchar* data, int i, int j, int height, int width){
  if(i > height)
    throw std::runtime_error("El direccionamiento vertical no puede ser mayor a la altura.");
  if(j > width)
    throw std::runtime_error("El direccionamiento horizontal no puede ser mayor al ancho.");
  return data[i*width + j];
}

vector<string> cargarImagenesYLabels(vector< vector <double> >& Imagenes, vector<int>& labels, ifstream &inFile){
  vector<string> vecDirecciones;
  string dirCaraSujeto;
  string sujeto;

   while(inFile >> dirCaraSujeto && inFile >> sujeto){

    vecDirecciones.push_back(dirCaraSujeto);


    dirCaraSujeto=dirCaraSujeto.substr(0, dirCaraSujeto.size()-1);//extrae ','
    uchar* data = NULL;
    int width = 0, height = 0;
    read_image(dirCaraSujeto, &data, &width, &height);//en data esta la img guardada por filas
    vector<double> cara;

    for(int i=0; i<width*height; i++){
      cara.push_back(data[i]);
    }
    Imagenes.push_back(cara);


    sujeto=sujeto.substr(0, sujeto.size()-1);//extrae ','
    int sujetoNum=atoi(sujeto.c_str());
    labels.push_back(sujetoNum);
  }

  return vecDirecciones;
}


// void test_image(){
//   uchar* data = NULL;
//   int width = 0, height = 0;
//   std::string filename = "prueba.ppm";
//   read_image(filename, &data, &width, &height); // Ejemplo de llamada
//
//   for (int h = 0; h < height; ++h){
//     for (int w = 0; w < width; ++w){
//       cout << get_pixel_average(data, h, w, height, width) << " "; // Ejemplo de lectura de un pixel
//     }
//     cout << endl;
//   }
//   delete [] data;
// }
//
// void test_load(){
//
//   uchar* data = NULL;
//   int width = 0, height = 0;
//   PPM_LOADER_PIXEL_TYPE pt = PPM_LOADER_PIXEL_TYPE_INVALID;
// //  std::string filename = "buda.0.ppm";
//   std::string filename = "prueba.ppm";
//
//   bool ret = LoadPPMFile(&data, &width, &height, &pt, filename.c_str());
//   if (!ret || width == 0|| height == 0|| pt!=PPM_LOADER_PIXEL_TYPE_RGB_8B)
//   {
//     throw std::runtime_error("test_load failed");
//   }
//
//   delete [] data;
// }
//
// void test_save(){
//
//   char comments[100];
//   sprintf(comments, "%s", "Hello world");
//
//   int width = 3, height =1;
//   uchar* data = new uchar[width*height*3];
//   data[0] = data[1] = data[2] = 100; // RGB
//   data[3] = data[4] = data[5] = 150; // RGB
//   data[6] = data[7] = data[8] = 245; // RGB
//   std::string filename = "prueba.ppm";
//
//   bool ret = SavePPMFile(filename.c_str(),data,width,height,PPM_LOADER_PIXEL_TYPE_RGB_8B, comments);
//   if (!ret)
//   {
//     std::cout << "ERROR: couldn't save Image to ppm file" << std::endl;
//   }
// }

void test_ConstruccionVacioYConParam(){
  matrix vacia;

  cout << "debe ser vacia"<< endl;
  cout << "filas: " << vacia.numfilas() << " cols: "<< vacia.numcolumnas() << endl;
  vacia.print_matrix();


  matrix conElemCuadrada(4,4);

  cout << "debe ser de 4x4"<<endl;
  cout << "filas: " << conElemCuadrada.numfilas() << " cols: "<< conElemCuadrada.numcolumnas() << endl;
  conElemCuadrada.print_matrix();


  matrix conElemNoCuadrada(3,6);

  cout << "debe ser de 3x6"<<endl;
  cout << "filas: " << conElemNoCuadrada.numfilas() << " cols: "<< conElemNoCuadrada.numcolumnas() << endl;
  conElemNoCuadrada.print_matrix();
}

void test_ConstruccionCopia(){
  matrix esCuadrada(4,4);
  esCuadrada[0][0]=10.5; //10.5  0  0   0
  esCuadrada[1][2]=9.5;  //0     0  9.5 0
  esCuadrada[2][3]=5.5;  //0     0  0   5.5
  esCuadrada[3][0]=3.5;  //3.5   0  0   0

  matrix copiaCuadrada(esCuadrada);

  cout << "es una copia de una cuadrada" << endl;
  cout << "filas: " << copiaCuadrada.numfilas() << " cols: "<< copiaCuadrada.numcolumnas() << endl;
  copiaCuadrada.print_matrix();


  matrix noEsCuadrada(3,6);
  noEsCuadrada[0][0]=2;    //2 0 1 0   3 0
  noEsCuadrada[0][2]=1;    //0 8 0 0   0 6.2
  noEsCuadrada[0][4]=3;    //0 0 0 2.5 0 0
  noEsCuadrada[1][1]=8;
  noEsCuadrada[1][5]=6.2;
  noEsCuadrada[2][3]=2.5;

  matrix copiaNoCuadrada(noEsCuadrada);

  cout << "es una copia de una no cuadrada" << endl;
  cout << "filas: " << copiaNoCuadrada.numfilas() << " cols: "<< copiaNoCuadrada.numcolumnas() << endl;
  copiaNoCuadrada.print_matrix();
}

void test_Asignacion(){
  matrix A(4,4);
  A[0][0]=1;  //A= 1 2 1 6.5
  A[0][1]=2;  //   0 2 3 6.5
  A[0][2]=1;  //   0 0 6 5.5
  A[0][3]=6.5;//   0 0 0 1.5
  A[1][1]=2;
  A[1][2]=3;
  A[1][3]=6.5;
  A[2][2]=6;
  A[2][3]=5.5;
  A[3][3]=1.5;

  matrix BVacia;
  matrix B4X4SinElem(4,4);
  matrix C4X4ConElem(4,4);
  matrix D4x7(4,7);

  C4X4ConElem[1][0]=1.5;//C=0   0   0   0
  C4X4ConElem[2][0]=2.5;//  1.5 0   0   0
  C4X4ConElem[2][1]=3.5;//  2.5 3.5 0   0
  C4X4ConElem[3][0]=2.2;//  2.2 3.2 6.2 0
  C4X4ConElem[3][1]=3.2;
  C4X4ConElem[3][2]=6.2;

  D4x7[0][0]=3.5;//D=3.5 0   7.5 0 0 0   0
  D4x7[0][2]=7.5;//  0   6.2 0   0 0 6.2 0
  D4x7[1][1]=6.2;//  0   0   0   0 0 0   8.2
  D4x7[1][5]=6.2;//  0   0   0   0 0 0   0
  D4x7[2][6]=8.2;

  cout << "matriz vacia antes y despues de asignar" << endl;
  BVacia.print_matrix();
  cout << endl;
  BVacia=A;
  BVacia.print_matrix();
  cout << endl;

  cout << "matriz de 4x4 sin elementos antes y despues de asignar" << endl;
  B4X4SinElem.print_matrix();
  cout << endl;
  B4X4SinElem=A;
  B4X4SinElem.print_matrix();
  cout << endl;


  cout << "matriz de 4x4 con elementos antes y despues de asignar" << endl;
  C4X4ConElem.print_matrix();
  cout << endl;
  C4X4ConElem=A;
  C4X4ConElem.print_matrix();
  cout << endl;


  cout << "matriz de 4x7 antes y despues de asignar" << endl;
  D4x7.print_matrix();
  cout << endl;
  D4x7=A;
  D4x7.print_matrix();
  cout << endl;


  cout << endl;
  cout <<"uso otra matriz para asginar" << endl;
  cout << endl;


  matrix E(3,8);

  E[0][0]=1;    //A= 1 0 0   6.5 0   0   8 0
  E[0][3]=6.5;  //   0 0 3.5 0   6.5 0   0 9
  E[0][6]=8;    //   0 0 0   0   0   3.5 0 0
  E[1][2]=3.5;
  E[1][4]=6.5;
  E[1][7]=9;
  E[2][5]=3.5;

  matrix FVacia;
  matrix G5X10(5,10);
  matrix H2X4(2,4);

  G5X10[0][9]=1;  //G=0 0 0 0 0 0 0 0 0 1
  G5X10[1][0]=1;  //  1 0 0 0 0 0 0 0 0 0
  G5X10[2][0]=2;  //  2 5 0 0 0 0 0 5 0 0
  G5X10[2][1]=5;  //  0 0 0 0 0 0 0 0 1 0
  G5X10[2][7]=5;  //  1 0 0 0 0 0 0 0 0 1
  G5X10[3][8]=1;
  G5X10[4][0]=1;
  G5X10[4][9]=1;

  H2X4[0][1]=8;//H=0 8 5 0
  H2X4[0][2]=5;//  0 0 0 9
  H2X4[1][3]=9;

  cout << "matriz vacia antes y despues de asignar" << endl;
  FVacia.print_matrix();
  cout << endl;
  FVacia=E;
  FVacia.print_matrix();
  cout << endl;


  cout << "matriz mas grande antes y despues de asignar" << endl;
  G5X10.print_matrix();
  cout << endl;
  G5X10=E;
  G5X10.print_matrix();
  cout << endl;


  cout << "matriz mas pequena vacia antes y despues de asignar" << endl;
  H2X4.print_matrix();
  cout << endl;
  H2X4=E;
  H2X4.print_matrix();
  cout << endl;
}

void test_Transponer(){
  matrix cuadrada(5,5);

  cuadrada[0][0]=1;//cuadrada=1 0 4 0 9
  cuadrada[0][2]=4;//         0 3 0 7 0
  cuadrada[0][4]=9;//         2 0 0 0 9
  cuadrada[1][1]=3;//         0 3 5 0 0
  cuadrada[1][3]=7;//         0 0 0 8 9
  cuadrada[2][0]=2;
  cuadrada[3][1]=3;
  cuadrada[3][2]=5;
  cuadrada[4][3]=8;
  cuadrada[4][4]=9;

  //resultado esperado
  //1 0 2 0 0
  //0 3 0 3 0
  //4 0 0 5 0
  //0 7 0 0 8
  //9 0 0 0 9

  cout << "matriz original de 5x5" << endl;
  cuadrada.print_matrix();
  cout << endl;

  matrix copiaCuad(cuadrada.transponer());

  cout << "matriz traspuesta de 5x5" << endl;
  copiaCuad.print_matrix();
  cout << endl;


  matrix noCuadrada(5,10);

  noCuadrada[0][1]=7;//noCuadrada=0 7 0 0 0 0 0 0 0 8
  noCuadrada[0][9]=8;//           6 0 0 0 0 0 0 0 0 0
  noCuadrada[1][0]=6;//           0 9 0 0 0 5 0 0 0 5
  noCuadrada[2][1]=9;//           3 0 0 0 0 0 0 0 0 3
  noCuadrada[2][5]=5;//           0 0 0 0 0 0 0 0 0 6
  noCuadrada[2][9]=5;
  noCuadrada[3][0]=3;
  noCuadrada[3][9]=3;
  noCuadrada[4][9]=6;

  //resultado esperado
  //0 6 0 3 0
  //7 0 9 0 0
  //0 0 0 0 0
  //0 0 0 0 0
  //0 0 0 0 0
  //0 0 5 0 0
  //0 0 0 0 0
  //0 0 0 0 0
  //0 0 0 0 0
  //8 0 5 3 6


  cout << "matriz original de 5x10" << endl;
  noCuadrada.print_matrix();
  cout << endl;

  matrix copiaNoCuad(noCuadrada.transponer());

  cout << "matriz traspuesta de 5x10" << endl;
  copiaNoCuad.print_matrix();
  cout << endl;

}

void test_MultiplicacionMatricial(){
  matrix A(3,3);
  matrix B(3,3);

  A[0][0]=-1;//A=-1 0 7
  A[0][2]=7; //  0 -2 0
  A[1][1]=-2;//  3 0 1
  A[2][0]=3;
  A[2][2]=1;

  B[0][1]=-9;//B=0 -9 0
  B[1][0]=2; //  2 -2 5
  B[1][1]=-2;//  0 -3 0
  B[1][2]=5;
  B[2][1]=-3;

  matrix AxB(A*B);

  cout << "prod de 3x3 y 3x3" << endl;
  AxB.print_matrix();
  cout << endl;

  matrix resAxB(3,3);

  resAxB[0][1]=-12;//A*B=0  -12  0
  resAxB[1][0]=-4; //    -4 4   -10
  resAxB[1][1]=4;  //    0  -30  0
  resAxB[1][2]=-10;
  resAxB[2][1]=-30;

  cout << "el resultado de 3x3 y 3x3 debe ser" << endl;
  resAxB.print_matrix();
  cout << endl;


  matrix C(3,5);
  matrix D(5,3);

  C[0][1]=6; //C=0  6 0  0 6
  C[0][4]=6; //  -1 0 -2 0 0
  C[1][0]=-1;//  0  0 0  8 0
  C[1][2]=-2;
  C[2][3]=8;

  D[0][0]=3; //D=3 0 0
  D[1][1]=2; //  0 2 0
  D[2][0]=6; //  6 0 -3
  D[2][2]=-3;//  4 0 0
  D[3][0]=4; //  0 1 -2
  D[4][1]=1;
  D[4][2]=-2;

  matrix CxD(C*D);

  cout << "prod de 3x5 y 5x3" << endl;
  CxD.print_matrix();
  cout << endl;

  matrix resCxD(3,3);

  resCxD[0][1]=18; //C*D=0   18 -12
  resCxD[0][2]=-12;//    -15 0 6
  resCxD[1][0]=-15;//    32  0 0
  resCxD[1][2]=6;
  resCxD[2][0]=32;


  cout << "el resultado de 3x5 y 5x3 debe ser" << endl;
  resCxD.print_matrix();
  cout << endl;


  matrix DxC(D*C);

  cout << "prod de 5x3 y 3x5" << endl;
  DxC.print_matrix();
  cout << endl;

  matrix resDxC(5,5);

  resDxC[0][1]=18;//D*C=0  18 0  0   18
  resDxC[0][4]=18;//    -2  0  -4 0   0
  resDxC[1][0]=-2;//    0   36 0  -24 36
  resDxC[1][2]=-4;//    0   24 0  0   24
  resDxC[2][1]=36;//    -1  0  -2 -16   0
  resDxC[2][3]=-24;
  resDxC[2][4]=36;
  resDxC[3][1]=24;
  resDxC[3][4]=24;
  resDxC[4][0]=-1;
  resDxC[4][2]=-2;
  resDxC[4][3]=-16;

  cout << "el resultado de 5x3 y 3x5 debe ser" << endl;
  resDxC.print_matrix();
  cout << endl;

}

void test_MultiplicacionPorEscalar(){
  matrix A(3,5);

  A[0][0]=6; //A=6 0 0 9 -2
  A[0][3]=9; //  0 0 3 0 0
  A[0][4]=-2;//  0 5 0 0 -1
  A[1][2]=3;
  A[2][1]=5;
  A[2][4]=-1;

  double val1=5.0;

  cout << "matriz original" << endl;
  A.print_matrix();
  cout << endl;
  A*val1;
  cout << "matriz al multip por "<< val1 << endl;
  A.print_matrix();
  cout << endl;


  matrix B(4,6);

  B[0][0]=3;   //B=3 0 5 0 0 -11
  B[0][2]=5;   //  0 1 0 0 8 0
  B[0][5]=-11; //  6 0 0 9 0 -13
  B[1][1]=1;   //  0 0 0 0 2 0
  B[1][4]=8;
  B[2][0]=6;
  B[2][3]=9;
  B[2][5]=-13;
  B[3][4]=2;

  double val2=0.5;

  cout << "matriz original" << endl;
  B.print_matrix();
  cout << endl;
  B*val2;
  cout << "matriz al multip por "<< val2 << endl;
  B.print_matrix();
  cout << endl;
}

void test_Resta(){
  matrix A(4,4);
  matrix B(4,4);

  A[0][0]=6;  //A=6 0   1 1.5
  A[0][2]=1;  //  0 2   0 0
  A[0][3]=1.5;//  3 0   9 3.5
  A[1][1]=2;  //  0 3.5 0 0
  A[2][0]=3;
  A[2][2]=9;
  A[2][3]=3.5;
  A[3][1]=3.5;

  B[0][0]=3;  //B=3 2 0   0
  B[0][1]=2;  //  0 3 2.5 0
  B[1][1]=3;  //  9 2 1   6.5
  B[1][2]=2.5;//  0 3 0   1
  B[2][0]=9;
  B[2][1]=2;
  B[2][2]=1;
  B[2][3]=6.5;
  B[3][1]=3;
  B[3][3]=1;


  A-B;

  cout << "resta de matrices 4x4" << endl;
  A.print_matrix();
  cout << endl;

  matrix resA_B(4,4);

  resA_B[0][0]=3;  //A-B=3  -2  1    1.5
  resA_B[0][1]=-2; //    0  -1  -2.5 0
  resA_B[0][2]=1;  //    -6 -2  8    -3
  resA_B[0][3]=1.5;//    0  0.5 0    -1
  resA_B[1][1]=-1;
  resA_B[1][2]=-2.5;
  resA_B[2][0]=-6;
  resA_B[2][1]=-2;
  resA_B[2][2]=8;
  resA_B[2][3]=-3;
  resA_B[3][1]=0.5;
  resA_B[3][3]=-1;

  cout << "resta de matrices 4x4 debe ser" << endl;
  resA_B.print_matrix();
  cout << endl;


  matrix C(3,6);
  matrix D(3,6);

  C[0][1]=2.5;//C=0 2.5 3    0   0  9
  C[0][2]=3;  //  0 0   0    5.5 0  0
  C[0][5]=9;  //  0 0   -3.5 0   -7 0
  C[1][3]=5.5;
  C[2][2]=-3.5;
  C[2][4]=-7;

  D[0][1]=-6;  //D=0   -6  0  0 -5.5 0
  D[0][4]=-5.5;//  9   0  -2  0 0    2.5
  D[1][0]=9;   //  2.5 0   0  7 1    0
  D[1][2]=-2;
  D[1][5]=2.5;
  D[2][0]=2.5;
  D[2][3]=7;
  D[2][4]=1;

  C-D;

  cout << "resta de matrices 3x6" << endl;
  C.print_matrix();
  cout << endl;

  matrix resC_D(3,6);

  resC_D[0][1]=8.5; //C-D=0    8.5 3    0   5.5 9
  resC_D[0][2]=3;   //    -9   0   2    5.5 0   -2.5
  resC_D[0][4]=5.5; //    -2.5 0   -3.5 -7  -8  0
  resC_D[0][5]=9;
  resC_D[1][0]=-9;
  resC_D[1][2]=2;
  resC_D[1][3]=5.5;
  resC_D[1][5]=-2.5;
  resC_D[2][0]=-2.5;
  resC_D[2][2]=-3.5;
  resC_D[2][3]=-7;
  resC_D[2][4]=-8;

  cout << "resta de matrices 3x6 debe ser" << endl;
  resC_D.print_matrix();
  cout << endl;
}

void test_ElimGauss(){
  cout << "---TRIANGULAR---" << endl;
  matrix A4x4(4,4);
  A4x4[0][0]=1;//A4x4=1  1  0   3
  A4x4[0][1]=1;//     2  1  -1  1
  A4x4[0][3]=3;//     3  -1 -1  2
  A4x4[1][0]=2;//     -1 2  3  -1
  A4x4[1][1]=1;
  A4x4[1][2]=-1;
  A4x4[1][3]=1;
  A4x4[2][0]=3;
  A4x4[2][1]=-1;
  A4x4[2][2]=-1;
  A4x4[2][3]=2;
  A4x4[3][0]=-1;
  A4x4[3][1]=2;
  A4x4[3][2]=3;
  A4x4[3][3]=-1;

  vector<double> b; //bt=4 1 -3 4

  b.push_back(4);
  b.push_back(1);
  b.push_back(-3);
  b.push_back(4);

  A4x4.eliminacionGaussiana(b);

  cout << "matriz desp de Gauss" <<endl;
  // A4x4.print_matrix();
  cout << endl;

  cout << "termino ind desp de Gauss" <<endl;
  // A4x4.print_vector(b);
  cout << endl;


  matrix A4x4_Esperado(4,4);

  A4x4_Esperado[0][0]=1; //A_Esp=1 1  0  3
  A4x4_Esperado[0][1]=1; //      0 -1 -1 -5
  A4x4_Esperado[0][3]=3; //      0 0  3  13
  A4x4_Esperado[1][1]=-1;//      0 0  0  -13
  A4x4_Esperado[1][2]=-1;
  A4x4_Esperado[1][3]=-5;
  A4x4_Esperado[2][2]=3;
  A4x4_Esperado[2][3]=13;
  A4x4_Esperado[3][3]=-13;

  vector<double> b_Esperado;//b_Esp=4 -7 13 -13

  b_Esperado.push_back(4);
  b_Esperado.push_back(-7);
  b_Esperado.push_back(13);
  b_Esperado.push_back(-13);

  cout << "matriz Esperada desp de Gauss" <<endl;
  // A4x4_Esperado.print_matrix();
  cout << endl;

  cout << "termino ind Esperado desp de Gauss" <<endl;
  // A4x4_Esperado.print_vector(b_Esperado);
  cout << endl;


  cout << "---RESOLVER SISTEMA---" << endl;


  vector<double> vecSolucion;
  vecSolucion=A4x4.sustitucionParaAtras(b);

  cout << "solucion de la matriz 4x4" <<endl;
  // A4x4.print_vector(vecSolucion);
  cout << endl;

  vector<double> vecSolucion_Esperado;//sol_Esp=-1 2 0 1

  vecSolucion_Esperado.push_back(-1);
  vecSolucion_Esperado.push_back(2);
  vecSolucion_Esperado.push_back(0);
  vecSolucion_Esperado.push_back(1);

  cout << "solucion Esperada de la matriz 4x4" <<endl;
  // A4x4_Esperado.print_vector(vecSolucion_Esperado);
  cout << endl;


  /*
  //caso donde la matriz esta triangulada y hay un cero en la diagonal
  matrix B(4,4);
  B[1][1]=5;
  B[2][2]=10;
  B[3][3]=20;

  vector<double> terInd;
  terInd.push_back(0);
  terInd.push_back(10);
  terInd.push_back(20);
  terInd.push_back(40);

  vector<double> sol;
  sol=B.sustitucionParaAtras(terInd);
  cout<<"fin de ejecucion"<<endl;*/

}

void test_Suma(){
  matrix A(3,3);
  matrix B(3,3);

  A[0][0]=2; //A=2 0 -3
  A[0][2]=-3;//  6 0 0
  A[1][0]=6; //  0 0 2
  A[2][2]=2;

  B[0][0]=1;//B=1 0 5
  B[0][2]=5;//  3 1 0
  B[1][0]=3;//  0 0 -1.9999
  B[1][1]=1;
  B[2][2]=-1.9999;

  A+B;

  cout << "matriz 3x3 luego de la suma" << endl;
  A.print_matrix();
  cout << endl;

  matrix resSuma(3,3);

  resSuma[0][0]=3;//A+B=3 0 2
  resSuma[0][2]=2;//    9 1 0
  resSuma[1][0]=9;//    0 0 0.0001
  resSuma[1][1]=1;
  resSuma[2][2]=0.0001;

  cout << "matriz 3x3 luego de la suma debe ser" << endl;
  resSuma.print_matrix();
  cout << endl;


  matrix C(2,4);
  matrix D(2,4);

  C[0][0]=1;//C=1 5 0 0.005
  C[0][1]=5;//  0 0 7 0.1
  C[0][3]=0.005;
  C[1][2]=7;
  C[1][3]=0.1;

  D[0][0]=-0.9;   //D=-0.9 0 0  -0.0045
  D[0][3]=-0.0045;//  0    0 -6 0
  D[1][2]=-6;

  C+D;

  cout << "matriz 2x4 luego de la suma" << endl;
  C.print_matrix();
  cout << endl;

  matrix resSuma2(2,4);

  resSuma2[0][0]=0.1;//C+D=0.1 5 0 0.0005
  resSuma2[0][1]=5;//    0   0 1 0.1
  resSuma2[0][3]=0.0005;
  resSuma2[1][2]=1;
  resSuma2[1][3]=0.1;

  cout << "matriz 2x4 luego de la suma debe ser" << endl;
  resSuma2.print_matrix();
  cout << endl;
}

void test_MetodoPotencia(int numIter, int alpha){
	cout << "CASO 1" << endl;
	matrix Diag(4,4);

	Diag[0][0]=9;//Diag=9 0 0 0
	Diag[1][1]=7;//     0 7 0 0
	Diag[2][2]=5;//     0 0 5 0
	Diag[3][3]=3;//     0 0 0 3

	vector< vector< double > > autoVec;
	vector< double > autoVal;

	metodoPotencia(Diag, numIter, alpha, autoVec, autoVal);
	for(int x=0; x<alpha ; x++){
		cout << "x: " << x << endl;
		cout << "autovalor: " << autoVal[x] << endl;
		cout << "autovector: ";
		print_vector(autoVec[x]);
	}
	bool baseOrt=esBaseOrtonormal(autoVec);
	cout << "los autovec forman una base ortonormal: " << baseOrt << endl;


	cout << "CASO 2" << endl;
	matrix Diag2(4,4);

	Diag2[0][0]=3;//Diag2=3 0 0 0
	Diag2[1][1]=5;//      0 5 0 0
	Diag2[2][2]=7;//      0 0 7 0
	Diag2[3][3]=9;//      0 0 0 9

	vector< vector< double > > autoVec2;
	vector< double > autoVal2;

	metodoPotencia(Diag2, numIter, alpha, autoVec2, autoVal2);
	for(int x=0; x<alpha ; x++){
		cout << "x: " << x << endl;
		cout << "autovalor: " << autoVal2[x] << endl;
		cout << "autovector: ";
		print_vector(autoVec2[x]);
	}
	bool baseOrt2=esBaseOrtonormal(autoVec2);
	cout << "los autovec forman una base ortonormal: " << baseOrt2 << endl;


	cout << "CASO 3" << endl;
	matrix C(3,3);

	C[0][0]=2;//C=2 1 0
	C[0][1]=1;//  1 2 0
	C[1][0]=1;//  0 0 1
	C[1][1]=2;
	C[2][2]=1;

	vector< vector< double > > autoVec3;
	vector< double > autoVal3;

	metodoPotencia(C, numIter, 3, autoVec3, autoVal3);
	for(int x=0; x<3 ; x++){
		cout << "x: " << x << endl;
		cout << "autovalor: " << autoVal3[x] << endl;
		cout << "autovector: ";
		print_vector(autoVec3[x]);
	}
	bool baseOrt3=esBaseOrtonormal(autoVec3);
	cout << "los autovec forman una base ortonormal: " << baseOrt3 << endl;


	cout << "CASO 4" << endl;
	matrix SemiDP(3,3);

	SemiDP[0][0]=3750;//C=3750   3600   2500
    SemiDP[0][1]=3600;//  3600   4125   2450
    SemiDP[0][2]=2500;//  2500   2450   2300
	SemiDP[1][0]=3600;
	SemiDP[1][1]=4125;
	SemiDP[1][2]=2450;
	SemiDP[2][0]=2500;
	SemiDP[2][1]=2450;
	SemiDP[2][2]=2300;

	vector< vector< double > > autoVec4;
	vector< double > autoVal4;

	metodoPotencia(SemiDP, numIter, 3, autoVec4, autoVal4);
	for(int x=0; x<3 ; x++){
		cout << "x: " << x << endl;
		cout << "autovalor: " << autoVal4[x] << endl;
		cout << "autovector: ";
		print_vector(autoVec4[x]);
	}
	bool baseOrt4=esBaseOrtonormal(autoVec4);
	cout << "los autovec forman una base ortonormal: " << baseOrt4 << endl;

}

void test_Catedra(vector < vector < double> > &imagenesTrain, int numIter, int alpha, char* archivo){
	//autovalores obtenidos
	pca pca(imagenesTrain, numIter, alpha);

	//autovalores esperados
	string test(archivo);
	test = test.substr(0, test.size()-3);
	test = test + ".expected";
	//cout << test << endl;
	ifstream inAutoval;
	inAutoval.open(test.c_str());

	double num;
	vector<double> resultadosEsperados;
	if(inAutoval.is_open()){
		cout << "leyendo archivo" << endl;
		inAutoval.precision(10);
		//cout.precision(10);
		while(inAutoval >> num){
			//cout << num << endl;
			//num*=num;
			resultadosEsperados.push_back(num);
		}
	}

	//Comparacion
	bool sonIguales=true;
	ofstream outRes("resAutovalores.txt");
	for(int i=0; i<pca.pcaAutovalores().size() && i<resultadosEsperados.size(); i++){
		outRes.precision(10);
		outRes << "resultado esperado " << i <<" : " << fixed << resultadosEsperados[i] << endl;
		outRes << "resultado obtenido " << i <<" : " << fixed << sqrt(pca.pcaAutovalores()[i]) << endl;
		outRes << "-------------------------" << endl;
 		if( !eq(resultadosEsperados[i], sqrt(pca.pcaAutovalores()[i])) ){
			sonIguales=false;
			cout << "paso algo con el autovalor " << i << endl;
		}
	}
	cout << "Tenemos los mismo autovalores: " << sonIguales << endl;


	bool baseOrt=esBaseOrtonormal(pca.pcaAutovectores());
	cout << "Los autovec forman una base ortonormal: " << baseOrt << endl;
}

int main(int argc, char** argv) {
  //TETST
  /*test_ConstruccionVacioYConParam();
  cout << "----------" << endl;
  test_ConstruccionCopia();
  cout << "----------" << endl;
  test_Asignacion();
  cout << "----------" << endl;
  test_Transponer();
  cout << "----------" << endl;
  test_MultiplicacionMatricial();
  cout << "----------" << endl;
  test_MultiplicacionPorEscalar();
  cout << "----------" << endl;
  test_Resta();
  cout << "----------" << endl;
  test_ElimGauss();
  cout << "----------" << endl;
  test_Suma();*/
  //FIN DE TEST

  istringstream ss(argv[2]);
  int metodo;
  if (!(ss >> metodo))
    cerr << "Método inválido " << argv[2] << '\n';

  //cout << metodo << endl;


  vector < vector < double> > imagenesTrain;
  vector < int > labelsTrain;

  char* archivo = argv[4];
  ifstream inputTrain(archivo);
  if (!inputTrain) {
    //se abrio mal
    cerr << "Error abriendo train.csv" << endl;
    return -1;
  } else {
    //se abrio bien
    cargarImagenesYLabels(imagenesTrain, labelsTrain, inputTrain);
    //cout << "fin de train" << endl;

  }

  vector < vector < double> > imagenesTest;
  vector < int > labelsTest;
  vector < string > dirImagenesTest;

  archivo = argv[6];
  ifstream inputTest(archivo);
 // cout << "abrir el archivo" << endl;
  if (!inputTest) {
    //se abrio mal
    cerr << "Error abriendo test.csv" << endl;
    return -1;
  } else {
    //se abrio bien
    dirImagenesTest = cargarImagenesYLabels(imagenesTest, labelsTest, inputTest);
    cout << "fin de test" << endl;

  }


  istringstream ss1(argv[10]);
  int k;
  if (!(ss1 >> k))
    cerr << "k inválido " << argv[10] << '\n';

  istringstream ss2(argv[12]);
  int alpha;
  if (!(ss2 >> alpha))
    cerr << "alpha inválido" << argv[12] << '\n';

  istringstream ss3(argv[14]);
  int numeroIteraciones;
  if (!(ss3 >> numeroIteraciones))
    cerr << "numeroIteraciones" << argv[14] << '\n';

  if(metodo==0){//knn
  int start = clock();
	archivo = argv[8];
	ofstream outputResult;
	outputResult.open(archivo);
	if(outputResult.is_open()){
		//se abrio bien
    double cantidadAciertos=0;
		for (int i = 0; i < imagenesTest.size(); i++) {
			int sujeto = knn(k, imagenesTest[i], imagenesTrain, labelsTrain);
			outputResult << dirImagenesTest[i] << " " << sujeto << "," << endl;

      //calculo accuracy:
      if(sujeto == labelsTest[i]){
        cantidadAciertos++;
      }
		}

		cout << "ya clasifique" << endl;
    cout << "accuracy: " << cantidadAciertos / imagenesTest.size() << " k: " << k << " alpha: " << alpha << " cantidad iteraciones: " << numeroIteraciones << endl;
     //imprimo tiempos para EXPERIMENTACION
    double difClocks=clock() - start;
    double difSegundos=(difClocks)/(CLOCKS_PER_SEC);
    double diffPorImg = difSegundos / imagenesTest.size();
    //cout << "clasifico " << imagenesTest.size() << " imagenes. Tardo : " << difSegundos << endl;
    //cout<<"tardo por imagen: " << diffPorImg << endl;
    ofstream tiempoTardado;
    tiempoTardado.open("./archivosGenerados/tiempoEjecucionDespuesDePca.txt");
    tiempoTardado << std::setprecision(9) << diffPorImg;
    tiempoTardado.close();
	} else {
		//se abrio mal
		cerr << "Error abriendo outputResult.csv" << endl;
		return -1;
	}
	outputResult.close();
  }else if(metodo==1){//PCA + knn
	cout << "antes de pca" << endl;
  //int start = clock();
	pca pca(imagenesTrain, numeroIteraciones, alpha);
	cout << "despues de pca" << endl;
  //time_t start = time(0);


	//ahi tengo los autovalores y autovectores de XtX
	vector < vector < double > > TcImgsTraining;
	for (int i = 0; i < imagenesTrain.size(); i++) {

		//quiero hacer Autovects * imagenesTrain[i] y eso meterlo en tci


		vector < double > tci(alpha, 0);
		for (int j = 0; j < alpha; j++) {
		  tci[j] = prodInterno(pca.pcaAutovectores()[j], imagenesTrain[i]);
		}


		TcImgsTraining.push_back(tci);
	}
	//ahora ya tengo las tcs de training.
	cout << "tci en img de Train" << endl;
  //inicio el reloj:
  int start = clock();
	vector < vector < double > > TcImgsTest;

	for (int i = 0; i < imagenesTest.size(); i++) {

		vector < double > tcit(alpha, 0);
		for (int j = 0; j < alpha; j++) {
		  tcit[j] = prodInterno((pca.pcaAutovectores())[j], imagenesTest[i]);
		}
		TcImgsTest.push_back(tcit);
	}

	//ahora tengo las tcs de tests
	cout << "tci en img de Test" << endl;

	//   //PARA EXPERIMENTACION IMPRIMO AUTOVALS Y AUTVECTS
	//  ofstream AutoValsExperimentacion;
	//  AutoValsExperimentacion.open("./archivosGenerados/autovaloresExp.txt");
  //
	//  if(AutoValsExperimentacion.is_open()){
	// 	 AutoValsExperimentacion << fixed;
	// 	 AutoValsExperimentacion << setprecision(15);
	// 		AutoValsExperimentacion << pca.pcaAutovalores().size() << endl;
	// 		for (int i = 0; i < pca.pcaAutovalores().size() ; i++){
	// 			AutoValsExperimentacion << pca.pcaAutovalores()[i] << " ";
	// 		}
	// }
	// AutoValsExperimentacion.close();
  //
	//   	 ofstream AutoVectsExperimentacion;
	//  AutoVectsExperimentacion.open("./archivosGenerados/autovectoresExp.txt");
  //
	//  if(AutoVectsExperimentacion.is_open()){
	// 	 AutoVectsExperimentacion << fixed;
	// 	 AutoVectsExperimentacion << setprecision(15);
	// 		AutoVectsExperimentacion << pca.pcaAutovectores().size() << " " << pca.pcaAutovectores()[0].size() << endl;
	// 		for (int i = 0; i < pca.pcaAutovectores().size() ; i++){
	// 			for (int j = 0; j < pca.pcaAutovectores()[0].size(); j++)
	// 			{
	// 				AutoVectsExperimentacion << pca.pcaAutovectores()[i][j] << " ";
	// 			}
	// 			AutoVectsExperimentacion << endl;
  //
	// 		}
	// }
	// AutoVectsExperimentacion.close();
	//   // FIN DE IMPRESION PARA EXPERIMENTACION
	archivo = argv[8];
	ofstream outputResult;
	outputResult.open(archivo);
	if(outputResult.is_open()){
		//se abrio bien
    double cantidadAciertos = 0;
		for (int i = 0; i < TcImgsTest.size(); i++) {
			int sujeto = knn(k, TcImgsTest[i], TcImgsTraining, labelsTrain);
			outputResult << dirImagenesTest[i] << " " << sujeto << "," << endl;
      if(sujeto == labelsTest[i]){
        cantidadAciertos++;
      }
		}
    //double segundostardados = difftime( time(0), start);
    //double segundostardados = time(0)- start;

		cout << "ya clasifique" << endl;
    cout << "accuracy: " << cantidadAciertos / imagenesTest.size() << " k: " << k << " alpha: " << alpha << " cantidad iteraciones: " << numeroIteraciones << endl;

    //imprimo tiempos para EXPERIMENTACION
    double difClocks=clock() - start;
    double difSegundos=(difClocks)/(CLOCKS_PER_SEC);
  //  cout<<"tardo " << difSegundos << endl;
    cout << "se clasificaron " << imagenesTest.size() << " imagenes"<<endl;
    double diffPorImg = difSegundos / imagenesTest.size();
    //cout << "se tardo por imagen: " << diffPorImg << endl;
    ofstream tiempoTardado;
    tiempoTardado.open("./archivosGenerados/tiempoEjecucionDespuesDePca.txt");
    tiempoTardado << std::setprecision(9) << diffPorImg;//difSegundos;
    tiempoTardado.close();
	} else {
		//se abrio mal
		cerr << "Error abriendo outputResult.csv" << endl;
			return -1;
	}
	outputResult.close();
  }else if(metodo==2){
	//test_MetodoPotencia(numeroIteraciones, alpha);
	test_Catedra(imagenesTrain, numeroIteraciones, alpha, argv[4]);
  }


  return 0;
}
