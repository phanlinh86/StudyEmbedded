import logging

from test.TestInstances import *
import random
from time import sleep

class WriteReadRam(TestInstances):
    def run(self):
        self.log("Test1. Write/Read RAM command ------------------------------------------------------")
        self.log("Test case2. Write/Read 32-bit RAM at random delay")
        # Get the symbol list
        for led_period in range(100,1000,10):
            timewait = random.randint(0,10) / 100
            self.log("Led period = %d. 1. Setting LED period" % led_period)
            self.mcu.writeram32("u32_LedPeriodInMs", led_period)
            self.log("Led period = %d. 2. Pause for %.2f second" % (led_period, timewait) )
            sleep(timewait)
            led_period_rb = self.mcu.readram32("u32_LedPeriodInMs")
            self.log("Led period = %d. 3. Reading back LED period : %d" % (led_period, led_period_rb))
            if led_period != led_period_rb:
                self.error("Test case1. Failed. Led period expected %d, but got %d" % (led_period, led_period_rb))
            timewait = random.randint(0, 10) / 100
            self.log("Led period = %d. 4. Pause for %.2f second" % (led_period, timewait))
            sleep(timewait)
        self.log("Test case2. Write/Read 32-bit, 16bit, 8bit RAM with random value")
        for loop in range(1000):
            value32 = random.randint(0, 0xFFFFFFFF)
            # Check readram32, readram16 and readram8
            self.log("Loop = %d. Check readram32, readram16 and readram8 from random value 0x%8x" % (loop, value32))
            self.mcu.writeram32("u32_ButtonPressCount", value32)
            value32_rb = self.mcu.readram32("u32_ButtonPressCount")
            if value32 != value32_rb:
                self.error("Test case2. Failed. readram32 expected 0x%8x, but got 0x%8x" % (value32, value32_rb))
            value16_rb = self.mcu.readram16("u32_ButtonPressCount")

            if value16_rb != (value32 & 0xFFFF):
                self.error("Test case2. Failed. readram16 expected 0x%4x, but got 0x%4x" % (value32 & 0xFFFF, value16_rb))

            value8_rb = self.mcu.readram8("u32_ButtonPressCount")
            if value8_rb != (value32 & 0xFF):
                self.error("Test case2. Failed. readram8 expected 0x%2x, but got 0x%2x" % (value32 & 0xFF, value8_rb))
            self.log("Passed")

            # Check writeram32, writeram16 and writeram8
            value32 = random.randint(0, 0xFFFFFFFF)
            value16 = random.randint(0, 0xFFFF)
            value8 = random.randint(0, 0xFF)
            self.log("Loop = %d. Check writeram32, writeram16 and writeram8 from random value 0x%8x 0x%4x 0x%2x" % (loop, value32,value16,value8))
            self.mcu.writeram32("u32_ButtonPressCount", value32)
            value32_rb = self.mcu.readram32("u32_ButtonPressCount")
            if value32 != value32_rb:
                self.error("Test case2. Failed. writeram32 expected 0x%8x, but got 0x%8x" % (value32, value32_rb))

            self.mcu.writeram16("u32_ButtonPressCount", value16)
            value32_rb = self.mcu.readram32("u32_ButtonPressCount")
            value32 = (value32 & 0xFFFF0000) | value16
            if value32_rb != value32:
                self.error("Test case2. Failed. writeram16 expected 0x%8x, but got 0x%8x" % (value32, value32_rb))
            self.mcu.writeram8("u32_ButtonPressCount", value8)
            value32_rb = self.mcu.readram32("u32_ButtonPressCount")
            value32 = (value32 & 0xFFFFFF00) | value8
            if value32_rb != value32:
                self.error("Test case2. Failed. writeram8 expected 0x%8x, but got 0x%8x" % (value32, value32_rb))
            self.log("Passed")


if __name__ == "__main__":
    test_instance = WriteReadRam()
    test_instance.init(port = 'COM7', baudrate = 500000)      # Arduino
    # test_instance.init(port = 'COM11', baudrate = 500000)       # STM32
    test_instance.mcu.readsym(test_instance.main_path + "\\build\\main.sym")
    try:
        test_instance.run()
    except Exception as e:
        print("TEST FAILED: ", e)
    test_instance.cleanup()