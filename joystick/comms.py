#
# Copyright (c) 2020 FRC Team 3260
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
        self.left_joystick = JoystickState()  # 2 axes -512 to 512
        self.right_joystick = JoystickState()  # 2 axes -512 to 512
        self.dpad = DpadState()  # 4 axes 0 or 1
        self.right_bumper = ButtonState()  # 0 or 1
        self.left_bumper = ButtonState()  # 0 or 1
        self.left_trigger = TriggerState()  # 0 to 512
        self.right_trigger = TriggerState()  # 0 to 512
        self.a = ButtonState()  # 0 or 1
        self.b = ButtonState()  # 0 or 1
        self.x = ButtonState()  # 0 or 1
        self.y = ButtonState()  # 0 or 1
        self.back = ButtonState()  # 0 or 1
        self.guide = ButtonState()  # 0 or 1
        self.start = ButtonState()  # 0 or 1

    def toJson(self):
        return "{ 'leftJoystick': [ %04d, %04d ], 'rightJoystick': [ %04d, %04d ], 'dpad': [ %01d, %01d, %01d, %01d ], 'leftTrigger': %04d, 'rightTrigger': %04d, 'leftBumper': %01d, 'rightBumper': %01d, 'a': %01d, 'b': %01d, 'x': %01d, 'y': %01d, 'back': %01d, 'guide': %01d, 'start': %01d }" % \
            (self.left_joystick.x, self.left_joystick.y, self.right_joystick.x, self.right_joystick.y,
             self.dpad.up.pressed, self.dpad.down.pressed, self.dpad.left.pressed, self.dpad.right.pressed,
             self.left_trigger.value, self.right_trigger.value,
             self.left_bumper.pressed, self.right_bumper.pressed,
             self.a.pressed, self.b.pressed, self.x.pressed, self.y.pressed,
             self.back.pressed,
             self.guide.pressed,
             self.start.pressed)


class JoystickState:
    def __init__(self):
        """
        X and y displacement which varies from -512 to 512
        """
        self.x = 0
        self.y = 0


class TriggerState:
    def __init__(self):
        """
        Displacement which varies from 0 to 512
        """
        self.value = 0


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
