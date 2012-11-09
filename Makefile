
wolf3d: main.o id_vh.o
	gcc main.o id_vh.o -o wolf3d 

main.o: main.c
	gcc -c main.c -Wall

id_vh.o: id_vh.c
	gcc -c id_vh.c -Wall

clean:
	rm id_vh.o main.o wolf3d
