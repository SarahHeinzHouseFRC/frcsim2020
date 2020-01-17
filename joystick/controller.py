#
# Copyright (c) 2019 FRC Team 3260
#

from PyQt4.QtCore import *
from PyQt4.QtGui import *
from PyQt4.QtSvg import *
import math

IMG_CONTROLLER = "assets/xbox-controller-white.svg"
IMG_JOYSTICK = "assets/xbox-joystick.svg"
IMG_DPAD = "assets/xbox-dpad.svg"
IMG_DPAD_UP = "assets/xbox-dpad-up.svg"
IMG_DPAD_DOWN = "assets/xbox-dpad-down.svg"
IMG_DPAD_LEFT = "assets/xbox-dpad-left.svg"
IMG_DPAD_RIGHT = "assets/xbox-dpad-right.svg"
IMG_DPAD_UP_RIGHT = "assets/xbox-dpad-up-right.svg"
IMG_DPAD_DOWN_RIGHT = "assets/xbox-dpad-down-right.svg"
IMG_DPAD_DOWN_LEFT = "assets/xbox-dpad-down-left.svg"
IMG_DPAD_UP_LEFT = "assets/xbox-dpad-up-left.svg"
IMG_BUTTON_A_RELEASED = "assets/xbox-button-a-released.svg"
IMG_BUTTON_A_PRESSED = "assets/xbox-button-a-pressed.svg"
IMG_BUTTON_B_RELEASED = "assets/xbox-button-b-released.svg"
IMG_BUTTON_B_PRESSED = "assets/xbox-button-b-pressed.svg"
IMG_BUTTON_X_RELEASED = "assets/xbox-button-x-released.svg"
IMG_BUTTON_X_PRESSED = "assets/xbox-button-x-pressed.svg"
IMG_BUTTON_Y_RELEASED = "assets/xbox-button-y-released.svg"
IMG_BUTTON_Y_PRESSED = "assets/xbox-button-y-pressed.svg"
IMG_BUMPER_LEFT_RELEASED = "assets/xbox-bumper-left-released.svg"
IMG_BUMPER_LEFT_PRESSED = "assets/xbox-bumper-left-pressed.svg"
IMG_BUMPER_RIGHT_RELEASED = "assets/xbox-bumper-right-released.svg"
IMG_BUMPER_RIGHT_PRESSED = "assets/xbox-bumper-right-pressed.svg"


