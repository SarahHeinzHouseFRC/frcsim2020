#!/usr/bin/python

#
# Copyright (c) 2020 FRC Team 3260
#

import sys
import argparse
import yaml
from virtualcontroller import VirtualXboxController
from physicalcontroller import PhysicalXboxController
from PyQt4.QtGui import QApplication

CONFIG_FILE = "../config/robotConfig.yml"


def main():
    # Read command line args
    parser = argparse.ArgumentParser()
    parser.add_argument("controller_type", help='"virtual" or "physical"')
    parser.add_argument("--verbose", help="increase output verbosity", action="store_true")
    args = parser.parse_args()

    # Read config file
    with open(CONFIG_FILE) as f:
        data = yaml.load(f)
        comms_config = {
            "rx_ip": "127.0.0.1",
            "rx_port": data['joystick']['port'],
            "tx_ip": data['core']['ip'],
            "tx_port": data['core']['joystickPort']
        }
    print "Running at {}:{}".format(comms_config["rx_ip"], comms_config["rx_port"])
    print "Sending to {}:{}".format(comms_config["tx_ip"], comms_config["tx_port"])

    # Launch main window
    if args.controller_type == "virtual":
        app = QApplication(sys.argv)
        window = VirtualXboxController(comms_config, args.verbose)
        window.show()
        sys.exit(app.exec_())
    elif args.controller_type == "physical":
        xbox_controller = PhysicalXboxController(comms_config, args.verbose)


if __name__ == '__main__':
    main()
