#
# Copyright (c) 2019 FRC Team 3260
#

from PyQt4.QtCore import *
from PyQt4.QtGui import *
from PyQt4.QtSvg import *

IMG_HELP = "assets/xbox-hotkeys.svg"


class HotkeysWidget(QSvgWidget):
    def __init__(self, parent=None):
        super(HotkeysWidget, self).__init__(parent)
        self.load(IMG_HELP)
        self.hide()

