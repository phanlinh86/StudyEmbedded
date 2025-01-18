# __all__ = ["mcu", "os", "power", "logic", "firmware"]
__all__ = ["mcu"]

# Import the necessary libraries
from .Mcu import Mcu
from .Firmware import Firmware

# Create the MCU object
mcu = Mcu()
firmware = Firmware()
