import subprocess

import pytest

from utils import compress, decompress, get_content, generate_file


@pytest.mark.parametrize(
    'filesize',
    (0, 666, 1024, 1024*16, 1024*32, 1024*100),
)
def test_compress_decompress(teardown_env, filesize):
    generate_file(filesize=filesize)
    compress('_test')
    decompress('_test_decomp')

    source_file_data = get_content('_test')
    dest_file_data = get_content('_test_decomp')

    assert source_file_data == dest_file_data


@pytest.mark.parametrize(
    'content,result',
    [
        pytest.param(
            b'',
            b'',
            id='empty_file',
        ),
        pytest.param(
            b'aaa',
            b'\xb0\x98',
            id='several_symbols',
        ),
        pytest.param(
            b'aaaaaaaaaaaa',
            b'\xb0\x90\x04',
            id='long_sequence',
        ),
        pytest.param(
            b'aaabbcccccccdddd',
            b'\xb0\x9d\x8a\x160\xd3\x16@2\x80',
            id='different_symbols',
        )
    ],
)
def test_compress(teardown_env, content, result):
    generate_file(content=content, random=False)
    compress('_test')
    input_content: bytes = get_content('test.huf')
    assert input_content == result


def test_wrong_parameters():
    return_code = subprocess.call(
        ['./huffman', '-d', '-c', '-f', 'test', '-o', 'test.huf'],
    )
    assert return_code == 1
