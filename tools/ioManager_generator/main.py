import csv
from Pin import pin_from_str
from string import Template
from ioPin import IOPin
from pinOption import PinOption


def get_pin_function(hardware_peripheral, pin_function):
    usart_pin_functions = {'RX': 'SerialPort_RXD', 'TX': 'SerialPort_TXD', 'CTS': 'SerialPort_CTS',
                           'RTS': 'SerialPort_RTS', 'CK': 'SerialPort_CK', 'RTS_DE': 'SerialPort_RTS', 'RTS_DE_CK': 'SerialPort_RTS'}
    if "ADC" in hardware_peripheral:
        channel = {'CH0': 'Channel0', 'CH1': 'Channel1', 'CH2': 'Channel2', 'CH3': 'Channel3', 'CH4': 'Channel4',
                   'CH5': 'Channel5', 'CH6': 'Channel6', 'CH7': 'Channel7', 'CH8': 'Channel8', 'CH9': 'Channel9',
                   'CH10': 'Channel10', 'CH11': 'Channel11', 'CH12': 'Channel12', 'CH13': 'Channel13',
                   'CH14': 'Channel14', 'CH15': 'Channel15', 'CH16': 'Channel16', 'CH17': 'Channel17',
                   'CH18': 'Channel18'}
        return "ADC_" + channel[pin_function]
    elif "CAN" in hardware_peripheral:
        return f"CAN_{pin_function}"
    elif "DAC" in hardware_peripheral:
        return f"DAC_{pin_function}"
    elif "USART" in hardware_peripheral:
        return usart_pin_functions[pin_function]
    elif 'I2C' in hardware_peripheral:
        return f"I2C_{pin_function}"
    elif 'SPI' in hardware_peripheral:
        return f"SPI_{pin_function}"
    elif 'I2S' in hardware_peripheral:
        return f"I2S_{pin_function}"
    elif 'TIM' in hardware_peripheral:
        return f"Timer_{pin_function}"


def get_pin_option_list(port_info_dict, serial_ports):
    pin_option_list = []
    for port_pin in port_info_dict:
        pin = IOPin('Port' + port_pin[1], int(port_pin[2:]))
        for function in port_info_dict[port_pin]:
            if function[1] != 'Invalid':
                if "_" in function[1]:
                    hardware_peripheral, pin_function = function[1].split("_", 1)
                    pin_function = get_pin_function(hardware_peripheral, pin_function)
                    if pin_function is None:
                        print("Warning! Don't know what to do with", function, 'Skipping')
                    else:
                        if is_serial_port(hardware_peripheral):
                            peripheral = get_serial_port_peripheral(hardware_peripheral, serial_ports)
                        else:
                            peripheral = hardware_peripheral
                        pin_option_list.append(PinOption(pin,
                                                         hardware_peripheral,
                                                         peripheral,
                                                         pin_function,
                                                         function[0]))
    return pin_option_list


def get_serial_port_peripheral(hardware_peripheral, serial_ports):
    for serial_port in serial_ports:
        if serial_port['hardware'] == hardware_peripheral:
            return serial_port['name']


