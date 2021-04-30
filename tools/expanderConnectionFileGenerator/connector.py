class Connector:
    def __init__(self, number):
        self.number = number
        self.section = []

    def add_section(self, section):
        self.section.append(section)