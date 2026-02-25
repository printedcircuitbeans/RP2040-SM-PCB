# RP2040-SM-PCB
PCB and circuit designs for my RP2040 open source smartwatch

- Features :
- 160*120 TFT screen 
- RP2040 Dual core microcontroller
- USB-C port for charging and programming	
- u-Blox MAX M10S GPS chip
- SD card reader
- MPU6050 IMU (Inertial Measurement Unit)
- MAX30100 Pulse Oximeter
- Renesas DA14531 (as Bluetooth slave)
- IIS2MDCTR 3 axis magnetic sensor
- Bosch BME680
- Integrated charge circuit and BMS (Battery Management System)
- 400 mAh LiPo battery and UPS (Uninterruptible Power Supply) like power path design. 


I'm working on a small datasheet to make the watch easier to understand and to build if someone wishes to do so.

The watch runs a low level OS which handles all functions and manages a GUI interface. Currently I am having issues with flashing the watch - which is a new development as the flash worked before. I'll likely write up an errata for the watch.

<img width="348" height="475" alt="image" src="https://github.com/user-attachments/assets/6c2ac127-7a2a-4924-9ef8-f3939657c862" />
<img width="348" height="475" alt="image" src="https://github.com/user-attachments/assets/8bf6a951-1638-43d7-adb3-79ef8d951da8" />
<img width="1540" height="853" alt="image" src="https://github.com/user-attachments/assets/62799625-8b3d-48fe-8a55-8fbfa5c0275a" />



## License

This project is licensed under the Creative Commons 
Attribution-NonCommercial-ShareAlike 4.0 International License (CC BY-NC-SA 4.0).

You are free to:
- Share — copy and redistribute the material
- Adapt — remix, transform, and build upon the material

Under the following terms:
- Attribution — You must give appropriate credit.
- NonCommercial — You may not use the material for commercial purposes.
- ShareAlike — Derivatives must be licensed under the same terms.

Full license text: https://creativecommons.org/licenses/by-nc-sa/4.0/

### Required Attribution

If you use or adapt this project, you must:

- Credit: [Your Name]
- Link to this repository
- Indicate if changes were made
- State that it is licensed under CC BY-NC-SA 4.0
