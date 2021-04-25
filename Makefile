CC=g++
CCOPTS=-O3

huffman : main.o write_buffers.o read_buffers.o huffman_tree.o
	$(CC) -o huffman main.o write_buffers.o read_buffers.o huffman_tree.o

main.o : src/main.cpp
	$(CC) -c src/main.cpp $(CCOPTS)

write_buffers.o : src/write_buffers.cpp src/write_buffers.h
	$(CC) -c src/write_buffers.cpp $(CCOPTS)

read_buffers.o : src/read_buffers.cpp src/read_buffers.h
	$(CC) -c src/read_buffers.cpp $(CCOPTS)

huffman_tree.o : src/huffman_tree.cpp src/huffman_tree.h
	$(CC) -c src/huffman_tree.cpp $(CCOPTS)

clean:
	rm -f main.o write_buffers.o read_buffers.o huffman_tree.o

test : huffman
	python3 -m pytest -vv tests

profile : huffman
	python3 tests/measure.py
