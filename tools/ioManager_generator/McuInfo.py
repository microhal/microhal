class McuInfo:
    def __init__(self):
        self.namespace = 'stm32g0xx'
        self.family_name = 'STM32G0XX'
        self.paripheal_name_map = {'serial': 'USART',
                                   'i2c': 'I2C'}