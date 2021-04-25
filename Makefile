CC=g++
CCOPTS=-O3

huffman : main.o
	$(CC) -o huffman main.o

main.o : src/main.cpp
	$(CC) -c src/main.cpp $(CCOPTS)

clean:
	rm -f main.o

test:
	python3 -m pytest -vv tests
