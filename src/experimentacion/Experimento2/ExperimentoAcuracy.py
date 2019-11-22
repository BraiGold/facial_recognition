import os
import matplotlib.pyplot as plt
import numpy as np
import subprocess
import numpy as np; np.random.seed(0)
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


ListakdeKfolds=[2,5,10]
for kdeKfold in ListakdeKfolds:
    Acuracys=[[]] * (kdeKfold - 1)
    for i in range(0,kdeKfold-1):
        Acuracys[i] = []
        for k in range(1,31):
            subprocess.call(['../KFold/main','todo', str(kdeKfold)])
            nombreArchivo = './Output.csv'
            subprocess.call(['./main','-m',str(0), '-i' ,'./CSVs/trainCSV'+ str(i)+ '.csv', '-q', './CSVs/testsCSV' + str(i) + '.csv', '-o', 'Output.csv', '-k', str(k), '-a', str(50),'-n',str(300)])
            Acuracys[i].append(calculoAccuracy(nombreArchivo))


    vecProm = []
    vecError = []
    vecEntradas = []

    for i in range(0,30):
        listaR = [Acuracys[x][i] for x in range(0,kdeKfold-1)]
    #    listaR = [Acuracys0[i],Acuracys1[i],Acuracys2[i],Acuracys3[i],Acuracys4[i]]
        vecError.append(np.std(listaR))
        vecProm.append(np.average(listaR))
        vecEntradas.append(i)
    if(kdeKfold == 2):
        fm = 'ro'
    elif(kdeKfold == 5):
        fm = 'bs'
    else:
        fm = 'g^'
    plt.errorbar(vecEntradas, vecProm, vecError, label="KNN sin PCA Kfold-"+str(kdeKfold), fmt=fm,linewidth=3,  elinewidth=0.5, ecolor=fm[:-1], capsize=5, capthick=0.5)

plt.legend()
plt.xlabel("K")
plt.ylabel("Accuracy")
plt.show()
