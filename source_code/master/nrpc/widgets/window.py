from PySide2.QtWidgets import QComboBox, QHBoxLayout, QVBoxLayout, QWidget

from widgets.camera import Camera
from widgets.dashboard import Dashboard
from widgets.display import Display


class Window(QWidget):
    def __init__(self):
        super().__init__()

        self.layout = QVBoxLayout()
        self.setLayout(self.layout)

        # Create widgets
        self.visualizations = QWidget()
        self.vis_layout = QHBoxLayout()
        self.camera = Camera()
        self.display = Display(self.camera)
        self.vis_layout.addWidget(self.camera)
        self.vis_layout.addWidget(self.display)
        self.visualizations.setLayout(self.vis_layout)

        self.dashboard = Dashboard()

        self.layout.addWidget(self.visualizations)
        self.layout.addWidget(self.dashboard)

        # Slots and signals.
        self.connect_singals_and_slots()

    def connect_singals_and_slots(self):
        # [Camera] Paint.
        self.dashboard.available_cameras_combobox.currentTextChanged.connect(
            self.camera.setup_camera
        )

        # [Display] Paint.
        self.camera.selected_color_changed.connect(self.display.set_selected_color)

        # [Dashboard] HSV spin boxes.
        self.dashboard.hsv_min_hue_sb.valueChanged.connect(self.display.set_hsv_min_hue)
        self.dashboard.hsv_max_hue_sb.valueChanged.connect(self.display.set_hsv_max_hue)
        self.dashboard.hsv_min_value_sb.valueChanged.connect(
            self.display.set_hsv_min_value
        )
        self.dashboard.hsv_max_value_sb.valueChanged.connect(
            self.display.set_hsv_max_value
        )
        self.dashboard.hsv_min_saturation_sb.valueChanged.connect(
            self.display.set_hsv_min_saturation
        )
        self.dashboard.hsv_max_saturation_sb.valueChanged.connect(
            self.display.set_hsv_max_saturation
        )

        # [Dashboard] Color labels.
        self.camera.current_color_changed.connect(self.dashboard.set_current_color)
        self.camera.selected_color_changed.connect(self.dashboard.set_selected_color)
        self.camera.selected_color_changed.connect(self.dashboard.update_hsv)
