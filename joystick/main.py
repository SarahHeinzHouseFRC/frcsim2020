#
# Copyright (c) 2019 FRC Team 3260
#

from controller import *
from comms import *
from connected import ConnectedWidget


class MainWindow(QMainWindow):

    def __init__(self, parent=None):
        super(MainWindow, self).__init__(parent)
        self.setWindowTitle("Xbox Controller")
        self.setStyleSheet("background-color: gray")
        # self.setFixedSize(640, 480)

        # Create state variables for each thread so they can pass data
        self.controller_state = ControllerState()
        self.comms_state = CommsState()

        # Launch comms thread in background
        self.comms = CommsThread(2000, "127.0.0.1", 4000, self.comms_state, self.controller_state)
        self.comms.start()

        # Init UI
        self.connected = ConnectedWidget(parent=self)
        self.comms.connection_status.connect(self.connected.show_connection_status)
        self.controller = ControllerWidget(self.controller_state, self.comms_state, parent=self)
        self.setCentralWidget(self.controller)

        print self.geometry()

    def keyPressEvent(self, event):
        """
        Close application on Esc key
        """
        if event.key() == Qt.Key_Escape:
            print("Controller: Shutting down")
            self.close()
        event.accept()


def main():
    app = QApplication(sys.argv)

    # Launch main window
    window = MainWindow()
    window.show()

    sys.exit(app.exec_())


if __name__ == '__main__':
    main()
