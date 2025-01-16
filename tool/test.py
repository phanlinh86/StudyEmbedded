import serial
import time
import re
import random

class Dut(object):
    # List of supported command for the DUT through USART
    GET_INFO    = 0x0000
    WRITE_RAM   = 0x0001
    READ_RAM    = 0x0002
    WRITE_GPIO  = 0x0101
    READ_GPIO   = 0x0102

    def __init__(self, port=None, baudrate=None, sym_file="../build/main.sym"):
        self.usart      = None          # The serial port object
        self.port       = port          # Port name associated with the serial port
        self.baudrate   = baudrate      # Baud Rate
        self.sym_file   = sym_file      # Symbol file path
        self.sym_dict   = None          # Symbol list from the .sym file
                                        # First column is variable name.
                                        # 2nd column is variable address


    def connect(self):
        self.usart = serial.Serial(     port        =self.port,
                                        baudrate    =self.baudrate,
                                        timeout     =0.5,
                                        parity      =serial.PARITY_NONE,
                                        stopbits    =serial.STOPBITS_ONE,
                                        bytesize    =serial.EIGHTBITS
                                    )
    def send(self, data):
        self.usart.write(data)

    def recv(self):
        return self.usart.readall()

    def read(self):
        data = list(self.usart.read(18))
        if len(data):
            result = {  'status': data[0],
                        'resp'  : [ int.from_bytes(data[i:i+4], byteorder='big', signed=False) for i in range(1, 17, 4)] }
        else:
            result = {  'status': 0,
                        'resp'  : [] }
        return result


    def disconnect(self):
        self.usart.close()

    def send_cmd(self, cmd):
        cmd_str =   f"/cmd".encode("utf-8")
        cmd_str += cmd[0].to_bytes(4, byteorder='big')
        for param in cmd[1:]:
            cmd_str += param.to_bytes(4, byteorder='big')
        cmd_str += "\n".encode("utf-8")
        self.usart.write(cmd_str)
        return cmd_str

    def get_var_address(self, var_name):
        if var_name in self.sym_dict:
            return self.sym_dict[var_name]
        else:
            return var_name

    def get_info(self, info, option=0):
        if info.upper() == "BOARD":
            self.send_cmd([self.GET_INFO, 0x0000, 0x00, 0x00, 0x00])
            data = self.read()
            return data['resp'][0]
        elif info.upper() == "PROJECT":
            self.send_cmd([self.GET_INFO, 0x0001, 0x00, 0x00, 0x00])
            data = self.read()
            return data['resp'][0]
        elif info.upper() == "CLOCK":
            self.send_cmd([self.GET_INFO, 0x0002, 0x00, 0x00, 0x00])
            data = self.read()
            return data['resp'][0]
        elif info.upper() == "USART":
            return_dict  = {}
            self.send_cmd([self.GET_INFO, 0x100, 0x00, 0x00, 0x00])
            data = self.read()
            return_dict['usart'] = data['resp'][0]
            self.send_cmd([self.GET_INFO, 0x100, 0x01, 0x00, 0x00])
            data = self.read()
            return_dict['baudrate'] = data['resp'][0]
            if data['resp'][1] == 0:
                return_dict['bytesize'] = serial.EIGHTBITS
            else:
                return_dict['bytesize'] = 9
            if data['resp'][2] == 1:
                return_dict['stopbits'] = serial.STOPBITS_ONE
            elif data['resp'][2] == 2:
                return_dict['stopbits'] = serial.STOPBITS_ONE_POINT_FIVE
            elif data['resp'][2] == 3:
                return_dict['stopbits'] = serial.STOPBITS_TWO
            if data['resp'][3] == 0:
                return_dict['parity'] = serial.PARITY_NONE
            elif data['resp'][3] == 1:
                return_dict['parity'] = serial.PARITY_ODD
            elif data['resp'][3] == 2:
                return_dict['parity'] = serial.PARITY_EVEN

            return return_dict
        else:
            raise Exception("Invalid option")

    def write_ram(self, var_name, value, byte_size=4):
        var_name = self.get_var_address(var_name)
        self.send_cmd([self.WRITE_RAM, var_name, value, byte_size, 0x00])
        # Get response
        data = self.read()
        #print(f"Status: {data['status']}, Response: {data['resp']}")
        return data['status']

    def write_ram32(self, var_name, value):
        return self.write_ram(var_name, value, 4)

    def write_ram16(self, var_name, value):
        return self.write_ram(var_name, value, 2)

    def write_ram8(self, var_name, value):
        return self.write_ram(var_name, value, 1)

    def read_ram(self, var_name, byte_size=4):
        var_name = self.get_var_address(var_name)
        self.send_cmd([self.READ_RAM, var_name, byte_size, 0x00, 0x00])
        data = self.read()
        #print(f"Status: {data['status']}, Response: {data['resp']}")
        if data['status'] == 0:
            return []
        else:
            return data['resp'][0]

    def read_ram32(self, var_name):
        return self.read_ram(var_name, 4)

    def read_ram16(self, var_name):
        return self.read_ram(var_name, 2)

    def read_ram8(self, var_name):
        return self.read_ram(var_name, 1)

    def write_gpio(self, pin, value):
        cmd_str =   f"/cmd".encode("utf-8")
        cmd_str += self.WRITE_GPIO.to_bytes(4, byteorder='big')
        cmd_str += pin.encode() + b'\0'*(4-len(pin))
        for param in [value,0x00, 0x00]:
            cmd_str += param.to_bytes(4, byteorder='big')
        cmd_str += "\n".encode("utf-8")
        self.usart.write(cmd_str)
        data = self.read()
        return data['status']

    def read_gpio(self, pin):
        cmd_str =   f"/cmd".encode("utf-8")
        cmd_str += self.READ_GPIO.to_bytes(4, byteorder='big')
        cmd_str += pin.encode() + b'\0'*(4-len(pin))
        for param in [0x00, 0x00, 0x00]:
            cmd_str += param.to_bytes(4, byteorder='big')
        cmd_str += "\n".encode("utf-8")
        self.usart.write(cmd_str)
        data = self.read()
        if data['status'] == 0:
            return []
        else:
            return data['resp'][0]

    def read_sym(self, log_file = "../build/main.sym"):
        if log_file:
            # Read text .sym file and break by lines
            with open(log_file, 'r') as f:
                data = f.read().splitlines()
            # Parse .sym file and store in list
            self.sym_dict = {}
            for line in data:
                temp = re.split(r'(\d+): (.*) (\d+) (.*) (.*) (\d+) (.*)', line)
                if len(temp) > 1 and temp[7]:
                    self.sym_dict[temp[7]] = int(temp[2],16)
        return self.sym_dict


