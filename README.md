## Motivation
The SERCOS II fiber optics protocol was popular a couple of decades ago, but now modern devices use SERCOS III based on Ethernet. Some older machines (in my case, Trumph TC2000R) are economically inefficient to repair, the new CNC unit cost>$10k. This is why I implemented SERCOS II for myself, leveraging simple and free LinuxCNC with self-made hardware for $500 all.

## Legal status
It is **not** an end-to-end software for operation with SERCOS II. You **can not** clone this project and get your drives to work with LinuxCNC. 

**Developing the SERCOS II hardware without a proper license is restricted by the SERCOS initiative group.**

This published code implements **ONLY** the publicly available *software layout* of the protocol. The references are in **Industrial Communication Technology Handbook**, edited by Richard Zurawski, ISA Group, San Francisco, California, USA.

## Introduction
This repository contains the LinuxCNC module for link connection with fiber-optics SERCOS II drives. The SERCOS synchronous communications consist of two stages - synchronization and operation. The operation stage is implemented well, but instead of synchronization, I captured the signals from the Beckhoff FC7502 device and just re-sent them without any modification.

Once the synchronization is done, It is possible to operate the SERCOS II drive as a common servo axis, in many modes: position control, velocity control, and so on.
## Hardware
You need to develop the hardware device by yourself, I can not publish its code. 
Practical tips:
* I leveraged STM32F767ZI microcontroller as the main device
* This module sends raw Ethernet frames via consumer-scale network device
* The main device gets raw Ethernet frames and simply retranslates them to fiber optics transmitter and receiver
* When transmitting is not you need to send a square wave with the required frequency
* Additionally, I use tiny ArrowBlaster FPGA for zero-latency NRZI encoding between STM and physical devices

## Contact
If you have any questions, feel free to ask me via Telegram (@arabel1a)