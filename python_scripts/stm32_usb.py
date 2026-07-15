import serial
import threading
import time

class USB_COM:
    def __init__(self, port, baudrate=115200):
        self.running = True
        self.ser = serial.Serial(port, baudrate, timeout=1)
        time.sleep(1)  # Wait for the serial connection to initialize
        self.thread = threading.Thread(target=self.read_data)
        self.thread.start()
    
    def read_data(self):
        while self.running:
            if self.ser.in_waiting > 0:
                data = self.ser.read(self.ser.in_waiting).decode("utf-8")
                print(f"Received: {data}")
    
    def stop(self):
        self.running = False
        self.thread.join()
        self.ser.close()


if __name__ == "__main__":
    com_number = input("Enter COM port number (for example, 3 or a number): ").strip()
    usb_com = USB_COM(f"COM{com_number}")

    print("Connected")
    try:
        while True:
            cmd = input("")
            if cmd:
                usb_com.ser.write(cmd.encode("utf-8") + b'\n')
    except KeyboardInterrupt:
        print("Exiting...")
    finally:
        usb_com.stop()
