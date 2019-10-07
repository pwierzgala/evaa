class LedAction:

    SET_0 = 0
    SET_20 = 1
    SET_40 = 2
    SET_60 = 3
    SET_80 = 4
    SET_100 = 5

    @classmethod
    def values(cls):
        return [
            cls.SET_0,
            cls.SET_20,
            cls.SET_40,
            cls.SET_60,
            cls.SET_80,
            cls.SET_100
        ]
