import numpy as np; np.random.seed(0)
from numpy import linalg as LA
import math

def autovalMetodoPotencia(A, v):
    Av = A.dot(v)
    return v.dot(Av)

def MetodoPotencia(A,cantIter):
    n, d = A.shape

    v = np.ones(d) #/ np.sqrt(d) pruebo esto para debuguear
    ev = autovalMetodoPotencia(A, v)

    for i in range(0,cantIter):
        Av = A.dot(v)
        v_new = Av / LA.norm(Av)
        ev_new = autovalMetodoPotencia(A, v_new)
        v = v_new
        ev = ev_new
        #print('iteracion: '+ str(i) )

    return ev_new, v_new

def multiplesMetodoPotencia(A,alpha,cantIter):
    autovals = []
    autovects = []
    for i in range(0,alpha):
        lamb,v = MetodoPotencia(A,cantIter)
        autovals.append(lamb)
        autovects.append(v)
        A = A - np.matmul(v.reshape(len(v),1),v.reshape(1,len(v)))*lamb

        #print('calcule el '+ str(i) +' autoval y autovect')
    return np.array(autovals),np.array(autovects)
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
    archivo = open(nombreArchivo,'r')
    imagenes = []
    ids = []
    rutas = []
    for linea in archivo:
        ruta_id = linea[:-2].split(",")
        imagenes.append(leerImagen(ruta_id[0]))
        ids.append(ruta_id[1])
        rutas.append(ruta_id[0])
    return np.array(imagenes),np.array(ids),rutas


def leerAutovals(nombreArchivoAutovals):
    archivo = open(nombreArchivoAutovals,'r')
    cantidad = int(archivo.readline()[:-1])
    linea = archivo.readline()[:-2].split()
    autovals = np.zeros(cantidad)
    for i in range(0,cantidad):
        autovals[i] = float(linea[i])
    #print(autovals)
    return autovals

def leerAutovects(nombreArchivo):
    archivo = open(nombreArchivo,'r')
    cantidad_size = archivo.readline()[:-1].split()
    cantidad = int(cantidad_size[0])
    size = int(cantidad_size[1])

    autovects = []
    for linea in archivo:
        autovector = np.array([ float(x) for x in linea[:-2].split() ])
        autovects.append(autovector)

    return np.array(autovects)

# devuelve autovals y autovects ordenados
def calcularMu(M):
    return np.divide(np.sum(M,axis=0),len(M))

def generarMatrizDesdeImagen(A):
    mu = calcularMu(A)
    M = np.zeros((len(A),len(A[0])))
    i=0
    for f in A:
        M[i] = np.divide((f - mu),math.sqrt(len(A)-1))
        i = i + 1
    return np.matmul(np.transpose(M),M)

def distanciaAl0(v):
    return LA.norm(np.zeros(len(v))-v)



print("----------empieza la posta -------------------------")
nombreTrainingCSV = './trainCSV0.csv'
imagenesTrain, identidadesTrain,rutasTrain = cargarImagenesDesdeCSV(nombreTrainingCSV)
A = generarMatrizDesdeImagen(imagenesTrain)
autovectsCalculados=leerAutovects('./autovectoresExp.txt')
autovalsCalculados = leerAutovals('./autovaloresExp.txt')

autovals, autovects = LA.eig(A)
autovects = np.transpose(autovects)
ordenAutovals = np.argsort(autovals)
autovectsOrdenados = np.copy(autovects)
for j in ordenAutovals:
    autovectsOrdenados[j] = autovects[ordenAutovals[j]]
autovalsOrdenados = np.sort(autovals)[::-1]
autovectsOrdenados = autovectsOrdenados[::-1]


erroresEsperados = np.zeros(len(autovalsCalculados))
print('esperados : distancias al 0 de (A -lamba.I).V')
for i in range(0,len(autovalsCalculados)):
    print(distanciaAl0(np.matmul(A-autovalsOrdenados[i]*np.identity(len(A)),autovectsOrdenados[i])))


autovalsPy,autovectsPy = multiplesMetodoPotencia(A,alpha = 3, cantIter=1000)
print('Calculados py : distancias al 0 de (A -lamba.I).V')
for i in range(0,len(autovalsCalculados)):
    print(distanciaAl0(np.matmul(A-autovalsPy[i]*np.identity(len(A)),autovectsPy[i])))



erroresCalculados = np.zeros(len(autovalsCalculados))


print('Calculados c++ : distancias al 0 de (A -lamba.I).V')
for i in range(0,len(autovalsCalculados)):
    print(distanciaAl0(np.matmul(A-autovalsCalculados[i]*np.identity(len(A)),autovectsCalculados[i])))

print('autovalores esperados')
print(autovalsOrdenados[:3])
print('autovals Calculados c++')
print(autovalsCalculados)
print('autovals Calculados py')
print(autovalsPy)
print('diferencia entre autovals esperados y calculados')
print(autovalsOrdenados[:3] - autovalsCalculados)
# print('norma esperados:')
# print (erroresEsperados)
# print('norma calculados:')
# print (erroresCalculados)
print(autovectsCalculados)
