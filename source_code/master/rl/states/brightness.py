class BrightnessState:
    OFF = 0
    VERY_LOW = 10
    LOW = 30
    MEDIUM = 50
    HIGH = 70
    VERY_HIGH = 90

    @classmethod
    def values(cls):
        return [cls.VERY_LOW, cls.LOW, cls.MEDIUM, cls.HIGH, cls.VERY_HIGH]

    @classmethod
    def measurement_to_state(cls, x):
        if x == 0:
            return cls.OFF
        elif 0 < x < 20:
            return cls.VERY_LOW
        elif 20 <= x < 40:
            return cls.LOW
        elif 40 <= x < 60:
            return cls.MEDIUM
        elif 60 <= x < 80:
            return cls.HIGH
        elif 80 <= x:
            return cls.VERY_HIGH
        else:
            Exception("Unexpected measurement value: {}".format(x))
