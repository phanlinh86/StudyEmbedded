from test.TestInstances import TestInstances
import matplotlib
from matplotlib import pyplot as plt

matplotlib.use('TkAgg')

if __name__ == "__main__":
    # Test the class
    test_instance = TestInstances()
    # test_instance.init(port = 'COM4', baudrate = 500000)      # STM32
    test_instance.init(port='COM7', baudrate=500000)  # Arduino
    test_instance.mcu.readsym(test_instance.main_path + "\\build\\main.sym")
    test_instance.mcu.readmacro(test_instance.main_path + "\\build\\main.macro")

    period = 5

    plt.figure()
    # plt.ion()

    test_instance.mcu.writeram32('u32_LedPeriodInMs', period)
    data10 = test_instance.mcu.getdata8('u32_LedPeriodInMs', 'u32_TimerCounter', 0x25, None)
    # data10 = test_instance.mcu.getdata8('u32_LedPeriodInMs', 'u32_TimerCounter', 0x40020C15, None, 512 * 4)
    # data10 = test_instance.mcu.getdata16('u32_LedPeriodInMs', 'u32_TimerCounter', 0x40020C14, None, 512 * 2)
    # data10 = test_instance.mcu.getdata32('u32_LedPeriodInMs', 'u32_TimerCounter', 0x40020C14, None, 512 )
    plt.plot(data10[0], 'k--', label=f'{period}ms - Period' )
    plt.plot(data10[1], 'k--', label=f'{period}ms - Counter')
    plt.plot([(x & (2 ** 5)) / (2 ** 5) for x in data10[2]], 'g--', label=f'{period} - Green Status')

    """
    # 32-bit / 16 bit. 4 LEDs are on pin 12, pin 13, pin 14, pin 15 of GPIOD.
    plt.plot([( x & (2**12) ) / (2**12) for x in data10[2]], 'g--', label=f'{period}ms - Green')
    plt.plot([( x & (2**13) ) / (2**13) for x in data10[2]], 'y--', label=f'{period}ms - Orange')
    plt.plot([( x & (2**14) ) / (2**14) for x in data10[2]], 'r--', label=f'{period}ms - Red')
    plt.plot([( x & (2**15) ) / (2**15) for x in data10[2]], 'b--', label=f'{period}ms - Blue')

    # 8-bit. 4 LEDs are on pin 12, pin 13, pin 14, pin 15 of GPIOD.
    # Getdata was done by moving the address by 1 byte
    plt.plot([( x & (2**4) ) / (2**4) for x in data10[2]], 'g--', label=f'{period}ms - Green')
    plt.plot([( x & (2**5) ) / (2**5) for x in data10[2]], 'y--', label=f'{period}ms - Orange')
    plt.plot([( x & (2**6) ) / (2**6) for x in data10[2]], 'r--', label=f'{period}ms - Red')
    plt.plot([( x & (2**7) ) / (2**7) for x in data10[2]], 'b--', label=f'{period}ms - Blue')
    """

    plt.legend()
    plt.grid(True)
    plt.title('LED Period, Counter and status')
    plt.xlabel('ISRs')
    plt.show()
    test_instance.cleanup()