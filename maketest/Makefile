#Makefile v01

all : ../bin/make_test

../bin/make_test : ./main.o \
					./sub1/dummy1.o \
					./sub1/dummy2.o \
					./sub2/dummy3.o \
					./sub2/dummy4.o
	g++ -o ../bin/make_test ./main.o \
					./sub1/dummy1.o \
					./sub1/dummy2.o \
					./sub2/dummy3.o \
					./sub2/dummy4.o -I ../inc
./main.o : ../src/main.cpp
	g++ -o ./main.o -c ../src/main.cpp -I ../inc
./sub1/dummy1.o : ../src/sub1/dummy1.cpp
	g++ -o ./sub1/dummy1.o -c ../src/sub1/dummy1.cpp -I ../inc
./sub1/dummy2.o : ../src/sub1/dummy1.cpp
	g++ -o ./sub1/dummy2.o -c ../src/sub1/dummy2.cpp -I ../inc
./sub2/dummy3.o : ../src/sub1/dummy1.cpp
	g++ -o ./sub1/dummy3.o -c ../src/sub1/dummy3.cpp -I ../inc
./sub2/dummy4.o : ../src/sub1/dummy1.cpp
	g++ -o ./sub1/dummy4.o -c ../src/sub1/dummy4.cpp -I ../inc

clean :
	rm -rf ../bin/make_test main.o sub1/* sub2/*