def generate_io_manager_file(port_info_dict, filename, namespace):
    #print(port_info_dict)
    print("================================================================")
    pin_function_set = build_pin_function_set(port_info_dict)
    peripherals = generate_peripheral_set(pin_function_set)
    serial_ports = generate_serial_ports(peripherals)

    pin_option_list = get_pin_option_list(port_info_dict, serial_ports)
    print(pin_option_list)
    get_alternate_function_text = generate_get_alternate_function(pin_option_list)
    #print(get_alternate_function_text)
    static_assert_error_messages = generate_static_assert_error_message(port_info_dict, pin_function_set)
    functions_text = ''
    functions_text += generate_route_adc_function(pin_option_list, namespace)
    functions_text += generate_route_dac_function(pin_option_list, namespace)
    functions_text += generate_route_can_function(static_assert_error_messages, namespace)
    functions_text += generate_route_i2c_function(static_assert_error_messages, namespace)
    functions_text += generate_route_serial_port_function(static_assert_error_messages, serial_ports, namespace)
    functions_text += generate_route_spi_function(static_assert_error_messages, namespace)
    functions_text += generate_route_timer_function(static_assert_error_messages, namespace)

    #print(functions_text)

    file_template = Template("""
    #ifndef $include_guard
    #define $include_guard

    #include <array>
    #include "../gpio_$namespace.h"
    #include "ports/stmCommon/ioManager/ioPinOrUnused.h"
    #include "ports/stmCommon/ioManager/pinOption.h"
    
    namespace microhal {
    namespace $namespace {
    
    class IOManager {
    using AlternateFunction = GPIO::AlternateFunction;
    
    IOManager() = delete;
    IOManager(const IOManager&) = delete;
    IOManager(const IOManager&&) = delete;
    void operator=(const IOManager&) = delete;
    void operator=(const IOManager&&) = delete;
    ~IOManager() = delete;
        
    $get_af_func
    
    public:
    $functions
    
    };   
     
    }
    }
    
    #endif // $include_guard
""")

    with open('generated/' + filename, 'w') as write_file:
        write_file.write(file_template.substitute(include_guard="_MICROHAL_PORTS_{}_".format(filename.upper().replace('.', '_')),
                                                  namespace=namespace,
                                                  get_af_func=get_alternate_function_text,
                                                  functions=functions_text))

    # #print(generate_peripheral_pin_pair_enum_class(pin_function_set))

    # print(peripherals)

    # print(serial_port)
    #
    # file_content = generate_route_serial_port_function(static_assert_error_messages, serial_port)
    # print(file_content)
    #
    # pin_options = [PinOption(IOPin('PortA', 1), 'USART1', 'SerialPort1', 'SerialPort_RxD', 'AF0')]
    # print(generate_get_alternate_function(pin_options))


def is_serial_port(peripheral):
    if 'UART' in peripheral or 'USART' in peripheral or 'LPUART' in peripheral:
        return True
    return False


def generate_serial_ports(preipherals):
    serial_ports_hardware = []
    for peripheral in preipherals:
        if is_serial_port(peripheral):
            serial_ports_hardware.append(peripheral)
    serial_ports = []
    for sp_hw in serial_ports_hardware:
        number = int(sp_hw.replace('LPUART', '').replace('UART', '').replace('USART', ''))
        serial_ports.append({'name': 'SerialPort' + str(number),
                             'number': number,
                             'hardware': sp_hw})
    return serial_ports


def get_peripheral_pin_config(pin_info, peripheral_names, pin_types):
    peripherals = {}
    for peripheral_name in peripheral_names:
        peripherals[peripheral_name] = {}
        for pin_type in pin_types:
            peripherals[peripheral_name][pin_type] = []
        for pin in pin_info:
            for alternate_function in pin:
                if peripheral_name in pin[alternate_function]:
                    for pin_type in pin_types:
                        if pin_types[pin_type] in pin[alternate_function]:
                            peripherals[peripheral_name][pin_type].append(pin_from_str(pin["Port"], alternate_function))
    return peripherals


def load_pin_information_file(filename):
    rows = []
    with open(filename) as pin_info:
        reader = csv.DictReader(pin_info)
        for row in reader:
            rows.append(row)
    return rows


def build_port_dictionary(pin_info):
    pin_dictionary = {}
    for pin_functions in pin_info:
        dictionary_key = pin_functions['Port']
        pin_dictionary[dictionary_key] = []
        for alternate_function in pin_functions:
            if alternate_function != 'Port':
                function = pin_functions[alternate_function]
                if function == '-':
                    pair = (alternate_function, 'Invalid')
                    pin_dictionary[dictionary_key].append(pair)
                elif '/' in function:
                    for function in function.split('/'):
                        function.strip()
                        pair = (alternate_function, function)
                        pin_dictionary[dictionary_key].append(pair)
                else:
                    assert len(function) > 0, pin_functions
                    pair = (alternate_function, function)
                    pin_dictionary[dictionary_key].append(pair)
    return pin_dictionary


