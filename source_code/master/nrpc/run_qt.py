import sys

from PySide2.QtWidgets import QApplication

from widgets.window import Window

if __name__ == "__main__":
    app = QApplication(sys.argv)
    widget = Window()
    widget.resize(800, 600)
    widget.show()
    sys.exit(app.exec_())
