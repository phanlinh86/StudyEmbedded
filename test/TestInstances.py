import test.lib as lib
import time
import os
import sys

# Import necessary libraries
class TestInstances(object):
    LIST_BAUDRATE = [9600, 19200, 38400, 57600, 115200, 230400, 460800, 500000, 921600]
    def __init__(self, port=None, baudrate=None):
        self.port = port                # Port name associated with the serial port
        self.baudrate = baudrate        # Baud Rate
        self.mcu = lib.mcu              # MCU under test
        self.logic = None               # Logic analyzer
        self.power = None               # Power supply
        self.os = None                  # Window OS object
        self.firmware = None            # Firmware object
        self.log = None                 # Log object
        self.main_path = '\\'.join(os.path.realpath(__file__).split('\\')[:-2])  # Main path of the whole project including firmware, test, etc.
        self.test_path = self.main_path + "\\test"                # Path of the test folder

    def init(self, port=None, baudrate=None):
        # Initialize the test instances
        self.port = port
        self.baudrate = baudrate
        # Add path to the system path
        sys.path.append(self.main_path)

        # 1. Connect to the MCU through USART
        board_id = None
        print(f"Connecting to the MCU through serial port: {self.port}", end=" ")
        if self.baudrate:
            self.mcu.connect(   port= self.port,
                                baudrate= self.baudrate )
            board_id = self.mcu.getinfo("board")
            if board_id:
                print(f"Found board id: {board_id} using baudrate: {baudrate}")
        else:
            for baudrate in self.LIST_BAUDRATE:
                self.mcu.connect(port=self.port,
                                 baudrate=self.baudrate)
                board_id = self.mcu.getinfo("board")
                if board_id:
                    print(f"Found board id: {board_id} using baudrate: {baudrate}")
                    break
                self.mcu.disconnect()
        if not board_id:
            raise Exception("Can't connect to the MCU using port: {self.port}")

        print("Waiting 2 seconds for the connection to be stable")
        time.sleep(2)       # Waiting for a while to get the connection stable

    def run(self):
        """ This is the main function to run the test
        The function will be overriden in the test class
        """
        pass

    def cleanup(self):
        self.mcu.disconnect()

# Test the class
if __name__ == "__main__":
    test_instance = TestInstances()
    test_instance.init(port = 'COM10', baudrate = 460800)
    test_instance.mcu.readsym(test_instance.main_path + "\\build\\main.sym")
    test_instance.run()
    test_instance.cleanup()
    print("TestInstances test passed")