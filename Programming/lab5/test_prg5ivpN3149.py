import pytest
from prg5ivpN3149 import FormatError, UndoError, RedoError, MySet

def test_add():
    s = MySet()
    s.add("11:11:11:11:11:11")
    s.add("22:22:22:22:22:22")
    assert s.symmetric_difference({"11:11:11:11:11:11", "22:22:22:22:22:22"}) == set()

def test_formatError():
    s = MySet()
    with pytest.raises(FormatError):
        s.add(123)

def test_redo():
    s = MySet()
    s.add("11:11:11:11:11:11")
    s.undo()
    s.redo()
    assert s.symmetric_difference({"11:11:11:11:11:11"}) == set()

def test_redoError():
    s = MySet()
    s.add("11:11:11:11:11:11")
    s.add("22:22:22:22:22:22")
    s.undo()
    s.add("33:33:33:33:33:33")
    with pytest.raises(RedoError):
        s.redo()

def test_undo():
    s = MySet(["22:22:22:22:22:22"])
    s.add("11:11:11:11:11:11")
    s.undo()
    assert s.symmetric_difference({"22:22:22:22:22:22"}) == set()

def test_undoError():
    s = MySet(["22:22:22:22:22:22"])
    s.add("11:11:11:11:11:11")
    s.undo()
    with pytest.raises(UndoError):
        s.undo()

def test_ior():
    s = MySet()
    s |= {"11:11:11:11:11:11", "22:22:22:22:22:22"}
    assert s.symmetric_difference({"11:11:11:11:11:11", "22:22:22:22:22:22"}) == set()