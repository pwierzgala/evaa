import numpy as np


class DayHourState:

    @staticmethod
    def day_values():
        return np.linspace(1, 90, 90)

    @staticmethod
    def hour_values():
        return np.linspace(0, 23, 24)
