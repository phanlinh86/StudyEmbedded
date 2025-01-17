# __all__ = ["mcu", "os", "power", "logic", "firmware"]
__all__ = ["mcu"]

# Import the necessary libraries
from .Mcu import Mcu


# Create the MCU object
mcu = Mcu()
