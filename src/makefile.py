import os
import subprocess

subprocess.call(['g++', 'inputOutput.cpp', 'main.cpp', 'pca.cpp', 'metodoPotencia.cpp', 'knn.cpp', 'operacionesMatriciales.cpp', '-o', 'main'])

print('recuerde para ejecutar el main el orden de los parametros: ')
print('./main -m 0 -i rutaCSVtrain -q rutaCSVtest -o rutaCSVoutput -k k -a alpha -n cantIter')
print('ejemplo: ')
print('./main -m 1 -i ./CSVs/trainCSV0.csv -q ./CSVs/testsCSV0.csv -o Output.csv -k 5 -a 45 -n 300')
