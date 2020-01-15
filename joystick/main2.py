#
# Copyright (c) 2019 FRC Team 3260
#

import socket
from thirdParty.frc4564_xbox_python.frc4564_xbox_python import Joystick
from comms import ControllerState
import time


class Comms:
    def __init__(self, comms_config):
        self.rx_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # UDP
        self.rx_socket.bind((comms_config['rx_ip'], comms_config['rx_port']))
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


class XboxController:
    def __init__(self):
        self.joystick = Joystick()

    def get_controller_state(self):
        controller_state = ControllerState()
        (left_joystick_x, left_joystick_y) = self.joystick.leftStick()
        (right_joystick_x, right_joystick_y) = self.joystick.rightStick()
        controller_state.left_joystick.x = left_joystick_x * 512
        controller_state.left_joystick.y = left_joystick_y * 512
        controller_state.right_joystick.x = right_joystick_x * 512
        controller_state.right_joystick.y = right_joystick_y * 512
        controller_state.dpad.up.pressed = self.joystick.dpadUp()
        controller_state.dpad.down.pressed = self.joystick.dpadDown()
        controller_state.dpad.left.pressed = self.joystick.dpadLeft()
        controller_state.dpad.right.pressed = self.joystick.dpadRight()
        return controller_state


def main():
    controller_state = ControllerState()
    comms_config = {
        "rx_ip": "127.0.0.1",
        "rx_port": 2000,
        "tx_ip": "127.0.0.1",
        "tx_port": 4000
    }

    comms = Comms(comms_config)
    xbox_controller = XboxController()

    while True:
        # Get controller state
        controller_state = xbox_controller.get_controller_state()

        # Send controller state
        tx_msg = controller_state.toJson()
        print tx_msg
        comms.tx(tx_msg)

        # Receive heartbeat
        rx_msg = comms.rx()
        if rx_msg is None:
            print "Disconnected"


if __name__ == '__main__':
    main()
