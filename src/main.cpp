#include <unistd.h>
#include <iostream>

#include "write_buffers.h"
#include "read_buffers.h"
#include "huffman_tree.h"


HuffmanTree huffman_tree;

void compress(char *inputFile, char *outputFile) {
    ReadBuffer inp(inputFile);
    inp.readFile();

    WritePackBuffer out(outputFile);
    if (!inp.FILE_SIZE)
        return;
    unsigned char k;
    int count = 1;
    while (inp.readChar(&k)) {
        if (huffman_tree.numOfNode(k) == 0) {
            Code curESC = huffman_tree.getCode(huffman_tree.getESCindex());
            out.write(curESC.code, curESC.lengthCode);
            out.writeChar(k);
            huffman_tree.addElem(k);
        } else {
            Code curCode = huffman_tree.getCode(huffman_tree.numOfNode(k));
            out.write(curCode.code, curCode.lengthCode);
        }
        huffman_tree.updateTree(k);
        count++;
    }
    Code curEOF = huffman_tree.getCode(huffman_tree.getEOFindex());
    out.write(curEOF.code, curEOF.lengthCode);
    out.writeFile();
    inp.closeFile();
    out.closeFile();
    huffman_tree.eraseTree();
}

void decompress(char *inputFile, char *outputFile) {
    ReadPackBuffer inp(inputFile);
    WriteBuffer out(outputFile);
    inp.readFile();
    if (!inp.FILE_SIZE)
        return;
    bool cur;
    unsigned int pos = 1;
    unsigned char curChar;
    while (inp.readBit(&cur)) {
        if (cur)
            pos = huffman_tree.tree[pos].rightSon;
        else
            pos = huffman_tree.tree[pos].leftSon;

        if (huffman_tree.tree[pos].isLeaf) {
            if (pos == huffman_tree.getESCindex()) {
                inp.read(&curChar, 8);
                huffman_tree.addElem(curChar);
                huffman_tree.updateTree(curChar);
                pos = 1;
            } else if (pos == huffman_tree.getEOFindex()) {
                break;
            } else {
                curChar = huffman_tree.tree[pos].sym;
                huffman_tree.updateTree(curChar);
                pos = 1;
            }
            out.writeChar(curChar);
        }
    }

    out.writeFile();
    inp.closeFile();
    out.closeFile();
    huffman_tree.eraseTree();
}

int main(int argc, char *argv[]) {
    int opt;
    char filename[256];
    char archFile[256];
    char mode;

    while ((opt = getopt(argc, argv, ":f:o:cd")) != -1) {
        switch (opt) {
            case 'f':
                strcpy(filename, optarg);
                break;
            case 'o':
                strcpy(archFile, optarg);
                break;
            case 'c':
                mode = 'c';
                break;
            case 'd':
                mode = 'd';
                break;
            default:
                break;
        }
    }

    if (mode == 'c')
        compress(filename, archFile);
    else if (mode == 'd')
        decompress(archFile, filename);

    return 0;
}