#ifndef HUFFMAN_CPP_UTILS_H
#define HUFFMAN_CPP_UTILS_H

#endif //HUFFMAN_CPP_UTILS_H

#pragma once
#define SIZE_TREE 516
#define BUFFER_SIZE 4096*4
#define SIZE_CODE 258

unsigned inline int set_bit(unsigned int a, unsigned int pos) {
    return (1 << pos) & a ? a : a ^ (1 << pos);
}
