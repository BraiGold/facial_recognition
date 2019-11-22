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

#estoy usando el codigo del experimento 3 pero cambiando Acuracys por tiempo de ejecucion
def calculoAccuracy(archivoEntrada):
    # arch = open(archivoEntrada,'r')
    # acum = float(0)
    # cantImagenes = 0
    # for line in arch:
    # 	cantImagenes+=1
    # 	linea = line.split(',')
    # 	linea_Parte_Uno = linea[0]
    # 	linea_Parte_Dos = linea[1]
    # 	lineaTemp = linea[0].split('/')
    # 	if(int(lineaTemp[1][1:]) == int(linea_Parte_Dos)):
    # 		#esta es la imagen correcta!
    # 		acum+=1
    archivoTiempo = open('./archivosGenerados/tiempoEjecucionDespuesDePca.txt','r')
    tiempoFi = float(archivoTiempo.readline())
    return tiempoFi


ListakdeKfolds=[5]
for kdeKfold in ListakdeKfolds:
    Acuracys=[[]] * (kdeKfold - 1)
    for i in range(0,kdeKfold-1):
        Acuracys[i] = []
        for alpha in range(5,46,5):
            subprocess.call(['../KFold/main','todo', str(kdeKfold)])
            nombreArchivo = './Output.csv'
            tiempoInicial = time.time()
            subprocess.call(['./main','-m',str(1), '-i' ,'./CSVs/trainCSV'+ str(i)+ '.csv', '-q', './CSVs/testsCSV' + str(i) + '.csv', '-o', 'Output.csv', '-k', str(3), '-a', str(alpha),'-n',str(300)])
            tiempo = time.time() - tiempoInicial
            Acuracys[i].append(tiempo)


    vecProm = []
    vecError = []
    vecEntradas = []
    print('Acuracys')
    print(Acuracys)
    print('Acuracys[0]')
    print(Acuracys[0])
    for i in range(0,9): #(46-1)/5
        print('la i' + str(i))
        listaR = [Acuracys[x][i] for x in range(0,kdeKfold-1)]
    #    listaR = [Acuracys0[i],Acuracys1[i],Acuracys2[i],Acuracys3[i],Acuracys4[i]]
        vecError.append(np.std(listaR))
        vecProm.append(np.average(listaR))
        vecEntradas.append((i*5+6))
    if(kdeKfold == 2):
        fm = 'bs'
    elif(kdeKfold == 5):
        fm = 'ro'
    else:
        fm = 'g^'
    plt.errorbar(vecEntradas, vecProm, vecError, label="KNN con PCA, k=3, 300 iteraciones Kfold-"+str(kdeKfold), fmt=fm,linewidth=3,  elinewidth=0.5, ecolor='k', capsize=5, capthick=0.5)


    AccSinPca = []
    for i in range(0,kdeKfold-1):
        if(kdeKfold == 2):
            fm = 'b-'
        elif(kdeKfold == 5):
            fm = 'r-'
        else:
            fm = 'g-'
        subprocess.call(['../KFold/main','todo', str(kdeKfold)])
        nombreArchivo = './Output.csv'
        tiempoInicial = time.time()
        subprocess.call(['./main','-m',str(0), '-i' ,'./CSVs/trainCSV'+ str(i)+ '.csv', '-q', './CSVs/testsCSV' + str(i) + '.csv', '-o', 'Output.csv', '-k', str(3), '-a', str(alpha),'-n',str(300)])
        AccSinPca.append(time.time()-tiempoInicial)

    accPromedioSinPca = np.average(AccSinPca)
    salidasSinPca = [accPromedioSinPca]*len(vecEntradas)
    plt.plot(vecEntradas,salidasSinPca,'b--',label="kNN sin PCA K-fold"+str(kdeKfold))
plt.legend()
plt.xlabel("alpha")
plt.ylabel("Tiempo (seg)")
plt.show()

plt.show()
