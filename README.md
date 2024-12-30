![image alt](https://gitlab.com/Training_Academic/muhtas1/200207032_NidaMert_Muhtas1/-/raw/main/Images/Muhtas_PMOD_ESP32_front.JPG?ref_type=heads)

**Overview**

This project focuses on the design and testing of a custom 2-layer PMOD ESP32 module, closely resembling Digilent's PMOD ESP32 product. The PMOD ESP32 module provides Wi-Fi and Bluetooth connectivity, making it an ideal choice for embedded systems and IoT applications that require wireless communication. The module is designed to interface seamlessly with microcontrollers and FPGAs via the PMOD interface, enabling rapid prototyping and testing.

The functionality of the designed PMOD ESP32 board will be verified by sending AT commands through an STM32 microcontroller.

This repository includes:
- **Hardware Design Files** for the custom PMOD ESP32 board.
- **Firmware and Documentation** for the ESP32-WROOM-32 module.
- **Testing Examples** demonstrating STM32-based communication using AT commands.

The project serves as a practical solution for developers seeking an easy-to-use wireless module for their embedded systems.

**Project Structre**

- **Documentation/**: Contains connection diagrams and datasheets for reference.
- **Firmware/ESP32-WROOM-32-AT-V3.4.0.0/**: Includes firmware files, user guides, release notes, and other documentation related to ESP32-WROOM-32-AT V3.4.0.0.
- **Hardware Designs/**: Contains hardware design files for the PMOD ESP32 module in 2-layer and 4-layer configurations.
- **STM32 Applications/**: Includes example applications and tests for STM32, such as;
> Logic analyzer tests, STM32 UART AT commands
- **.gitmodules**: Configuration file for Git submodules.
- **README.md**: This file provides an overview of the project and instructions.