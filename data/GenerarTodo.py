file1 = open('todo.in',"w")
for i in range(0,40):
    for j in range(0,10):
        escribo = 'reduced/s' + str(i+1) + '/' + str(j+1) + '.pgm, ' + str(i+1) + ',\n'
        file1.write(escribo)
