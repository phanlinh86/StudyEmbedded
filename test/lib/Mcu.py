import serial
import re
from time import sleep

class Mcu(object):
    # List of supported command for the DUT through USART
    GET_INFO    = 0x0000
    WRITE_RAM   = 0x0001
    READ_RAM    = 0x0002
    WRITE_GPIO  = 0x0101
    READ_GPIO   = 0x0102
    CAPTURE8    = 0x0201
    CAPTURE16   = 0x0202
    CAPTURE32   = 0x0204
    SEND_DATA   = 0x0202
    SOFT_RESET  = 0XFEEF

    def __init__(self, port=None, baudrate=None, sym_file="../build/main.sym"):
        self.usart      = None          # The serial port object
        self.port       = port          # Port name associated with the serial port
        self.baudrate   = baudrate      # Baud Rate
        self.sym_file   = sym_file      # Symbol file path
        self.sym_dict   = None          # Symbol list from the .sym file
                                        # First column is variable name.
                                        # 2nd column is variable address
        self.macro_dict = None          # Macro list from the .macro file


    def connect(self, port=None, baudrate=None):
        if port:
            self.port = port
        if baudrate:
            self.baudrate = baudrate
        self.usart = serial.Serial(     port        =self.port,
                                        baudrate    =self.baudrate,
                                        timeout     =0.1,
                                        parity      =serial.PARITY_NONE,
                                        stopbits    =serial.STOPBITS_ONE,
                                        bytesize    =serial.EIGHTBITS
                                    )

        self.usart.reset_input_buffer() # Clear input buffer
        sleep(2)                   # Waiting for a while to get the connection stable

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

    def sendcmd(self, cmd):
        cmd_str =   f"/cmd".encode("utf-8")
        cmd_str += cmd[0].to_bytes(4, byteorder='big')
        for param in cmd[1:]:
            cmd_str += param.to_bytes(4, byteorder='big')
        cmd_str += "\n".encode("utf-8")
        self.usart.write(cmd_str)
        return cmd_str

    def getvaraddress(self, var_name):
        if var_name in self.sym_dict:
            return self.sym_dict[var_name]
        else:
            return var_name

    def getinfo(self, info, option=0):
        result = None
        if info.upper() == "BOARD":
            self.sendcmd([self.GET_INFO, 0x0000, 0x00, 0x00, 0x00])
            data = self.read()
            if data['resp']: result=data['resp'][0]
        elif info.upper() == "PROJECT":
            self.sendcmd([self.GET_INFO, 0x0001, 0x00, 0x00, 0x00])
            data = self.read()
            if data['resp']: result=data['resp'][0]
        elif info.upper() == "CLOCK":
            self.sendcmd([self.GET_INFO, 0x0002, 0x00, 0x00, 0x00])
            data = self.read()
            if data['resp']: result=data['resp'][0]
        elif info.upper() == "FIRMWARE":
            self.sendcmd([self.GET_INFO, 0x003, 0x00, 0x00, 0x00])
            data = self.read()
            if data['resp']: result=data['resp'][0]
        elif info.upper() == "USART":
            return_dict  = {}
            self.sendcmd([self.GET_INFO, 0x100, 0x00, 0x00, 0x00])
            data = self.read()
            return_dict['usart'] = data['resp'][0]
            self.sendcmd([self.GET_INFO, 0x100, 0x01, 0x00, 0x00])
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

            result=return_dict
        else:
            raise Exception("Invalid option")
        return result

    def writeram(self, var_name, value, bytesize=4):
        var_name = self.getvaraddress(var_name)
        self.sendcmd([self.WRITE_RAM, var_name, value, bytesize, 0x00])
        # Get response
        data = self.read()
        return data['status']

    def writeram32(self, var_name, value):
        return self.writeram(var_name, value, 4)

    def writeram16(self, var_name, value):
        return self.writeram(var_name, value, 2)

    def writeram8(self, var_name, value):
        return self.writeram(var_name, value, 1)

    def readram(self, var_name, bytesize=4, length=1):
        var_name = self.getvaraddress(var_name)
        if length == 1:
            self.sendcmd([self.READ_RAM, var_name, bytesize, 0x00, 0x00])
            data = self.read()
            if data['status'] == 0:
                return []
            else:
                return data['resp'][0]
        else:
            result = []
            for i in range(length):
                self.sendcmd([self.READ_RAM, var_name + bytesize*i, bytesize, i, 0x00])
                data = self.read()
                if data['status'] == 0:
                    result.append([])
                else:
                    result.append(data['resp'][0])
            return result

    def readram32(self, varName, length=1):
        return self.readram(varName, 4, length=length)

    def readram16(self, var_name, length=1):
        return self.readram(var_name, 2, length=length)

    def readram8(self, var_name, length=1):
        return self.readram(var_name, 1, length=length)

    def capture(self, var_name=None, var_name1=None, var_name2=None, var_name3=None, bytesize=4):
        if var_name is not None:
            var_name = self.getvaraddress(var_name)
        else:
            var_name = 0x00
        if var_name1 is not None:
            var_name1 = self.getvaraddress(var_name1)
        else:
            var_name1 = 0x00
        if var_name2 is not None:
            var_name2 = self.getvaraddress(var_name2)
        else:
            var_name2 = 0x00
        if var_name3 is not None:
            var_name3 = self.getvaraddress(var_name3)
        else:
            var_name3 = 0x00

        if bytesize == 4:
            self.sendcmd([self.CAPTURE32, var_name, var_name1, var_name2, var_name3])
            sleep(0.1)      # Wait for the data to be captured. Estimate time to capture data is 100ms
        elif bytesize == 2:
            self.sendcmd([self.CAPTURE16, var_name, var_name1, var_name2, var_name3])
            sleep(1)        # Wait for the data to be captured. Estimate time to capture data is 100ms
        elif bytesize == 1:
            self.sendcmd([self.CAPTURE8, var_name, var_name1, var_name2, var_name3])
            sleep(1)        # Wait for the data to be captured. Estimate time to capture data is 100ms

        data = self.read()
        if data['status'] == 0:
            return []
        else:
            return data['resp']

    def getdata(self, var_name=None, var_name1=None, var_name2=None, var_name3=None, length=None, bytesize=4):
        self.capture(var_name, var_name1, var_name2, var_name3, bytesize)
        if (length is None) and ('MAX_BATCH_DATA' in self.macro_dict):
            length = self.macro_dict['MAX_BATCH_DATA'] * ( 4 // bytesize )
        no_var = sum([1 for x in [var_name, var_name1, var_name2, var_name3] if x is not None])
        data = self.readram('batch_data', bytesize, length=length)
        result = []
        for i in range(no_var):
            result.append(data[i::no_var])
        return result

    def getdata32(self, var_name=None, var_name1=None, var_name2=None, var_name3=None, length=None):
        return self.getdata(var_name, var_name1, var_name2, var_name3, length=length, bytesize=4)

    def getdata16(self, var_name=None, var_name1=None, var_name2=None, var_name3=None, length=None):
        return self.getdata(var_name, var_name1, var_name2, var_name3, length=length, bytesize=2)

    def getdata8(self, var_name=None, var_name1=None, var_name2=None, var_name3=None, length=None):
        return self.getdata(var_name, var_name1, var_name2, var_name3, length=length, bytesize=1)

    def writegpio(self, pin, value, af=0):
        cmd_str =   f"/cmd".encode("utf-8")
        cmd_str += self.WRITE_GPIO.to_bytes(4, byteorder='big')
        cmd_str += pin.encode() + b'\0'*(4-len(pin))
        for param in [value,af, 0x00]:
            cmd_str += param.to_bytes(4, byteorder='big')
        cmd_str += "\n".encode("utf-8")
        self.usart.write(cmd_str)
        data = self.read()
        return data['status']

    def readgpio(self, pin):
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

    def reset(self):
        self.sendcmd([self.SOFT_RESET, 0x00, 0x00, 0x00, 0x00])
        data = self.read()
        return data['status']

    def readsym(self, log_file = "../build/main.sym"):
        if log_file:
            # read text .sym file and break by lines
            with open(log_file, 'r') as f:
                data = f.read().splitlines()
            # Parse .sym file and store in list
            self.sym_dict = {}
            for line in data:
                temp = re.split(r'(\d+): (.*) (\d+) (.*) (.*) (\d+) (.*)', line)
                if len(temp) > 1 and temp[7]:
                    self.sym_dict[temp[7]] = int(temp[2],16)
        return self.sym_dict

    def readmacro(self, macro_file="../build/main.macro"):
        """
        Read the macro file and return the dictionary of the macros
        :param macro_file: Macro file path
        :return: Dictionary of the macros
        """
        self.macro_dict = {}

        # Read the macro file
        with open(macro_file, 'r') as file:
            for line in file:
                line = line.strip()
                if line:
                    if line.startswith("#define"):
                        # Parse the line and extract the macro and value
                        if len(line.split(' ')) == 3:
                            macro, value = line.split(' ')[1:]

                            if self.isint(value):
                                self.macro_dict[macro] = int(value)
                            elif self.ishex(value):
                                self.macro_dict[macro] = int(value, 16)
                            else:
                                self.macro_dict[macro] = value
        # Return the macros dictionary
        return self.macro_dict

    @staticmethod
    def isint(value):
        try:
            int(value)
            return True
        except ValueError:
            return False

    @staticmethod
    def ishex(value):
        try:
            int(value, 16)
            return True
        except ValueError:
            return False
