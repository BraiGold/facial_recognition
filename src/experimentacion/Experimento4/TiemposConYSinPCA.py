import os
import matplotlib.pyplot as plt
import numpy as np
import subprocess
import numpy as np; np.random.seed(0)
import time
#from numpy import linalg as LA
#import math
#import seaborn as sns; sns.set()
#from sklearn.decomposition import PCA


def calculoAccuracy(archivoEntrada):
    arch = open(archivoEntrada,'r')
    acum = float(0)
    cantImagenes = 0
    for line in arch:
    	cantImagenes+=1
    	linea = line.split(',')
    	linea_Parte_Uno = linea[0]
    	linea_Parte_Dos = linea[1]
    	lineaTemp = linea[0].split('/')
    	if(int(lineaTemp[1][1:]) == int(linea_Parte_Dos)):
    		#esta es la imagen correcta!
    		acum+=1
    return (float(acum)/cantImagenes)
def tomoTiempos(csvEntrenamiento,csvTesteo):
    tiempoSinPca = 0
    tiemposConPCA = []

    tiempoInicial = time.time()
    nombreArchivo = './Output.csv'
    subprocess.call(["./main",'-m',str(0), '-i' ,csvEntrenamiento, '-q', csvTesteo, '-o', 'Output.csv', '-k', str(3), '-a', str(160),'-n',str(300)])
    tiempoFinal = time.time() - tiempoInicial


    for alph in range(1,52,10):
        tiempoIn = time.time()
        nombreArchivo = './Output.csv'
        subprocess.call(["./main",'-m',str(1), '-i' ,csvEntrenamiento, '-q', csvTesteo, '-o', 'Output.csv', '-k', str(3), '-a', str(alph),'-n',str(20)])
        tiempoFi = time.time() - tiempoIn
        tiemposConPCA.append(tiempoFi)

    return tiempoFinal,tiemposConPCA



tiemposSinPCAGeneral = []
tiemposAlpha1 = []
tiemposAlpha2 = []
tiemposAlpha3 = []
tiemposAlpha4 = []
tiemposAlpha5 = []
for k in range(0,9):
    archivo = './CSVs/Entrenamiento' + str(k) + '.csv'
    archivo2 = './CSVs/Testeando' + str(k) + '.csv'

    tiempoSinPCA,tiemposConPCA = tomoTiempos(archivo,archivo2)
    tiemposSinPCAGeneral.append(tiempoSinPCA)
    tiemposAlpha1.append(tiemposConPCA[0])
    tiemposAlpha2.append(tiemposConPCA[1])
    tiemposAlpha3.append(tiemposConPCA[2])
    tiemposAlpha4.append(tiemposConPCA[3])
    tiemposAlpha5.append(tiemposConPCA[4])

plt.plot(tiemposSinPCAGeneral,label = 'Sin PCA')
plt.plot(tiemposAlpha1,label = 'PCA alpha: 1')
plt.plot(tiemposAlpha2, label = 'PCA alpha:11')
plt.plot(tiemposAlpha3, label = 'PCA alpha:21')
plt.plot(tiemposAlpha4, label = 'PCA alpha:31')
plt.plot(tiemposAlpha5,label = 'PCA alpha:41')
plt.xlabel("Tamaño training")
plt.ylabel("Tiempo en segundos.")
plt.legend()
plt.show()








#a
