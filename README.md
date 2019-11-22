Para compilar el tp, desde la carpeta src:

python3 makefile.py

para ejecutar el codigo:

./main -m 0 -i rutaCSVtrain -q rutaCSVtest -o rutaCSVoutput -k k -a alpha -n cantIter

ejemplo:

./main -m 1 -i ./CSVs/trainCSV0.csv -q ./CSVs/testsCSV0.csv -o Output.csv -k 5 -a 45 -n 300


Para los experimentos 2,3 y 4 es necesario copiar los archivos .py a /src/ para que funcionen correctamente.

el experimento 2 y 4 requieren ejecutar el archivo GenerarCSVs.py (que genera sets de Train y Test ) para poder ser corridos.
