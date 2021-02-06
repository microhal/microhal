class Pin:
    def __init__(self, port, pin, af=None):
        self.port = port
        self.pin = pin
        self.af = af

    def __str__(self):
        return self.port + '.' + str(self.pin)

    def __repr__(self):
        return self.__str__() + "-AF" + str(self.af)

def pin_from_str(str, alternate_function_str):
    port_map = {'PA': 'PortA', 'PB': 'PortB', 'PC': 'PortC', 'PD': 'PortD', 'PE': 'PortE', 'PF': 'PortF'}
    port_str = str[:2]
    return Pin(port_map[port_str], int(str[2:]), int(alternate_function_str[2:]))


def test_pin_class():
    test_pin = Pin('PortA', 1)
    print(test_pin)

