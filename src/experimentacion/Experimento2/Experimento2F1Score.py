import os
import matplotlib.pyplot as plt
import numpy as np
import subprocess
import numpy as np; np.random.seed(0)
from sklearn.metrics import f1_score
#from numpy import linalg as LA
#import math
#import seaborn as sns; sns.set()
#from sklearn.decomposition import PCA
def calculoF1(archivoEntrada):
    arch = open(archivoEntrada,'r')
    acum = float(0)
    cantImagenes = 0
    yTrue = []
    yPred = []
    for line in arch:
        cantImagenes+=1
        linea = line.split(',')
        linea_Parte_Uno = linea[0]
        linea_Parte_Dos = linea[1]
        lineaTemp = linea[0].split('/')
        yTrue.append(int(lineaTemp[1][1:]))
        yPred.append(int(linea_Parte_Dos))
    return f1_score(yTrue, yPred, average='macro')

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


ListakdeKfolds=[5]
for kdeKfold in ListakdeKfolds:
    Acuracys=[[]] * (kdeKfold - 1)
    F1s =[[]] * (kdeKfold - 1)
    for i in range(0,kdeKfold-1):
        Acuracys[i] = []
        F1s[i] = []
        for k in range(1,31):
            subprocess.call(['../KFold/main','todo', str(kdeKfold)])
            nombreArchivo = './Output.csv'
            subprocess.call(['./main','-m',str(0), '-i' ,'./CSVs/trainCSV'+ str(i)+ '.csv', '-q', './CSVs/testsCSV' + str(i) + '.csv', '-o', 'Output.csv', '-k', str(k), '-a', str(50),'-n',str(300)])
            #subprocess.call(['python3','TpEnPythonConPca.py','./CSVs/trainCSV'+ str(i)+ '.csv', './CSVs/testsCSV' + str(i) + '.csv','Output.csv',str(k),str(50),str(0)])
            Acuracys[i].append(calculoAccuracy(nombreArchivo))
            F1s[i].append(calculoF1(nombreArchivo))

    vecProm = []
    vecProm1 = []
    vecError = []
    vecError1 = []
    vecEntradas = []

    for i in range(0,30):
        listaR = [Acuracys[x][i] for x in range(0,kdeKfold-1)]
        listaR1 = [F1s[x][i] for x in range(0,kdeKfold-1)]
    #    listaR = [Acuracys0[i],Acuracys1[i],Acuracys2[i],Acuracys3[i],Acuracys4[i]]
        vecError.append(np.std(listaR))
        vecError1.append(np.std(listaR1))
        vecProm.append(np.average(listaR))
        vecProm1.append(np.average(listaR1))
        vecEntradas.append(i)
    if(kdeKfold == 2):
        fm = 'bo'
        fm1 = 'b^'
    elif(kdeKfold == 5):
        fm = 'ro'
        fm1='r^'
    else:
        fm = 'go'
        fm1 ='g^'
    plt.errorbar(vecEntradas, vecProm, vecError, label="Accuracy KNN sin PCA Kfold-"+str(kdeKfold), fmt=fm,linewidth=3,  elinewidth=0.5, ecolor='k', capsize=5, capthick=0.5)
    plt.errorbar(vecEntradas, vecProm1, vecError1, label="f1-score KNN sin PCA Kfold-"+str(kdeKfold), fmt='b^',linewidth=3,  elinewidth=0.5, ecolor='k', capsize=5, capthick=0.5)
plt.legend()
plt.xlabel("K")
plt.ylabel("Accuracy / f1-score")
plt.show()
