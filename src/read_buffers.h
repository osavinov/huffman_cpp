#include "utils.h"

#ifndef HUFFMAN_CPP_READ_BUFFERS_H
#define HUFFMAN_CPP_READ_BUFFERS_H

#endif //HUFFMAN_CPP_READ_BUFFERS_H


class ReadBuffer {
private:
    unsigned char buffer[BUFFER_SIZE];
    unsigned int pos;
    unsigned int size;
    FILE *file;
public:
    unsigned int FILE_SIZE;
    explicit ReadBuffer(char *fileName);
    void readFile();
    bool readChar(unsigned char *c);
    void closeFile();
};

class ReadPackBuffer {
private:
    unsigned char buffer[BUFFER_SIZE];
    unsigned int pos;
    unsigned int bit_pos;
    unsigned int size;
    FILE *file;
public:
    unsigned int FILE_SIZE;
    bool lastBuf;
    void readFile();
    explicit ReadPackBuffer(char *fileName);
    bool readBit(bool *bit);
    bool read(unsigned char *n, int blockSize);
    void closeFile();
};
