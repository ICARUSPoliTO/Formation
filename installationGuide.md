This guide will help you setup everything that you're going to need for programming and debugging STM32 boards using STM32CubeMX projects.

1. STM32CubeMX
Download the .zip from [here](https://www.st.com/en/development-tools/stm32cubemx.html) (sign up to ST's website, you'll eventually need it when creating your first project) and install the right version for your OS.

2. GNU ARM Embedded Toolchain (not needed for Windows)
Through terminal, write the following:
-Arch Linux: arm-none-eabi-gcc arm-none-eabi-gdb arm-none-eabi-newlib
-Debian/Ubuntu: gcc-arm-none-eabi gdb-multiarch libnewlib-arm-none-eabi

3. Make (not needed for Windows, should be already installed...?)
Install make from your package manager.

4. OpenOCD (not needed for Windows)
Open On-Chip Debugger is the interface between your computer and the programmer.
It will take care of uploading the compiled software to the STM32 and during debug it will open the connection between the computer and the STM32.
This section hasn't been updated with the exact command yet, but you need to install it through the package manager.

5. USB drivers (not needed for Linux)
If you have problems with flashing code while using Windows, there's likely an issue with USB drivers.
Install them from [here](https://zadig.akeo.ie/).

6. Visual Studio Code
Linux [installation](https://code.visualstudio.com/docs/setup/linux)
Windows [installation](https://code.visualstudio.com/)

7. Visual Studio Code Extensions
Install the following extensions from  Visual Studio Code:
-C/C++
-stm32-for-vscode
-Live Share