def pin_name_to_pretty_str(pin_name):
    return 'Port{}.{}'.format(pin_name[1], pin_name[2:])


def peripheral_name_to_pretty_str(peripheral_name):
    if 'SPI' in peripheral_name or 'I2C' in peripheral_name:
        peripheral, pin = peripheral_name.split('_')
        return "{} pin '{}'".format(peripheral, pin)
    elif 'USART' in peripheral_name:
        peripheral, pin = peripheral_name.split('_', 1)
        return "Serial Port {}, pin '{}'".format(peripheral.replace("USART", ''), pin)
    else:
        return peripheral_name


def generate_peripheral_pin_pair_enum_class(pin_function_set):
    return 'enum class PeripheralPin {};'.format(pin_function_set).replace('USART', 'SerialPort')


def generate_get_alternate_function(pin_options):
    pin_options_str = ''
    for pin_option in pin_options:
        pin_options_str += f'PinOption{{ {pin_option.io_pin}, Peripheral::{pin_option.peripheral}, PinFunction::{pin_option.function}, AF::{pin_option.alternate_function_value if "ADC" not in pin_option.alternate_function_value else "ADC"}}},\n'
    function = Template("""
constexpr static GPIO::AlternateFunction get_alternate_function(Peripheral peripheral, PinFunction pin_function, IOPin pin) {
    using AF = GPIO::AlternateFunction;
    const std::array<const PinOption, $len> pin_options = {
        $array_content
    };
    for (const auto pin_option : pin_options) {
        if (pin_option.pin == pin && pin_option.peripheral == peripheral && pin_option.function == pin_function) {
            return pin_option.af;
        }
    }
    return AF::Invalid;
}
""")
    return function.substitute(len=len(pin_options), array_content=pin_options_str)


def generate_route_adc_function(adc_pin_list, namespace):
    static_asserts = ''
    for adc_pin in adc_pin_list:
        if "ADC" in adc_pin.peripheral:
            sa = f'static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::{adc_pin.function} || peripheral != Peripheral::{adc_pin.peripheral}, "{adc_pin.peripheral} {adc_pin.function.replace("ADC_", "")} can be connected only to {adc_pin.io_pin.pretty_print()}");\n'
            static_asserts += sa
    if len(static_asserts) > 0:
        function_template = Template("""
        template <Peripheral peripheral, ADCChannel channel, IOPin pin>
        static void route_ADC($namespace::GPIO::PullType pull = $namespace::GPIO::PullType::NoPull) {        
            static_assert(is_adc(peripheral), "You need to pass one of ADC peripherals.");
            
            constexpr PinFunction pinFunction = static_cast<PinFunction>(channel);
            constexpr AlternateFunction alternate_function = get_alternate_function(peripheral, pinFunction, pin);
               
            $static_asserts
            static_assert(alternate_function == AlternateFunction::ADC, "Invalid ADC channel.");     
            
            GPIO gpio(pin);
            gpio.configureAsInput(pull);
        }
    """)
        return function_template.substitute(namespace=namespace, static_asserts=static_asserts)
    else:
        return ''