if __name__ == '__main__':
    # Test write/read RAM command
    print("Test1. Write/Read RAM command")
    # dut = Dut(port='COM7', baudrate=500000, sym_file='../build/main.sym')
    dut = Dut(port='COM8', baudrate=460800, sym_file='../build/main.sym')       # STM32F411E-DISCO
    print("Test1. Connect to DUT through USART : ", end="")
    dut.connect()  # Connect dut through USART
    print("OK")
    print("Test1. Read symbol file : ", end="")
    dut.read_sym()  # Parse symbol file
    print("OK")
    print("Wating 2 seconds for the connection to be stable")
    time.sleep(2)       # Waiting for a while to get the connection stable
    print("Test1. Write/Read RAM command ------------------------------------------------------")
    print("Test case2. Write/Read 32-bit RAM at random delay")
    # Get the symbol list
    for led_period in range(100,1000,10):
        time_wait_random = random.randint(0,10) / 100
        print("Led period = %d. 1. Setting LED period" % led_period)
        dut.write_ram32("u32_LedPeriodInMs", led_period)
        print("Led period = %d. 2. Pause for %.2f second" % (led_period, time_wait_random) )
        time.sleep(time_wait_random)
        led_period_rb = dut.read_ram32("u32_LedPeriodInMs")
        print("Led period = %d. 3. Reading back LED period : %d" % (led_period, led_period_rb))
        if led_period != led_period_rb:
            raise Exception("Test case1. Failed. Led period expected %d, but got %d" % (led_period, led_period_rb))
        time_wait_random = random.randint(0, 10) / 100
        print("Led period = %d. 4. Pause for %.2f second" % (led_period, time_wait_random))
        time.sleep(time_wait_random)
    print("Test case2. Write/Read 32-bit, 16bit, 8bit RAM with random value")
    for loop in range(1000):
        value32 = random.randint(0, 0xFFFFFFFF)
        # Check readram32, readram16 and readram8
        print("Loop = %d. Check readram32, readram16 and readram8 from random value 0x%8x" % (loop, value32), end=" ")
        dut.write_ram32("u32_ButtonPressCount", value32)
        value32_rb = dut.read_ram32("u32_ButtonPressCount")
        if value32 != value32_rb:
            raise Exception("Test case2. Failed. readram32 expected 0x%8x, but got 0x%8x" % (value32, value32_rb))
        value16_rb = dut.read_ram16("u32_ButtonPressCount")

        if value16_rb != (value32 & 0xFFFF):
            raise Exception("Test case2. Failed. readram16 expected 0x%4x, but got 0x%4x" % (value32 & 0xFFFF, value16_rb))

        value8_rb = dut.read_ram8("u32_ButtonPressCount")
        if value8_rb != (value32 & 0xFF):
            raise Exception("Test case2. Failed. readram8 expected 0x%2x, but got 0x%2x" % (value32 & 0xFF, value8_rb))
        print("Passed")

        # Check writeram32, writeram16 and writeram8
        value32 = random.randint(0, 0xFFFFFFFF)
        value16 = random.randint(0, 0xFFFF)
        value8 = random.randint(0, 0xFF)
        print("Loop = %d. Check writeram32, writeram16 and writeram8 from random value 0x%8x 0x%4x 0x%2x" % (loop, value32,value16,value8), end=" ")
        dut.write_ram32("u32_ButtonPressCount", value32)
        value32_rb = dut.read_ram32("u32_ButtonPressCount")
        if value32 != value32_rb:
            raise Exception("Test case2. Failed. writeram32 expected 0x%8x, but got 0x%8x" % (value32, value32_rb))

        dut.write_ram16("u32_ButtonPressCount", value16)
        value32_rb = dut.read_ram32("u32_ButtonPressCount")
        value32 = (value32 & 0xFFFF0000) | value16
        if value32_rb != value32:
            raise Exception("Test case2. Failed. writeram16 expected 0x%8x, but got 0x%8x" % (value32, value32_rb))
        dut.write_ram8("u32_ButtonPressCount", value8)
        value32_rb = dut.read_ram32("u32_ButtonPressCount")
        value32 = (value32 & 0xFFFFFF00) | value8
        if value32_rb != value32:
            raise Exception("Test case2. Failed. writeram8 expected 0x%8x, but got 0x%8x" % (value32, value32_rb))
        print("Passed")

    dut.disconnect()    # Disconnect dut through USART