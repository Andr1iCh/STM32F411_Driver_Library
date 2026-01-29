# STM32F411xx Driver Library

![Language](https://img.shields.io/badge/language-C-blue.svg)
![Platform](https://img.shields.io/badge/platform-STM32F411RE-green.svg)
[![Documentation](https://img.shields.io/badge/Documentation-Online-green?style=flat&logo=doxygen)](https://andr1ich.github.io/STM32F411_Driver_Library/)

**A bare-metal Embedded C driver library for the STM32F411xx microcontroller family, featuring a custom SPI driver implementation for the MFRC522 RFID module.**

This project demonstrates low-level driver development without using HAL or Standard Peripheral Libraries. It includes drivers for GPIO, SPI, and an initial device driver for the MFRC522 RFID reader.

---

## 📂 Project Structure

The project is organized to separate reusable drivers from application code:

```text
STM32_Driver_Library/
├── Drivers/              # Core MCU Peripherals (SPI, GPIO)
├── Components/           # External Device Drivers (MFRC522)
├── Examples/             # Main Application (RFID SPI Access Demo)
└── Validation/           # Hardware Verification tests (Logic Analyzer/Arduino)

```

---

## 🚀 Key Features

### Peripheral Drivers

* **SPI:** Full-Duplex, Master Mode, 8/16-bit DFF. Supports both **Blocking** and **Interrupt-based** communication.
* **GPIO:** Output, Input, Alternate Functions, and External Interrupts.
* **USART:** Work In Progress.

### MFRC522 Driver

* Custom implementation of read/write functions over SPI.
* Integrated hardware and software reset sequences.

### Documentation

* Full API documentation generated via **Doxygen**.
* Call graphs and module hierarchy visualizations.

---

## 🛠 Hardware Used

* **MCU:** STM32F411RE (Nucleo-64)
* **RFID Module:** MFRC522 (SPI Interface)
* **Logic Analyzer:** Saleae (used for timing validation)
* **Slave Device:** Arduino Uno (used for SPI full-duplex testing)

---

## ⚙️ Build & Testing

This project uses **Build Configurations** to switch between the main demo and hardware tests without modifying the source code manually.

**How to run a specific test in STM32CubeIDE:**

1. Right-click on the project → **Build Configurations** → **Set Active**.
2. Choose one of the following:

* **`MFRC522_Read_Write_Demo` (Default):**
A demonstration application that interfaces with the MFRC522 module over SPI and forwards the retrieved RFID data to a serial terminal via UART.
* **`Test_SPI_Arduino_Link`:**
Validates data integrity in full-duplex communication with an Arduino Slave.
* **`Test_SPI_LogicAnalyzer`:**
Sends continuous data patterns for Oscilloscope/Logic Analyzer inspection.
* **`Test_GPIO_Hardware`:**
Basic I/O verification (LED toggling, Button interrupts) to ensure board health.

---

## 📚 Documentation

Complete documentation with API references, call graphs, and hardware dependency diagrams is available here:

👉 **[View Full Documentation Online](https://andr1ich.github.io/STM32F411_Driver_Library/)**

---

## 👨‍💻 Author

**Andrii**
*Aspiring Embedded Systems Engineer*

Open to opportunities in Embedded Linux & Bare-metal development.