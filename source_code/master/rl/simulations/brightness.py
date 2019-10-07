import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm
from mpl_toolkits.mplot3d import Axes3D
from scipy.stats import norm

from actions.led import LedAction
from states.brightness import BrightnessState

plt.rcParams.update({
    "figure.autolayout": True})


class BrightnessStateSimulation:

    def __init__(self):
        self.hour_min = 0
        self.hour_max = 23
        self.brightness_min = 0
        self.brightness_max = 100

        self.n = 50  # Resolution.
        self.hours_space = np.linspace(self.hour_min, self.hour_max, self.n)
        self.brightness_space = np.linspace(
            self.brightness_min, self.brightness_max, self.n)
        self.rewards = self.build_rewards_array()

    def build_rewards_array(self):
        max_brightness_hour = self.hour_max / 2
        std = 15

        rewards = np.zeros((self.n, self.n))
        for i in range(self.n):
            # The ratio changes
            # - from 0 to 1 for hours 0-12
            # - from 1 to 0 for hours 12-23
            ratio = 1 - abs(
                (self.hours_space[i] - max_brightness_hour) / max_brightness_hour)

            # The mean changes
            # - from 0 to 100 for hours 0-12
            # - from 100 to 0 for hours 12-23
            # We want to simulate natural day/night cycle have by gradually changing
            # maximum reward from (brightness = 0, hour = 0) to (brightness = 100,
            # hour = 12).
            mean = ratio**0.5 * self.brightness_max

            # Calculate reward distribution for currently iterated hour.
            reward = norm.pdf(self.brightness_space, mean, std)
            reward = np.interp(reward, (reward.min(), reward.max()), (0, 1))
            rewards[i] = reward

        return rewards.T

    def get_reward(self, hour, brightness):
        hour_idx = (np.abs(self.hours_space - hour)).argmin()
        brightness = BrightnessState.measurement_to_state(brightness)
        brightness_idx = (np.abs(self.brightness_space - brightness)).argmin()
        return self.rewards[hour_idx][brightness_idx]

    @staticmethod
    def get_new_state(red_led_action, blue_led_action):
        """
        Parameters
        ----------
        red_led_action : LedAction
        blue_led_action : LedAction

        Returns
        -------
        float
            Simulated value of light intensity sensor.
        """
        action_to_state = {
            LedAction.SET_0: BrightnessState.OFF,
            LedAction.SET_20: BrightnessState.VERY_LOW,
            LedAction.SET_40: BrightnessState.LOW,
            LedAction.SET_60: BrightnessState.MEDIUM,
            LedAction.SET_80: BrightnessState.HIGH,
            LedAction.SET_100: BrightnessState.VERY_HIGH,
        }
        return (action_to_state[red_led_action] + action_to_state[blue_led_action])*0.5

    def plot(self):
        elev = 55
        azim = 100
        xmg, ymg = np.meshgrid(self.hours_space, self.brightness_space)

        fig = plt.figure(figsize=(30, 10))
        ax = fig.add_subplot(1, 1, 1, projection='3d')
        ax.view_init(elev, azim)
        ax.plot_surface(xmg, ymg, self.rewards, cmap=cm.jet)
        ax.set_xlabel('hour')
        ax.set_ylabel('brightness')
        ax.set_zlabel('reward')
        ax.set_title("brightness reward")
        plt.show()


if __name__ == "__main__":
    brightness_state = BrightnessStateSimulation()
    brightness_state.plot()
