

#   full/s1/1.pgm, 1,
nombreArchivo = '../../CSVs/Entrenamiento'
nombreArchivo2 = '../../CSVs/Testeando'

for k in range(0,9):
    archivo = nombreArchivo + str(k) + '.csv'
    archivo2 = nombreArchivo2 + str(k) + '.csv'
    file2 = open(archivo2,"w")
    file1 = open(archivo,"w")
    for i in range(0,9):
        for j in range(0,k+1):
            escribo = 'reduced/s' + str(i+1) + '/' + str(j+1) + '.pgm, ' + str(i+1) + ',\n'
            file1.write(escribo)

        for j in range(k+1,9):
            escribo = 'reduced/s' + str(i+1) + '/' + str(j+1) + '.pgm, ' + str(i+1) + ',\n'
            file2.write(escribo)


    file1.close()
    file2.close()
