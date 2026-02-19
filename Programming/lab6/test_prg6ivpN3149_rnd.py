import pytest
from prg6ivpN3149_rnd import MyPRNG, AdditiveGenerator

prng = MyPRNG(AdditiveGenerator(seed=[1, 2, 3, 4, 5], offsets=[1, 2], m=4))

def test_equal_shaffle():
    a = MyPRNG(AdditiveGenerator(seed=[1, 1, 1], offsets=[1, 2, 3], m=1))
    assert a.shuffle_str("12345") == "12345"

def test_range():
    gen = AdditiveGenerator(seed=[1, 1], offsets=[1, 2], m=4)
    result = [next(gen) for _ in range(10)]
    assert all(0 <= x < 16 for x in result)

def test_isInt():
    for _ in range(10):
        assert isinstance(prng.next_int(), int)

def test_floatRange():
    for _ in range(1000):
        assert 0 <= prng.next_float() < 1

def test_shaffle():
    orig = "abcdef"
    shuf = prng.shuffle_str(orig)
    assert len(shuf) == len(orig)
    assert sorted(shuf) == sorted(orig)

def test_valueError():
    with pytest.raises(ValueError):
        AdditiveGenerator(seed=[1], offsets=[2], m=4)