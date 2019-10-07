class TemperatureState:
    VERY_LOW = 17
    LOW = 20
    MEDIUM = 23  # Optimal
    HIGH = 25
    VERY_HIGH = 29

    @classmethod
    def values(cls):
        return [cls.VERY_LOW, cls.LOW, cls.MEDIUM, cls.HIGH, cls.VERY_HIGH]

    @classmethod
    def measurement_to_state(cls, x):
        if x < 18:
            return cls.VERY_LOW
        elif 18 <= x < 22:
            return cls.LOW
        elif 22 <= x < 24:  # Optimal value
            return cls.MEDIUM
        elif 24 <= x < 28:
            return cls.HIGH
        elif 28 <= x:
            return cls.VERY_HIGH
        else:
            Exception("Unexpected measurement value: {}".format(x))