def generate_route_dac_function(pin_list, namespace):
    static_asserts = ''
    for dac_pin in pin_list:
        if "DAC" in dac_pin.peripheral:
            sa = f'static_assert(alternate_function == AlternateFunction::DAC || pinFunction != PinFunction::{dac_pin.function} || peripheral != Peripheral::{dac_pin.peripheral}, "{dac_pin.peripheral} {dac_pin.function.replace("DAC_", "")} can be connected only to {dac_pin.io_pin.pretty_print()}");\n'
            static_asserts += sa
    if len(static_asserts) > 0:
        function_template = Template("""
        template <Peripheral peripheral, DACChannel channel, IOPin pin>
        static void route_DAC($namespace::GPIO::PullType pull = GPIO::PullType::NoPull) {        
            static_assert(is_dac(peripheral), "You need to pass one of DAC peripherals.");
    
            constexpr PinFunction pinFunction = static_cast<PinFunction>(channel);
            constexpr AlternateFunction alternate_function = get_alternate_function(peripheral, pinFunction, pin);
    
            $static_asserts
            static_assert(alternate_function == AlternateFunction::DAC, "Invalid DAC channel.");     
    
            GPIO gpio(pin);
            gpio.configureAsInput(pull);
        }
    """)
        return function_template.substitute(namespace=namespace, static_asserts=static_asserts)
    else:
        return ''


def generate_route_can_function(static_assert_error_messages, namespace):
    rx_static_asserts = ''
    tx_static_asserts = ''
    for can_number in range(1, 5):
        if f"CAN{can_number}" in '_'.join(static_assert_error_messages.keys()):
            rx_message = static_assert_error_messages[f'CAN{can_number}_RX']
            rx_static_asserts += f'static_assert(rx_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::CAN{can_number}, "{rx_message}");\n'
            tx_message = static_assert_error_messages[f'CAN{can_number}_TX']
            tx_static_asserts += f'static_assert(tx_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::CAN{can_number}, "{tx_message}");\n'

    if len(rx_static_asserts) > 0:
        function_str = Template("""
        template <Peripheral peripheral, IOPin rx_pin, IOPin tx_pin>
        static void route_CAN() {        
            static_assert(is_can(peripheral), "You need to pass CAN peripheral.");
    
            constexpr AlternateFunction rx_alternate_function = get_alternate_function(peripheral, PinFunction::CAN_RX, rx_pin);
            constexpr AlternateFunction tx_alternate_function = get_alternate_function(peripheral, PinFunction::CAN_TX, tx_pin);
    
            $rx_sa
            $tx_sa
    
            GPIO gpio_rx(rx_pin);
            gpio_rx.setAlternateFunction(rx_alternate_function);
            GPIO gpio_tx(tx_pin);
            gpio_tx.setAlternateFunction(tx_alternate_function);
        }
    """)
        return function_str.substitute(namespace=namespace, rx_sa=rx_static_asserts, tx_sa=tx_static_asserts)
    else:
        return ''

def generate_route_serial_port_function(static_assert_error_messages, serial_ports, namespace):
    rxd_static_asserts = ''
    txd_static_asserts = ''
    for serial_port in serial_ports:
        print(static_assert_error_messages)
        serial_port_number = serial_port['number']
        rxd_message = static_assert_error_messages[f'{serial_port["hardware"]}_RX']
        rxd_static_asserts += f'static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SerialPort{serial_port_number}, "{rxd_message}");\n'
        txd_message = static_assert_error_messages[f'{serial_port["hardware"]}_TX']
        txd_static_asserts += f'static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SerialPort{serial_port_number}, "{txd_message}");\n'

    function_template = Template("""
    template <Peripheral peripheral, IOPinOrUnused rxd_pin, IOPinOrUnused txd_pin>
    static void route_SerialPort($namespace::GPIO::PullType rxd_pull = GPIO::PullType::NoPull,
                                  $namespace::GPIO::OutputType txd_type = GPIO::OutputType::PushPull,
                                  $namespace::GPIO::PullType txd_pull = GPIO::PullType::NoPull) {
        using AlternateFunction = GPIO::AlternateFunction;
        static_assert(is_serial_port(peripheral), "You need to pass SerialPort peripheral.");        
        if constexpr (rxd_pin != IOPinOrUnused::Unused) {
            constexpr AlternateFunction alternate_function = get_alternate_function(peripheral, PinFunction::SerialPort_RXD, rxd_pin.getIOPin());
            
            $n1            
            GPIO gpio_rxd(rxd_pin.getIOPin());
            gpio_rxd.setAlternateFunction(alternate_function, rxd_pull);
        }
        if constexpr (txd_pin != IOPinOrUnused::Unused) {
            constexpr AlternateFunction alternate_function = get_alternate_function(peripheral, PinFunction::SerialPort_TXD, txd_pin.getIOPin());
            
            $n2            
            GPIO gpio_txd(txd_pin.getIOPin());
            gpio_txd.setAlternateFunction(alternate_function, txd_pull, txd_type);
        }
    }
""")
    return function_template.substitute(namespace=namespace, n1=rxd_static_asserts, n2=txd_static_asserts)


