import nucleoExtensionDecoder


def generater_reduced_connector_section(connector):
    connectorFull = f"""namespace {connector.section} {{
    constexpr microhal::IOPin io1{{microhal::IOPin::{connector.gpio[0].port}, {connector.gpio[0].pin}}};
    constexpr microhal::IOPin io2{{microhal::IOPin::{connector.gpio[1].port}, {connector.gpio[1].pin}}};
    constexpr microhal::IOPin io3{{microhal::IOPin::{connector.gpio[2].port}, {connector.gpio[2].pin}}};
    constexpr microhal::IOPin io4{{microhal::IOPin::{connector.gpio[3].port}, {connector.gpio[3].pin}}};"""
    connectorFull += f"""
    constexpr microhal::IOPin io5{{microhal::IOPin::{connector.gpio[4].port}, {connector.gpio[4].pin}}};
    constexpr microhal::IOPin io6{{microhal::IOPin::{connector.gpio[5].port}, {connector.gpio[5].pin}}};""" if len(connector.gpio) > 4 else ""
    connectorFull += f"""
    constexpr microhal::IOPin sda{{microhal::IOPin::{connector.sda.port}, {connector.sda.pin}}};
    constexpr microhal::IOPin scl{{microhal::IOPin::{connector.scl.port}, {connector.scl.pin}}};
    constexpr microhal::IOPin mosi{{microhal::IOPin::{connector.mosi.port}, {connector.mosi.pin}}};
    constexpr microhal::IOPin miso{{microhal::IOPin::{connector.miso.port}, {connector.miso.pin}}};
    constexpr microhal::IOPin sck{{microhal::IOPin::{connector.sck.port}, {connector.sck.pin}}};
    constexpr microhal::IOPin cs{{microhal::IOPin::{connector.cs.port}, {connector.cs.pin}}};"""
    connectorFull += f"""
    constexpr microhal::IOPin rxd{{microhal::IOPin::{connector.rx.port}, {connector.rx.pin}}};
    constexpr microhal::IOPin txd{{microhal::IOPin::{connector.tx.port}, {connector.tx.pin}}};""" if connector.rx else ""
    connectorFull += f"""
}}  // namespace {connector.section}"""
    return connectorFull


def generate_connector_text(connector):
    text = f"""namespace con{connector.number} {{
{generater_reduced_connector_section(connector.section[0])}
{generater_reduced_connector_section(connector.section[1]) if len(connector.section) > 1 else ""}
{generater_reduced_connector_section(connector.section[2]) if len(connector.section) > 1 else ""}
}}  // namespace con{connector.number}"""
    return text


def generate_file_content(connectors, filename):
    define_guard = filename.upper() + '_H_'
    file_content = f"""#ifndef {define_guard}
#define {define_guard}

/* *********************************************************************************************************************
 * INCLUDES
 */
#include "microhal.h"

namespace bsp {{    
{generate_connector_text(connectors[0])}
{generate_connector_text(connectors[1])}
}}  // namespace bsp

#endif  // {define_guard}
"""
    return file_content


if __name__ == '__main__':

    [board_data, connectors] = nucleoExtensionDecoder.decode_file('nucleo-f411re expansion header connection.ods')
    filename = board_data['eval_board_name'] + '_extension_' + board_data['extension_board_version']
    filename = filename.replace('-', '_').replace('.', '_').lower()

    with open(filename+'.h', 'w') as file:
        file.write(
            generate_file_content(connectors, filename))
