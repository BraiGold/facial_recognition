import os
import numpy as np
import subprocess
import numpy as np; np.random.seed(0)
from numpy import linalg as LA
import math
from sklearn.decomposition import PCA
from scipy import stats
import sys

#devuelve un np array float con la imagen
def leerImagen(ruta):
    #Hago tramocha para usar siempre imagenes de reduced las grandes tienen basura en las primeras lineas
    if(ruta[:4]=='full'):
        ruta = 'reduced' + ruta[4:]


    nombreArchivo = './'+ ruta
    archivo = open(nombreArchivo,'rb')
    header = archivo.readline()
    alto, ancho, maxVal = [int(item) for item in header.split()[1:]]
    return np.fromfile(archivo, dtype=np.uint8).astype(float)

#devuelve np array de imagenes(np arrays float) y np array de sus identidades
def cargarImagenesDesdeCSV(nombreArchivo):
    print(nombreArchivo)
    archivo = open(nombreArchivo,'r')
    imagenes = []
    ids = []
    rutas = []
    for linea in archivo:
        print(linea.split('+'))
        if linea != '\n':
            ruta_id = linea[:-2].split(",")
            imagenes.append(leerImagen(ruta_id[0]))
            ids.append(ruta_id[1])
            rutas.append(ruta_id[0])
    return np.array(imagenes),np.array(ids),rutas

def kNN(imagenesTrain,identidadesTrain,k,imagenAEvaluar):
    distancias = np.zeros(len(imagenesTrain))
    for i in range(0,len(imagenesTrain)):
        distancias[i] = LA.norm(imagenesTrain[i]- imagenAEvaluar)

    ordenDistancias = np.argsort(distancias)
    identidadesAConsiderar = []
    for i in range(0,min(k,len(imagenesTrain))):
        identidadesAConsiderar.append(identidadesTrain[ordenDistancias[i]])
    moda,countbin = stats.mode(np.array(identidadesAConsiderar))
    return moda[0]

# devuelve autovals y autovects ordenados
def calcularMu(M):
    return np.divide(np.sum(M,axis=0),len(M))

def pca(A):
    mu = calcularMu(A)
    M = np.zeros((len(A),len(A[0])))
    i=0
    for f in A:
        M[i] = np.divide((f - mu),math.sqrt(len(A)-1))
        i = i + 1
    MtransxM = np.matmul(np.transpose(M),M)
    autovals, autovects = LA.eig(MtransxM)
    autovects = np.transpose(autovects)#funcionaba sin esto pero no tiene sentido
    ordenAutovals = np.argsort(autovals)
    autovectsOrdenados = np.copy(autovects)
    for j in ordenAutovals:
        autovectsOrdenados[j] = autovects[ordenAutovals[j]]
    autovalsOrdenados = np.sort(autovals)[::-1]
    autovectsOrdenados = autovectsOrdenados[::-1]
    return autovalsOrdenados , autovectsOrdenados

def tc(alphaAutovects,x):
    return np.matmul(alphaAutovects,np.transpose(x)) ##ese transpose es alcuete porque lo interpreta automaticamente como vector columna

#clasifica una imagen usando pca
#Ejemplo de imput: nombreTrainingCSV = './trainCSV0.csv' , k=3, alpha = 50    # full : len(autovects) osea 644
def clasificarConPca(k,alpha,nombreTrainingCSV,imagenAClasificar):
    imagenesTrain, identidadesTrain,rutasTrain = cargarImagenesDesdeCSV(nombreTrainingCSV)
    autovals,autovects = pca(imagenesTrain)
    alphaAutovects = autovects[:alpha]
    tcImagenesTrain = []
    for i in range(0 , len(imagenesTrain)):
        tcImagenesTrain.append(tc(alphaAutovects,imagenesTrain[i]))

    tcImagenesTrain = np.array(tcImagenesTrain)

    prediccion = kNN(tcImagenesTrain,identidadesTrain,k,tc(alphaAutovects,imagenAClasificar))
    return prediccion

def escribirOutput(nombreArchivoOutput,rutas,ids):
    archivo = open(nombreArchivoOutput,'w')
    for i in range(0,len(rutas)):
        archivo.write(rutas[i] + ', ' + ids[i] +',\n' )

def ejecutarAlgoritmo(algoritmo,k,alpha,nombreTrainingCSV,nombreTestingCSV,nombreArchivoOutput):
    imagenesTest, identidadesTest,rutasTest = cargarImagenesDesdeCSV(nombreTestingCSV)
    predicciones = []
    aciertos = 0
    for i in range(0,len(imagenesTest)):
        if(algoritmo == 1):
            prediccion = clasificarConPca(k,alpha,nombreTrainingCSV,imagenesTest[i])
        else:
            imagenesTrain, identidadesTrain,rutasTrain = cargarImagenesDesdeCSV(nombreTrainingCSV)
            prediccion = kNN(imagenesTrain,identidadesTrain,k,imagenesTest[i])
        predicciones.append(prediccion)
        if(prediccion == identidadesTest[i]):
            aciertos = aciertos + 1
    print('acuracy: ' +str(float(aciertos)/len(imagenesTest)))
    escribirOutput(nombreArchivoOutput, rutasTest, predicciones)

#main
algoritmo = int(sys.argv[6])#0
nombreTrainingCSV = sys.argv[1]#'./trainCSV0.csv'
nombreTestingCSV = sys.argv[2]#'./testsCSV0.csv'
k= int(sys.argv[4]) #3
alpha = int(sys.argv[5]) #12
nombreArchivoOutput = sys.argv[3] #'./OutputTpConPy.csv'
ejecutarAlgoritmo(algoritmo,k,alpha,nombreTrainingCSV,nombreTestingCSV,nombreArchivoOutput)