def generate_route_spi_function(static_assert_error_messages, namespace):
    miso_static_asserts = ''
    mosi_static_asserts = ''
    sck_static_asserts = ''
    for spi_number in range(1, 5):
        if f"SPI{spi_number}" in '_'.join(static_assert_error_messages.keys()):
            miso_message = static_assert_error_messages[f'SPI{spi_number}_MISO']
            miso_static_asserts += f'static_assert(miso_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SPI{spi_number}, "{miso_message}");\n'
            mosi_message = static_assert_error_messages[f'SPI{spi_number}_MOSI']
            mosi_static_asserts += f'static_assert(mosi_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SPI{spi_number}, "{mosi_message}");\n'
            sck_message = static_assert_error_messages[f'SPI{spi_number}_SCK']
            sck_static_asserts += f'static_assert(sck_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SPI{spi_number}, "{sck_message}");\n'

    function = Template("""
    template <Peripheral peripheral, IOPinOrUnused miso_pin, IOPinOrUnused mosi_pin, IOPin sck_pin>
    static void route_SPI($namespace::GPIO::PullType miso_pull = GPIO::PullType::NoPull,
                          $namespace::GPIO::OutputType mosi_type = GPIO::OutputType::PushPull) {           
        static_assert(is_spi(peripheral), "You need to pass SPI peripheral.");

        constexpr AlternateFunction miso_alternate_function = get_alternate_function(peripheral, PinFunction::SPI_MISO, miso_pin.getIOPin());
        constexpr AlternateFunction mosi_alternate_function = get_alternate_function(peripheral, PinFunction::SPI_MOSI, mosi_pin.getIOPin());
        constexpr AlternateFunction sck_alternate_function = get_alternate_function(peripheral, PinFunction::SPI_SCK, sck_pin);
        
        $miso_sa
        $mosi_sa
        $sck_sa

        GPIO gpio_miso(miso_pin.getIOPin());
        gpio_miso.setAlternateFunction(miso_alternate_function, miso_pull);
        GPIO gpio_mosi(mosi_pin.getIOPin());
        gpio_mosi.setAlternateFunction(mosi_alternate_function, GPIO::PullType::NoPull, mosi_type);
        GPIO gpio_sck(sck_pin);
        gpio_sck.setAlternateFunction(sck_alternate_function, GPIO::PullType::NoPull, GPIO::OutputType::PushPull);
    }
""")
    return function.substitute(namespace=namespace,
                               miso_sa=miso_static_asserts,
                               mosi_sa=mosi_static_asserts,
                               sck_sa=sck_static_asserts)


