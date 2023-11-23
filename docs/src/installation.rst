Installation Guide
==================

1. **Install PlatformIO on Visual Studio Code:**
    - Ensure that Visual Studio Code is installed on your system.
    - Open VS Code and go to the Extensions view by clicking on the Extensions icon in the Activity Bar on the side of the window or use the keyboard shortcut `Ctrl+Shift+X`.
    - Search for the `PlatformIO IDE <https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide>`_ in the Extensions view search bar.
    - Install the PlatformIO IDE extension.

2. **Access the Firmware Repository:**
    - The firmware source code is hosted on our GitHub repository.
    - Clone the repository using the following link: `Solar-Charger-IoT-Firmware-Repo <https://github.com/ELE400-SolarSync/firmware-esp32>`_.

3. **Compile and Upload to ESP using PlatformIO:**
    - Open the cloned repository in Visual Studio Code.
    - Use the PlatformIO sidebar at the bottom of VS Code to build and upload the firmware to the ESP board:
        - Click on the PlatformIO icon in the Activity Bar.
        - Navigate to the `Project Tasks` section.
        - Click on `Build` (checkmark logo) to compile the firmware.
        - Click on `Upload` (right arrow) to upload the compiled firmware to the ESP board.

   - Ensure that the ESP board is connected to your development machine before starting the upload process.

By following these steps, you'll have successfully installed PlatformIO on Visual Studio Code, cloned the firmware repository, and compiled/uploaded the firmware to the ESP board.