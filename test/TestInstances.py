import test.lib as lib
import os
import sys
import logging

# Import necessary libraries
class TestInstances(object):
    LIST_BAUDRATE = [9600, 19200, 38400, 57600, 115200, 230400, 460800, 500000, 921600]
    BOARD_NAME = {  1   : "STM32F411E-DISCO",
                    2   : "STM32L0xx-Nucleo",
                    3   :  "Arduino Uno R3"}

    def __init__(self, port=None, baudrate=None):
        self.port = port                # Port name associated with the serial port
        self.baudrate = baudrate        # Baud Rate
        self.mcu = lib.mcu              # MCU under test
        self.logic = None               # Logic analyzer
        self.power = None               # Power supply
        self.os = None                  # Window OS object
        self.firmware = lib.firmware    # Firmware object
        self.main_path = '\\'.join(os.path.realpath(__file__).split('\\')[:-2])  # Main path of the whole project including firmware, test, etc.
        self.test_path = self.main_path + "\\test"                # Path of the test folder
        self.log_path = self.main_path + "\\log"                  # Path of the log folder
        self.log_name =  self.__class__.__name__ + ".log"                  # Log file name
        # Initialize the file logger
        logging.basicConfig(filename=self.log_path + "\\" + self.log_name,
                            level=logging.INFO,
                            format='%(asctime)s.%(msecs)03d %(filename)s %(levelname)5s - %(message)s',
                            datefmt='%m/%d/%Y %H:%M:%S',
                            )

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
        # 2. Print out the information of the test instances
        self.print_info()

    def get_board_name(self):
        # Get the MCU name
        return(self.BOARD_NAME[self.mcu.getinfo("board")])

    def get_firmware(self):
        # Get the firmware object
        return hex(self.mcu.getinfo("firmware"))

    def print_info(self):
        # Print out the information of the test instances
        self.log("=====================================================")
        self.log(f"Main path        : {self.main_path}")
        self.log(f"Test path        : {self.test_path}")
        self.log(f"Log path         : {self.log_path}")
        self.log(f"Log name         : {self.log_name}")
        self.log(f"Port             : {self.port}")
        self.log(f"Baudrate         : {self.baudrate}")
        self.log(f"MCU              : {self.get_board_name()}")
        self.log(f"Logic Analyzer   : {self.logic}")
        self.log(f"Power            : {self.power}")
        self.log(f"Firmware Rev     : {self.get_firmware()}")
        self.log("=====================================================")

    def run(self):
        """ This is the main function to run the test
        The function will be overriden in the test class
        """
        pass

    def cleanup(self):
        self.mcu.reset()                # Soft reset the MCU
        self.mcu.disconnect()           # Disconnect MCU connection

    @staticmethod
    def log(message):
        print(message)
        logging.info(message)

    @staticmethod
    def error(message):
        logging.error(message)
        raise Exception(message)

# Test the class
if __name__ == "__main__":
    test_instance = TestInstances()
    # Print out git information
    test_instance.log("Get git information")
    test_instance.firmware.git_info(print_info=True)
    # Build and download the firmware
    test_instance.firmware.build("blink_c")
    test_instance.firmware.download("flash_c")
    # Initialize the test instance
    # test_instance.init(port = 'COM11', baudrate = 500000)
    test_instance.init(port='COM7', baudrate=500000)
    # Read the symbol file
    test_instance.mcu.readsym(test_instance.main_path + "\\build\\main.sym")
    # Run the test
    test_instance.run()
    test_instance.cleanup()
    print("TestInstances test passed")