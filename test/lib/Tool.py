from saleae.automation import Manager, DeviceType, CaptureConfiguration, LogicDeviceConfiguration, TimedCaptureMode
import os
import pandas as pd


class Logic2(object):
    SPECS = {   "Logic":        {   "enabled_digital_channels": [0, 1, 2, 3, 4, 5, 6, 7],
                                    "digital_sample_rate": 24000000,
                                    "digital_threshold_volts": 0,
                                },
                "Logic 4":      {   "enabled_digital_channels": [0, 1, 2, 3],
                                    "digital_sample_rate": 12000000,
                                    "digital_threshold_volts": 0
                                },
                "Logic 8":      {   "enabled_digital_channels": [0, 1, 2, 3, 4, 5, 6, 7],
                                    "digital_sample_rate": 100000000,
                                    "digital_threshold_volts": 0,
                                },
                "Logic Pro 8":  {   "enabled_digital_channels": [0, 1, 2, 3, 4, 5, 6, 7],
                                    "digital_sample_rate": 500000000,
                                    "digital_threshold_volts": 3.3,
                                },
                "Logic 16":     {   "enabled_digital_channels": [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15],
                                    "digital_sample_rate": 500000000,
                                    "digital_threshold_volts": 3.3,
                                },
                "Logic Pro 16": {   "enabled_digital_channels": [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15],
                                    "digital_sample_rate": 500000000,
                                    "digital_threshold_volts": 3.3,
                                },
                }
    def __init__(self, port=10430):
        self.main_path = '\\'.join(os.path.realpath(__file__).split('\\')[:-3])  # Main path of the whole project
        self.port = port
        # Connect to the Logic 2 Application
        self.manager = Manager.connect(port=self.port)      # Connect to the running Logic 2 Application on port `10430`
        self.list_devices = []                              # List of devices connected to this PC
        self.id = None # Device ID
        self.type = None  # Device type
        self.name = None  # Device name
        self.config = None  # Device configuration
        # Update the device information
        self.update()

    def update(self):
        self.list_devices = self.manager.get_devices()            # List of devices connected to this PC
        if not self.list_devices:
            self.id = None # Device ID
            self.type = None  # Device type
            self.name = None  # Device name
            self.config = None  # Device configuration
        else:
            self.id = self.list_devices[0].device_id            # Device ID
            self.type = self.list_devices[0].device_type        # Device type
            self.name = self.get_device_name()                  # Device name
            self.config = self.get_config()                     # Device configuration


    def disconnect(self):
        self.manager.close()

    def capture(self, duration = 1.0):
        # Start the capture
        capture = self.manager.start_capture(   device_id= self.id,
                                                device_configuration=LogicDeviceConfiguration(
                                                    enabled_digital_channels=self.config["enabled_digital_channels"],
                                                    digital_sample_rate=self.config["digital_sample_rate"],
                                                    digital_threshold_volts=self.config["digital_threshold_volts"]
                                                ),
                                                capture_configuration=CaptureConfiguration(capture_mode=TimedCaptureMode(duration_seconds=duration))
        )
        capture.wait()
        # Export the capture data to a temporary CSV file under log folder
        capture.export_raw_data_csv(directory=self.main_path + "\\log", digital_channels=self.config["enabled_digital_channels"])
        capture.close()
        # Read the temporary CSV file
        result = pd.read_csv(self.main_path + "\\log\\digital.csv")
        # Delete the log file after reading
        os.remove(self.main_path + "\\log\\digital.csv")
        return result

    def get_device_name(self):
        if self.type == DeviceType.LOGIC:
            device_name = "Logic"
        elif self.type == DeviceType.LOGIC_4:
            device_name = "Logic 4"
        elif self.type == DeviceType.LOGIC_8:
            device_name = "Logic 8"
        elif self.type == DeviceType.LOGIC_PRO_8:
            device_name = "Logic Pro 8"
        elif self.type == DeviceType.LOGIC_16:
            device_name = "Logic 16"
        elif self.type == DeviceType.LOGIC_PRO_16:
            device_name = "Logic Pro 16"
        else:
            device_name = None

        return device_name

    def get_config(self):
        return self.SPECS[self.name]


if __name__ == "__main__":
    logic = Logic2()
    if logic.list_devices:
        print(f"Found device: {logic.name}")
        print(f"Device ID: {logic.id}")
        print(f"Device Type: {logic.type}")
        print(f"Device Configuration: {logic.config}")
        data = logic.capture(5)
        data.head(5)
        import matplotlib
        matplotlib.use('TkAgg')
        data.plot(x = "Time [s]", y = ["Channel 0", "Channel 1", "Channel 2", "Channel 3"], drawstyle="steps")
        matplotlib.pyplot.title("Logic Analyzer Data Capture in 5 seconds")
        matplotlib.pyplot.show()
        logic.disconnect()
    else:
        print("No device found")
