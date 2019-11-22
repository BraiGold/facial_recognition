import os
import matplotlib.pyplot as plt
import subprocess
import numpy as np; np.random.seed(0)
#from numpy import linalg as LA
#import math
#import seaborn as sns; sns.set()
#from sklearn.decomposition import PCA

##para este exp es necesario primero haber corrido "GenerarCVS.py"
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



for sizeTrain in range(1,9):
    Acuracys=[[]] * (sizeTrain)
    for i in range(0,sizeTrain):
        Acuracys[i] = []
        for k in range(1,31):
            print('i :' + str(i) + '| k: '+str(k))
            nombreArchivo = './Output.csv'
            subprocess.call(['./main','-m',str(0), '-i' ,'./CSVs/Entrenamiento'+ str(i)+ '.csv', '-q', './CSVs/Testeando' + str(i) + '.csv', '-o', 'Output.csv', '-k', str(k), '-a', str(50),'-n',str(300)])
            Acuracys[i].append(calculoAccuracy(nombreArchivo))

    vecProm = []
    vecError = []
    vecEntradas = []
    print('len de Acuracys')
    print(len(Acuracys))
    print('len de acruacy[0]')
    print(len(Acuracys[0]))
    for i in range(0,30):
        print('segundo i:'+str(i))
        listaR = [Acuracys[x][i] for x in range(0,len(Acuracys))]
    #    listaR = [Acuracys0[i],Acuracys1[i],Acuracys2[i],Acuracys3[i],Acuracys4[i]]
        vecError.append(np.std(listaR))
        vecProm.append(np.average(listaR))
        vecEntradas.append(i)

    plt.plot(vecEntradas, vecProm,'o' ,label="#(Train)=" + str(sizeTrain))

plt.xlabel("K")
plt.legend()
plt.ylabel("Accuracy")
plt.show()
