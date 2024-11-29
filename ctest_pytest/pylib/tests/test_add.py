from pylib.add import add
from pylib._pylib import add as add_cpp

def test_add():
    res = add(20, 22)
    assert res == 42

def test_add_cpp():
    res = add_cpp(20,22)
    assert res == 42
