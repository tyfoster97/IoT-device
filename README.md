# Course Code
Copyright(C) 2020, Arizona State University
All rights reserved

## Setup files
> **NOTE:** bash commands in files below were provided by instructor, I wrote the scripts to save time when setting up the dev environment multiple times.

### headers-setup.sh
> Bash script to install linux headers in the dev environment

### avr-setup.sh
> Bash script to install avr libraries in the dev environment

### code::blocks-setup.sh
> Bash script to install Code Blocks, runs **install.sh**

### install.sh
> Instructor provided bash script to install Code Blocks

### simulavr.sh
> Bash script to install avr simulator

## Assignment 1
> implement UART functions for an Atmega328p

## Assignment 2
> implement a delay loop for an Atmega328p to toggle the LED state

## Assignment 3
> implement code to print hexadecimal characters to UART and implement code to blink morse code message through the LED

## Assignment 4
> implement Atmega328p timer to print temperature at regular interval

## Project 1
> implement code for Atmega328p to update LED and print date string every 500ms

## Project 2
> implement interrupt vector and cache system to write config and vital product data to EEPROM while updating LED state

## Project 3
> implement code to use REST API to change config settings, temperature sensor finite state machine updated every second, have the LED indicate the state of the machine, and implement a watchdog timer to reboot device if hung code occurs

## Final Project
> implement code to use REST API with TCP sockets to change config settings and read vital product data on EEPROM, update temperature sensor finite state machine every second, and change the LED blink rate to indicate if temperature is in warning, critical, or normal range.
