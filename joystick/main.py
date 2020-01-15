#
# Copyright (c) 2019 FRC Team 3260
#

from controller import *
from comms import *
from connected import ConnectedWidget
from help import HelpWidget
import yaml

CONFIG_FILE = "../config/robotConfig.yml"


class MainWindow(QMainWindow):

    def __init__(self, comms_config, parent=None):
        super(MainWindow, self).__init__(parent)
        self.setWindowTitle("Xbox Controller")
        self.setStyleSheet("background-color: gray")
        # self.setFixedSize(640, 480)
        self.move(1280, 0)

        # Create state variables for each thread so they can pass data
        self.controller_state = ControllerState()

        # Launch comms thread in background

        self.comms = CommsThread(comms_config, self.controller_state)
        self.comms.start()

        # Init UI
        self.connected = ConnectedWidget(parent=self)
        self.comms.connection_status.connect(self.connected.show_connection_status)
        self.controller = ControllerWidget(self.controller_state, parent=self)
        self.help = HelpWidget(comms_config, parent=self.controller)
        self.setCentralWidget(self.controller)
        self.adjustSize()

        self.help_text = QLabel(self)
        self.help_text.setAlignment(Qt.AlignCenter)
        self.help_text.setText("Hold H for help")
        self.help_text.adjustSize()
        self.help_text.move(self.width()/2 - self.help_text.width()/2, 500)
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
        if event.key() == Qt.Key_H:
            self.help.show()
        else:
            self.controller.keyPressEvent(event)
        event.accept()

    def keyReleaseEvent(self, event):
        if event.key() == Qt.Key_H:
            self.help.hide()
        else:
            self.controller.keyReleaseEvent(event)
        event.accept()


def main():
    with open(CONFIG_FILE) as f:
        data = yaml.load(f)
        comms_config = {
            "rx_ip": "127.0.0.1",
            "rx_port": data['joystick']['port'],
            "tx_ip": data['core']['ip'],
            "tx_port": data['core']['joystickPort']
        }

    app = QApplication(sys.argv)

    # Launch main window
    window = MainWindow(comms_config)
    window.show()

    sys.exit(app.exec_())


if __name__ == '__main__':
    main()
