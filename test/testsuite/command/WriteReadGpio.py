from test.TestInstances import *
import random


class WriteReadGpio(TestInstances):
    LIST_GPIO_PIN = ["D12", "D13", "D14", "D15", "A0", "A1", "A2", "A3", "A4", "A5", "C6", "C7"]
    LIST_GPIO_USART = ["C6", "C7"]

    def test_gpio_output(self, gpio):
        # 2. Set GPIO to output
        print(f"2. Set {gpio} to output")
        self.mcu.writegpio(gpio, 4)
        # 2.a Write GPIO to 0. Read back GPIO should return 0
        print(f"2.a Write {gpio} to 0 and read back. Return value should be 0. ", end="")
        self.mcu.writegpio(gpio, 0)
        value = self.mcu.readgpio(gpio)  # Read back GPIO
        if value != 0:
            raise Exception(f"Failed. {gpio} expected 0, but got {value}")
        else:
            print("Passed")
        # 2.b Write GPIO to 1. Read back GPIO should return 1
        print(f"2.b Write {gpio} to 1 and read back. Return value should be 1. ", end="")
        self.mcu.writegpio(gpio, 1)
        value = self.mcu.readgpio(gpio)
        if value != 1:
            raise Exception(f"Failed. {gpio} expected 1, but got {value}")
        else:
            print("Passed")
        # 2.c Toggle GPIO. Read back GPIO should return 0
        print(f"2.c Toogle {gpio}. Read back. Return value should be 0. ", end="")
        self.mcu.writegpio(gpio, 2)
        value = self.mcu.readgpio(gpio)
        if value != 0:
            raise Exception(f"Failed. {gpio} expected 0, but got {value}")
        else:
            print("Passed")
        # 2.d Toggle GPIO again. Read back GPIO should return 1
        print(f"2.d Toogle {gpio}. Read back. Return value should be 1. ", end="")
        self.mcu.writegpio(gpio, 2)
        value = self.mcu.readgpio(gpio)
        if value != 1:
            raise Exception(f"Failed. {gpio} expected 1, but got {value}")
        else:
            print("Passed")

    def test_gpio_input(self, gpio):
        # 3. Set GPIO to input
        print(f"3. Set {gpio} to input")
        self.mcu.writegpio(gpio, 3)
        # 3.a Read back GPIO. Store the current value
        print(f"3.a Read back {gpio}: ", end="")
        gpio_value = self.mcu.readgpio(gpio)
        if gpio_value > 1:
            raise Exception(f"Failed. {gpio} expected 0 or 1, but got {gpio_value}")
        else:
            print(f"{gpio_value}")
        # 3.b Write GPIO to 1. Read back GPIO should return 0
        print(f"3.b Write {gpio} to 1 and read back. Return value should be {gpio_value}. ", end="")
        self.mcu.writegpio(gpio, 1)
        value = self.mcu.readgpio(gpio)
        if value != gpio_value:
            raise Exception(f"Failed. {gpio} expected {gpio_value}, but got {value}")
        else:
            print("Passed")
        # 3.c Toggle GPIO. Read back GPIO should return 0
        print(f"3.c Toogle {gpio}. Read back. Return value should be {gpio_value}. ", end="")
        self.mcu.writegpio(gpio, 2)
        value = self.mcu.readgpio(gpio)
        if value != gpio_value:
            raise Exception(f"Failed. {gpio} expected {gpio_value}, but got {value}")
        else:
            print("Passed")
        # 3.d Toggle GPIO again. Read back GPIO should return 0
        print(f"3.d Toogle {gpio}. Read back. Return value should be {gpio_value}. ", end="")
        self.mcu.writegpio(gpio, 2)
        value = self.mcu.readgpio(gpio)
        if value != gpio_value:
            raise Exception(f"Failed. {gpio} expected {gpio_value}, but got {value}")
        else:
            print("Passed")

    def test_gpio_af(self, gpio, af):
        print(f"4. Set {gpio} to random alternate function")
        self.mcu.writegpio(gpio, 5, af)
        # 4.a Read back GPIO. Store the current value
        print(f"4.a Read back {gpio}: ", end="")
        gpio_value = self.mcu.readgpio(gpio)
        if gpio_value > 1:
            raise Exception(f"Failed. {gpio} expected 0 or 1, but got {gpio_value}")
        else:
            print(f"{gpio_value}")
        # 4.b Write GPIO to 1. Read back GPIO should return 0
        print(f"4.b Write {gpio} to 1 and read back. Return value should be {gpio_value}. ", end="")
        self.mcu.writegpio(gpio, 1)
        value = self.mcu.readgpio(gpio)
        if value != gpio_value:
            raise Exception(f"Failed. {gpio} expected {gpio_value}, but got {value}")
        else:
            print("Passed")
        # 4.c Toggle GPIO. Read back GPIO should return 0
        print(f"4.c Toogle {gpio}. Read back. Return value should be {gpio_value}. ", end="")
        self.mcu.writegpio(gpio, 2)
        value = self.mcu.readgpio(gpio)
        if value != gpio_value:
            raise Exception(f"Failed. {gpio} expected {gpio_value}, but got {value}")
        else:
            print("Passed")
        # 4.d Toggle GPIO again. Read back GPIO should return 0
        print(f"4.d Toogle {gpio}. Read back. Return value should be {gpio_value}. ", end="")
        self.mcu.writegpio(gpio, 2)
        value = self.mcu.readgpio(gpio)
        if value != gpio_value:
            raise Exception(f"Failed. {gpio} expected {gpio_value}, but got {value}")
        else:
            print("Passed")

    def run(self):
        for gpio in self.LIST_GPIO_PIN:
            # 1. If the pin is USART, skip
            if gpio in self.LIST_GPIO_USART:
                print(f"{gpio} is USART pin. Skip")
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
    test_instance.init(port='COM10', baudrate=460800)
    test_instance.mcu.readsym(test_instance.main_path + "\\build\\main.sym")
    test_instance.mcu.usart.reset_input_buffer()
    test_instance.mcu.writeram32('u32_ButtonPressCount', 1)       # Turn off blinking LED for GPIO testing
    test_instance.run()
    test_instance.cleanup()