from test.TestInstances import TestInstances
import matplotlib

matplotlib.use('TkAgg')


class BlinkLed(TestInstances):
    # LED GPIO pins
    LED_SEQUENCE = {"STM32F411E-DISCO": ["D12", "D13", "D14", "D15"],
                    "STM32L0xx-Nucleo": ["A5"],
                    "Arduino Uno R3": ["B5"]
                    }
    # Constant for GPIO command
    SET_LOW = 0
    SET_HIGH = 1
    TOGGLE = 2
    SET_OUTPUT = 3
    SET_INPUT = 4

    def __init__(self, port=None, baudrate=None, period=500, time_limit=10*1000):
        super().__init__(port, baudrate)
        self.period = period
        self.time_limit = time_limit

    def run(self):
        self.init(port=self.port, baudrate=self.baudrate)
        board_name = self.get_board_name()
        led_sequence = self.LED_SEQUENCE[board_name]
        # Step1. Read the symbol and macro files
        self.mcu.readsym(self.main_path + "\\build\\main.sym")
        self.mcu.readmacro(self.main_path + "\\build\\main.macro")
        # Step2. Initialize the LED by setting GPIO pins as output
        for led in led_sequence:
            self.mcu.writegpio(led, self.SET_OUTPUT)

        # Step3. Toggle LED in sequence every PERIOD ms
        led_num = 0
        while True:
            self.mcu.writeram32('u32_TimerCounter', 0)
            while self.mcu.readram32('u32_TimerCounter') < self.period:
                pass
            self.log(f"Toggle LED: {led_sequence[led_num]}")
            self.mcu.writegpio(led_sequence[led_num], self.TOGGLE)
            led_num = (led_num + 1) % len(led_sequence)
            self.time_limit -= self.period
            if self.time_limit < 0:
                break


if __name__ == "__main__":
    # Test the class
    blink_led = BlinkLed(port='COM9', baudrate=500000)
    blink_led.run()
    blink_led.cleanup()