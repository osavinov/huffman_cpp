CC=g++
#CCOPTS=-g -D_FILE_OFFSET_BITS=64 -I. -I/usr/local/include
#LDOPTS=-L/usr/local/lib -L. -lmongoclient -lfuse -lboost_thread-mt -lboost_filesystem-mt -lboost_system-mt

huffman : main.o
	$(CC) -o huffman main.o

main.o : src/main.cpp
	$(CC) -c src/main.cpp

clean:
	rm -f main.o

test:
	python3 -m pytest tests