class ControllerWidget(QSvgWidget):
    def __init__(self, controller_state, parent=None):
        """
        @param controller_state Pointer to where controller should place its state
        @param parent Parent of this widget
        """
        super(ControllerWidget, self).__init__(parent)

        # Save pointers to state variables
        self.controller_state = controller_state

        # Init UI
        self.load(IMG_CONTROLLER)
        self.left_joystick = JoystickWidget(self.controller_state.left_joystick, 158, 141, self)
        self.right_joystick = JoystickWidget(self.controller_state.right_joystick, 387, 232, self)
        self.a_button = ButtonWidget(IMG_BUTTON_A_RELEASED, IMG_BUTTON_A_PRESSED,
                                     self.controller_state.a, 477, 196, self)
        self.b_button = ButtonWidget(IMG_BUTTON_B_RELEASED, IMG_BUTTON_B_PRESSED,
                                     self.controller_state.b, 517, 159, self)
        self.x_button = ButtonWidget(IMG_BUTTON_X_RELEASED, IMG_BUTTON_X_PRESSED,
                                     self.controller_state.x, 436, 158, self)
        self.y_button = ButtonWidget(IMG_BUTTON_Y_RELEASED, IMG_BUTTON_Y_PRESSED,
                                     self.controller_state.y, 477, 117, self)
        self.dpad = DpadWidget(self.controller_state.dpad, 218, 224, self)
        self.bumper_left = ButtonWidget(IMG_BUMPER_LEFT_RELEASED, IMG_BUMPER_LEFT_PRESSED,
                                        self.controller_state.bumper_left, 138, 61, self)
        self.bumper_right = ButtonWidget(IMG_BUMPER_RIGHT_RELEASED, IMG_BUMPER_RIGHT_PRESSED,
                                         self.controller_state.bumper_right, 430, 61, self)
        size_policy = QSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
        size_policy.setHeightForWidth(True)
        self.setSizePolicy(size_policy)

    def heightForWidth(self, width):
        return width

    def keyPressEvent(self, event):
        """
        Handle key press events
        """
        if event.key() == Qt.Key_Up:
            self.right_joystick.set_y(512)
        if event.key() == Qt.Key_Down:
            self.right_joystick.set_y(-511)
        if event.key() == Qt.Key_Left:
            self.right_joystick.set_x(-511)
        if event.key() == Qt.Key_Right:
            self.right_joystick.set_x(512)
        if event.key() == Qt.Key_W:
            self.left_joystick.set_y(512)
        if event.key() == Qt.Key_S:
            self.left_joystick.set_y(-511)
        if event.key() == Qt.Key_A:
            self.left_joystick.set_x(-511)
        if event.key() == Qt.Key_D:
            self.left_joystick.set_x(512)
        event.accept()

    def keyReleaseEvent(self, event):
        """
        Handle key release events
        """
        if event.key() == Qt.Key_Up:
            self.right_joystick.set_y(0)
        if event.key() == Qt.Key_Down:
            self.right_joystick.set_y(0)
        if event.key() == Qt.Key_Left:
            self.right_joystick.set_x(0)
        if event.key() == Qt.Key_Right:
            self.right_joystick.set_x(0)
        if event.key() == Qt.Key_W:
            self.left_joystick.set_y(0)
        if event.key() == Qt.Key_S:
            self.left_joystick.set_y(0)
        if event.key() == Qt.Key_A:
            self.left_joystick.set_x(0)
        if event.key() == Qt.Key_D:
            self.left_joystick.set_x(0)
        event.accept()


class JoystickWidget(QSvgWidget):
    def __init__(self, joystick_state, x, y, parent=None):
        """
        @param joystick_state Container variable to place state into
        @param x X-coord to draw joystick
        @param y Y-coord to draw joystick
        """
        super(JoystickWidget, self).__init__(parent)

        self.state = joystick_state
        self.x = x
        self.y = y
        self.max_deflection = 10
        self.clickX = 0
        self.clickY = 0
        self.load(IMG_JOYSTICK)
        self.move(x, y)

    def set_x(self, x):
        """
        Sets the joystick's x-axis state to x and updates the visualization accordingly
        """
        if x < -511:
            x = -511
        if x > 512:
            x = 512
        self.state.x = x
        self.move(self.x + x / 512 * self.max_deflection, self.geometry().y())

    def set_y(self, y):
        """
        Sets the joystick's y-axis state to x and updates the visualization accordingly
        """
        if y < -511:
            y = -511
        if y > 512:
            y = 512
        self.state.y = y
        self.move(self.geometry().x(), self.y - y / 512 * self.max_deflection)

    def mousePressEvent(self, event):
        self.clickX = event.globalPos().x()
        self.clickY = event.globalPos().y()

    def mouseMoveEvent(self, event):
        x = event.globalPos().x() - self.clickX
        y = event.globalPos().y() - self.clickY
        theta = math.atan2(y, x)
        r = math.sqrt(x ** 2 + y ** 2)
        if r > self.max_deflection:
            r = self.max_deflection
        x = r * math.cos(theta)
        y = r * math.sin(theta)
        self.move(x + self.x, y + self.y)
        self.state.x = x * (512 / self.max_deflection)
        self.state.y = -y * (512 / self.max_deflection)

    def mouseReleaseEvent(self, event):
        self.move(self.x, self.y)
        self.state.x = 0
        self.state.y = 0


