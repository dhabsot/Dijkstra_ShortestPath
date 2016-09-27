#Compilazione del file
Dijkstra_ShortestPath.o:
	gcc -ansi -Wall -O Dijkstra_ShortestPath.c -o Dijkstra_ShortestPath.o

#Pulizia
pulisci:
	rm -f Dijkstra_ShortestPath.o