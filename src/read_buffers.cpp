#include <cstdio>
#include <iostream>

#include "read_buffers.h"
#include "utils.h"


ReadBuffer::ReadBuffer(char *fileName) {
    file = fopen(fileName, "rb");
    if (!file) {
        std::cout << "Failed to open file \"" << fileName << "\"." << std::endl;
        exit(1);
    }
    pos = 0;
    size = 0;
    fseek(file, 0, SEEK_END);
    FILE_SIZE = ftell(file);
    fseek(file, 0, SEEK_SET);
}

void ReadBuffer::readFile() {
    size = fread(buffer, sizeof(char), BUFFER_SIZE, file);
    pos = 0;
}

bool ReadBuffer::readChar(unsigned char *c) {
    if (pos == size && size == BUFFER_SIZE)
        readFile();
    if (pos == size && size != BUFFER_SIZE)
        return false;

    *c = buffer[pos];
    pos++;
    return true;
}

void ReadBuffer::closeFile() {
    fclose(file);
}

void ReadPackBuffer::readFile() {
    size = fread(buffer, sizeof(char), BUFFER_SIZE, file);
    pos = 0;
    if (size < BUFFER_SIZE)
        lastBuf = true;
    else if (size == BUFFER_SIZE)
        lastBuf = false;
}

ReadPackBuffer::ReadPackBuffer(char *fileName) {
    lastBuf = false;
    file = fopen(fileName, "rb");
    if (!file) {
        std::cout << "Failed to open file \"" << fileName << "\"." << std::endl;
        exit(1);
    }

    pos = 0;
    bitPos = 0;
    size = 0;
    for (unsigned char & i : buffer)
        i = 0;
    fseek(file, 0, SEEK_END);
    FILE_SIZE = ftell(file);
    fseek(file, 0, SEEK_SET);
}

bool ReadPackBuffer::readBit(bool *bit) {
    *bit = false;
    if (((size - pos) * 8 - bitPos) == 0 && lastBuf)
        return false;
    unsigned char mask = 1 << (7 - bitPos);
    if (buffer[pos] & mask)
        *bit = true;
    bitPos++;
    if (bitPos == 8) {
        bitPos = 0;
        pos++;
        //mask = 1 << 7;
        if (pos == size)
            readFile();
    }
    return true;
}

bool ReadPackBuffer::read(unsigned char *n, int blockSize) {
    *n = 0;
    if ((((size - pos) * 8 - bitPos) < blockSize) && lastBuf)
        return false;
    else {
        unsigned char mask = 1 << (7 - bitPos);
        for (int i = 0; i < blockSize; i++) {
            if (buffer[pos] & mask)
                *n = set_bit(*n, blockSize - i - 1);

            mask = mask >> 1;
            bitPos++;
            if (bitPos == 8) {
                bitPos = 0;
                pos++;
                mask = 1 << 7;
                if (pos == size)
                    readFile();
            }
        }
        return true;
    }
}

void ReadPackBuffer::closeFile() {
    fclose(file);
}
