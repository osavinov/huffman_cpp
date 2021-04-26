import pytest

from utils import generate_file, remove_file


@pytest.fixture(scope='function')
def teardown_env():
    yield
    remove_file('_test')
    remove_file('_test_decomp')
    remove_file('test.huf')
