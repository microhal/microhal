from Pin import Pin


def generate_static_assert(name, pins):
    template = 'static_assert({condition}, "{message}");'
    message = name + " can be connected only to: {pins}"
    condition_template = "pin == IOPin{{IOPin::{port}, {pin}}}"
    condition = ''
    for i, pin in enumerate(pins):
        condition += condition_template.format(port=pin.port, pin=pin.pin)
        if i + 1 < len(pins):
            condition += " || "

    pins_str = ''
    for i, pin in enumerate(pins):
        pins_str += str(pin)
        if i + 1 < len(pins):
            pins_str += ', '
    return template.format(condition=condition, message=message.format(pins=pins_str))


def test_generate_static_assert():
    pins = [Pin('PortA', 1), Pin('PortA', 2)]
    return generate_static_assert("Serial1 Txd", pins)
