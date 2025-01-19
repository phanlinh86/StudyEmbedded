from test.TestInstances import *
import random


class WriteReadGpio(TestInstances):
    LIST_GPIO_PIN_ARDUINO   = [ "B0", "B1", "B2", "B3", "B4", "B5", "B6", "B7",
                                "C0", "C1", "C2", "C3", "C4", "C5", "C6", "C7",
                                "D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7"]
    LIST_GPIO_SKIP_ARDUINO  = [ "C0", "C1", "C2", "C3", "C4", "C5", "C6", "C7", "D4"] # Skip Analog pin PC0-PC7 and Timer0 PD4
    LIST_GPIO_PIN_STM32F4   = [ "A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "A9", "A10", "A11", "A12", "A13", "A14", "A15",
                                "B0", "B1", "B2", "B3", "B4", "B5", "B6", "B7", "B8", "B9", "B10", "B11", "B12", "B13", "B14", "B15",
                                "C0", "C1", "C2", "C3", "C4", "C5", "C6", "C7", "C8", "C9", "C10", "C11", "C12", "C13", "C14", "C15",
                                "D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8", "D9", "D10", "D11", "D12", "D13", "D14", "D15",
                                "E0", "E1", "E2", "E3", "E4", "E5", "E6", "E7", "E8", "E9", "E10", "E11", "E12", "E13", "E14", "E15",
                                ]
    LIST_GPIO_SKIP_STM32F4  = [ "A9",               # Micro USB VCC
                                "C6", "C7",         # Usart6
                                "B11",              # SWIM (reflash)
                                "E1","E4","E5", "E6", "E7", "E8", "E9", "E10", "E11", "E12", "E14",    # Gyro, Accel, Mag, Temp
                                ]

    def test_gpio_output(self, gpio):
        # 2. Set GPIO to output
        self.log(f"2. Set {gpio} to output")
        self.mcu.writegpio(gpio, 3)
        # 2.a Write GPIO to 0. Read back GPIO should return 0
        self.log(f"2.a Write {gpio} to 0 and read back. Return value should be 0. ")
        self.mcu.writegpio(gpio, 0)
        value = self.mcu.readgpio(gpio)  # Read back GPIO
        if value != 0:
            self.error(f"Failed. {gpio} expected 0, but got {value}")
        else:
            self.log("Passed")
        # 2.b Write GPIO to 1. Read back GPIO should return 1
        self.log(f"2.b Write {gpio} to 1 and read back. Return value should be 1. ")
        self.mcu.writegpio(gpio, 1)
        value = self.mcu.readgpio(gpio)
        if value != 1:
            self.error(f"Failed. {gpio} expected 1, but got {value}")
        else:
            self.log("Passed")
        # 2.c Toggle GPIO. Read back GPIO should return 0
        self.log(f"2.c Toggle {gpio}. Read back. Return value should be 0. ")
        self.mcu.writegpio(gpio, 2)
        value = self.mcu.readgpio(gpio)
        if value != 0:
            self.error(f"Failed. {gpio} expected 0, but got {value}")
        else:
            self.log("Passed")
        # 2.d Toggle GPIO again. Read back GPIO should return 1
        self.log(f"2.d Toggle {gpio}. Read back. Return value should be 1. ")
        self.mcu.writegpio(gpio, 2)
        value = self.mcu.readgpio(gpio)
        if value != 1:
            self.error(f"Failed. {gpio} expected 1, but got {value}")
        else:
            self.log("Passed")

    def test_gpio_input(self, gpio):
        # 3. Set GPIO to input
        self.log(f"3. Set {gpio} to input")
        self.mcu.writegpio(gpio, 4)

        # 3.a Read back GPIO. Store the current value
        self.log(f"3.a Read back {gpio}: ")
        gpio_value = self.mcu.readgpio(gpio)
        if gpio_value > 1:
            self.error(f"Failed. {gpio} expected 0 or 1, but got {gpio_value}")
        else:
            self.log(f"{gpio_value}")

        # 3.b Write GPIO to 1. Read back GPIO should return 0
        self.log(f"3.b Write {gpio} to 1 and read back. Return value should be {gpio_value}. ")
        self.mcu.writegpio(gpio, 1)
        value = self.mcu.readgpio(gpio)
        if value != gpio_value:
            self.error(f"Failed. {gpio} expected {gpio_value}, but got {value}")
        else:
            self.log("Passed")

        # 3.c Toggle GPIO. Read back GPIO should return 0
        self.log(f"3.c Toggle {gpio}. Read back. Return value should be {gpio_value}. ")
        self.mcu.writegpio(gpio, 2)
        value = self.mcu.readgpio(gpio)
        if value != gpio_value:
            self.error(f"Failed. {gpio} expected {gpio_value}, but got {value}")
        else:
            self.log("Passed")

        # 3.d Toggle GPIO again. Read back GPIO should return 0
        self.log(f"3.d Toggle {gpio}. Read back. Return value should be {gpio_value}. ")
        self.mcu.writegpio(gpio, 2)
        value = self.mcu.readgpio(gpio)
        if value != gpio_value:
            self.error(f"Failed. {gpio} expected {gpio_value}, but got {value}")
        else:
            self.log("Passed")

    def test_gpio_af(self, gpio, af):
        self.log(f"4. Set {gpio} to random alternate function")
        self.mcu.writegpio(gpio, 5, af)
        # 4.a Read back GPIO. Store the current value
        self.log(f"4.a Read back {gpio}: ")
        gpio_value = self.mcu.readgpio(gpio)
        if gpio_value > 1:
            self.error(f"Failed. {gpio} expected 0 or 1, but got {gpio_value}")
        else:
            self.log(f"{gpio_value}")
        # 4.b Write GPIO to 1. Read back GPIO should return 0
        self.log(f"4.b Write {gpio} to 1 and read back. Return value should be {gpio_value}. ")
        self.mcu.writegpio(gpio, 1)
        value = self.mcu.readgpio(gpio)
        if value != gpio_value:
            self.error(f"Failed. {gpio} expected {gpio_value}, but got {value}")
        else:
            self.log("Passed")
        # 4.c Toggle GPIO. Read back GPIO should return 0
        self.log(f"4.c Toggle {gpio}. Read back. Return value should be {gpio_value}. ")
        self.mcu.writegpio(gpio, 2)
        value = self.mcu.readgpio(gpio)
        if value != gpio_value:
            self.error(f"Failed. {gpio} expected {gpio_value}, but got {value}")
        else:
            self.log("Passed")
        # 4.d Toggle GPIO again. Read back GPIO should return 0
        self.log(f"4.d Toggle {gpio}. Read back. Return value should be {gpio_value}. ")
        self.mcu.writegpio(gpio, 2)
        value = self.mcu.readgpio(gpio)
        if value != gpio_value:
            self.error(f"Failed. {gpio} expected {gpio_value}, but got {value}")
        else:
            self.log("Passed")

    def run(self):
        # Get list gpio pin based on the board
        board_id = self.mcu.getinfo("board")
        if board_id == 1:
            self.LIST_GPIO_PIN  = self.LIST_GPIO_PIN_STM32F4
            self.LIST_GPIO_SKIP = self.LIST_GPIO_SKIP_STM32F4
        elif board_id == 2:
            self.LIST_GPIO_PIN  = self.LIST_GPIO_PIN_NUCLEO
            self.LIST_GPIO_SKIP = self.LIST_GPIO_SKIP_NUCLEO
        elif board_id == 3:
            self.LIST_GPIO_PIN  = self.LIST_GPIO_PIN_ARDUINO
            self.LIST_GPIO_SKIP = self.LIST_GPIO_SKIP_ARDUINO

        for gpio in self.LIST_GPIO_PIN:
            # 1. Check whether the pin should be skipped (either Analog pin or Timer pin or USART pin)
            if gpio in self.LIST_GPIO_SKIP:
                self.log(f"{gpio} is skipped !!!")
                continue

            # 2. Test write/read gpio command when GPIO pin set to output
            self.test_gpio_output(gpio)

            # 3. Test write/read gpio command when GPIO pin set to input
            self.test_gpio_input(gpio)

            # 4. Set GPIO to ramdom alternate function
            af = random.randint(0, 15)
            self.test_gpio_af(gpio, af)


if __name__ == "__main__":
    test_instance = WriteReadGpio()
    test_instance.init(port='COM10', baudrate=500000)
    #test_instance.init(port='COM7', baudrate=500000)
    test_instance.mcu.readsym(test_instance.main_path + "\\build\\main.sym")
    test_instance.mcu.writeram32('u32_ButtonPressCount', 1)       # Turn off blinking LED for GPIO testing
    test_instance.run()
    test_instance.cleanup()