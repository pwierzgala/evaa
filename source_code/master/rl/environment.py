import pandas as pd

from states.brightness import BrightnessState
from states.day_hour import DayHourState
from states.water_level import WaterLevelState


class Environment:

    def __init__(self):
        iterables = [
            DayHourState.day_values,
            DayHourState.hour_values,
            BrightnessState.values,
            WaterLevelState.values,
        ]
        names = [
            "day",
            "hour",
            "brightness",
            "water_level",
        ]
        index = pd.MultiIndex.from_product(iterables=iterables, names=names)
        self.state_df = pd.DataFrame(index=index).reset_index()

    def get_reward(self, plant_id):
        """
        Parameters
        ----------
        plant_id : int
            Plant identifier
        Returns
        -------
        float
            Reward value
        """

        normalized_weight = 0.3
        normalized_green_area = 0.7
        total_reward = 0.5*(normalized_weight + normalized_green_area)
        return total_reward

    def get_current_state(self, plant_id):
        """
        Parameters
        ----------
        plant_id : int
            Plant identifier.
        Returns
        -------
        Series
            Series describing current state.
        """
        # Read state values from controller.
        day = 1
        hour = 0
        brightness = 0
        water_level = 0

        # Convert continuous values to discrete values.
        brightness = BrightnessState.measurement_to_state(brightness)
        water_level = WaterLevelState.measurement_to_state(water_level)

        # Get DataFrame row matching current state.
        mask = \
            (self.state_df["day"] == day) & \
            (self.state_df["hour"] == hour) & \
            (self.state_df["brightness"] == brightness) & \
            (self.state_df["water_level"] == water_level)
        state = self.state_df[mask]
        return state
