from test.TestInstances import TestInstances
import matplotlib
from matplotlib import pyplot as plt
from time import sleep

matplotlib.use('TkAgg')

if __name__ == "__main__":
    # Test the class
    test_instance = TestInstances()
    test_instance.init(port = 'COM9', baudrate = 500000)      # STM32
    # test_instance.init(port='COM7', baudrate=500000)  # Arduino
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

    # Set the accelerometer to 400Hz
    test_instance.mcu.writei2c(0x19, 0x20, 0x77)    # ODR = 400Hz (0x7)
    test_instance.mcu.writei2c(0x19, 0x23, 0x08)    # Set the accelerometer to 2g (0x00) and HS=1 (High Resolution 12-bit)
                                                    # Set BDU=1 to read the high and low byte of the data at the same time


    # Read the accelerometer data
    for i in range(100):
        data = test_instance.mcu.readi2c(0x19, 0x28, 6)
        accel_x = ( (data[1] << 8) | data[0] )
        accel_y = (data[3] << 8) | data[2]
        accel_z = (data[5] << 8) | data[4]
        #convert to signed 16-bit value
        if accel_x > 32767:
            accel_x -= 65536
        if accel_y > 32767:
            accel_y -= 65536
        if accel_z > 32767:
            accel_z -= 65536
        accel_x = accel_x * 0.98 / 2**12
        accel_y = accel_y * 0.98 / 2**12
        accel_z = accel_z * 0.98 / 2**12
        print(f'Accel: {accel_x}, {accel_y}, {accel_z}')

    # Read the temperature
    data = test_instance.mcu.readi2c(0x19, 0x0C, 2)
    temperature = ((data[1] << 8) | data[0]) / (2 ** 8) + 25
