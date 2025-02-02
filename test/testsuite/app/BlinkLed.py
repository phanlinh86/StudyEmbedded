from test.TestInstances import TestInstances
import matplotlib
from matplotlib import pyplot as plt

matplotlib.use('TkAgg')

if __name__ == "__main__":
    # Test the class
    test_instance = TestInstances()
    test_instance.init(port = 'COM4', baudrate = 500000)      # Arduino
    test_instance.mcu.readsym(test_instance.main_path + "\\build\\main.sym")
    test_instance.mcu.readmacro(test_instance.main_path + "\\build\\main.macro")

    plt.figure()
    plt.ion()

    test_instance.mcu.writeram32('u32_LedPeriodInMs', 5)
    # data10 = test_instance.mcu.getdata32('u32_LedPeriodInMs', 'u32_TimerCounter', 0x25, 0x00)
    data10 = test_instance.mcu.getdata32('u32_LedPeriodInMs', 'u32_TimerCounter', 0x40020C14)
    plt.plot(data10[0], 'k--', label='10ms - Period', )
    plt.plot(data10[1], 'k--', label='10ms - Counter' )
    # plt.plot([(x & (2 ** 5)) / (2 ** 5) for x in data10[2::4]], 'g--', label='10ms - Green Status')
    plt.plot([( x & (2**12) ) / (2**12) for x in data10[2]], 'g--', label='10ms - Green')
    plt.plot([( x & (2**13) ) / (2**13) for x in data10[2]], 'y--', label='10ms - Orange')
    plt.plot([( x & (2**14) ) / (2**14) for x in data10[2]], 'r--', label='10ms - Red')
    plt.plot([( x & (2**15) ) / (2**15) for x in data10[2]], 'b--', label='10ms - Blue')

    plt.legend()
    plt.grid(True)
    plt.title('LED Period, Counter and status')
    plt.xlabel('ISRs')
    plt.show()
    test_instance.cleanup()