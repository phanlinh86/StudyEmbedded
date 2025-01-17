from test.TestInstances import *
import random

class WriteReadRam(TestInstances):
    def run(self):
        print("Test1. Write/Read RAM command ------------------------------------------------------")
        print("Test case2. Write/Read 32-bit RAM at random delay")
        # Get the symbol list
        for led_period in range(100,1000,10):
            timewait = random.randint(0,10) / 100
            print("Led period = %d. 1. Setting LED period" % led_period)
            self.mcu.writeram32("u32_LedPeriodInMs", led_period)
            print("Led period = %d. 2. Pause for %.2f second" % (led_period, timewait) )
            time.sleep(timewait)
            led_period_rb = self.mcu.readram32("u32_LedPeriodInMs")
            print("Led period = %d. 3. Reading back LED period : %d" % (led_period, led_period_rb))
            if led_period != led_period_rb:
                raise Exception("Test case1. Failed. Led period expected %d, but got %d" % (led_period, led_period_rb))
            timewait = random.randint(0, 10) / 100
            print("Led period = %d. 4. Pause for %.2f second" % (led_period, timewait))
            time.sleep(timewait)
        print("Test case2. Write/Read 32-bit, 16bit, 8bit RAM with random value")
        for loop in range(1000):
            value32 = random.randint(0, 0xFFFFFFFF)
            # Check readram32, readram16 and readram8
            print("Loop = %d. Check readram32, readram16 and readram8 from random value 0x%8x" % (loop, value32), end=" ")
            self.mcu.writeram32("u32_ButtonPressCount", value32)
            value32_rb = self.mcu.readram32("u32_ButtonPressCount")
            if value32 != value32_rb:
                raise Exception("Test case2. Failed. readram32 expected 0x%8x, but got 0x%8x" % (value32, value32_rb))
            value16_rb = self.mcu.readram16("u32_ButtonPressCount")
    
            if value16_rb != (value32 & 0xFFFF):
                raise Exception("Test case2. Failed. readram16 expected 0x%4x, but got 0x%4x" % (value32 & 0xFFFF, value16_rb))
    
            value8_rb = self.mcu.readram8("u32_ButtonPressCount")
            if value8_rb != (value32 & 0xFF):
                raise Exception("Test case2. Failed. readram8 expected 0x%2x, but got 0x%2x" % (value32 & 0xFF, value8_rb))
            print("Passed")
    
            # Check writeram32, writeram16 and writeram8
            value32 = random.randint(0, 0xFFFFFFFF)
            value16 = random.randint(0, 0xFFFF)
            value8 = random.randint(0, 0xFF)
            print("Loop = %d. Check writeram32, writeram16 and writeram8 from random value 0x%8x 0x%4x 0x%2x" % (loop, value32,value16,value8), end=" ")
            self.mcu.writeram32("u32_ButtonPressCount", value32)
            value32_rb = self.mcu.readram32("u32_ButtonPressCount")
            if value32 != value32_rb:
                raise Exception("Test case2. Failed. writeram32 expected 0x%8x, but got 0x%8x" % (value32, value32_rb))
    
            self.mcu.writeram16("u32_ButtonPressCount", value16)
            value32_rb = self.mcu.readram32("u32_ButtonPressCount")
            value32 = (value32 & 0xFFFF0000) | value16
            if value32_rb != value32:
                raise Exception("Test case2. Failed. writeram16 expected 0x%8x, but got 0x%8x" % (value32, value32_rb))
            self.mcu.writeram8("u32_ButtonPressCount", value8)
            value32_rb = self.mcu.readram32("u32_ButtonPressCount")
            value32 = (value32 & 0xFFFFFF00) | value8
            if value32_rb != value32:
                raise Exception("Test case2. Failed. writeram8 expected 0x%8x, but got 0x%8x" % (value32, value32_rb))
            print("Passed")


if __name__ == "__main__":
    test_instance = WriteReadRam()
    test_instance.init(port = 'COM10', baudrate = 460800)
    test_instance.mcu.readsym(test_instance.main_path + "\\build\\main.sym")
    test_instance.run()
    test_instance.cleanup()
    print("TestInstances test passed")