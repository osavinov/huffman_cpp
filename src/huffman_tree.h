#include "utils.h"

#ifndef HUFFMAN_CPP_HUFFMAN_TREE_H
#define HUFFMAN_CPP_HUFFMAN_TREE_H

#endif //HUFFMAN_CPP_HUFFMAN_TREE_H

struct Code {
    bool code[SIZE_CODE];
    int lengthCode;
    Code();
};

class Node {
public:
    unsigned char sym;
    int weight;
    int leftSon;
    int rightSon;
    unsigned int parent;
    bool isLeaf;
    void setNode(unsigned char s, int w, int ls, int rs, int p, bool l);
};

class HuffmanTree {
private:
    unsigned int indexESC;
    unsigned int indexEOF;
    unsigned int sizeTree;
public:
    Node tree[SIZE_TREE];
    HuffmanTree();
    void eraseTree();
    unsigned int getEOFindex() const;
    unsigned int getESCindex() const;
    int numOfNode(unsigned char c);
    Code getCode(unsigned int pos);
    void addElem(unsigned char c);
    void swap(unsigned int n1, unsigned int n2);
    void updateTree(unsigned char c);
};
