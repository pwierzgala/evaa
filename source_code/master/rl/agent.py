import pandas as pd

from actions.led import LedAction
from actions.water_level import WaterLevelAction


class Agent:

    def __init__(self):
        iterables = [
            LedAction.values,
            LedAction.values,
            WaterLevelAction.values,
        ]
        names = [
            "red_led",
            "blue_led",
            "water_level",
        ]
        index = pd.MultiIndex.from_product(iterables=iterables, names=names)
        self.action_df = pd.DataFrame(index=index).reset_index()

    def get_action(self, next_action_idx):
        """
        Parameters
        ----------
        next_action_idx : int
            Index of an action in actions DataFrame.
        Returns
        -------
        Series
            Series describing an action.
        """

        return self.action_df.iloc[next_action_idx]
