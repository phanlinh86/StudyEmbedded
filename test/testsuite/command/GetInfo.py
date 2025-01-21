from test.TestInstances import *


def compare_dict(dict1, dict2):
    for key in dict1:
        if dict1[key] != dict2[key]:
            return False
    return True


class GetInfo(TestInstances):
    SUPPORTED_BOARDS = [1, 2, 3]
    SUPPORTED_PROJECT = [1]
    SUPPORTED_FPU = [16000000]

    def run(self):
        self.log("Test1. Check board ID  ------------------------------------------------------------------")
        board_id = self.mcu.getinfo("board")
        if board_id in self.SUPPORTED_BOARDS:
            self.log("Test1. Passed. Board ID: %d" % board_id)
        else:
            self.error("Test1. Failed. Board ID: %d" % board_id)

        self.log("Test2. Check project information   ------------------------------------------------------")
        project_id = self.mcu.getinfo("project")
        if project_id in self.SUPPORTED_PROJECT:
            self.log("Test2. Passed. Project ID: %d" % project_id)
        else:
            self.error("Test2. Failed. Project ID: %d" % project_id)

        self.log("Test3. Check FPU frequency information   ------------------------------------------------------")
        fpu_freq = self.mcu.getinfo("clock")
        if fpu_freq in self.SUPPORTED_FPU:
            self.log("Test3. Passed. FPU =  %d" % fpu_freq)
        else:
            self.error("Test3. Failed. Read back FPU =  %d" % fpu_freq)

        self.log("Test4. Check USART information   --------------------------------------------------------------")
        usart = self.mcu.getinfo("usart")

        if (( board_id == 1 ) and           # STM32F411E-DISCO
            compare_dict(usart, {'usart': 6, 'baudrate': 500000, 'bytesize': 8,  'stopbits': 1, 'parity': 'N'}) ):
            self.log("Test4. Passed. USART =  %s" % usart)
        elif (( board_id == 2 ) and         # STM32L0xx-Nucleo
            compare_dict(usart, {'usart': 2, 'baudrate': 500000, 'bytesize': 8,  'stopbits': 1, 'parity': 'N'}) ):
            self.log("Test4. Passed. USART =  %s" % usart)
        elif ((board_id == 3) and           # Arduino Uno R3
            compare_dict(usart, {'usart': 0, 'baudrate': 500000, 'bytesize': 8, 'stopbits': 1, 'parity': 'N'}) ):
            self.log(f"Test4. Passed. USART =  %s" % usart)
        else:
            self.error(f"Test4. Failed. Read back USART =  %s" % usart)

        self.log("Test5. Check Firmware information   --------------------------------------------------------------")
        firmware = self.mcu.getinfo("firmware")
        if firmware:
            self.log("Test5. Passed. Firmware =  %4x" % firmware)
        else:
            self.error("Test5. Failed. Read back Firmware =  %4x" % firmware)



if __name__ == "__main__":
    test_instance = GetInfo()
    test_instance.init(port='COM11', baudrate=500000)
    test_instance.mcu.readsym(test_instance.main_path + "\\build\\main.sym")
    test_instance.run()
    test_instance.cleanup()