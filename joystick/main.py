#!/usr/bin/python

#
# Copyright (c) 2020 FRC Team 3260
#

import sys
import argparse
import yaml
from virtual_xbox_controller import VirtualXboxController
from physical_xbox_controller import PhysicalXboxController
from PyQt4.QtGui import QApplication

CONFIG_FILE = "../config/robotConfig.yml"


def main():
    # Read command line args
    parser = argparse.ArgumentParser()
    parser.add_argument("controller_type", help='"virtual" for onscreen controller or "physical" for USB controller')
    parser.add_argument("--verbose", help="increase output verbosity", action="store_true")
    parser.add_argument("--player", help="player 1-6", type=int, default=1)
    args = parser.parse_args()

    # Read config file
    with open(CONFIG_FILE) as f:
        data = yaml.load(f)
        comms_config = {
            "rx_ip": "127.0.0.1",
            "rx_port": data['joystick']['port'] + 10 * (args.player-1),
            "tx_ip": data['core']['ip'],
            "tx_port": data['core']['joystickPort'] + 10 * (args.player-1)
        }
    print "Rx at {}:{}".format(comms_config["rx_ip"], comms_config["rx_port"])
    print "Tx to {}:{}".format(comms_config["tx_ip"], comms_config["tx_port"])

    # Launch main window
    if args.controller_type == "virtual":
        app = QApplication(sys.argv)
        window = VirtualXboxController(args.player, comms_config, args.verbose)
        window.show()
        sys.exit(app.exec_())
    elif args.controller_type == "physical":
        xbox_controller = PhysicalXboxController(args.player, comms_config, args.verbose)


if __name__ == '__main__':
    main()
