import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm
from mpl_toolkits.mplot3d import Axes3D
from scipy.stats import norm


plt.rcParams.update({
    "figure.autolayout": True})


class ElevatorStateSimulation:

    def __init__(self, max_lowering, max_days):
        self.x_min = 0
        self.x_max = max_days
        self.y_min = 0
        self.y_max = max_lowering

        std = 3

        n = 20
        xs = np.linspace(self.x_min, self.x_max, n)
        ys = np.linspace(self.y_min, self.y_max, n)
        self.vs = np.zeros((n, n))
        for i in range(n):
            # The mean should change from "max lowering" to 0 from day 0 to "max day".
            mean = self.y_max - ys[i]
            v = norm.pdf(ys, mean, std)
            v = np.interp(v, (v.min(), v.max()), (0, 1))
            self.vs[i] = v

        self.xmg, self.ymg = np.meshgrid(xs, ys)

    def plot(self):
        elev = 55
        azim = 100

        fig = plt.figure(figsize=(30, 10))
        ax = fig.add_subplot(1, 1, 1, projection='3d')
        ax.view_init(elev, azim)
        ax.plot_surface(self.xmg, self.ymg, self.vs, cmap=cm.jet)
        ax.set_xlabel('day')
        ax.set_ylabel('elevation')
        ax.set_zlabel('reward')
        ax.set_title("elevator reward")
        plt.show()


if __name__ == "__main__":
    reward = ElevatorStateSimulation(10, 90)
    reward.plot()
