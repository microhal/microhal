
devel: [![Build Status](https://jenkins.microhal.org/buildStatus/icon?job=microhal/devel)](https://jenkins.microhal.org/job/microhal/job/devel/)

# microhal
Microhal is project that try to deliver Hardware Abstraction Layer (HAL) that helps you develop software faster and easier to portable to different platforms.

## microhal API
We provide API for:
- GPIO
- External Inettupts
- Serial Ports
- SPI
- I2C

## Supported platforms
STM32F4xx, STM32F3xx <br>
Currently we are testing our software on:<br>
- STM32F407VGT6
- STM32F411RE
- STM32F344R8

## Device drivers
With microhal we provide device drivers working above our HAL.
List of supported devices:
- BMP180
- DS2782
- DS2786
- HX711 (SPI Implementation)
- ISL29023
- Lepton IR Cammera
- LIS2DH12
- LIS302
- LIS3DSH
- LSM330DL
- M24C16
- MAG3110
- MPL115A2
- MPL3115A2
- MRF89XA
- PCF8563
- RFM70
- SHT21
- TMP006
- uCAM-II
- WS2812 (SPI Implementation)
