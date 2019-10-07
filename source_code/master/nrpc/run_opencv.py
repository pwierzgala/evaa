import time

import cv2
import numpy as np

from settings import (
    DEFAULT_CAM_FPS,
    DEFAULT_HSV_MAX_HUE,
    DEFAULT_HSV_MAX_SATURATION,
    DEFAULT_HSV_MAX_VALUE,
    DEFAULT_HSV_MIN_HUE,
    DEFAULT_HSV_MIN_SATURATION,
    DEFAULT_HSV_MIN_VALUE
)
from utils import apply_mask, calculate_nrpc, draw_text


def measure_fps(f):
    """
    Decorates rendering function with frames per second calculation.

    Parameters
    ----------
    f : function
        Rendering function.

    Returns
    -------
        Decorated rendering function.
    """

    def f_():
        global fps
        start = time.time()
        img = f()
        end = time.time()
        num_frames = 1
        seconds = end - start
        fps = num_frames / seconds
        return img

    return f_


@measure_fps
def render():
    """
    Renders a single frame.

    Returns
    -------
    ndarray
        Array representing a frame in BGR color format.
    """
    global masked_frame, image

    ret, frame = camera.read()
    frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    mask, masked_frame = apply_mask(
        frame=frame,
        r=selected_color[0][0][2],
        g=selected_color[0][0][1],
        b=selected_color[0][0][0],
        negdh=cv2.getTrackbarPos(hue_delta_minus_name, window_name),
        posdh=cv2.getTrackbarPos(hue_delta_plus_name, window_name),
        mins=cv2.getTrackbarPos(min_saturation_name, window_name),
        maxs=cv2.getTrackbarPos(max_saturation_name, window_name),
        minv=cv2.getTrackbarPos(min_value_name, window_name),
        maxv=cv2.getTrackbarPos(max_value_name, window_name),
    )

    # Calculate and draw metric value.
    nrpc = calculate_nrpc(mask)
    time_str = time.strftime("%H:%M:%S", time.localtime())
    text = "{}, FPS: {:05.2f}, NRPC: {:.4f}".format(time_str, fps, nrpc)
    draw_text(img=frame, text=text)

    # Change color format back from RGB to BGR.
    frame = cv2.cvtColor(frame, cv2.COLOR_RGB2BGR)
    masked_frame = cv2.cvtColor(masked_frame, cv2.COLOR_RGB2BGR)

    # Concatenate horizontally (1) frames and (2) colors. Then, concatenate
    # vertically (1) and (2) vertically.
    frames = np.concatenate((frame, masked_frame), axis=1)
    colors = np.concatenate(
        (current_color, selected_color, lower_filtered_color, upper_filtered_color),
        axis=1,
    )
    image = np.concatenate((frames, colors), axis=0)

    return image


def slider_callback(x):
    """
    Slider callback is required by its constructor but in this code sliders are
    global objects and their value is read directly from them.

    Parameters
    ----------
    x : int
        Slider value
    Returns
    -------
    None
    """
    pass


def mouse_rgb(event, x, y, flag, param):
    """
    Handles MOUSEMOVE and LBUTTONDOWN events. MOUSEMOVE updates value of current
    color. LBUTTONDOWN updates value of selected color.

    Parameters
    ----------
    event : int
        Type of mouse event.
    x : int
        X coordinate of the mouse cursor.
    y : int
        Y coordinate of the mouse cursor.
    Returns
    -------
    None
    """
    b = image[y, x, 0]
    g = image[y, x, 1]
    r = image[y, x, 2]
    if event == cv2.EVENT_MOUSEMOVE:
        current_color[:] = [b, g, r]
    elif event == cv2.EVENT_LBUTTONDOWN:
        selected_color[:] = [b, g, r]


# Window
window_name = "image"
cv2.namedWindow(window_name)
cv2.setMouseCallback(window_name, mouse_rgb)

# Camera
camera = cv2.VideoCapture(0)
camera.set(cv2.CAP_PROP_FPS, DEFAULT_CAM_FPS)
cap_width = int(camera.get(3))
cap_height = int(camera.get(4))
frames_height = cap_height
frames_width = cap_width * 2
masked_frame = np.zeros((cap_height, cap_width, 3), np.uint8)

# Sliders
hue_delta_minus_name = "Hue Delta -"
cv2.createTrackbar(
    hue_delta_minus_name, window_name, DEFAULT_HSV_MIN_HUE, 255, slider_callback
)

hue_delta_plus_name = "Hue Delta +"
cv2.createTrackbar(
    hue_delta_plus_name, window_name, DEFAULT_HSV_MAX_HUE, 255, slider_callback
)

min_value_name = "Min Value"
cv2.createTrackbar(
    min_value_name, window_name, DEFAULT_HSV_MIN_VALUE, 255, slider_callback
)

max_value_name = "Max Value"
cv2.createTrackbar(
    max_value_name, window_name, DEFAULT_HSV_MAX_VALUE, 255, slider_callback
)

min_saturation_name = "Min Saturation"
cv2.createTrackbar(
    min_saturation_name, window_name, DEFAULT_HSV_MIN_SATURATION, 255, slider_callback
)

max_saturation_name = "Max Saturation"
cv2.createTrackbar(
    max_saturation_name, window_name, DEFAULT_HSV_MAX_SATURATION, 255, slider_callback
)

# Colors
colors_height = 30
colors_width = cap_width * 2
color_width = colors_width // 4
current_color = np.zeros((30, color_width, 3), np.uint8)
selected_color = np.zeros((30, color_width, 3), np.uint8)
lower_filtered_color = np.zeros((30, color_width, 3), np.uint8)
upper_filtered_color = np.zeros((30, color_width, 3), np.uint8)

fps = 0
while True:
    image = render()
    cv2.imshow(window_name, image)
    if cv2.waitKey(1) & 0xFF == ord("q"):
        break

# When everything done, release the capture.
camera.release()
cv2.destroyAllWindows()
