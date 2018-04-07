import os

n = 11

for iterador in range(1,12):
	bashCommand = "gcc -o exemplo" + str(iterador) + " exemplo" + str(iterador) + ".c"
	os.system(bashCommand)