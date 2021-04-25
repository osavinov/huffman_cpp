from utils import compress, decompress, get_content


def test_compress_decompress(handle_env):
    compress('_test')
    decompress('_test_decomp')

    source_file_data = get_content('_test')
    dest_file_data = get_content('_test_decomp')

    assert source_file_data == dest_file_data
