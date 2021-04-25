import timeit
from utils import generate_file, remove_file, compress, decompress


def compress_decompress():
    compress('_test')
    decompress('_test_decomp')


def main():
    generate_file(1024*100)
    elapsed_time: float = timeit.timeit(
        'compress_decompress()',
        setup='from __main__ import compress_decompress',
        number=100,
    )
    print(f'time = {elapsed_time} sec')
    remove_file('_test')
    remove_file('_test_decomp')
    remove_file('test.huf')


if __name__ == '__main__':
    main()
