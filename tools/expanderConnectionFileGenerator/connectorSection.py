class ConnectorSection:
    def __init__(self, section, gpios, sda, scl, mosi, miso, sck, cs, rx=None, tx=None):
        self.section = section
        self.gpio = gpios
        self.sda = sda
        self.scl = scl
        self.miso = miso
        self.mosi = mosi
        self.sck = sck
        self.cs = cs
        self.rx = rx
        self.tx = tx
