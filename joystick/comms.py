#
# Copyright (c) 2019 FRC Team 3260
#

from PyQt4.QtCore import *
import sys
import socket


class ControllerState:
    """
    Represents which buttons are pressed
    """
    def __init__(self):
        self.x = ButtonState()  # 0 or 1
        self.y = ButtonState()  # 0 or 1
        self.a = ButtonState()  # 0 or 1
        self.b = ButtonState()  # 0 or 1
        self.dpad_up = ButtonState()  # 0 or 1
        self.dpad_down = ButtonState()  # 0 or 1
        self.dpad_left = ButtonState()  # 0 or 1
        self.dpad_right = ButtonState()  # 0 or 1
        self.bumper_right = ButtonState()  # 0 or 1
        self.bumper_left = ButtonState()  # 0 or 1
        self.left_joystick = JoystickState()  # -511 to 512
        self.right_joystick = JoystickState()  # -511 to 512

    def toJson(self):
        return "{ %05d %05d %05d %05d %01d%01d%01d%01d %01d%01d%01d%01d }" % \
            (self.left_joystick.x, self.left_joystick.y, self.right_joystick.x, self.right_joystick.y,
                self.a.pressed, self.b.pressed, self.x.pressed, self.y.pressed,
                self.dpad_up.pressed, self.dpad_down.pressed, self.dpad_left.pressed, self.dpad_right.pressed)


class JoystickState:
    def __init__(self):
        """
        X and y displacement which vary from -511 to 512
        """
        self.x = 0
        self.y = 0


class ButtonState:
    def __init__(self):
        """
        Only one field which is 0 for unpressed and 1 for pressed
        """
        self.pressed = 0


class CommsState:
    """
    Whether or not controller is connected
    """
    def __init__(self):
        self.connected = False


class CommsThread(QThread):
    def __init__(self, rx_port, tx_ip, tx_port, comms_state, controller_state):
        QThread.__init__(self)
        self.controller_state = controller_state
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
            state = self.controller_state.toJson()
            # print state
            self.tx(state)

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