class DpadWidget(QSvgWidget):
    """
    Directional pad consists of four buttons, but also supports dragging to fluidly move between buttons. Each of the
    four buttons covers a region of 3*pi/4. This allows, e.g. clicking the up and left button simultaneously like a
    physical dpad. There is a deadzone of 10px in the center which does not count as a press on any button.
    """
    def __init__(self, dpad_state, x, y, parent=None):
        """
        @param dpad_state Container variable to place state into
        @param x X-coord to draw dpad
        @param y Y-coord to draw dpad
        """
        super(DpadWidget, self).__init__(parent)

        self.load(IMG_DPAD)
        self.move(x, y)
        self.state = dpad_state
        self.x = x
        self.y = y

    def reset(self):
        self.state.up.pressed = 0
        self.state.down.pressed = 0
        self.state.left.pressed = 0
        self.state.right.pressed = 0
        self.load(IMG_DPAD)

    def update(self, x, y):
        self.reset()
        center_x = self.width() / 2
        center_y = self.height() / 2
        x = x - center_x
        y = y - center_y
        theta = math.atan2(y, x)
        r = math.sqrt(x ** 2 + y ** 2)
        if r > 10:
            if math.pi/8 >= theta >= -math.pi/8:
                self.state.right.pressed = 1
                self.load(IMG_DPAD_RIGHT)
            elif -3*math.pi/8 <= theta <= -math.pi/8:
                self.state.up.pressed = 1
                self.state.right.pressed = 1
                self.load(IMG_DPAD_UP_RIGHT)
            elif -5*math.pi/8 <= theta <= -3*math.pi/8:
                self.state.up.pressed = 1
                self.load(IMG_DPAD_UP)
            elif -7*math.pi/8 <= theta <= -5*math.pi/8:
                self.state.up.pressed = 1
                self.state.left.pressed = 1
                self.load(IMG_DPAD_UP_LEFT)
            elif -7*math.pi/8 >= theta or theta >= 7*math.pi/8:
                self.state.left.pressed = 1
                self.load(IMG_DPAD_LEFT)
            elif 5*math.pi/8 <= theta <= 7*math.pi/8:
                self.state.down.pressed = 1
                self.state.left.pressed = 1
                self.load(IMG_DPAD_DOWN_LEFT)
            elif 3*math.pi/8 <= theta <= 5*math.pi/8:
                self.state.down.pressed = 1
                self.load(IMG_DPAD_DOWN)
            elif math.pi/8 <= theta <= 3*math.pi/8:
                self.state.down.pressed = 1
                self.state.right.pressed = 1
                self.load(IMG_DPAD_DOWN_RIGHT)

    def mousePressEvent(self, event):
        self.update(event.pos().x(), event.pos().y())

    def mouseMoveEvent(self, event):
        self.update(event.pos().x(), event.pos().y())

    def mouseReleaseEvent(self, event):
        self.reset()


class ButtonWidget(QSvgWidget):
    """
    SVG-based button class
    """
    def __init__(self, released_img, pressed_img, button_state, x, y, parent=None):
        """
        @param released_img Path to released image SVG
        @param pressed_img Path to pressed image SVG
        @param button_state Pointer to where this button's state should be stored
        @param x X-position
        @param y Y-position
        @param parent Parent widget
        """
        # TODO: Not great that we load/unload the SVGs all the time, fix later
        super(ButtonWidget, self).__init__(parent)
        self.button_state = button_state
        self.released_img = released_img
        self.pressed_img = pressed_img
        self.move(x, y)
        self.show_released()

    def mousePressEvent(self, event):
        self.show_pressed()
        self.button_state.pressed = 1

    def mouseReleaseEvent(self, event):
        self.show_released()
        self.button_state.pressed = 0

    def show_released(self):
        self.load(self.released_img)

    def show_pressed(self):
        self.load(self.pressed_img)
