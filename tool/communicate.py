import serial

class Uart(object):
    def __init__(self, port):
        self.port       = port
        self.baudrate   = 115200
        self.parity     = serial.PARITY_NONE
        self.stopbits   = serial.STOPBITS_ONE
        self.len        = serial.EIGHTBITS
        self.timeout    = 0.5
        self.ser = serial.Serial(   port        =self.port,
                                    baudrate    =self.baudrate,
                                    timeout     =self.timeout,
                                    parity      =self.parity,
                                    stopbits    =self.stopbits,
                                    bytesize    =self.len,
                                )

    def send(self, data):
        self.ser.write(data)

    def recv(self):
        return self.ser.readall()

    def close(self):
        self.ser.close()


if __name__ == '__main__':
    # Case1. Test USART
    uart = Uart('COM8')
    while True:
        #print("Sending string: ", f"Hello Nguyen Phan Linh 12234125 {i}")
        #uart.send(f"Hello Nguyen Phan Linh 12234125 {i}\n".encode('utf-8'))
        data = uart.ser.readline().decode('utf-8')
        if data:
            print("String receiver: ", data, end="")
    uart.close()