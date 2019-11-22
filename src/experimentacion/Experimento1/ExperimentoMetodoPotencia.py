import os
import matplotlib.pyplot as plt
import numpy as np
import subprocess
import numpy as np; np.random.seed(0)
from numpy import linalg as LA
import math
import seaborn as sns; sns.set()
from sklearn.decomposition import PCA



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
rangoDimension = 50; #corre hasta 1 mas
rangoCantIteraciones =100;
error = np.zeros((rangoCantIteraciones,rangoDimension))
for cantIter in range(10,rangoCantIteraciones + 10):
    for dimension in range(11,rangoDimension + 11):
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
        error[cantIter-10][dimension-11] =LA.norm(autovals - leerAutovals(nombreArchivoAutovals))

print (error)


cantidadesDimensiones = list(range(2,rangoDimension + 11))
#cantidadesIteraciones = list(range(10,rangoCantIteraciones+10))
cantidadesIteraciones = np.arange(10,50,5).tolist()
ax = sns.heatmap(error, xticklabels=5,yticklabels=10)
ax.invert_yaxis()
plt.show()
#for lamd in autovals:
#    autovalsDeM.append(math.sqrt(lamd))
#print(autovalsDeM)


#print("Pca: \n")
#cantIter=300;
#alpha = 3;
#pca = PCA(copy=True, iterated_power=cantIter, n_components=alpha, random_state=None, svd_solver='randomized', tol=0.0, whiten=False)
#pca.fit(M)
#print(pca.singular_values_)
