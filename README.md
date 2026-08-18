# 🔐 Anti-Theft Locker System

An embedded systems-based electronic locker security system using Arduino UNO, 4x4 keypad authentication, servo motor locking, LCD display, LEDs, and buzzer-based alarm detection.

## 📌 Overview

The Anti-Theft Locker System is a low-cost electronic access-control system designed to protect a locker from unauthorized access.

The user enters a password through a 4x4 matrix keypad. The Arduino UNO verifies the entered password and controls the servo-based locking mechanism.

If the password is correct, the locker unlocks temporarily. If incorrect passwords are entered repeatedly, the system activates a buzzer alarm and keeps the locker locked.

## 🎯 Problem Statement

Traditional lockers rely on physical keys, which can be lost, copied, or stolen.

This project demonstrates an electronic alternative using password authentication and automatic security responses.

## ✨ Features

- 🔢 4x4 keypad password input
- 🔐 Password-based authentication
- 🔒 Servo-controlled locker
- 📺 16x2 I2C LCD
- 🟢 Access-granted LED
- 🔴 Access-denied LED
- 🔊 Buzzer security alarm
- 🚨 Three-attempt detection
- ⏱️ Automatic locking after 5 seconds
- ⭐ Password masking on LCD
- 🧪 Wokwi virtual simulation

## 🛠️ Components

| Component | Quantity |
|---|---:|
| Arduino UNO | 1 |
| 4x4 Matrix Keypad | 1 |
| Servo Motor | 1 |
| 16x2 I2C LCD | 1 |
| Buzzer | 1 |
| Red LED | 1 |
| Green LED | 1 |
| 220Ω Resistor | 2 |
| Jumper Wires | As required |

## 🔌 Pin Configuration

### Keypad

| Keypad | Arduino UNO |
|---|---|
| R1 | D2 |
| R2 | D3 |
| R3 | D4 |
| R4 | D5 |
| C1 | D6 |
| C2 | D7 |
| C3 | D8 |
| C4 | D9 |

### Servo

| Servo | Arduino UNO |
|---|---|
| Signal | D10 |
| VCC | 5V |
| GND | GND |

### Buzzer

| Buzzer | Arduino UNO |
|---|---|
| Positive | D11 |
| Negative | GND |

### LEDs

| Component | Arduino UNO |
|---|---|
| Red LED | D12 |
| Green LED | D13 |

Each LED is connected through a 220Ω resistor.

### LCD

| LCD | Arduino UNO |
|---|---|
| GND | GND |
| VCC | 5V |
| SDA | A4 |
| SCL | A5 |

## 🏗️ System Architecture

```text
        ┌─────────────────┐
        │   4x4 Keypad    │
        └────────┬────────┘
                 │
                 ▼
        ┌─────────────────┐
        │   Arduino UNO   │
        │                 │
        │ Authentication  │
        │ Attempt Counter │
        │ Security Logic  │
        └───┬─────┬─────┬─┘
            │     │     │
            ▼     ▼     ▼
         Servo   LCD   LEDs
          Lock         + Buzzer
