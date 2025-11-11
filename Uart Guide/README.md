[this parts still needs to be written, there are just random notes for me and Burak while writing]
Please comment and describe in detail everything that is done in this part, using these messages as guidelines but further explainin all that goes on in the process :) (also include the files needed, but not the whole project, just the actual necessary files)

# Uart Guide


-Create the parts you described in this messages:

there are a lot of ways to read and write to a serial port.
you can start with putty (to read) - others in the team use this as well -

other options that work for both tx and rx are:

using pyserial (python)

for windows:
using powershell, such as this https://stackoverflow.com/questions/31297365/how-to-continuously-read-serial-com-port-in-powershell-and-occasionally-write-to   
using windows api to interact with the socket (c/c++) (isn't worth the hassle for now)

for unix:
using cli, cat and echo to read and write to the device
using unix api to interact with the socket (c/c++) (isn't worth the hassle for now)


Nucleo boards can use their power cable as UART connection. It's configured as uart2 by default.


STEP 1:
Transmit simple data (like a string) from the board to computer.
You can use HAL_UART_Transmit() and HAL_UART_Transmit_IT() - you need to enable interrupts for UART in CubeMX -.
Do it periodically, you can use a timer or transmit in the super-loop (while (1)) with a delay.
It'll be easier to use PuTTY to view it, but you can use other options as well (I'd go with Python - since we'll use for transmitting as well).
Bonus: Use a button to transmit data. 

 
STEP 2:
Transmit data to the microcontroller. 
You can use a callback function to manage logic.
Example tasks:
Use the debugger to see the data 
Blink the LED when the board receives data.


STEP 3:
Handle both TX/RX
Start with an echo server - transmit some message from the computer, and send it back  -.
Bonus: Use a button to control it.
Bonus 2: Implement a two-way-handshake.








Echo and handshake server using UART for formations.

## Usage
Install pyserial. Run client/server.py. In echo-mode, the board will send your message back. In handshake mode, the board will check the integrity of the packet and send a response. There are 3 phases of communication as defined in the comm_status enum.
~~~
typedef enum {
    COMM_NOT_INIT,
    COMM_IN_PROGRESS,
    ECHO_MODE
} comm_status;

~~~

If you are running in handshake mode, it starts with COMM_NOT_INIT, then as it verifies the connection moves to ECHO_MODE. In case of a invalid packet e.g. wrong header or checksum, it resets the status to COMM_NOT_INIT.
