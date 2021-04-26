import subprocess
import os

import typing


def remove_file(filename: str):
    run_command(['rm', '-f', filename, '||', 'true'])


def generate_file(
        filesize: int = None,
        random: bool = True,
        content: bytes = None,
        filename: str = '_test',
):
    file = open(filename, 'wb+')
    if random:
        file.write(os.urandom(filesize))
    else:
        file.write(content)
    file.close()


def compress(filename: str):
    assert subprocess.call(
        ['./huffman', '-c', '-f', filename, '-o', 'test.huf'],
    ) == 0


def decompress(filename: str):
    assert subprocess.call(
        ['./huffman', '-d', '-f', filename, '-o', 'test.huf'],
    ) == 0


def run_command(args: typing.List[str]):
    assert subprocess.call(args) == 0


def get_content(filename: str):
    file = open(filename, 'rb')
    content = file.read()
    file.close()
    return content