def generate_route_i2c_function(static_assert_error_messages, namespace):
    sda_static_asserts = ''
    scl_static_asserts = ''
    for i2c_number in range(1, 5):
        if f"I2C{i2c_number}" in '_'.join(static_assert_error_messages.keys()):
            sda_message = static_assert_error_messages[f'I2C{i2c_number}_SDA']
            sda_static_asserts += f'static_assert(sda_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::I2C{i2c_number}, "{sda_message}");\n'
            scl_message = static_assert_error_messages[f'I2C{i2c_number}_SCL']
            scl_static_asserts += f'static_assert(scl_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::I2C{i2c_number}, "{scl_message}");\n'

    function_str = Template("""
    template <Peripheral peripheral, IOPin sda_pin, IOPin scl_pin>
    static void route_I2C($namespace::GPIO::PullType sda_pull = GPIO::PullType::NoPull,
                          $namespace::GPIO::PullType scl_pull = GPIO::PullType::NoPull) {        
        static_assert(is_i2c(peripheral), "You need to pass I2C peripheral.");
             
        constexpr AlternateFunction sda_alternate_function = get_alternate_function(peripheral, PinFunction::I2C_SDA, sda_pin);
        constexpr AlternateFunction scl_alternate_function = get_alternate_function(peripheral, PinFunction::I2C_SCL, scl_pin);
        
        $sda_sa
        $scl_sa
        
        GPIO gpio_sda(sda_pin);
        gpio_sda.setAlternateFunction(sda_alternate_function, sda_pull, GPIO::OutputType::OpenDrain);
        GPIO gpio_scl(scl_pin);
        gpio_scl.setAlternateFunction(scl_alternate_function, scl_pull, GPIO::OutputType::OpenDrain);
    }
""")
    return function_str.substitute(namespace=namespace, sda_sa=sda_static_asserts, scl_sa=scl_static_asserts)


def generate_route_timer_function(static_assert_error_messages, namespace):
    static_asserts = ''
    for timer_number in range(1, 5):
        if f"TIM{timer_number}" in '_'.join(static_assert_error_messages.keys()):
            channel_prefix = f'TIM{timer_number}_CH'
            for channel_number in range(1, 10):
                channel_name = channel_prefix + str(channel_number)
                if channel_name in static_assert_error_messages:
                    message = static_assert_error_messages[channel_name]
                    static_asserts += f'static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM{timer_number} || pinFunction != PinFunction::Timer_CH{channel_number}, "{message}");\n'

    function_str = Template("""
    template <Peripheral peripheral, TimerChannel channel, IOPin pin>
    static void route_timer($namespace::GPIO::PullType pull = GPIO::PullType::NoPull,
                            $namespace::GPIO::OutputType type = GPIO::OutputType::PushPull) {        
        static_assert(is_timer(peripheral), "You need to pass Timer peripheral.");

        constexpr PinFunction pinFunction = static_cast<PinFunction>(channel);
        constexpr AlternateFunction alternate_function = get_alternate_function(peripheral, pinFunction, pin);        

        $channel_sa        

        GPIO gpio(pin);
        gpio.setAlternateFunction(alternate_function, pull, type);        
    }
""")
    return function_str.substitute(namespace=namespace, channel_sa=static_asserts)


def build_pin_function_set(pin_info):
    pin_function_set = set()
    for pin in pin_info:
        for alternate_function_pair in pin_info[pin]:
            pin_function_set.add(alternate_function_pair[1])
    return pin_function_set


def generate_peripheral_set(pin_function_set):
    peripheral_set = set()
    for pin_function in pin_function_set:
        if '_' in pin_function:
            peripheral, function = pin_function.split('_', maxsplit=1)
            peripheral_set.add(peripheral.strip())
        else:
            peripheral_set.add(pin_function)
    return peripheral_set


