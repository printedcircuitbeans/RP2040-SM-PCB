# RP2040-SM-PCB
PCB and circuit designs for my RP2040 open source smartwatch

This project was created as I wanted to develop my own smartwatch from the ground up, learning PCB design, routing, component selection and layout skills along the way.
I picked the RP2040 microcontroller as I had heard about it's release and it seemed interesting for my application, that is, running a large amount of devices on relatively low power, but with sufficient peformance. It had enough pins, could support up to 16MB of flash, and importantly was small and cheap.

I prototyped the watch on a breadboard intially so I was able to swap components out and make changes quite quickly. I decided to use a GPS to fetch the time and be able to have positioning data - I originally prototyped with a BN180 GPS in the breadboard phase but decided to use a U-Blox M10s as it was smaller and had a better feature set. The presence of a GPS on the board opened the door to have a local map tool, and so I needed storage. I decided to use an SD card for this, as it provided an upgradeable storage medium with ample storage.

I wanted to also have rich set of sensors to be able to be able to gather and use real world data for features in the watch. I wanted to be able to wake the watch when moving my wrist in a certain manner, so I needed an IMU : I decided to go with the MP6050 because it was very popular and I was already familiar with it. In conjunction to this I decided to go with a magnometer to be able to integrate a compass/orientation into the map app. This was a new sensor to me which I did not test in the prototyping phase so I approached it differently. I started by doing research to find something that would work well for my needs (small footprint, minimal external components) which lead me to choose the IIS2MDCTR as it was simple to use, and had a minimal footprint

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

The watch runs a low level OS which handles all functions and manages a GUI interface. Currently I am having issues with flashing the watch (see the issue) - which is a new development as the flash worked before. I'll likely write up an errata for the watch if I find a consistent problem.
I'm also working on developing a full "OS" for the watch but development on that front is stalled until the flash problem is resolved.

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
