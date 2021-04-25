#include "utils.h"
#include "huffman_tree.h"


Code::Code() {
    for (bool & i : code)
        i = false;
    length_code = 0;
}

void Node::setNode(unsigned char s, int w, int ls, int rs, int p, bool l) {
    sym = s;
    weight = w;
    left_son = ls;
    right_son = rs;
    parent = p;
    is_leaf = l;
}

HuffmanTree::HuffmanTree() {
    tree[1].setNode('\0', 2, 2, 3, 0, false);
    tree[2].setNode('\0', 1, 0, 0, 1, true);
    tree[3].setNode('\0', 1, 0, 0, 1, true);
    index_esc = 3;
    index_eof = 2;
    size_tree = 3;
}

void HuffmanTree::eraseTree() {
    for (auto & i : tree)
        i.setNode(0, 0, 0, 0, 0, false);
    tree[1].setNode('\0', 2, 2, 3, 0, false);
    tree[2].setNode('\0', 1, 0, 0, 1, true);
    tree[3].setNode('\0', 1, 0, 0, 1, true);
    index_esc = 3;
    index_eof = 2;
    size_tree = 3;
}

unsigned int HuffmanTree::getEOFindex() const {
    return index_eof;
}

unsigned int HuffmanTree::getESCindex() const {
    return index_esc;
}

int HuffmanTree::numOfNode(unsigned char c) {
    for (int i = 1; i < SIZE_TREE; i++)
        if (tree[i].sym == c && tree[i].is_leaf && i != index_eof && i != index_esc)
            return i;
    return 0;
}

Code HuffmanTree::getCode(unsigned int pos) {
    Code cd;
    int depth = 0;
    while (pos > 1) {
        if (pos == tree[tree[pos].parent].right_son)
            cd.code[depth] = true;
        pos = tree[pos].parent;
        depth++;
    }
    cd.length_code = depth;
    return cd;
}

void HuffmanTree::addElem(unsigned char c) {
    tree[index_esc].setNode('\0', 1, (int)index_esc + 1, (int)index_esc + 2, (int)tree[index_esc].parent, false);
    tree[index_esc + 1].setNode(c, 0, 0, 0, (int)index_esc, true);
    tree[index_esc + 2].setNode('\0', 1, 0, 0, (int)index_esc, true);
    index_esc += 2;
    size_tree += 2;
}

void HuffmanTree::swap(unsigned int n1, unsigned int n2) {
    if (n1 == index_esc)
        index_esc = n2;
    else if (n2 == index_esc)
        index_esc = n1;

    if (n1 == index_eof)
        index_eof = n2;
    else if (n2 == index_eof)
        index_eof = n1;

    if (tree[n1].left_son) {
        tree[tree[n1].left_son].parent = n2;
        tree[tree[n1].right_son].parent = n2;
    }
    if (tree[n2].left_son) {
        tree[tree[n2].left_son].parent = n1;
        tree[tree[n2].right_son].parent = n1;
    }

    Node temp = tree[n1];
    tree[n1] = tree[n2];
    tree[n2] = temp;

    unsigned int tempPar = tree[n1].parent;
    tree[n1].parent = tree[n2].parent;
    tree[n2].parent = tempPar;
}

void HuffmanTree::updateTree(unsigned char c) {
    unsigned int cur = numOfNode(c);
    while (cur >= 1) {
        tree[cur].weight++;
        unsigned int pos = 0;
        for (unsigned int i = cur - 1; i > 1; i--) {
            if (tree[i].weight < tree[cur].weight)
                pos = i;
        }
        if (pos != 0) {
            swap(cur, pos);
            cur = pos;
        }
        cur = tree[cur].parent;
    }
}
