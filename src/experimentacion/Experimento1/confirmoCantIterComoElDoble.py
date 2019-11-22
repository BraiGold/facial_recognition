import os
import subprocess
import numpy as np; np.random.seed(0)
from numpy import linalg as LA
import math
import matplotlib.pyplot as plt

def leerMatriz(NombreArchivo):

    archivo = open(nombreArchivo,'r')
    dimensiones = archivo.readline()[:-1].split(" ")
    n = int(dimensiones[0])
    m = int(dimensiones[1])
    Mat = np.zeros((n,m))
    i = 0
    for linea in archivo:
        fila = linea[:-2].split(" ")
        for j in range(0,len(fila)):
            Mat[i][j] = float(fila[j])
        i = i + 1

    return Mat

def leerAutovals(nombreArchivoAutovals):
    archivo = open(nombreArchivoAutovals,'r')
    cantidad = int(archivo.readline()[:-1])
    linea = archivo.readline()[:-2].split()
    autovals = np.zeros(cantidad)
    for i in range(0,cantidad):
        autovals[i] = float(linea[i])
    #print(autovals)
    return autovals

def calcularMu(M):
    return np.divide(np.sum(M,axis=0),len(M))


nombreArchivo = '../../archivosGenerados/Exp1_MatrizAutoval.txt'

nombreArchivoAutovals ='../../archivosGenerados/Exp1_autovalores.txt'

subprocess.call(["g++","../../inputOutput.cpp", "mainExpAutoval.cpp" , "../../pca.cpp","../../metodoPotencia.cpp","../../knn.cpp","../../operacionesMatriciales.cpp","-o","exp1_generar"])


dimension = 100
cantIter = 200
subprocess.call(["./exp1_generar", str(dimension), str(dimension) , str(cantIter),str(dimension)])
A = leerMatriz(nombreArchivo)
mu = calcularMu(A)
M = np.zeros((len(A),len(A[0])))
i=0
for f in A:
    M[i]= np.divide((f - mu),math.sqrt(len(A)-1))
    i = i + 1
MxMtrans = np.matmul(M,np.transpose(M))
autovals, autovects = LA.eig(MxMtrans)
autovals = np.sort(autovals)[::-1]
print(autovals)
print(autovals - leerAutovals(nombreArchivoAutovals))






plt.plot(leerAutovals(nombreArchivoAutovals), 'bo', label="Autovalores calculados")
plt.plot(autovals,'k',label="Autovalores reales")
plt.ylabel('autovalores')
plt.xlabel('numero de autovalor (en orden)')
plt.legend()
plt.show()

plt.plot(autovals- leerAutovals(nombreArchivoAutovals),'r-', label='Autovalores calculados - Autovalres reales ')
plt.ylabel('diferencia entre autovalores calculados y verdaderos')
plt.xlabel('numero de autovalor (en orden)')
plt.legend()
plt.show()


plt.plot(autovals,'r-',label="autovalores calculados")
plt.plot(leerAutovals(nombreArchivoAutovals), 'bo', label="Autovalores calculados")
plt.plot(autovals,'k',label="Autovalores reales")
plt.plot([ x*100  for x in(autovals- leerAutovals(nombreArchivoAutovals))],'r-',label="calculados - reales (escalado * e+02)")
plt.ylabel('autovalores y diferencia(escalada)')
plt.xlabel('numero de autovalor (en orden)')
plt.legend()
plt.show()
