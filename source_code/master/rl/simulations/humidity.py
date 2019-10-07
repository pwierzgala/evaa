import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import norm


class HumidityStateSimulation:

    def __init__(self):
        self.x_min = 0
        self.x_max = 10

        mean = 5
        std = 0.5

        self.x = np.linspace(self.x_min, self.x_max, 150)
        y = norm.pdf(self.x, mean, std)
        self.y = np.interp(y, (y.min(), y.max()), (0, 1))

    def __getitem__(self, value):
        x_idx = (np.abs(self.x - value)).argmin()
        return self.y[x_idx]

    def plot(self):
        plt.plot(self.x, self.y, color='coral')
        plt.grid()
        plt.xlim(self.x_min, self.x_max)
        plt.xlabel('humidity')
        plt.ylabel('reward')
        plt.show()


if __name__ == "__main__":
    reward = HumiditySimulation()
    reward.plot()
