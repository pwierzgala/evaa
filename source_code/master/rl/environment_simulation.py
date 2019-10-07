import numpy as np

from simulations.brightness import BrightnessStateSimulation
from simulations.day_hour import DayHourStateSimulation
from simulations.water_level import WaterLevelStateSimulation


class EnvironmentSimulation:

    def __init__(self):
        self.day_hour_state_sim = DayHourStateSimulation()
        self.brightness_state_sim = BrightnessStateSimulation()
        self.water_state_sim = WaterLevelStateSimulation()

    def get_next_state(self, state, action):
        day = state["day"]
        hour = state["hour"]
        new_day, new_hour = self.day_hour_state_sim.get_new_state(day=day, hour=hour)

        new_brightness = self.brightness_state_sim.get_new_state(
            red_led_action=action["red_lef"],
            blue_led_action=action["blue_lef"]
        )

        water_level = state["water_level"]
        new_water_level = self.water_state_sim.get_next_state(
            state=water_level,
            action=action["water_level"]
        )

        return {
            "day": new_day,
            "hour": new_hour,
            "brightness": new_brightness,
            "water_level": new_water_level
        }

    def get_reward(self, state):
        hour = state["hour"]

        brightness = state["brightness"]
        brightness_reward = self.brightness_state_sim.get_reward(hour, brightness)

        water_level = state["water_level"]
        water_level_reward = self.water_state_sim.get_reward(water_level)

        total_reward = np.prod([
            brightness_reward,
            water_level_reward
        ])
        return total_reward
