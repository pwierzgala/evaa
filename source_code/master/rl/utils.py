from matplotlib.collections import LineCollection
import numpy as np
import matplotlib.pyplot as plt


def make_segments(x, y):
    """
    Create list of line segments from x and y coordinates, in the correct format for
    LineCollection:
    an array of the form   numlines x (points per line) x 2 (x and y) array
    """

    points = np.array([x, y]).T.reshape(-1, 1, 2)
    segments = np.concatenate([points[:-1], points[1:]], axis=1)

    return segments


def colorline(x, y, z=None, cmap=plt.get_cmap('hsv'), norm=plt.Normalize(0.0, 1.0),
              linewidth=3, alpha=1.0):
    """
    Plot a colored line with coordinates x and y
    Optionally specify colors in the array z
    Optionally specify a colormap, a norm function and a line width
    """

    # Default colors equally spaced on [0,1]:
    if z is None:
        z = np.linspace(0.0, 1.0, len(x))

    # Special case if a single number:
    if not hasattr(z, "__iter__"):  # to check for numerical input -- this is a hack
        z = np.array([z])

    z = np.asarray(z)

    segments = make_segments(x, y)
    lc = LineCollection(segments, array=z, cmap=cmap, norm=norm, linewidth=linewidth,
                        alpha=alpha)

    ax = plt.gca()
    ax.add_collection(lc)

    return lc
