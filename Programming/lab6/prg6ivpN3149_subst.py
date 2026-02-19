from prg6ivpN3149_rnd import MyPRNG, AdditiveGenerator
from types import FunctionType
from datetime import datetime
import sys

class TaskError(Exception): """Невозможно выполнить условие задания"""

class subst_int_args:
    """
    Декоратор заменяет любые целые числа,
    которые передаются как позиционные или
    именованные аргументы в функцию или метод, на
    случайные значения, полученные с помощью
    указанного ГПСЧ.
    """
    def __init__(self, prng: MyPRNG, filename: str = ""):
        """
        Инициализация декоратора.

        :param prng: мой генератор псевдослучайных чисел
        :param filename: файл для вывода логирования, по умолчанию в stderr
        """
        self.prng = prng
        self.filename = sys.stderr
        if filename != "":
            self.filename = open(filename, "w")
            
    def _decorate_func(self, func: FunctionType):
        """Декорирут функцию `func`"""
        def wrapper(*args, **kwargs):
            logs = []
            new_args = []
            for i, arg in enumerate(args):
                if isinstance(arg, int):
                    new_args.append(self.prng.next_int())
                    j = 0
                    while new_args[i] == args[i]:
                        new_args[i] = self.prng.next_int()
                        j += 1
                        if j == 100000: # Я понимаю, что это магическое число, но я не придумал как по другому проверить на цикличность
                            raise TaskError("Невозможно выполнить условие " + \
                                            "'Декоратор должен заменить значение аргумента на случайное, " + \
                                            "но не совпадающее со старым значением.'\n" +\
                                            "Попробуйте изменить параметры генератора")
                    logs.append(f"{func.__name__}: аргумент {arg} (args[{i}]) заменен на {new_args[i]}")
                else:
                    new_args.append(arg)
            new_kwargs = dict()
            for key, arg in kwargs.items():
                if isinstance(arg, int):
                    new_kwargs[key] = self.prng.next_int()
                    j = 0
                    while new_kwargs[key] == kwargs[key]:
                        new_kwargs[key] = self.prng.next_int()
                        j += 1
                        if j == 100000:
                            raise TaskError("Невозможно выполнить условие " + \
                                            "'Декоратор должен заменить значение аргумента на случайное, " + \
                                            "но не совпадающее со старым значением.'\n" +\
                                            "Попробуйте изменить параметры генератора")
                    logs.append(f"{func.__name__}: аргумент {arg} (kwargs['{key}']) заменен на {new_kwargs[key]}")
                else:
                    new_kwargs[key] = arg
            res = func(*new_args, **new_kwargs)
            
            curr_time = datetime.now().strftime("%d.%m.%Y %H:%M:%S")
            print(f"{curr_time} {func.__name__}({args=}, {kwargs=}) -> {res}", file=self.filename)
            for log in logs:
                print(curr_time, log, file=self.filename)

            return res
        return wrapper
    
    def __call__(self, obj):
        if isinstance(obj, FunctionType):
            return self._decorate_func(obj)
        elif isinstance(obj, type):
            for name, arg in obj.__dict__.items():
                if len(name) > 4 and name[:2] == "__" and name[-2:] == "__":
                    continue
                if isinstance(arg, FunctionType):
                    setattr(obj, name, self._decorate_func(arg))
                elif isinstance(arg, staticmethod):
                    setattr(obj, name, staticmethod(self._decorate_func(arg.__func__)))
                elif isinstance(arg, classmethod):
                    setattr(obj, name, classmethod(self._decorate_func(arg.__func__)))
            return obj
        else:
            raise TypeError(f"subst_int_args не может быть применён к {type(obj)}")
