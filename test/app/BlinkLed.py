from test.TestInstances import TestInstances
import matplotlib

matplotlib.use('TkAgg')

# LED GPIO pins
LED_SEQUENCE =  {   "STM32F411E-DISCO"  : ["D12", "D13", "D14", "D15"],
                    "STM32L0xx-Nucleo"  : ["A5"],
                    "Arduino Uno R3"    : ["B5"]
                }


# Constant for GPIO command
SET_LOW     = 0
SET_HIGH    = 1
TOGGLE      = 2
SET_OUTPUT  = 3
SET_INPUT   = 4

# Period of the LED blinking
PERIOD = 500

if __name__ == "__main__":
    # Test the class
    test_instance = TestInstances()
    test_instance.init(port = 'COM9', baudrate = 500000)      # STM32
    board_name = test_instance.get_board_name()
    led_sequence = LED_SEQUENCE[board_name]
    # Step1. Read the symbol and macro files
    test_instance.mcu.readsym(test_instance.main_path + "\\build\\main.sym")
    test_instance.mcu.readmacro(test_instance.main_path + "\\build\\main.macro")

    # Step2. Initialize the LED by setting GPIO pins as output
    for led in led_sequence:
        test_instance.mcu.writegpio(led, SET_OUTPUT)

    # Step3. Toggle LED in sequence every PERIOD ms
    led_num = 0
    while True:
        test_instance.mcu.writeram32('u32_TimerCounter', 0)
        while test_instance.mcu.readram32('u32_TimerCounter') < PERIOD:
            pass

        test_instance.mcu.writegpio(led_sequence[led_num], TOGGLE)
        led_num = (led_num + 1) % len(led_sequence)