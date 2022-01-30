class PinOption:
    def __init__(self, io_pin, hardware_peripheral, peripheral, function, alternate_function_value):
        self.peripheral = peripheral
        self.function = function
        self.alternate_function_value = alternate_function_value
        self.hardware_peripheral = hardware_peripheral
        self.io_pin = io_pin

    def __str__(self):
        return f"PinOption({self.io_pin}, {self.hardware_peripheral}, {self.peripheral}, {self.function}, {self.alternate_function_value})"

    def __repr__(self):
        return self.__str__()
