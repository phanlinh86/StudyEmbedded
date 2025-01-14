import serial
import time
import re

class Dut(object):
    # List of supported command for the DUT through USART
    WRITE_RAM   = 0x0001
    READ_RAM    = 0x0002

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

    def write_ram(self, var_name, value):
        var_name = self.get_var_address(var_name)
        self.send_cmd([self.WRITE_RAM, var_name, value, 0x00, 0x00])
        # Get response
        data = self.read()
        #print(f"Status: {data['status']}, Response: {data['resp']}")
        return data['status']

    def read_ram(self, var_name):
        var_name = self.get_var_address(var_name)
        self.send_cmd([self.READ_RAM, var_name, 0x00, 0x00, 0x00])
        data = self.read()
        #print(f"Status: {data['status']}, Response: {data['resp']}")
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
    # Case1. Test USART
    dut = Dut(port='COM7', baudrate=500000, sym_file='../build/main.sym')
    dut.connect()       # Connect dut through USART
    time.sleep(2)       # Waiting for a while to get the connection stable
    dut.read_sym()     # Parse symbol file
    # Get the symbol list
    for led_period in range(100,1000,10):
        print("Setting LED period to ", led_period)
        dut.write_ram("u32_LedPeriodInMs", led_period)
        time.sleep(0.1)
        print("Reading LED period :", end=" ")
        print(dut.read_ram("u32_LedPeriodInMs"))
        time.sleep(0.1)
    dut.disconnect()    # Disconnect dut through USART