import numpy as np
import random
from agent import Agent
from environment import Environment
from environment_simulation import EnvironmentSimulation


class Model:

    def __init__(self, discount=.9, learning_rate=.1):
        """
        Parameters
        ----------
        discount : float
        learning_rate : float
        """
        self.discount = discount
        self.learning_rate = learning_rate

        self.environment = Environment()
        self.agent = Agent()

        self.n_states = self.environment.state_df.shape[0]
        self.n_actions = self.agent.action_df.shape[0]
        self.q_matrix = np.zeros(shape=(self.n_states, self.n_actions))

        self.current_state_idx = None
        self.current_action_idx = None

    def initialize_q_matrix(self):
        environment_simulation = EnvironmentSimulation()
        for state_i in range(self.n_states):
            for action_i in range(self.n_actions):
                state = self.environment.state_df.iloc[state_i]
                action = self.agent.action_df.iloc[action_i]

                next_state = environment_simulation.get_next_state(state, action)
                next_reward = environment_simulation.get_reward(next_state)

                self.q_matrix[state_i][action_i] = next_reward

    def get_next_action(self, current_state):
        """
        Parameters
        ----------
        current_state : Series
            Series describing current state.
        Returns
        -------
        int
            Index of an action in Q-matrix,
        """
        epsilon = 0.2
        if random.uniform(0, 1) < epsilon:
            # Exploration
            action = random.randint(0, self.n_actions)
        else:
            # Exploitation
            actions = self.q_matrix[current_state.index]
            action = np.argmax(actions)

        return action

    def pre_action_step(self, plant_id):
        """
        Select and perform action.

        Parameters
        ----------
        plant_id : int
             Plant identifier.
        Returns
        -------
        Series
            Series describing an action.
        """
        current_state = self.environment.get_current_state(plant_id)
        self.current_state_idx = current_state.index
        self.current_action_idx = self.get_next_action(current_state)
        action = self.agent.get_action(self.current_action_idx)
        return action

    def post_action_step(self, plant_id):
        """
        Update Q-value.

        Parameters
        ----------
        plant_id : int
             Plant identifier.
        Returns
        -------
        None
        """
        next_state = self.environment.get_current_state(plant_id)
        next_state_idx = next_state.index
        current_q = self.q_matrix[self.current_state_idx][self.current_action_idx]
        current_reward = self.environment.get_reward(plant_id)
        next_reward = np.max(self.q_matrix[next_state_idx])

        learned_value = current_reward + self.discount * next_reward
        self.q_matrix[self.current_state_idx][self.current_action_idx] = \
            current_q + self.learning_rate * (learned_value - current_q)
