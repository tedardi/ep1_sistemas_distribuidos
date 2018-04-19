import os
import sys

paramt = ""
for param in sys.argv :
    paramt += " " + param

n = 11

for iterador in range(1,12):
	bashCommand = paramt[13:] + " -o exemplo" + str(iterador) + " exemplo" + str(iterador) + ".c"
	print(bashCommand)
	os.system(bashCommand)