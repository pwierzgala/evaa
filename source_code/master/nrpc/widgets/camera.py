import cv2 as cv
import imutils
from PySide2.QtCore import QTimer, Signal
from PySide2.QtGui import QImage, QPainter, QPixmap
from PySide2.QtWidgets import QWidget


class Camera(QWidget):
    current_color_changed = Signal(int, int, int)
    selected_color_changed = Signal(int, int, int)

    def __init__(self):
        super().__init__()
        self.setMouseTracking(True)

        self._camera = None
        self.frequency = 1000 // 16
        self.q_timer = QTimer()
        self.q_timer.timeout.connect(self.update)

        self.q_image = None
        self.frame = None

    # -----------------------------------------------------------------------------
    # --- PAINT
    # -----------------------------------------------------------------------------

    def paintEvent(self, event):
        super().paintEvent(event)
        if self._camera is not None:
            painter = QPainter(self)
            _, frame = self._camera.read()

            # Convert BGR to RGB.
            frame = cv.cvtColor(frame, cv.COLOR_BGR2RGB)

            # Scale the frame to match widget's width.
            new_width = self.width() // 4 * 4
            self.frame = imutils.resize(frame, width=new_width)

            # Paint the frame.
            (h, w) = self.frame.shape[:2]
            self.q_image = QImage(self.frame, w, h, QImage.Format_RGB888)
            q_pixmap = QPixmap.fromImage(self.q_image)
            painter.drawPixmap(0, 0, q_pixmap)

    # -----------------------------------------------------------------------------
    # --- MOUSE EVENTS
    # -----------------------------------------------------------------------------

    def mouseMoveEvent(self, event):
        if self.q_image:
            pos = event.localPos().toPoint()
            rgb = self.q_image.pixelColor(pos)
            r, g, b = rgb.red(), rgb.green(), rgb.blue()
            self.current_color_changed.emit(r, g, b)

    def mousePressEvent(self, event):
        if self.q_image:
            pos = event.localPos().toPoint()
            rgb = self.q_image.pixelColor(pos)
            r, g, b = rgb.red(), rgb.green(), rgb.blue()
            self.selected_color_changed.emit(r, g, b)

    # -----------------------------------------------------------------------------
    # --- SLOTS
    # -----------------------------------------------------------------------------

    def setup_camera(self, device_name):
        """
        Setup recording parameters and start recording. Recording parameters depend
        on the camera and should be checked in its manual. This method has
        parameters hardcoded for "HD WEBCAM PRO C922 LOGITECH" which can work in two
        modes (1) higher resolution but lower sampling rate (2) or the opposite.

        Parameters
        ----------
        device_name : str
            Name of a device representing a camera.

        Returns
        -------
        None
        """
        if device_name:
            self._camera = cv.VideoCapture()
            self._camera.open(device_name)
            self._camera.set(cv.CAP_PROP_FRAME_WIDTH, 1280)
            self._camera.set(cv.CAP_PROP_FRAME_HEIGHT, 720)
            self._camera.set(cv.CAP_PROP_FPS, 60)
            # self._camera.set(cv.CAP_PROP_FRAME_WIDTH, 1920)
            # self._camera.set(cv.CAP_PROP_FRAME_HEIGHT, 1080)
            # self._camera.set(cv.CAP_PROP_FPS, 30)
            self.q_timer.start(self.frequency)
        else:
            self._camera = None
            self.q_timer.stop()
