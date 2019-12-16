#
# Copyright (c) 2019 FRC Team 3260
#

from PyQt4 import QtCore, QtGui
import sys
import socket


class JoystickView(QtGui.QWidget):
    def __init__(self, comms_state, joystick_state):
        super(JoystickView, self).__init__()
        self.state = joystick_state
        self.comms_state = comms_state
        self.initUi()

    def initUi(self):
        self.setGeometry(300, 300, 250, 150)

    def keyPressEvent(self, event):
        if event.key() == QtCore.Qt.Key_Up:
            self.state.y = 512
        elif event.key() == QtCore.Qt.Key_Down:
            self.state.y = -512
        elif event.key() == QtCore.Qt.Key_Left:
            self.state.x = -512
        elif event.key() == QtCore.Qt.Key_Right:
            self.state.x = 512
        elif event.key() == QtCore.Qt.Key_W:
            self.state.a = 1
        elif event.key() == QtCore.Qt.Key_S:
            self.state.b = 1
        elif event.key() == QtCore.Qt.Key_Escape:
            print("Joystick: Shutting down")
            self.close()
        event.accept()

    def keyReleaseEvent(self, event):
        if event.key() == QtCore.Qt.Key_Up:
            self.state.y = 0
        elif event.key() == QtCore.Qt.Key_Down:
            self.state.y = 0
        elif event.key() == QtCore.Qt.Key_Left:
            self.state.x = 0
        elif event.key() == QtCore.Qt.Key_Right:
            self.state.x = 0
        elif event.key() == QtCore.Qt.Key_W:
            self.state.a = 0
        elif event.key() == QtCore.Qt.Key_S:
            self.state.b = 0


class JoystickState:
    """
    Represents which buttons are pressed
    """
    def __init__(self):
        self.x = 0  # -511 to 512
        self.y = 0  # -511 to 512
        self.a = 0  # 0 or 1
        self.b = 0  # 0 or 1

    def toJson(self):
        return "{ %05d %05d %05d %05d }" % (self.x, self.y, self.a, self.b)


class CommsState:
    """
    Whether or not joystick is connected to controls
    """
    def __init__(self):
        self.connected = False


class CommsThread(QtCore.QThread):
    def __init__(self, rx_port, tx_ip, tx_port, comms_state, joystick_state):
        QtCore.QThread.__init__(self)
        self.joystick_state = joystick_state
        self.rx_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # UDP
        self.rx_socket.bind(("127.0.0.1", rx_port))
        self.rx_socket.settimeout(0.1)
        self.tx_ip = tx_ip
        self.tx_port = tx_port
        self.tx_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # UDP
        self.rx_msg = str()

    def run(self):
        while True:
            # Send state
            print self.joystick_state.toJson()
            self.tx(self.joystick_state.toJson())

            # Receive heartbeat
            self.rx()

    def tx(self, msg):
        self.tx_socket.sendto(bytes(msg), (self.tx_ip, self.tx_port))

    def rx(self):
        buffer_size = 1024
        try:
            self.rx_msg, addr = self.rx_socket.recvfrom(buffer_size)
        except socket.timeout:
            pass


def main():
    app = QtGui.QApplication(sys.argv)

    # Create state variables
    joystick_state = JoystickState()
    comms_state = CommsState()

    # Launch comms background thread
    comms = CommsThread(2000, "127.0.0.1", 4000, comms_state, joystick_state)
    comms.start()

    # Launch joystick window
    joystick = JoystickView(comms_state, joystick_state)
    joystick.show()
    sys.exit(app.exec_())


if __name__ == '__main__':
    main()
