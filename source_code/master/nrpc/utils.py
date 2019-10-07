import cv2
import numpy as np


def rgb_to_hsv_range(r, g, b, min_h, max_h, min_s, max_s, min_v, max_v):
    color_rgb = np.uint8([[[r, g, b]]])
    hue, _, _ = cv2.cvtColor(color_rgb, cv2.COLOR_RGB2HSV)[0][0]
    lower_hsv = np.array([hue - min_h, min_s, min_v])
    upper_hsv = np.array([hue + max_h, max_s, max_v])
    return lower_hsv, upper_hsv


def hsv_to_rgb(h, s, v):
    """
    Transforms color described with hue, saturation and value components to
    description with red, green and blue components.
    """
    color_hsv = np.uint8([[[h, s, v]]])
    r, g, b = cv2.cvtColor(color_hsv, cv2.COLOR_HSV2RGB)[0][0]
    return r, g, b


def apply_mask(frame, r, g, b, negdh, posdh, mins, maxs, minv, maxv):
    """
    Parameters
    ----------
    frame : ndarray
        Input image with color in RGB format.
    r : int (0-255)
        Red component of the base color.
    g : int (0-255)
        Green component of the base color.
    b : int (0-255)
        Blue component of the base color.
    negdh : int (0-255)
        Value removed from hue component of the base color in the HSV format to form
        lower boundary of filtered colors.
    posdh : int (0-255)
        Value added to hue component of base the color in the HSV format to form upper
        boundary of filtered colors.
    mins : int (0-255)
        Value of the saturation component of the base color in the HSV format used to
        form lower boundary of filtered colors.
    maxs : int (0-255)
        Value of saturation component of the base color in the HSV format used to form
        upper boundary of filtered colors.
    minv : int (0-255)
        Value of the value component of the base color in the HSV format used to form
        lower boundary of filtered colors.
    maxv : int (0-255)
        Value of the value component of the base color in the HSV format used to form
        upper boundary of filtered colors.
    Returns
    -------
    ndarray
        Mask applied to the frame.
    ndarray
        Masked frame.
    """
    frame_hsv = cv2.cvtColor(frame, cv2.COLOR_RGB2HSV)
    lower_hsv, upper_hsv = rgb_to_hsv_range(
        r=r,
        g=g,
        b=b,
        min_h=negdh,
        max_h=posdh,
        min_s=mins,
        max_s=maxs,
        min_v=minv,
        max_v=maxv,
    )
    mask = cv2.inRange(frame_hsv, lower_hsv, upper_hsv)
    image = cv2.bitwise_and(frame, frame, mask=mask)
    return mask, image


def calculate_nrpc(mask):
    """
    Calculate Normalized Relevant Pixels Count metric. The metric express how much
    mask covers its space. It's normalized to 0-1 range, where 0 means that the mask is
    empty and 1 that the mask covers whole image.

    Parameters
    ----------
    mask : ndarray
        Array representing image mask.
    Returns
    -------
    int
        Number of relevant pixels normalized by number of all pixels.
    """
    mask_pixels = np.sum(mask > 1)
    all_pixels = mask.shape[0] * mask.shape[1]
    return mask_pixels / all_pixels


def draw_text(img, text):
    """
    Adds black text on the white background at the upper left corner of the image.
    """
    background_color = (255, 255, 255)
    text_offset_x = 0
    text_offset_y = 0
    text_padding_x = 5
    text_padding_y = 5

    font_face = cv2.FONT_HERSHEY_SIMPLEX
    font_scale = 0.7
    font_thickness = 1
    font_color = (0, 0, 0)
    font_line_type = cv2.LINE_AA

    (text_width, text_height) = cv2.getTextSize(
        text=text, fontFace=font_face, fontScale=font_scale, thickness=1
    )[0]

    text_coords = (
        text_offset_x + text_padding_x,
        text_offset_y + text_padding_y + text_height,
    )
    background_coords = (
        (text_offset_x, text_offset_y),
        (
            text_offset_x + text_width + 2 * text_padding_x,
            text_offset_y + text_height + 2 * text_padding_y,
        ),
    )

    cv2.rectangle(
        img=img,
        pt1=background_coords[0],
        pt2=background_coords[1],
        color=background_color,
        thickness=cv2.FILLED,
    )

    cv2.putText(
        img=img,
        text=text,
        org=text_coords,
        fontFace=font_face,
        fontScale=font_scale,
        color=font_color,
        thickness=font_thickness,
        lineType=font_line_type,
        bottomLeftOrigin=False,
    )
