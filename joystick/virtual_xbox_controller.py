#
# Copyright (c) 2020 FRC Team 3260
#

from PyQt4.QtCore import *
from PyQt4.QtGui import *
from comms import *
from connected_widget import ConnectedWidget
from controller_widget import ControllerWidget
from hotkeys_widget import HotkeysWidget
from info_widget import InfoWidget


class CommsQThread(QThread):
    connection_status = pyqtSignal(object)

    def __init__(self, comms_config, controller_state, verbose):
        QThread.__init__(self)
        self.comms = Comms(comms_config)
        self.controller_state = controller_state
        self.verbose = verbose

    def run(self):
        while True:
            # Send controller state
            tx_msg = self.controller_state.toJson()
            self.comms.tx(tx_msg)
            if self.verbose:
                print tx_msg

            # Receive heartbeat
            rx_msg = self.comms.rx()
            if rx_msg:
                self.connection_status.emit(True)
            elif rx_msg is None:
                self.connection_status.emit(False)


class VirtualXboxController(QMainWindow):

    def __init__(self, comms_config, verbose, parent=None):
        super(VirtualXboxController, self).__init__(parent)
        self.setWindowTitle("Xbox Controller")
        self.setStyleSheet("background-color: gray")
        # self.setFixedSize(640, 480)
        self.move(1280, 0)
        self.controller_state = ControllerState()

        # Launch comms thread in background
        self.comms = CommsQThread(comms_config, self.controller_state, verbose)
        self.comms.start()

        # Init UI
        self.connected = ConnectedWidget(parent=self)
        self.comms.connection_status.connect(self.connected.show_connection_status)
        self.controller = ControllerWidget(self.controller_state, parent=self)
        self.hotkeys = HotkeysWidget(parent=self.controller)
        self.info = InfoWidget(comms_config, parent=self.controller)
        self.setCentralWidget(self.controller)
        self.adjustSize()

        self.help_text = QLabel(self)
        self.help_text.setAlignment(Qt.AlignCenter)
        self.help_text.setText("Hold H for hotkeys\nHold I for info\nPress Esc to quit")
        self.help_text.adjustSize()
        self.help_text.move(self.width()/2 - self.help_text.width()/2, 475)
        self.help_text.setStyleSheet("background-color: rgba(0,0,0,0); color: white")
        print self.geometry()

        print "Controller: Launched"

    def keyPressEvent(self, event):
        """
        Close application on Esc key
        """
        if event.key() == Qt.Key_Escape:
            print("Controller: Shutting down")
            self.close()
        elif event.key() == Qt.Key_H:
            self.hotkeys.show()
        elif event.key() == Qt.Key_I:
            self.info.show()
        else:
            self.controller.keyPressEvent(event)
        event.accept()

    def keyReleaseEvent(self, event):
        if event.key() == Qt.Key_H:
            self.hotkeys.hide()
        elif event.key() == Qt.Key_I:
            self.info.hide()
        else:
            self.controller.keyReleaseEvent(event)
        event.accept()
