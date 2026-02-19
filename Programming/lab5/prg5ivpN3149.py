class FormatError(Exception): pass
class UndoError(Exception): pass
class RedoError(Exception): pass

class MySet(set):
    def __init__(self, iterable=None):
        super().__init__()
        if iterable:
            super().update(iterable)
        self.history = list()
        self.history.append(self.copy())
        self.history_position = 0


    def undo(self):
        if self.history_position == 0:
            raise UndoError
        self.history_position -= 1
        self.__go_to_version(self.history_position)

    def redo(self):
        if self.history_position == len(self.history) - 1:
            raise RedoError
        self.history_position += 1
        self.__go_to_version(self.history_position)
    
    # Переход к определённой версии
    def __go_to_version(self, num: int):
        super().clear()
        super().update(self.history[num].copy())
    
    @staticmethod
    def check_MAC(mac: str) -> bool:
        alphabet = "0123456789abcdef"
        if type(mac) != str:
            return False
        mac = mac.lower()
        if mac.count(":") != 5:
            return False
        for octt in mac.split(":"):
            if len(octt) != 2 or octt[0] not in alphabet or octt[1] not in alphabet:
                return False
        return True
    

    def end_of_funcs(self):
        for elem in self:
            if not MySet.check_MAC(elem):
                self.__go_to_version(self.history_position)
                raise FormatError
            
        self.history = self.history[:self.history_position+1] + [self.copy()]
        self.history_position+=1

    #region пререопределённые функции
    def add(self, element):
        res = super().add(element)
        self.end_of_funcs()
        return res


    def update(self, *s):
        res = super().update(*s)
        self.end_of_funcs()
        return res
    
    def symmetric_difference_update(self, s):
        res = super().symmetric_difference_update(s)
        self.end_of_funcs()
        return res
    
    def __ior__(self, value):
        res = super().__ior__(value)
        self.end_of_funcs()
        return res
    
    def __ixor__(self, value):
        res = super().__ixor__(value)
        self.end_of_funcs()
        return res
    
    
    def pop(self):
        res = super().pop()
        self.end_of_funcs()
        return res
    
    def discard(self, element):
        res = super().discard(element)
        self.end_of_funcs()
        return res
    
    def clear(self):
        res = super().clear()
        self.end_of_funcs()
        return res
    
    def remove(self, element):
        res = super().remove(element)
        self.end_of_funcs()
        return res
    
    
    def difference_update(self, *s):
        res = super().difference_update(*s)
        self.end_of_funcs()
        return res
    
    def intersection_update(self, *s):
        res = super().intersection_update(*s)
        self.end_of_funcs()
        return res
    
    def __iand__(self, value):
        res = super().__iand__(value)
        self.end_of_funcs()
        return res
    
    def __isub__(self, value):
        res = super().__isub__(value)
        self.end_of_funcs()
        return res
    #endregion