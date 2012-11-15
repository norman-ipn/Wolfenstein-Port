
wolf3d: main.o id_vh.o id_vl.o id_mm.o
	gcc main.o id_vh.o id_vl.o id_mm.o -o wolf3d 

main.o: main.c
	gcc -c main.c -Wall

id_vh.o: id_vh.c
	gcc -c id_vh.c -Wall

id_vl.o: id_vl.c
	gcc -c id_vl.c -Wall

id_mm.o: id_mm.c
	gcc -c id_mm.c -Wall

clean:
	rm id_vh.o main.o wolf3d
