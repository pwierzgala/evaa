from PySide2.QtCore import QTimer
from PySide2.QtGui import QImage, QPainter, QPixmap
from PySide2.QtWidgets import QWidget

from utils import apply_mask
from settings import (
    DEFAULT_HSV_MAX_HUE,
    DEFAULT_HSV_MAX_SATURATION,
    DEFAULT_HSV_MAX_VALUE,
    DEFAULT_HSV_MIN_HUE,
    DEFAULT_HSV_MIN_SATURATION,
    DEFAULT_HSV_MIN_VALUE
)


class Display(QWidget):
    def __init__(self, camera):
        super().__init__()
        self.camera = camera

        self.selected_color_rgb = (None, None, None)

        self.frequency = 1000 // 16
        self.q_timer = QTimer()
        self.q_timer.timeout.connect(self.update)
        self.q_timer.start(self.frequency)

        self.hsv_min_hue = DEFAULT_HSV_MIN_HUE
        self.hsv_max_hue = DEFAULT_HSV_MAX_HUE
        self.hsv_min_value = DEFAULT_HSV_MIN_VALUE
        self.hsv_max_value = DEFAULT_HSV_MAX_VALUE
        self.hsv_min_saturation = DEFAULT_HSV_MIN_SATURATION
        self.hsv_max_saturation = DEFAULT_HSV_MAX_SATURATION

    # -----------------------------------------------------------------------------
    # --- PAINT
    # -----------------------------------------------------------------------------

    def paintEvent(self, event):
        super().paintEvent(event)
        frame = self.camera.frame
        if frame is not None:
            painter = QPainter(self)

            (h, w) = frame.shape[:2]

            if self.selected_color_rgb[0] is not None:
                _, frame = apply_mask(
                    frame=frame,
                    r=self.selected_color_rgb[0],
                    g=self.selected_color_rgb[1],
                    b=self.selected_color_rgb[2],
                    negdh=self.hsv_min_hue,
                    posdh=self.hsv_max_hue,
                    mins=self.hsv_min_saturation,
                    maxs=self.hsv_max_saturation,
                    minv=self.hsv_min_value,
                    maxv=self.hsv_max_value,
                )

            q_image = QImage(frame, w, h, QImage.Format_RGB888)
            q_pixmap = QPixmap.fromImage(q_image)
            painter.drawPixmap(0, 0, q_pixmap)

    # -----------------------------------------------------------------------------
    # --- SLOTS
    # -----------------------------------------------------------------------------

    def set_selected_color(self, r, g, b):
        self.selected_color_rgb = (r, g, b)

    def set_hsv_min_hue(self, value):
        self.hsv_min_hue = value

    def set_hsv_max_hue(self, value):
        self.hsv_max_hue = value

    def set_hsv_min_value(self, value):
        self.hsv_min_value = value

    def set_hsv_max_value(self, value):
        self.hsv_max_value = value

    def set_hsv_min_saturation(self, value):
        self.hsv_min_saturation = value

    def set_hsv_max_saturation(self, value):
        self.hsv_max_saturation = value
