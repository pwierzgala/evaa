import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import norm


class TemperatureStateSimulation:
    def __init__(self):
        self.temperature_min = 14
        self.temperature_max = 30

        self.n = 50
        self.temperatures = np.linspace(
            self.temperature_min, self.temperature_max, self.n)
        self.rewards = self.build_rewards_array()

    def build_rewards_array(self):
        mean = 23
        std = 1

        rewards = norm.pdf(self.temperatures, mean, std)
        rewards = np.interp(rewards, (rewards.min(), rewards.max()), (0, 1))
        return rewards

    def get_reward(self, temperature):
        temperature_idx = (np.abs(self.temperatures - temperature)).argmin()
        return self.rewards[temperature_idx]

    def plot(self):
        plt.plot(self.temperatures, self.rewards, color='coral')
        plt.grid()
        plt.xlim(self.temperature_min, self.temperature_max)
        plt.xlabel('temperature')
        plt.ylabel('reward')
        plt.show()
