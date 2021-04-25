Simple Huffman encoder/decoder
===========


Requirements
------------

* gcc
* python3.7+ with pytest

Building
--------

    $ make huffman

Run tests
---------
    $ make test

Usage
-----

    $ ./huffman -c -f input.txt -o test.huf
    $ ./huffman -d -f output.txt -o test.huf