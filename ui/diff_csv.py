import csv
import serial
import time
import sys
import glob

BAUD_RATE = 115200
TIMEOUT = 1

def serial_ports():
    """Lists serial ports

    Raises:
    EnvironmentError:
      On unsupported or unknown platforms
    Returns:
      A list of available serial ports
    """
    if sys.platform.startswith('win'):
        ports = ['COM' + str(i + 1) for i in range(256)]

    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this is to exclude your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')

    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')

    else:
        raise EnvironmentError('Unsupported platform')

    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result

def select_serial_port():
  ports = serial_ports()
  if ports:
    print("Available serial ports:")
    for (i, p) in enumerate(ports):
      print("%d) %s" % (i + 1, p))
  else:
    print("No ports available. Check serial connection and try again.")
    print("Exiting...")
    quit()
  selection = input("Select the port to use: ")

  # Note: seems like timeout of 1 doesn't work
  ser = serial.Serial(ports[int(selection) - 1], BAUD_RATE, timeout = TIMEOUT)
  return ser

class Diff_ADC():
    def __init__(self, serial_port):
        self.ser = serial_port

    def serial_write(self, data):
        if sys.platform.startswith('win'):
            self.ser.write([data, ])
        else:
            self.ser.write(data)

    def serial_request(self):
        self.serial_write(b'd')
        serial_data = self.ser.readline()
        print(serial_data)

    def serial_close(self):
        self.ser.close()

def main():
    print("Starting Quadchannel Differential ADC Test\n")
    s = select_serial_port()
    mcp = Diff_ADC(s)
    while(True):
        try:
            mcp.serial_request()
            time.sleep(1)
        except:
            print("Keyboard Interrupt")
            mcp.serial_close()
            break

if __name__ == '__main__':
    main()
