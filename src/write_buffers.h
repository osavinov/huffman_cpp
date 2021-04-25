#include <iostream>
#include "utils.h"

#ifndef HUFFMAN_CPP_WRITE_BUFFERS_H
#define HUFFMAN_CPP_WRITE_BUFFERS_H

#endif //HUFFMAN_CPP_WRITE_BUFFERS_H

class WritePackBuffer {
private:
    unsigned char buffer[BUFFER_SIZE];
    int pos;
    int bitPos;
    FILE *file;
public:
    explicit WritePackBuffer(char *fileName);
    void writeFile();
    void write(bool *n, int blockSize);
    void writeChar(unsigned char c);
    void closeFile();
};

class WriteBuffer {
private:
    unsigned char buffer[BUFFER_SIZE];
    unsigned int pos;
    FILE *file;
public:
    explicit WriteBuffer(char *fileName);
    void writeFile();
    void writeChar(unsigned char c);
    void closeFile();
};