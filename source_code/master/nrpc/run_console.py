import time

import cv2

import click
from settings import (
    DEFAULT_HSV_MAX_HUE,
    DEFAULT_HSV_MAX_SATURATION,
    DEFAULT_HSV_MAX_VALUE,
    DEFAULT_HSV_MIN_HUE,
    DEFAULT_HSV_MIN_SATURATION,
    DEFAULT_HSV_MIN_VALUE
)
from utils import apply_mask, calculate_nrpc, draw_text


@click.command()
@click.option(
    "--r", required=True, type=int, help="RGB 'red' component of filtered color"
)
@click.option(
    "--g", required=True, type=int, help="RGB 'green' component of filtered color"
)
@click.option(
    "--b", required=True, type=int, help="RGB 'blue' component of filtered color"
)
@click.option(
    "--negdh",
    required=True,
    type=int,
    default=DEFAULT_HSV_MIN_HUE,
    help="Negative delta of HSV 'hue' component",
)
@click.option(
    "--posdh",
    required=True,
    type=int,
    default=DEFAULT_HSV_MAX_HUE,
    help="Positive delta of HSV 'hue' component",
)
@click.option(
    "--minv",
    required=True,
    type=int,
    default=DEFAULT_HSV_MIN_VALUE,
    help="Min value of HSV 'value' component",
)
@click.option(
    "--maxv",
    required=True,
    type=int,
    default=DEFAULT_HSV_MAX_VALUE,
    help="Max value of HSV 'value' component",
)
@click.option(
    "--mins",
    required=True,
    type=int,
    default=DEFAULT_HSV_MIN_SATURATION,
    help="Min value of HSV 'saturation' component",
)
@click.option(
    "--maxs",
    required=True,
    type=int,
    default=DEFAULT_HSV_MAX_SATURATION,
    help="Max value of HSV 'saturation' component",
)
@click.option(
    "--input_path",
    type=click.Path(exists=True),
    default=None,
    help="Path to input image. If not specified a frame from camera will be used.",
)
@click.option(
    "--output_path",
    type=click.Path(exists=False),
    default=None,
    help="Path to output image. If not specified output won't be saved.",
)
def main(r, g, b, negdh, posdh, mins, maxs, minv, maxv, input_path, output_path):
    if input_path is None:
        camera = cv2.VideoCapture(0)
        camera.set(
            cv2.CAP_PROP_FRAME_WIDTH, 800
        )  # Higer value will cause error on FPGA
        camera.set(
            cv2.CAP_PROP_FRAME_HEIGHT, 600
        )  # Higer value will cause error on FPGA
        camera.set(cv2.CAP_PROP_FPS, 5)
        _, frame = camera.read()
    else:
        frame = cv2.imread(input_path)

    frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    mask, frame = apply_mask(frame, r, g, b, negdh, posdh, mins, maxs, minv, maxv)
    nrpc = calculate_nrpc(mask)

    if output_path is not None:
        time_str = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
        text = "{}, NRPC: {:.4f}".format(time_str, nrpc)
        draw_text(img=frame, text=text)
        cv2.imwrite(output_path, frame)

    return nrpc


if __name__ == "__main__":
    main()
