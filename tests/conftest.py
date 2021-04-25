import pytest

from utils import generate_file, remove_file


@pytest.fixture(scope='function')
def handle_env(request):
    generate_file(request.param)
    yield
    remove_file('_test')
    remove_file('_test_decomp')
    remove_file('test.huf')
