# Solar Panel Sensor Data Logger for ESP32

## Overview

This project is an ESP32 firmware designed to fetch data from a solar panel sensor and transmit it to the Azure IoT cloud using an API. The firmware is developed for the ESP32 platform, making it suitable for low-power, IoT applications.

## Features

- **Sensor Data Retrieval:** The firmware is capable of reading data from the connected solar panel sensor and additionnal sensors such as a temperature sensor.

- **Azure IoT Integration:** It securely sends the collected data to the Azure IoT cloud via API, providing a seamless way to monitor and analyze solar panel performance.

- **Power Efficiency:** The firmware is optimized for power efficiency to ensure minimal energy consumption, making it suitable for battery-powered applications.

- **Configurability:** The firmware allows easy configuration of parameters such as WiFi credentials, Azure IoT connection details, and sensor communication settings through a uart connection.

## Getting Started

### Prerequisites

- [PlatformIO](https://platformio.org/) - An open source ecosystem for IoT development. Can be installed as an extension for [Visual Studio Code](https://code.visualstudio.com/). You can read the documentation for the project by opening the [doc](docs/doc_out/sphinx/src/installation.html) file in your browser.

### Installation

1. Clone this repository to your local machine

    ```bash
    git clone https://github.com/ELE400-SolarSync/firmware-esp32.git
    ```

2. Upload the firmware to your ESP32 device using PlatformIO.

    ```bash
    pio run -t upload
    ```

### Usage

1. Open the project in [PlatformIO](https://platformio.org/).

2. Follow the instructions in the [documentation](docs/doc_out/sphinx/src/usage.html) to use the firmware.
