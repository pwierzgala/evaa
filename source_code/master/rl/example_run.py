import random
import numpy as np

np.set_printoptions(precision=2)

MOVE_LEFT = 0
MOVE_RIGHT = 1


class Model:

    def __init__(self):
        self.win_loss_states = [0, 6]

        self.s = [
            [None, 0],
            [-100, 0],
            [0, 0],
            [0, 0],
            [0, 0],
            [0, 100],
            [0, 0],
            [100, 0],
            [0, 0],
            [0, None]
        ]

        self.q = [
            [0, 0],
            [0, 0],
            [0, 0],
            [0, 0],
            [0, 0],
            [0, 0],
            [0, 0],
            [0, 0],
            [0, 0],
            [0, 0]
        ]

    def is_goal_state_reached(self, current_state):
        return current_state in [6]

    def get_all_possible_next_action(self, current_state):
        rewards = self.s[current_state]
        actions = []
        if rewards[0] is not None:
            actions.append(MOVE_LEFT)
        if rewards[1] is not None:
            actions.append(MOVE_RIGHT)
        return actions

    def get_next_state(self, current_state, action):
        if action == MOVE_LEFT:
            return current_state - 1
        elif action == MOVE_RIGHT:
            return current_state + 1

    def get_next_action(self, current_state):
        actions = self.get_all_possible_next_action(current_state)
        action = random.choice(actions)  # Select any one action randomly.
        return action

    def is_game_over(self, current_state):
        return current_state in self.win_loss_states

    def train(self, epochs=1000, discount=.9, learning_rate=.1):
        for i in range(epochs):
            current_state = random.choice(range(len(self.s)))  # Get a starting place.
            while not self.is_game_over(current_state):
                next_action = self.get_next_action(current_state)
                next_state = self.get_next_state(current_state, next_action)

                current_q = self.q[current_state][next_action]
                current_reward = self.s[current_state][next_action]
                next_reward = max(self.q[next_state])
                learned_value = current_reward + discount*next_reward
                self.q[current_state][next_action] = \
                    current_q + learning_rate * (learned_value - current_q)

                current_state = next_state

            print("Episode ", i, " done")
        print("Q-Matrix:")
        print(np.array(self.q))
        print("Training done...")


if __name__ == "__main__":
    model = Model()
    model.train()
