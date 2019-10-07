class PhReward:
    VERY_LOW = 4.5
    LOW = 5.5
    MEDIUM = 6.5  # Optimal
    HIGH = 7.5
    VERY_HIGH = 8.5

    @classmethod
    def values(cls):
        return [cls.VERY_LOW, cls.LOW, cls.MEDIUM, cls.HIGH, cls.VERY_HIGH]

    @classmethod
    def measurement_to_state(cls, x):
        if x < 5:
            return cls.VERY_LOW
        elif 5 <= x < 6:
            return cls.LOW
        elif 6 <= x < 7:  # Optimal value
            return cls.MEDIUM
        elif 7 <= x < 8:
            return cls.HIGH
        elif 8 <= x:
            return cls.VERY_HIGH
        else:
            Exception("Unexpected measurement value: {}".format(x))
