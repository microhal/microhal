from gpio import GPIO
from connectorSection import ConnectorSection
from connector import Connector
from pyexcel_ods3 import get_data


def decode_gpio(gpio_str):
    name_map = {'PA': 'PortA', 'PB': 'PortB', 'PC': 'PortC', 'PD': 'PortD', 'PE': 'PortE', 'PF': 'PortF'}
    return GPIO(name_map[gpio_str[0: 2]], int(gpio_str[2:]))


def decode_file(filename):
    data = get_data(filename)['Sheet1']
    decoded = {'eval_board_name': data[0][2],
               'extension_board_name': data[2][2],
               'extension_board_version': data[3][2]}

    con1a = ConnectorSection('a', [decode_gpio(data[7][7]),
                                   decode_gpio(data[7][2]),
                                   decode_gpio(data[8][7]),
                                   decode_gpio(data[21][16]),
                                   decode_gpio(data[25][7]),
                                   decode_gpio(data[8][2])],
                             sda=decode_gpio(data[9][11]),
                             scl=decode_gpio(data[8][11]),
                             mosi=decode_gpio(data[14][11]),
                             miso=decode_gpio(data[13][11]),
                             sck=decode_gpio(data[12][11]),
                             cs=decode_gpio(data[15][2]),
                             rx=decode_gpio(data[23][11]),
                             tx=decode_gpio(data[15][11])
                             )

    con1b = ConnectorSection('b', [decode_gpio(data[20][7]),
                                   decode_gpio(data[21][7]),
                                   decode_gpio(data[22][7]),
                                   decode_gpio(data[24][11])],
                             sda=decode_gpio(data[15][16]),
                             scl=decode_gpio(data[16][11]),
                             mosi=decode_gpio(data[19][16]),
                             miso=decode_gpio(data[20][16]),
                             sck=decode_gpio(data[16][11]),
                             cs=decode_gpio(data[14][16])
                             )

    con1c = ConnectorSection('c', [decode_gpio(data[23][7]),
                                   decode_gpio(data[24][2]),
                                   decode_gpio(data[25][2]),
                                   decode_gpio(data[25][11])],
                             sda=decode_gpio(data[7][11]),
                             scl=decode_gpio(data[18][11]),
                             mosi=decode_gpio(data[21][11]),
                             miso=decode_gpio(data[20][11]),
                             sck=decode_gpio(data[22][11]),
                             cs=decode_gpio(data[24][7])
                             )

    con2a = ConnectorSection('a', [decode_gpio(data[7][16]),
                                   decode_gpio(data[11][16]),
                                   decode_gpio(data[18][16]),
                                   decode_gpio(data[8][16]),
                                   decode_gpio(data[9][16]),
                                   decode_gpio(data[17][11])],
                             sda=decode_gpio(data[9][11]),
                             scl=decode_gpio(data[8][11]),
                             mosi=decode_gpio(data[14][11]),
                             miso=decode_gpio(data[13][11]),
                             sck=decode_gpio(data[12][11]),
                             cs=decode_gpio(data[17][16]),
                             rx=decode_gpio(data[12][16]),
                             tx=decode_gpio(data[13][16])
                             )

    con1 = Connector(1)
    con1.add_section(con1a)
    con1.add_section(con1b)
    con1.add_section(con1c)
    con2 = Connector(2)
    con2.add_section(con2a)
    return decoded, [con1, con2]
