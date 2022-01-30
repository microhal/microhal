class IOPin:
    def __init__(self, port, pin):
        self.port = port
        self.pin = pin

    def __str__(self):
        return f"IOPin{{IOPin::{self.port}, {self.pin}}}"

    def pretty_print(self):
        return f"{self.port}.{self.pin}"
