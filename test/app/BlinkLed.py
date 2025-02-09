from test.TestInstances import TestInstances
from matplotlib import pyplot as plt


class BlinkLed(TestInstances):
    # LED GPIO pins
    LED_SEQUENCE = {"STM32F411E-DISCO": { "Green": "D12", "Orange" : "D13", "Red": "D14", "Blue": "D15"},
                    "STM32L0xx-Nucleo": { "Green": "A5"},
                    "Arduino Uno R3": { "Green": "B5"}
                    }
    # Constant for GPIO command
    SET_LOW = 0
    SET_HIGH = 1
    TOGGLE = 2
    SET_OUTPUT = 3
    SET_INPUT = 4

    #
    PLOT_COLOR = ['g', 'y', 'r', 'b' ]

    def __init__(self, port=None, baudrate=None, period=50, time_limit=10*1000, plot=True):
        super().__init__(port, baudrate)
        self.period = period
        self.time_limit = time_limit
        self.led_pin = None
        self.led_name = None
        # Record data every run
        self.x_array = None
        self.y_array = None
        # For plotting
        self.plot = plot

    def init_led(self):
        # Step1.1 Connect to the MCU and load the symbol and macro files
        self.log(f"1.1 Connecting to the MCU through serial port: {self.port} using baudrate: {self.baudrate}")
        self.init(port=self.port, baudrate=self.baudrate)
        self.mcu.readsym(self.main_path + "\\build\\main.sym")
        self.mcu.readmacro(self.main_path + "\\build\\main.macro")
        board_name = self.get_board_name()
        self.log(f"Found board name: {board_name}")

        # Step1.2 Get the LED sequence for the board
        led_sequence = self.LED_SEQUENCE[board_name]
        self.led_name = list(led_sequence.keys())
        self.led_pin = list(led_sequence.values())
        self.log(f"1.2 LED sequence for this board {board_name} is {led_sequence}")

        # Step1.3 Initialize the LED by setting GPIO pins as output
        self.log(f"1.3 Initialize all LED by setting GPIO pins as output")
        for led_pin in self.led_pin:
            self.mcu.writegpio(led_pin, self.SET_OUTPUT)

        # Step1.4 Initialize the data array
        self.x_array = []
        self.y_array = [[] for i in range(len(self.led_name))]

    def setup_plot(self):
        # Setup figure, axis and line for plotting
        plt.figure()
        ax = plt.gca()
        line = []
        for i in range(len(self.led_name)):
            line.append(ax.plot([], [], self.PLOT_COLOR[i])[0])
        # Label, title, legend, grid
        plt.xlabel('Time (ms)')
        plt.ylabel('LED State')
        plt.title('LED Blinking')
        plt.legend(self.led_name)
        plt.grid()
        # Show the plot and put in interactive mode
        plt.ion()
        plt.show()
        plt.axis((0, self.time_limit + self.period, 0, 1.5))
        # Return the line object for updating the plot
        return line

    def blink(self, led_num=0):
        # Toggle the LED every time timer counter reaches inputted period
        counter = 0
        self.mcu.writeram32('u32_TimerCounter', 0)
        while counter < self.period:
            counter = self.mcu.readram32('u32_TimerCounter')

        self.time_limit -= counter
        self.mcu.writegpio(self.led_pin[led_num], self.TOGGLE)      # Toggle the LED

        # Update the data array after toggling the LED
        if self.x_array:
            self.x_array.append(self.x_array[-1] + counter)
        else:
            self.x_array = [counter]
        for i in range(len(self.led_pin)):
            self.y_array[i].append(self.mcu.readgpio(self.led_pin[i]))

    def update_plot(self, line):
        # Update the line object with the new data
        for i in range(len(self.led_name)):
            line[i].set_xdata(self.x_array)
            line[i].set_ydata(self.y_array[i])
        plt.draw()
        plt.pause(0.01)  # Have to sacrifice at least 0.01s to update the plot


    def run(self):
        # Step1. Initialize all LED
        self.log(f"------------------- 1. Initialization ----------------")
        self.init_led()

        # Setup figure, axis and line for plotting
        if self.plot:
            line = self.setup_plot()

        # Step2. Toggle LED in sequence every PERIOD ms
        self.log(f"---------------------- 2. Blink Led -------------------")
        led_num = 0
        while True:
            # Check counter, if counter reaches period, toggle LED
            self.blink(led_num)
            self.log(f"Blink {self.led_name[led_num]}")
            # Update plot
            if self.plot:
                self.update_plot(line)

            led_num = (led_num + 1) % len(self.led_name)
            if self.time_limit < 0:
                break

        # Step3. Show the data array
        self.log(f"---------------------- 3. Data -------------------------")
        self.log(f"{"Time":10s}: {self.x_array}")
        for i in range(len(self.led_name)):
            self.log(f"{self.led_name[i]:10s}: {self.y_array[i]}")

        # To keep the plot open need to turn off interactive mode and show the plot
        if self.plot:
            plt.ioff()
            plt.show()

if __name__ == "__main__":
    # Test the class
    blink_led = BlinkLed(   port        =   'COM9',
                            baudrate    =   500000,
                            period      =   500,
                            time_limit  =   10*1000,
                            plot        =   True)
    blink_led.run()
    blink_led.cleanup()