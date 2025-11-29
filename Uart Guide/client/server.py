from enum import Enum
import serial


# It'll probably be COM6 for Windows, check documentation
ser = serial.Serial("COM6", 115200)


### HANDSHAKE CLIENT
while True:
    ### TX SECTION
    msg = input("Enter your command - only the first two letters will be used\n")
    buf = bytearray(4)
    # setting the header
    buf[0] = 28
    # setting the message
    buf[1:2] = msg.encode()
    # calculating the checksum
    buf[3] = buf[0] ^ buf[1] ^ buf[2]
    ser.write(buf[:4])

    ### RX SECTION
    checksum = 0
    x = ser.read(4)
    print(chr(x[1]), x[2])

    if x[0] != 28:
        print(f"Header doesn't match. Expected 28, got {x[0]}")
        # TODO send an error packet
        
    checksum ^= x[0]
    checksum ^= x[1]
    checksum ^= x[2]
    if checksum != x[3]:
        print(f"Checksums don't match. Expected {checksum}, got {x[3]}")

    # 83 is 'S' in ascii, which is our SUCCESS signal
    if x[1]==83 and x[2]==1:
        print("Handshake successful, entering ECHO MODE")
        break



# ### ECHO CLIENT
while True:
    msg = input()
    if msg == "exit":
        print("Exiting...")
        break
    if len(msg) != 16:
        for i in range(16-len(msg)):
            msg += str(" ")
    if len(msg) > 16:
        msg = msg[:16]

    print(f"Message {msg.rstrip()}, length: {len(msg.rstrip())}")
    ser.write(msg.encode())
    x = ser.read(16)
    print(f"Received {x.decode().rstrip()}")