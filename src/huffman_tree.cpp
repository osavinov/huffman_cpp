#include "utils.h"
#include "huffman_tree.h"


Code::Code() {
    for (bool & i : code)
        i = false;
    lengthCode = 0;
}

void Node::setNode(unsigned char s, int w, int ls, int rs, int p, bool l) {
    sym = s;
    weight = w;
    leftSon = ls;
    rightSon = rs;
    parent = p;
    isLeaf = l;
}

HuffmanTree::HuffmanTree() {
    tree[1].setNode('\0', 2, 2, 3, 0, false);
    tree[2].setNode('\0', 1, 0, 0, 1, true);
    tree[3].setNode('\0', 1, 0, 0, 1, true);
    indexESC = 3;
    indexEOF = 2;
    sizeTree = 3;
}

void HuffmanTree::eraseTree() {
    for (auto & i : tree)
        i.setNode(0, 0, 0, 0, 0, false);
    tree[1].setNode('\0', 2, 2, 3, 0, false);
    tree[2].setNode('\0', 1, 0, 0, 1, true);
    tree[3].setNode('\0', 1, 0, 0, 1, true);
    indexESC = 3;
    indexEOF = 2;
    sizeTree = 3;
}

unsigned int HuffmanTree::getEOFindex() const {
    return indexEOF;
}

unsigned int HuffmanTree::getESCindex() const {
    return indexESC;
}

int HuffmanTree::numOfNode(unsigned char c) {
    for (int i = 1; i < SIZE_TREE; i++)
        if (tree[i].sym == c && tree[i].isLeaf && i != indexEOF && i != indexESC)
            return i;
    return 0;
}

Code HuffmanTree::getCode(unsigned int pos) {
    Code cd;
    int depth = 0;
    while (pos > 1) {
        if (pos == tree[tree[pos].parent].rightSon)
            cd.code[depth] = true;
        pos = tree[pos].parent;
        depth++;
    }
    cd.lengthCode = depth;
    return cd;
}

void HuffmanTree::addElem(unsigned char c) {
    tree[indexESC].setNode('\0', 1, (int)indexESC + 1, (int)indexESC + 2, (int)tree[indexESC].parent, false);
    tree[indexESC + 1].setNode(c, 0, 0, 0, (int)indexESC, true);
    tree[indexESC + 2].setNode('\0', 1, 0, 0, (int)indexESC, true);
    indexESC += 2;
    sizeTree += 2;
}

void HuffmanTree::swap(unsigned int n1, unsigned int n2) {
    if (n1 == indexESC)
        indexESC = n2;
    else if (n2 == indexESC)
        indexESC = n1;

    if (n1 == indexEOF)
        indexEOF = n2;
    else if (n2 == indexEOF)
        indexEOF = n1;

    if (tree[n1].leftSon) {
        tree[tree[n1].leftSon].parent = n2;
        tree[tree[n1].rightSon].parent = n2;
    }
    if (tree[n2].leftSon) {
        tree[tree[n2].leftSon].parent = n1;
        tree[tree[n2].rightSon].parent = n1;
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
