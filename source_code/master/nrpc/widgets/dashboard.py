from PySide2.QtCore import Qt
from PySide2.QtGui import QColor, QPalette
from PySide2.QtMultimedia import QCameraInfo
from PySide2.QtWidgets import QComboBox, QGridLayout, QLabel, QSlider, QSpinBox, QWidget

from settings import (
    DEFAULT_HSV_MAX_HUE,
    DEFAULT_HSV_MAX_SATURATION,
    DEFAULT_HSV_MAX_VALUE,
    DEFAULT_HSV_MIN_HUE,
    DEFAULT_HSV_MIN_SATURATION,
    DEFAULT_HSV_MIN_VALUE
)
from utils import hsv_to_rgb, rgb_to_hsv_range


class Dashboard(QWidget):
    def __init__(self):
        super().__init__()
        self.layout = QGridLayout()
        self.setMaximumHeight(300)

        # View
        available_cameras = self.get_available_cameras()
        self.available_cameras_combobox = QComboBox()
        self.available_cameras_combobox.addItems(available_cameras)
        self.layout.addWidget(QLabel("Camera:"), 1, 0, Qt.AlignRight)
        self.layout.addWidget(self.available_cameras_combobox, 1, 1)

        # HSV color
        self.hsv_min_hue_sb = QSpinBox(
            minimum=0, maximum=255, value=DEFAULT_HSV_MIN_HUE
        )
        self.hsv_max_hue_sb = QSpinBox(
            minimum=0, maximum=255, value=DEFAULT_HSV_MAX_HUE
        )
        self.hsv_min_value_sb = QSpinBox(
            minimum=0, maximum=255, value=DEFAULT_HSV_MIN_VALUE
        )
        self.hsv_max_value_sb = QSpinBox(
            minimum=0, maximum=255, value=DEFAULT_HSV_MAX_VALUE
        )
        self.hsv_min_saturation_sb = QSpinBox(
            minimum=0, maximum=255, value=DEFAULT_HSV_MIN_SATURATION
        )
        self.hsv_max_saturation_sb = QSpinBox(
            minimum=0, maximum=255, value=DEFAULT_HSV_MAX_SATURATION
        )

        self.hsv_min_hue_s = QSlider(
            minimum=0, maximum=255, value=DEFAULT_HSV_MIN_HUE, orientation=Qt.Horizontal
        )
        self.hsv_max_hue_s = QSlider(
            minimum=0, maximum=255, value=DEFAULT_HSV_MAX_HUE, orientation=Qt.Horizontal
        )
        self.hsv_min_value_s = QSlider(
            minimum=0,
            maximum=255,
            value=DEFAULT_HSV_MIN_VALUE,
            orientation=Qt.Horizontal,
        )
        self.hsv_max_value_s = QSlider(
            minimum=0,
            maximum=255,
            value=DEFAULT_HSV_MAX_VALUE,
            orientation=Qt.Horizontal,
        )
        self.hsv_min_saturation_s = QSlider(
            minimum=0,
            maximum=255,
            value=DEFAULT_HSV_MIN_SATURATION,
            orientation=Qt.Horizontal,
        )
        self.hsv_max_saturation_s = QSlider(
            minimum=0,
            maximum=255,
            value=DEFAULT_HSV_MAX_SATURATION,
            orientation=Qt.Horizontal,
        )

        max_width = 300
        self.hsv_min_hue_sb.setMaximumWidth(max_width)
        self.hsv_max_hue_sb.setMaximumWidth(max_width)
        self.hsv_min_value_sb.setMaximumWidth(max_width)
        self.hsv_max_value_sb.setMaximumWidth(max_width)
        self.hsv_min_saturation_sb.setMaximumWidth(max_width)
        self.hsv_max_saturation_sb.setMaximumWidth(max_width)
        self.hsv_min_hue_s.setMaximumWidth(max_width)
        self.hsv_max_hue_s.setMaximumWidth(max_width)
        self.hsv_min_value_s.setMaximumWidth(max_width)
        self.hsv_max_value_s.setMaximumWidth(max_width)
        self.hsv_min_saturation_s.setMaximumWidth(max_width)
        self.hsv_max_saturation_s.setMaximumWidth(max_width)

        self.layout.addWidget(QLabel("Min"), 0, 3, Qt.AlignBottom | Qt.AlignHCenter)
        self.layout.addWidget(QLabel("Max"), 0, 4, Qt.AlignBottom | Qt.AlignHCenter)

        self.layout.addWidget(QLabel("Hue:"), 1, 2, Qt.AlignRight)
        self.layout.addWidget(self.hsv_min_hue_sb, 1, 3)
        self.layout.addWidget(self.hsv_max_hue_sb, 1, 4)
        self.layout.addWidget(self.hsv_min_hue_s, 2, 3)
        self.layout.addWidget(self.hsv_max_hue_s, 2, 4)

        self.layout.addWidget(QLabel("Value:"), 3, 2, Qt.AlignRight)
        self.layout.addWidget(self.hsv_min_value_sb, 3, 3)
        self.layout.addWidget(self.hsv_max_value_sb, 3, 4)
        self.layout.addWidget(self.hsv_min_value_s, 4, 3)
        self.layout.addWidget(self.hsv_max_value_s, 4, 4)

        self.layout.addWidget(QLabel("Saturation:"), 5, 2, Qt.AlignRight)
        self.layout.addWidget(self.hsv_min_saturation_sb, 5, 3)
        self.layout.addWidget(self.hsv_max_saturation_sb, 5, 4)
        self.layout.addWidget(self.hsv_min_saturation_s, 6, 3)
        self.layout.addWidget(self.hsv_max_saturation_s, 6, 4)

        # Color selection
        self.current_color = QLabel()
        self.current_color.setFixedSize(50, 50)
        self.current_color.setAutoFillBackground(True)
        self.layout.addWidget(QLabel("Current color:"), 1, 5, Qt.AlignRight)
        self.layout.addWidget(self.current_color, 1, 6)

        self.selected_color = QLabel()
        self.selected_color.setFixedSize(50, 50)
        self.selected_color.setAutoFillBackground(True)
        self.layout.addWidget(QLabel("Base filtered color:"), 2, 5, Qt.AlignRight)
        self.layout.addWidget(self.selected_color, 2, 6)

        self.lower_filtered_color = QLabel()
        self.lower_filtered_color.setFixedSize(50, 50)
        self.lower_filtered_color.setAutoFillBackground(True)
        self.layout.addWidget(QLabel("Lower filtered color:"), 3, 5, Qt.AlignRight)
        self.layout.addWidget(self.lower_filtered_color, 3, 6)

        self.upper_filtered_color = QLabel()
        self.upper_filtered_color.setFixedSize(50, 50)
        self.upper_filtered_color.setAutoFillBackground(True)
        self.layout.addWidget(QLabel("Upper filtered color:"), 4, 5, Qt.AlignRight)
        self.layout.addWidget(self.upper_filtered_color, 4, 6)

        self.setLayout(self.layout)

        self.hsv_min_hue_sb.valueChanged.connect(self.update_hsv)
        self.hsv_min_hue_sb.valueChanged.connect(self.hsv_min_hue_s.setValue)
        self.hsv_min_hue_s.valueChanged.connect(self.hsv_min_hue_sb.setValue)

        self.hsv_max_hue_sb.valueChanged.connect(self.update_hsv)
        self.hsv_max_hue_sb.valueChanged.connect(self.hsv_max_hue_s.setValue)
        self.hsv_max_hue_s.valueChanged.connect(self.hsv_max_hue_sb.setValue)

        self.hsv_min_value_sb.valueChanged.connect(self.update_hsv)
        self.hsv_min_value_sb.valueChanged.connect(self.hsv_min_value_s.setValue)
        self.hsv_min_value_s.valueChanged.connect(self.hsv_min_value_sb.setValue)

        self.hsv_max_value_sb.valueChanged.connect(self.update_hsv)
        self.hsv_max_value_sb.valueChanged.connect(self.hsv_max_value_s.setValue)
        self.hsv_max_value_s.valueChanged.connect(self.hsv_max_value_sb.setValue)

        self.hsv_min_saturation_sb.valueChanged.connect(self.update_hsv)
        self.hsv_min_saturation_sb.valueChanged.connect(
            self.hsv_min_saturation_s.setValue
        )
        self.hsv_min_saturation_s.valueChanged.connect(
            self.hsv_min_saturation_sb.setValue
        )

        self.hsv_max_saturation_sb.valueChanged.connect(self.update_hsv)
        self.hsv_max_saturation_sb.valueChanged.connect(
            self.hsv_max_saturation_s.setValue
        )
        self.hsv_max_saturation_s.valueChanged.connect(
            self.hsv_max_saturation_sb.setValue
        )

    def get_available_cameras(self):
        available_cameras = [""]
        q_camera_info = QCameraInfo()
        for available_camera in q_camera_info.availableCameras():
            device_name = available_camera.deviceName()
            available_cameras.append(device_name)
        return available_cameras

    # -----------------------------------------------------------------------------
    # --- SLOTS
    # -----------------------------------------------------------------------------

    def set_current_color(self, r, g, b):
        palette = QPalette()
        palette.setColor(QPalette.Background, QColor(r, g, b))
        self.current_color.setPalette(palette)

    def set_selected_color(self, r, g, b):
        palette = QPalette()
        palette.setColor(QPalette.Background, QColor(r, g, b))
        self.selected_color.setPalette(palette)

    def update_hsv(self, _):
        color = self.selected_color.palette().color(QPalette.Background)
        r, g, b = color.red(), color.green(), color.blue()
        lower_hsv, upper_hsv = rgb_to_hsv_range(
            r,
            g,
            b,
            min_h=self.hsv_min_hue_sb.value(),
            max_h=self.hsv_max_hue_sb.value(),
            min_s=self.hsv_min_saturation_sb.value(),
            max_s=self.hsv_max_saturation_sb.value(),
            min_v=self.hsv_min_value_sb.value(),
            max_v=self.hsv_max_value_sb.value(),
        )

        r, g, b = hsv_to_rgb(*lower_hsv)
        palette = QPalette()
        palette.setColor(QPalette.Background, QColor(r, g, b))
        self.lower_filtered_color.setPalette(palette)

        r, g, b = hsv_to_rgb(*upper_hsv)
        palette = QPalette()
        palette.setColor(QPalette.Background, QColor(r, g, b))
        self.upper_filtered_color.setPalette(palette)
