#include "utils.h"

#ifndef HUFFMAN_CPP_HUFFMAN_TREE_H
#define HUFFMAN_CPP_HUFFMAN_TREE_H

#endif //HUFFMAN_CPP_HUFFMAN_TREE_H

struct Code {
    bool code[SIZE_CODE];
    int length_code;
    Code();
};

class Node {
public:
    unsigned char sym;
    int weight;
    int left_son;
    int right_son;
    unsigned int parent;
    bool is_leaf;
    void setNode(unsigned char s, int w, int ls, int rs, int p, bool l);
};

class HuffmanTree {
private:
    unsigned int index_esc;
    unsigned int index_eof;
    unsigned int size_tree;
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
