#
# Copyright (c) 2019 FRC Team 3260
#

from PyQt4.QtCore import *
from PyQt4.QtGui import *


class ConnectedWidget(QLabel):
    def __init__(self, parent):
        super(ConnectedWidget, self).__init__(parent)
        self.setAlignment(Qt.AlignCenter)
        self.setAutoFillBackground(True)
        self.show_disconnected()
        self.adjustSize()
        self.move(687 / 2 - self.width() / 2, 10)

    def show_connection_status(self, is_connected):
        if is_connected:
            self.show_connected()
        else:
            self.show_disconnected()

    def show_connected(self):
        self.setText("Connected")
        self.setStyleSheet("background-color: rgb(44, 160, 44); color: white; padding: 5px")

    def show_disconnected(self):
        self.setText("Disconnected")
        self.setStyleSheet("background-color: rgb(214, 39, 40); color: white; padding: 5px")
