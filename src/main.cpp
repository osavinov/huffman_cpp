#include <unistd.h>
#include <iostream>

#include "write_buffers.h"
#include "read_buffers.h"
#include "huffman_tree.h"


HuffmanTree huffman_tree;

void compress(char *inputFile, char *outputFile) {
    ReadBuffer input(inputFile);
    input.readFile();

    WritePackBuffer out(outputFile);
    if (!input.FILE_SIZE)
        return;
    unsigned char k;
    int count = 1;
    while (input.readChar(&k)) {
        if (huffman_tree.numOfNode(k) == 0) {
            Code cur_esc = huffman_tree.getCode(huffman_tree.getESCindex());
            out.write(cur_esc.code, cur_esc.length_code);
            out.writeChar(k);
            huffman_tree.addElem(k);
        } else {
            Code curCode = huffman_tree.getCode(huffman_tree.numOfNode(k));
            out.write(curCode.code, curCode.length_code);
        }
        huffman_tree.updateTree(k);
        count++;
    }
    Code curEOF = huffman_tree.getCode(huffman_tree.getEOFindex());
    out.write(curEOF.code, curEOF.length_code);
    out.writeFile();
    input.closeFile();
    out.closeFile();
    huffman_tree.eraseTree();
}

void decompress(char *inputFile, char *outputFile) {
    ReadPackBuffer input(inputFile);
    WriteBuffer output(outputFile);
    input.readFile();
    if (!input.FILE_SIZE)
        return;
    bool current;
    unsigned int pos = 1;
    unsigned char current_char;
    while (input.readBit(&current)) {
        if (current)
            pos = huffman_tree.tree[pos].right_son;
        else
            pos = huffman_tree.tree[pos].left_son;

        if (huffman_tree.tree[pos].is_leaf) {
            if (pos == huffman_tree.getESCindex()) {
                input.read(&current_char, 8);
                huffman_tree.addElem(current_char);
                huffman_tree.updateTree(current_char);
                pos = 1;
            } else if (pos == huffman_tree.getEOFindex()) {
                break;
            } else {
                current_char = huffman_tree.tree[pos].sym;
                huffman_tree.updateTree(current_char);
                pos = 1;
            }
            output.writeChar(current_char);
        }
    }

    output.writeFile();
    input.closeFile();
    output.closeFile();
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