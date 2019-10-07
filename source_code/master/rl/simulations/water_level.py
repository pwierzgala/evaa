import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import norm

from actions.water_level import WaterLevelAction
from states.water_level import WaterLevelState


class WaterLevelStateSimulation:

    def __init__(self):
        self.water_level_min = 0
        self.water_level_max = 100
        self.water_level_optimal = WaterLevelState.HIGH

        self.n = 50
        self.water_levels = np.linspace(
            self.water_level_min, self.water_level_max, self.n)
        self.rewards = self.build_rewards_array()

    def build_rewards_array(self):
        mean = self.water_level_optimal
        std = 5

        rewards = norm.pdf(self.water_levels, mean, std)
        rewards = np.interp(rewards, (rewards.min(), rewards.max()), (0, 1))

        # Drop reward significantly when it's above optimal level.
        mask = self.water_levels > self.water_level_optimal
        rewards[mask] **= 20
        return rewards

    def get_reward(self, water_level):
        water_level = WaterLevelState.measurement_to_state(water_level)
        water_level_idx = (np.abs(self.water_levels - water_level)).argmin()
        return self.rewards[water_level_idx]

    @staticmethod
    def get_next_state(state, action):
        if action == WaterLevelAction.NOTHING:
            return state
        elif action == WaterLevelAction.ADD:
            return state + 20

    def plot(self):
        plt.plot(self.water_levels, self.rewards, color='coral')
        plt.grid()
        plt.xlim(self.water_level_min, self.water_level_max)
        plt.ylim(0, 1.1)
        plt.xlabel('water level')
        plt.ylabel('reward')
        plt.show()
