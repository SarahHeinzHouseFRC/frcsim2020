#
# Copyright (c) 2019 FRC Team 3260
#

from PyQt4.QtCore import *
from PyQt4.QtGui import *
from PyQt4.QtSvg import *


class InfoWidget(QWidget):
    def __init__(self, comms_config, parent=None):
        super(InfoWidget, self).__init__(parent)

        # Comms label
        self.comms_label = QLabel(self)
        self.comms_label.setAlignment(Qt.AlignCenter)
        self.comms_label.setStyleSheet("background-color: rgba(0,0,0,0); color: white")
        self.comms_label.setText("Transmitting from {}:{} \nto {}:{}".format(comms_config['rx_ip'],
                                                                             comms_config['rx_port'],
                                                                             comms_config['tx_ip'],
                                                                             comms_config['tx_port']))
        self.comms_label.adjustSize()
        self.comms_label.move(687/2-self.comms_label.width()/2, 400)
        self.hide()
