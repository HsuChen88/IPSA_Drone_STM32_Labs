import serial
import threading
import time

class USB_COM:
    def __init__(self, gui, port, baudrate=115200):
        self.gui = gui
        self.running = True
        self.ser = serial.Serial(port, baudrate, timeout=1)
        time.sleep(1)  # Wait for the serial connection to initialize
        self.t1 = time.time()
        self.thread = threading.Thread(target=self.read_data)
        self.thread.start()
    
    def read_data(self):
        while self.running:
            if self.ser.in_waiting > 0:
                dt = time.time() - self.t1
                self.t1 = time.time()
                print(f"recv time: {dt:.4f} s")
                data = self.ser.readline().decode("utf-8").rstrip()
                print(f"received data: {data}")
                if self.gui is not None:
                    self.gui.update(data)
    
    def send_msg(self, msg):
        self.ser.write(msg.encode("utf-8"))
    
    def stop(self):
        self.running = False
        self.thread.join()
        self.ser.close()