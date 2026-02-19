import pytest, sys
from prg6ivpN3149_rnd import MyPRNG, AdditiveGenerator
from prg6ivpN3149_subst import subst_int_args, TaskError

prng = MyPRNG(AdditiveGenerator(seed=[1, 2, 3, 4, 5], offsets=[1, 2], m=4))

def test_taskError():
    @subst_int_args(MyPRNG(AdditiveGenerator(seed=[1, 1], offsets=[1], m=1)))
    def fnc(a: int):
        pass
    with pytest.raises(TaskError):
        fnc(-1)

def test_func_change():
    @subst_int_args(prng)
    def a(x, y):
        return x + y

    result = a(10, 20)
    assert result != 30

def test_no_changes():
    @subst_int_args(prng)
    def a(x, y):
        return f"{x}-{y}"

    result = a("hello", 3.14)
    assert result == "hello-3.14"

def test_class_change():
    @subst_int_args(prng)
    class A:
        def __init__(self, a):
            self.a=a
        def b(self, a):
            return a

    obj = A(7)
    assert obj.b(5) != 5
    assert obj.a == 7

def test_logs(capsys):
    @subst_int_args(prng=prng)
    def a(x):
        return x + 1
    
    a(15)
    logs = capsys.readouterr().err
    assert "a: аргумент 15" in logs
    assert "a(args=" in logs

def test_decorator_target():
    dec = subst_int_args(prng)
    with pytest.raises(TypeError):
        dec(42)
