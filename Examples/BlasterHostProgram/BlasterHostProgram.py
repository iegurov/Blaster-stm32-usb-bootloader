import sys
import serial
from serial import SerialException

port = 0

def port_selection():
    global port
    port_str = input("Введите порт: ")
    try:
        port = serial.Serial(port_str, 9600, timeout=5)
    except SerialException:
        print("Устройство не найдено!")
        port_selection()

def serial_check():
    global port
    port.write(b'\xA0')

    data = port.read(1)
    if data: 
        print("ECHO: " + data)
    else:
        print("Read timeout!")

    sys.exit()

if __name__ == "__main__":
    port_selection()
    serial_check()