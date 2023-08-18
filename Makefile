

INC:=-I$(HOME)/local/include
INC+=-IdSFMT-src-2.2.3
LIB:=-L$(HOME)/local/lib
OPTS:=-O3 -DDSFMT_MEXP=19937

all:
	mkdir -p data
	g++ -std=c++14 -c $(OPTS) dumbbell.cpp -o dumbbell.o
	g++ -std=c++14 -c $(OPTS) rand.cpp -o rand.o
	gcc -c $(OPTS) gsd.c -o gsd.o
	g++ -std=c++14 -c $(OPTS) timer.cpp -o timer.o
	g++ -std=c++14 -c $(INC) $(OPTS) dSFMT.cpp -o dSFMT.o
	g++ -std=c++14 -c $(INC) $(OPTS) main_dumbbell.cpp -o main_dumbbell.o
	g++ -std=c++14 $(INC) $(LIB) $(OPTS) gsd.o dSFMT.o rand.o timer.o dumbbell.o main_dumbbell.o -lyaml-cpp -o a.out

clean:
	rm -rf *.o a.out data



