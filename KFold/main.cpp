#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>

using namespace std;

int nthSubstr(int n, const string& s, const string& p) {
   string::size_type i = s.find(p);     // Find the first occurrence
  
   int j;
   for (j = 1; j < n && i != string::npos; ++j)
      i = s.find(p, i+1); // Find the next occurrence

   if (j == n)
     return(i);
   else
     return(-1);
}

int main(int argc, char** argv) {

  
  string nombreArchivo = argv[1];
  //cout << "Escriba el nombre del archivo: " << endl;
  //cin >> nombreArchivo;

  ifstream inputfile;
  inputfile.open(("../data/" + nombreArchivo + ".in").c_str(), ifstream::in);

  string line;
  string numeroCara;


  if (inputfile.is_open()) {
    while (getline(inputfile, line)) {
      int punto = line.find(".");
      // cout << punto << endl;
      numeroCara = line.substr(nthSubstr(2, line, "/"), punto);
      // cout << numeroCara << endl;
    }
  }
  inputfile.close();

  string num = numeroCara.substr(numeroCara.find("/")+1, numeroCara.find(".")-1);

  vector<string> bucketNumOfFaces(atoi(num.c_str()));
  vector< vector<string> > buckets(41, bucketNumOfFaces);

  ifstream inputfile2;
  inputfile2.open(("../data/" + nombreArchivo + ".in").c_str(), ifstream::in);
  if (inputfile2.is_open()) {
    int indiceBuckets = 0;
    int indiceBOF = 0;
    while (getline(inputfile2, line)) {
      // cout << line << endl;
      bucketNumOfFaces[indiceBOF] = line;
      indiceBOF++;
      if (indiceBOF == atoi(num.c_str())) {
        buckets[indiceBuckets] = bucketNumOfFaces;
        indiceBOF = 0;
        indiceBuckets++;
      }
    }
  }
  inputfile2.close();
/*
  for (int i = 0; i < 41; i++) {
    for (int j = 0; j <10 ; j++) {
      cout << buckets[i][j] << endl;
    }
  }*/

  ofstream traincsv;
  ofstream testcsv;
  ofstream outputcsv;



  int k = atoi(argv[2]);
  //cout << "Escriba el k correspondienta al K-Fold (un k que divida a la cantidad de imagenes por persona)" << endl;
  //cin >> k;

  int partTest = atoi(num.c_str()) / k;

  // cout << partTest << endl;

  for (int j = 0; j < k; j++) {
//    outputcsv.open("../src/outputCSV" + to_string(j) + ".csv");
    traincsv.open("../src/CSVs/trainCSV" + to_string(j) + ".csv");
    testcsv.open("../src/CSVs/testsCSV" + to_string(j) + ".csv");
    // cout << "j: " << j << endl;
    vector < vector < string > > pepe = buckets;
    for (int i = 0; i < 41; i++) {
      // cout << "i: " << i << endl;
      for (int p = 0; p < partTest; p++) {
        // cout << "p: " << p << endl;
        testcsv << buckets[i][j*partTest + p] << endl;
        // cout << "escribi en test: " << endl;
      }
      buckets[i].erase(buckets[i].begin() + (j*partTest), buckets[i].begin() + (j*partTest) + partTest);
      for (int m = 0; m < (atoi(num.c_str()) - partTest); m++) {
        // cout << "m: " << m << endl;
        // cout << "tamaño bucket: " << buckets[i].size() << endl;
        traincsv << buckets[i][m] << endl;;
      }

      buckets = pepe;
    }

    //outputcsv.close();
    testcsv.close();
    traincsv.close();
  }
  
  return 0;
}
