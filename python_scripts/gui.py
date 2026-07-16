import tkinter as tk
import tkinter.messagebox as msgbox
from usb_com import USB_COM

class GUI:
    def __init__(self):
        """ Constructor """
        self.config_master()
        self.configure_title()
        self.configure_state()
        self.configure_line()
        self.configure_gains()
        self.configure_des_alt()
        self.configure_buttons()
        self.usb_com = USB_COM(self, "COM3")   #TODO: enter the right COM port for your system
    

    def config_master(self):
        """ Configure the root widget """
        self.app = tk.Tk()
        self.app.title("Bicopter Control Interface")
        self.app.geometry("1400x600")
        self.app.minsize(1400, 700)
        self.app.config(bg="white")
        self.app.resizable(True, True)
        self.app.bind("<Escape>", self.quit)
    

    def configure_title(self):
        """ Configure the title of the app """
        self.title_frame = tk.Frame(master=self.app, bg="white")
        self.title_frame.place(relwidth=1, relheight=0.2)
        tk.Label(master=self.title_frame, text="Bicopter Control Interface", font=("Arial", 30), bg="white", fg="#00508c").place(relx=0.05, relwidth=0.9, relheight=1)

    
    def configure_state(self):
        self.state_frame = tk.Frame(master=self.app, bg="white")
        self.state_frame.place(rely=0.2, relwidth=1, relheight=0.15)
        tk.Label(master=self.state_frame, text="Roll (deg)", font=("Arial", 20), bg="white", fg="#6A7176").place(relx=0.05, relwidth=0.2025, relheight=0.45)
        tk.Label(master=self.state_frame, text="Altitude (cm)", font=("Arial", 20), bg="white", fg="#6A7176").place(relx=0.2825, relwidth=0.2025, relheight=0.45)
        tk.Label(master=self.state_frame, text="Pulse CW (us)", font=("Arial", 20), bg="white", fg="#6A7176").place(relx=0.5115, relwidth=0.2025, relheight=0.45)
        tk.Label(master=self.state_frame, text="Pulse CCW (us)", font=("Arial", 20), bg="white", fg="#6A7176").place(relx=0.7472, relwidth=0.2025, relheight=0.45)
        self.roll_var = tk.DoubleVar(master=self.state_frame, value=0.0)
        self.alt_var = tk.DoubleVar(master=self.state_frame, value=0.0)
        self.pulse_cw_var = tk.DoubleVar(master=self.state_frame, value=0.0)
        self.pulse_ccw_var = tk.DoubleVar(master=self.state_frame, value=0.0)
        tk.Label(master=self.state_frame, textvariable=self.roll_var, font=("Arial", 20), bg="white").place(relx=0.05, rely=0.55, relwidth=0.2025, relheight=0.45)
        tk.Label(master=self.state_frame, textvariable=self.alt_var, font=("Arial", 20), bg="white").place(relx=0.2825, rely=0.55, relwidth=0.2025, relheight=0.45)
        tk.Label(master=self.state_frame, textvariable=self.pulse_cw_var, font=("Arial", 20), bg="white").place(relx=0.5115, rely=0.55, relwidth=0.2025, relheight=0.45)
        tk.Label(master=self.state_frame, textvariable=self.pulse_ccw_var, font=("Arial", 20), bg="white").place(relx=0.7472, rely=0.55, relwidth=0.2025, relheight=0.45)


    def configure_line(self):
        self.line_frame = tk.Frame(master=self.app, bg="black")
        self.line_frame.place(rely=0.4, relwidth=1, relheight=0.001)


    def configure_gains(self):
        """ Configure the display of the PID gains """
        self.frame_gains = tk.Frame(master=self.app, bg="white")
        self.frame_gains.place(rely=0.45, relwidth=0.5, relheight=0.5)
        tk.Label(master=self.frame_gains, text="Kp_z", font=("Arial", 20), bg="white", fg="#00508c").place(relx=0.05, rely=0.05, relwidth=0.1, relheight=0.2)
        tk.Label(master=self.frame_gains, text="Ki_z", font=("Arial", 20), bg="white", fg="#00508c").place(relx=0.05, rely=0.4, relwidth=0.1, relheight=0.2)
        tk.Label(master=self.frame_gains, text="Kd_z", font=("Arial", 20), bg="white", fg="#00508c").place(relx=0.05, rely=0.75, relwidth=0.1, relheight=0.2)
        self.entry_kp_z = tk.Entry(master=self.frame_gains, font=("Arial", 20))
        self.entry_kp_z.insert(tk.END, "0.000")
        self.entry_kp_z.place(relx=0.2, rely=0.05, relwidth=0.25, relheight=0.2)
        self.entry_ki_z = tk.Entry(master=self.frame_gains, font=("Arial", 20))
        self.entry_ki_z.insert(tk.END, "0.000")
        self.entry_ki_z.place(relx=0.2, rely=0.4, relwidth=0.25, relheight=0.2)
        self.entry_kd_z = tk.Entry(master=self.frame_gains, font=("Arial", 20))
        self.entry_kd_z.insert(tk.END, "0.000")
        self.entry_kd_z.place(relx=0.2, rely=0.75, relwidth=0.25, relheight=0.2)
        tk.Label(master=self.frame_gains, text="Kp_r", font=("Arial", 20), bg="white", fg="#de2b07").place(relx=0.55, rely=0.05, relwidth=0.1, relheight=0.2)
        tk.Label(master=self.frame_gains, text="Ki_r", font=("Arial", 20), bg="white", fg="#de2b07").place(relx=0.55, rely=0.4, relwidth=0.1, relheight=0.2)
        tk.Label(master=self.frame_gains, text="Kd_r", font=("Arial", 20), bg="white", fg="#de2b07").place(relx=0.55, rely=0.75, relwidth=0.1, relheight=0.2)
        self.entry_kp_r = tk.Entry(master=self.frame_gains, font=("Arial", 20))
        self.entry_kp_r.insert(tk.END, "0.000")
        self.entry_kp_r.place(relx=0.7, rely=0.05, relwidth=0.25, relheight=0.2)
        self.entry_ki_r = tk.Entry(master=self.frame_gains, font=("Arial", 20))
        self.entry_ki_r.insert(tk.END, "0.000")
        self.entry_ki_r.place(relx=0.7, rely=0.4, relwidth=0.25, relheight=0.2)
        self.entry_kd_r = tk.Entry(master=self.frame_gains, font=("Arial", 20))
        self.entry_kd_r.insert(tk.END, "0.000")
        self.entry_kd_r.place(relx=0.7, rely=0.75, relwidth=0.25, relheight=0.2)
        self.bind_gain_entries()


    def bind_gain_entries(self):
        """ Bind keyboard adjustments for PID gain entries. """
        gain_entries = (
            self.entry_kp_z,
            self.entry_ki_z,
            self.entry_kd_z,
            self.entry_kp_r,
            self.entry_ki_r,
            self.entry_kd_r,
        )
        for entry in gain_entries:
            entry.bind("<Up>", lambda event, e=entry: self.adjust_gain(e, 0.001))
            entry.bind("<Down>", lambda event, e=entry: self.adjust_gain(e, -0.001))


    def adjust_gain(self, entry, step):
        """ Adjust the selected gain entry by step and keep three decimals. """
        try:
            value = float(entry.get())
        except ValueError:
            value = 0.0

        entry.delete(0, tk.END)
        entry.insert(0, f"{value + step:.3f}")
        entry.icursor(tk.END)
        return "break"
    

    def configure_des_alt(self):
        """ Configure the tkinter scale to set the desired altitude of the bicopter """
        self.frame_des_alt = tk.Frame(master=self.app, bg="white")
        self.frame_des_alt.place(relx=0.5, rely=0.45, relwidth=0.5, relheight=0.35)
        self.zd_var = tk.DoubleVar(master=self.frame_des_alt, value=12.0)   #use to link land altitude to desired altitude!
        self.scale_zd = tk.Scale(self.frame_des_alt, bg="white", orient="horizontal", from_=8.0, to=20.0, resolution=1,
                                 label="Target altitude in cm", tickinterval=2, troughcolor="white", font=("Arial", 20), variable=self.zd_var)
        self.scale_zd.place(relx=0.05, rely=0.05, relwidth=0.9, relheight=0.9)
        self.scale_zd.bind("<Button-1>", self.set_altitude_from_click)
        self.scale_zd.bind("<B1-Motion>", self.set_altitude_from_click)


    def set_altitude_from_click(self, event):
        """ Set target altitude directly from the clicked scale position. """
        from_value = float(self.scale_zd.cget("from"))
        to_value = float(self.scale_zd.cget("to"))
        resolution = float(self.scale_zd.cget("resolution"))
        start_x = self.scale_zd.coords(from_value)[0]
        end_x = self.scale_zd.coords(to_value)[0]

        ratio = (event.x - start_x) / (end_x - start_x)
        ratio = max(0.0, min(1.0, ratio))
        value = from_value + ratio * (to_value - from_value)
        if resolution > 0:
            value = round(value / resolution) * resolution

        self.zd_var.set(value)
        return "break"

    
    def configure_buttons(self):
        """ Configure the buttons and their callback functions """
        self.frame_buttons = tk.Frame(master=self.app, bg="white")
        self.frame_buttons.place(relx=0.5, rely=0.85, relwidth=0.5, relheight=0.1)
        tk.Button(master=self.frame_buttons, text="Takeoff", bg="#00508c", font=("Arial", 17), command=lambda : self.usb_com.send_msg("T\n")).place(relx=0.05, rely=0.05, relwidth=0.2, relheight=0.9)
        tk.Button(master=self.frame_buttons, text="Land", bg="white", font=("Arial", 17), command=self.land).place(relx=0.28, rely=0.05, relwidth=0.2, relheight=0.9)
        tk.Button(master=self.frame_buttons, text="Gains", bg="#de2b07", font=("Arial", 17), command=self.update_gains).place(relx=0.51, rely=0.05, relwidth=0.2, relheight=0.9)
        tk.Button(master=self.frame_buttons, text="Update z", bg="orange", font=("Arial", 17), command=lambda : self.usb_com.send_msg(f"Z,{self.zd_var.get()}\n")).place(relx=0.74, rely=0.05, relwidth=0.2, relheight=0.9)


    def update(self, data):
        """ Update the plots """
        try:
            state = data.split(" ")
            self.roll_var.set(float(state[0]))
            self.alt_var.set(float(state[1]))
            self.pulse_cw_var.set(int(state[2]))
            self.pulse_ccw_var.set(int(state[3]))
        except Exception:
            pass


    def update_gains(self):
        """ Send the PID gains only if they are all set in their entries """
        if self.entry_kp_r.get() and self.entry_ki_r.get() and self.entry_kd_r.get() and self.entry_kp_z.get() and self.entry_ki_z.get() and self.entry_kd_z.get():
            try:
                msg = f"G,{float(self.entry_kp_r.get())},{float(self.entry_ki_r.get())},{float(self.entry_kd_r.get())},{float(self.entry_kp_z.get())},{float(self.entry_ki_z.get())},{float(self.entry_kd_z.get())}\n"
                self.usb_com.send_msg(msg)
            except Exception as e:
                msgbox.showerror("Invalid gains", "Make sure to set float or integers!")
        else:
            msgbox.showerror("Gains incorrects", "Check your PID gains, some fields might be empty!")


    def land(self):
        """ Ask the bicopter to land and update the desired altitude plotted """
        self.zd_var.set(8.0)
        self.usb_com.send_msg("L\n")


    def quit(self, _):
        """ Stop the motors and close the window """
        self.usb_com.send_msg("S\n")
        res = msgbox.askokcancel("Closing window", "The bicopter will stop its motors. App will close after pressing ok...")
        if res:
            self.usb_com.stop()
            self.app.quit()


if __name__ == "__main__":
    import os
    if os.name == "nt": #if you are on Windows
        screen_resolution_to_fix = True  #Set this boolean to True if you face window resolution issues
        if screen_resolution_to_fix:
            import ctypes
            ctypes.windll.shcore.SetProcessDpiAwareness(1)  #Resolve the window resolution issue
    gui = GUI()
    gui.app.mainloop()
