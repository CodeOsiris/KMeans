CC=g++
all:
	$(CC) KMeans.cpp -o KMeans -g -std=c++0x
clear:
	rm -f KMeans output.txt
