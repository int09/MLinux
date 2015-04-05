cacu:add_int.o add_flaot.o sub_int.o sub_float.o main.o
	gcc -o cacu add/add_int.o add/add_float.o sub/sub_int.o sub/sub_float.o

add_int.o:add/add_int.c add/add.h
	gcc -o add/add_int.o add/add_int.c
add_float.o:add/add_float.c add/add.h
	gcc -o add/add_float.o add/add_float.c
sub_int.o:sub/sub_int.c sub/sub.h
	gcc -o sub/sub_int.o sub/sub_int.c
sub_float.o:sub/sub_float.c sub/sub.h
	gcc -o sub/sub_float.o sub/sub_float.c
main.o:main.c add/add.h sub/sub.h
	gcc -o main.o main.c -Iadd -Isub

clean:
	rm -f cacu add/add_int.o add/add_float.o sub/sub_int.o sub/sub_float.o main.o

