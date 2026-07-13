# Python scripts

This folder is a `uv` project for the STM32 USB/serial helper scripts.

## Prepare the environment

From this folder:

```powershell
uv sync
```

If `uv` is not recognized, install it first:

```powershell
winget install --id astral-sh.uv
```

Then close and reopen PowerShell, return to this folder, and run `uv sync`.

## Run the scripts

List possible STM32 serial ports:

```powershell
uv run python identify_com_port.py
```

Run the simple project entry point:

```powershell
uv run python main.py
```

Open the STM32 serial console:

```powershell
uv run python stm32_usb.py
```

Before running `stm32_usb.py`, update the COM port in the file if needed:

```python
usb_com = USB_COM("COM4")
```

Use the COM port reported by `identify_com_port.py`.

## Run every top-level script

These scripts are not all meant to be run blindly: `stm32_usb.py` opens an interactive serial console and keeps running until you press `Ctrl+C`.

For a quick check of the non-interactive scripts:

```powershell
uv run python main.py
uv run python identify_com_port.py
```
