import serial.tools.list_ports

def find_possible_stm32_ports():
    ports = serial.tools.list_ports.comports()
    candidates = []
    for port in ports:
        print(f"Port: {port.device}, Description: {port.description}")
        # Match generic USB serial descriptions often used by STM32
        if "USB" in port.description or "Périphérique série USB" in port.description or "STMicroelectronics Virtual COM Port" in port.description:
            candidates.append(port.device)
    return candidates

# Example usage
stm32_ports = find_possible_stm32_ports()
if stm32_ports:
    print(f"\n✅ Possible STM32 ports: {stm32_ports}")
else:
    print("⚠️ No STM32-like USB serial devices found.")