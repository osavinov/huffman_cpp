#include "write_buffers.h"
#include "utils.h"


WritePackBuffer::WritePackBuffer(char *fileName) {
    file = fopen(fileName, "wb");
    if (!file) {
        std::cout << "Failed to open file \"" << fileName << "\"." << std::endl;
        exit(1);
    }
    pos = 0;
    bitPos = 0;
    for (unsigned char & i : buffer)
        i = 0;
}

void WritePackBuffer::writeFile() {
    fwrite(buffer, sizeof(char), bitPos ? pos + 1 : pos, file);

    pos = 0;
    bitPos = 0;
    for (unsigned char & i : buffer)
        i = 0;
}

void WritePackBuffer::write(bool *n, int blockSize) {
    for (int i = blockSize - 1; i >= 0; i--) {
        if (n[i])
            buffer[pos] = set_bit(buffer[pos], sizeof(char) * 8 - bitPos - 1);
        bitPos++;
        if (bitPos == 8) {
            bitPos = 0;
            pos++;
            if (pos == BUFFER_SIZE)
                writeFile();
        }
    }
}

void WritePackBuffer::writeChar(unsigned char c) {
    unsigned char mask = 1 << 7;
    for (int i = 0; i < 8; i++) {
        if (c & mask)
            buffer[pos] = set_bit(buffer[pos], sizeof(char) * 8 - bitPos - 1);
        mask = mask >> 1;
        bitPos++;
        if (bitPos == 8) {
            bitPos = 0;
            pos++;
            if (pos == BUFFER_SIZE)
                writeFile();
        }
    }
}

void WritePackBuffer::closeFile() {
    fclose(file);
}

WriteBuffer::WriteBuffer(char *fileName) {
    file = fopen(fileName, "wb");
    if (!file) {
        std::cout << "Failed to open file \"" << fileName << "\"." << std::endl;
        exit(1);
    }
    pos = 0;
}

void WriteBuffer::writeFile() {
    fwrite(buffer, sizeof(char), pos, file);
    pos = 0;
}

void WriteBuffer::writeChar(unsigned char c) {
    buffer[pos] = c;
    pos++;

    if (pos == BUFFER_SIZE)
        writeFile();
}

void WriteBuffer::closeFile() {
    fclose(file);
}