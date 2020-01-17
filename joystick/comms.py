#
# Copyright (c) 2019 FRC Team 3260
#

from PyQt4.QtCore import *
import sys
import socket
import time


class Comms:
    def __init__(self, comms_config):
        self.rx_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # UDP
        self.rx_socket.bind((comms_config['rx_ip'], comms_config['rx_port']))
        self.rx_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.rx_socket.settimeout(0.1)
        self.tx_ip = comms_config['tx_ip']
        self.tx_port = comms_config['tx_port']
        self.tx_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # UDP

    def tx(self, msg):
        time.sleep(0.01)
        self.tx_socket.sendto(bytes(msg), (self.tx_ip, self.tx_port))

    def rx(self):
        buffer_size = 1024
        try:
            rx_msg, addr = self.rx_socket.recvfrom(buffer_size)
            return rx_msg
        except socket.timeout:
            return None


class ControllerState:
    """
    Represents which buttons are pressed
    """
    def __init__(self):
        self.x = ButtonState()  # 0 or 1
        self.y = ButtonState()  # 0 or 1
        self.a = ButtonState()  # 0 or 1
        self.b = ButtonState()  # 0 or 1
        self.dpad = DpadState()  # 4x 0 or 1
        self.bumper_right = ButtonState()  # 0 or 1
        self.bumper_left = ButtonState()  # 0 or 1
        self.left_joystick = JoystickState()  # -511 to 512
        self.right_joystick = JoystickState()  # -511 to 512

    def toJson(self):
        return "{ %05d %05d %05d %05d %01d%01d%01d%01d %01d%01d%01d%01d }" % \
            (self.left_joystick.x, self.left_joystick.y, self.right_joystick.x, self.right_joystick.y,
                self.a.pressed, self.b.pressed, self.x.pressed, self.y.pressed,
                self.dpad.up.pressed,
                self.dpad.down.pressed,
                self.dpad.left.pressed,
                self.dpad.right.pressed)


class JoystickState:
    def __init__(self):
        """
        X and y displacement which vary from -511 to 512
        """
        self.x = 0
        self.y = 0


class DpadState:
    def __init__(self):
        """
        Just four buttons that are 0 or 1
        """
        self.up = ButtonState()
        self.down = ButtonState()
        self.left = ButtonState()
        self.right = ButtonState()


class ButtonState:
    def __init__(self):
        """
        Only one field which is 0 for unpressed and 1 for pressed
        """
        self.pressed = 0
