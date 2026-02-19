from abc import ABC, abstractmethod
from collections.abc import Iterator
import string

class AdditiveGenerator:
    """Аддитивный генератор случайных чисел"""
    def __init__(self, *, seed: list[int], offsets: list[int], m: int):
        """
        Инициализация генератора.

        :param seed: начальное состояние — список целых чисел (X1, X2, ..., Xk)
        :param offsets: список отступов (i, j, l...)
        :param m: размер слова в битах
        """
        if len(seed) < max(offsets):
            raise ValueError("Длина начального состояния должна быть >= max(offsets)")

        self.state = seed.copy()
        self.offsets = offsets
        self.modulus = 2 ** m
        self.index = len(seed)
    def __iter__(self):
        return self
    def __next__(self):
        next_value = sum(self.state[-i] for i in self.offsets) % self.modulus
        self.state.append(next_value)
        self.index += 1
        return next_value


class PRNGBase(ABC):
    @abstractmethod
    def next_int(self) -> int:
        """Возвращает случайное целое число"""
        ...

    @abstractmethod
    def next_float(self) -> float:
        """Возвращает случайное вещественное число в диапазоне от 0.0 до 1.0 (не включая)"""
        ...

    @abstractmethod
    def next_str(self) -> str: 
        """Возвращает случайную последовательность символов"""
        ...

    @abstractmethod
    def shuffle_str(self, src: str) -> str:
        """Получает строку src и возвращает строку, содержащую символы из src, переставленные в случайном порядке"""
        ...


class MyPRNG(PRNGBase):
    SYMBOLS = string.ascii_letters + string.digits + string.punctuation
    def __init__(self, gen: Iterator):
        self.gen = iter(gen)

    def next_int(self) -> int:
        return next(self.gen)*(-1)**next(self.gen)

    def next_float(self) -> float:
        a = next(self.gen)
        b = next(self.gen)
        if a*b==0 or a==b:
            return 0
        a /= b
        return a if a < 1 else 1/a


    def next_str(self) -> str:
        s = ""
        curr_symb = 13
        for _ in range(next(self.gen)):
            curr_symb = (curr_symb+next(self.gen))%len(self.SYMBOLS)
            s += self.SYMBOLS[curr_symb]
        return s



    def shuffle_str(self, src: str) -> str:
        indexes = list(range(len(src)))
        while len(indexes) > 1:
            i1 = indexes[self.next_int()%len(indexes)]
            i2 = indexes[self.next_int()%len(indexes)]
            if i2 < i1:
                i1, i2 = i2, i1
            elif i1 == i2:
                indexes.remove(i1)
                continue
            src = src[:i1] + src[i2] + src[i1+1:i2] + src[i1] + src[i2+1:]
            indexes.remove(i1)
            indexes.remove(i2)
        return src

