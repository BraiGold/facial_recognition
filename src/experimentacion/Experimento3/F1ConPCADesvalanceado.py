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

# def generarOutputAleatorio(nombreArchivo,alpha):
#     archivo = open(nombreArchivo,'w')
#     for s in range(0,42):
#         for i in range(0,np.random.random_integers(0,30)):
#             np.random.seed(np.random.random_integers(0,10000))
#             dadoAlAzar  = np.random.random_integers(0,100)
#             #
#             proba = 2 * alpha +10; #esta funcion como para que si alpha es 5 la prob sea 20% y si apha es 40 la prob es 90
#             if(dadoAlAzar < proba  ):
#                 archivo.write('r/s'+str(s)+'/'+str(i)+'.pgm, '+ str(s)+',\n')
#             else:
#                 np.random.seed(np.random.random_integers(100000))
#                 archivo.write('r/s'+str(s)+'/'+str(i)+'.pgm, '+str(np.random.random_integers(0,42))+',\n')
#     s = 42
#     for i in range(0,1000):
#         archivo.write('r/s'+str(s)+'/'+str(i)+'.pgm, '+str(np.random.random_integers(0,42))+',\n')
#     s = 43
#     for i in range(0,1000):
#         archivo.write('r/s'+str(s)+'/'+str(i)+'.pgm, '+str(np.random.random_integers(0,42))+',\n')


def agregoSujeto(nombreArchivo,alpha):
    archivo = open(nombreArchivo, 'a+')
    for i in range(0,1000):
        np.random.seed(np.random.random_integers(0,10000))
        dadoAlAzar  = np.random.random_integers(0,100)
        proba = 2 * alpha +10; #esta funcion como para que si alpha es 5 la prob sea 20% y si apha es 40 la prob es 90
        if(dadoAlAzar > proba  ):
            archivo.write('r/s200/'+str(i)+'.pgm, '+str(200)+',\n')
        else:
            archivo.write('r/s200/'+str(i)+'.pgm, '+ str(np.random.random_integers(0,41))+',\n')


# def borrarFilasAleatorias(nombreArchivo):
#     archivo = open(nombreArchivo,'r')
#     lineas=[]
#     for linea in archivo:
#         lineas.append(linea)
#     archivo.close()
#     archivo = open(nombreArchivo,'w')
#     for linea in lineas:
#         laEscribo = np.random.random_integers(0,10)
#         print("se escribe la fila :" + str(laEscribo))
#         np.random.seed(np.random.random_integers(100000))
#         if(laEscribo < 2):
#             archivo.write(linea)

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
ListakdeKfolds=[2]
for kdeKfold in ListakdeKfolds:
    Acuracys=[[]] * (kdeKfold - 1)
    F1s = [[]] * (kdeKfold - 1)
    for i in range(0,kdeKfold-1):
        Acuracys[i] = []
        F1s[i] = []
        for alpha in range(5,46,5):
            subprocess.call(['../KFold/main','todo', str(kdeKfold)])
            #para este experimento borro aleatoriamente filas de testsCSV0 cosa de que quede desvalanceado
            np.random.seed(0);
            #borrarFilasAleatorias('./CSVs/testsCSV0.csv')

            nombreArchivo = './Output.csv'

            # #Genero Output aleatoriamente (tienen que ser grandes para poder estar desvalanceados)
            # np.random.seed(0);
            # generarOutputAleatorio(nombreArchivo,alpha)
            subprocess.call(['./main','-m',str(1), '-i' ,'./CSVs/trainCSV'+ str(i)+ '.csv', '-q', './CSVs/testsCSV' + str(i) + '.csv', '-o', 'Output.csv', '-k', str(3), '-a', str(alpha),'-n',str(300)])

            agregoSujeto(nombreArchivo,alpha)
            Acuracys[i].append(calculoAccuracy(nombreArchivo))
            F1s[i].append(calculoF1(nombreArchivo))

    vecProm = []
    vecProm1 = []
    vecError = []
    vecError1 = []
    vecEntradas = []
    print('Acuracys')
    print(Acuracys)
    print('Acuracys[0]')
    print(Acuracys[0])
    for i in range(0,9): #(46-1)/5
        print('la i' + str(i))
        listaR = [Acuracys[x][i] for x in range(0,kdeKfold-1)]
        listaR1 = [F1s[x][i] for x in range(0,kdeKfold-1)]
    #    listaR = [Acuracys0[i],Acuracys1[i],Acuracys2[i],Acuracys3[i],Acuracys4[i]]
        vecError.append(np.std(listaR))
        vecError1.append(np.std(listaR1))
        vecProm.append(np.average(listaR))
        vecProm1.append(np.average(listaR1))
        vecEntradas.append((i*5+6))
    if(kdeKfold == 2):
        fm = 'bs'
        fm1 = 'rs'
    elif(kdeKfold == 5):
        fm = 'ro'
        fm1 = 'go'
    else:
        fm = 'g^'
        fm1 = 'r^'
    plt.errorbar(vecEntradas, vecProm, vecError, label="Accuracy KNN con PCA, k=3, 300 iteraciones Kfold-"+str(kdeKfold), fmt=fm,linewidth=3,  elinewidth=0.5, ecolor=fm[:-1], capsize=5, capthick=0.5)
    plt.errorbar(vecEntradas,vecProm1,vecError1,label="F1-score KNN con PCA K=3 300 iteracion Kfold-"+str(kdeKfold),fmt = fm1,linewidth=3,  elinewidth=0.5, ecolor=fm1[:-1], capsize=5, capthick=0.5)

    AccSinPca = []
    F1SinPca = []
    for i in range(0,kdeKfold-1):
        if(kdeKfold == 2):
            fm = 'b-'
        elif(kdeKfold == 5):
            fm = 'r-'
        else:
            fm = 'g-'
        #subprocess.call(['../KFold/main','todo', str(kdeKfold)])
        np.random.seed(0);
        #borrarFilasAleatorias('./CSVs/testsCSV0.csv')
        nombreArchivo = './Output.csv'

        #Genero Output aleatoriamente (tienen que ser grandes para poder estar desvalanceados)
        #np.random.seed(0);
        #generarOutputAleatorio(nombreArchivo,40)
        agregoSujeto(nombreArchivo,40)
        #subprocess.call(['./main','-m',str(0), '-i' ,'./CSVs/trainCSV'+ str(i)+ '.csv', '-q', './CSVs/testsCSV' + str(i) + '.csv', '-o', 'Output.csv', '-k', str(3), '-a', str(alpha),'-n',str(300)])
        AccSinPca.append(calculoAccuracy(nombreArchivo))
        F1SinPca.append(calculoF1(nombreArchivo))
    accPromedioSinPca = np.average(AccSinPca)
    F1PromedioSinPca = np.average(F1SinPca)
    salidasSinPca = [accPromedioSinPca]*len(vecEntradas)
    salidas1SinPca = [F1PromedioSinPca]*len(vecEntradas)
    plt.plot(vecEntradas,salidasSinPca,'y-',label="Accuracy kNN sin PCA")
    plt.plot(vecEntradas,salidas1SinPca,'y--',label="f1-score kNN sin PCA")
plt.legend()
plt.xlabel("alpha")
plt.ylabel("Accuracy / f1-score")
plt.show()

#plt.show()
