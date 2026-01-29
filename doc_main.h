/**
 * @file doc_main.h
 * @brief Landing page for Doxygen documentation.
 * This file contains no code, only documentation markup.
 */

/**
 * @mainpage STM32F411xx Bare-Metal Drivers & Components
 *
 * @section intro_sec Introduction
 * Welcome to the **STM32F411xx Driver Library**. 
 * This project provides a bare-metal implementation of peripheral drivers 
 * for the STM32F411 microcontroller. The library focuses on register-level 
 * manipulations using custom-built @ref RegDef structures.
 *
 * @section status_sec Driver Maturity & Capabilities
 * | Peripheral | Status | Key Features |
 * | :--- | :--- | :--- |
 * | **GPIO** | **Production Ready** | Clock control, Pin Modes, Pull-up/down, and IRQ Handling. |
 * | **SPI** | **Production Ready** | Master/Slave, Polling & Interrupt modes, Error Handling. |
 * | **USART** | **Work In Progress** | Basic register mapping and Handle structure. APIs pending. |
 * | **RCC** | **Core Ready** | Clock enable/disable and peripheral reset macros. |
 *
 * @section components_sec Integrated Components
 * - **MFRC522** (@ref MFRC522_Driver): High-level driver for the RFID module. 
 * Leverages the SPI Interrupt mode for non-blocking communication.
 *
 * @section design_sec Design Philosophy
 * 1. **Zero Abstraction**: Minimal overhead, direct register access using `volatile` definitions.
 * 2. **Handle-Based API**: Each peripheral is managed via a dedicated `Handle_t` structure.
 * 3. **Non-Blocking Flow**: SPI drivers support interrupt-driven transactions to maximize CPU efficiency.
 *
 * @section author_sec Project Meta
 * - **Author**: Andrii
 * - **Target**: STM32F411xx (ARM Cortex-M4)
 * - **Development Environment**: STM32CubeIDE.
 *
 * 
 */