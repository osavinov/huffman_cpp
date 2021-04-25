import subprocess
import os


def remove_file(filename: str):
    assert subprocess.call(['rm', filename]) == 0


def generate_file(filesize: int):
    file = open('_test', 'wb+')
    file.write(os.urandom(filesize))
    file.close()


def compress(filename: str):
    assert subprocess.call(
        ['./huffman', '-c', '-f', filename, '-o', 'test.huf'],
    ) == 0


def decompress(filename: str):
    assert subprocess.call(
        ['./huffman', '-d', '-f', filename, '-o', 'test.huf'],
    ) == 0


def get_content(filename: str):
    file = open(filename, 'rb')
    content = file.read()
    file.close()
    return content