def generate_static_assert_error_message(pin_info, pin_function_set):
    pin_alternatives = {}
    for pin_function in pin_function_set:
        if pin_function != 'Invalid':
            supporting_pins = []
            for pin in pin_info:
                for function in pin_info[pin]:
                    if function[1] == pin_function:
                        supporting_pins.append(pin)
            pin_alternatives[pin_function] = supporting_pins

    error_message = {}
    for pin_function in pin_alternatives:
        pins = pin_alternatives[pin_function]
        if len(pins) == 1:
            error_message[pin_function] = '{} can be connected only to: {}'.format(
                peripheral_name_to_pretty_str(pin_function),
                pin_name_to_pretty_str(pins[0]))
        elif len(pins) == 2:
            error_message[pin_function] = '{} can be connected to: {} or {}'.format(
                peripheral_name_to_pretty_str(pin_function),
                pin_name_to_pretty_str(pins[0]),
                pin_name_to_pretty_str(pins[1]))
        else:
            pins_pretty = ''
            for pin in pins[0:-1]:
                pins_pretty += pin_name_to_pretty_str(pin) + ' '
            pins_pretty = pins_pretty.strip().replace(' ', ', ')
            error_message[pin_function] = '{} can be connected to: {} or {}'.format(
                peripheral_name_to_pretty_str(pin_function),
                pins_pretty,
                pin_name_to_pretty_str(pins[-1]))
    return error_message


if __name__ == '__main__':
    # mcu_pin_info_dir = Path('../mcu_pin_info')
    # assert (mcu_pin_info_dir.is_dir())
    #pin_info = load_pin_information_file("../mcu_pin_info/stm32g0xx/stm32g071.csv")
    #pin_info = load_pin_information_file("../mcu_pin_info/stm32g0xx/stm32g070.csv")

    pin_info = load_pin_information_file("../mcu_pin_info/stm32f0xx/stm32f051xx.csv")
    filename = 'IOManager_' + 'stm32f051xx' + '.h'
    port_info_dict = build_port_dictionary(pin_info)
    generate_io_manager_file(port_info_dict, filename, "stm32f0xx")

    pin_info = load_pin_information_file("../mcu_pin_info/stm32f3xx/stm32f334xx.csv")
    filename = 'IOManager_' + 'stm32f334xx' + '.h'
    port_info_dict = build_port_dictionary(pin_info)
    generate_io_manager_file(port_info_dict, filename, "stm32f3xx")

    pin_info = load_pin_information_file("../mcu_pin_info/stm32f4xx/stm32f40x.csv")
    filename = 'IOManager_' + 'stm32f40xxx' + '.h'
    port_info_dict = build_port_dictionary(pin_info)
    generate_io_manager_file(port_info_dict, filename, "stm32f4xx")

    pin_info = load_pin_information_file("../mcu_pin_info/stm32f4xx/stm32f411.csv")
    filename = 'IOManager_' + 'stm32f411xx' + '.h'
    port_info_dict = build_port_dictionary(pin_info)
    generate_io_manager_file(port_info_dict, filename, "stm32f4xx")


    # print(pin_info)


    #
    # mcu_family_name = 'STM32G0XX'
    #
    # # peripheral_name = 'i2c'
    # # peripheral_names = ['I2C1', 'I2C2', 'I2C3', 'I2C4', 'I2C5', 'I2C6']
    # # pin_types = {'SDA': 'SDA', 'SCL': 'SCL'}
    #
    # # peripheral_name = 'spi'
    # # peripheral_names = ['SPI1', 'SPI2', 'SPI3', 'SPI4', 'SPI5', 'SPI6']
    # # pin_types = {'MOSI': 'MOSI', 'MISO': 'MISO', 'SCK': 'SCK'}
    #
    # peripheral_name = 'serial'
    # peripheral_names = ['USART1', 'USART2',  'USART6']
    # pin_types = {'Rxd': 'RX', 'Txd': 'TX'}
    #
    # usarts_config = get_peripheral_pin_config(pin_info, peripheral_names, pin_types)
    # print(usarts_config)
    # usarts = []
    # for usart in usarts_config:
    #     usarts.append(usarts_config[usart])
    # #mcu_info = McuInfo(namespace='stm32g0xx', family_name='STM32G0XX')
    # mcu_info = McuInfo(namespace='stm32f4xx', family_name='STM32F4XX')
    # print(generate_routeSerial_function(mcu_info, peripheral_name, 'routeSerial', usarts))
