#ifndef RFID_RC522_SPI_INC_MFRC522_C_
#define RFID_RC522_SPI_INC_MFRC522_C_

#include "stm32f411xx.h"

/*
 * Global shared variables
 */
extern volatile uint8_t SPITransfrComplete;

/*
 *definitions with tech specs or rules of data transmission
 */
#define MAX_LEN_BYTES 		64
#define MFRC522_READ_BIT    (1 << 7)
#define MFRC522_ADDR_MASK   (0x3F)   // 0011 1111 mask for safety

/*
 *structure with key elements for SPI
 */
typedef struct {
    SPI_Handle_t *pSPI;
    GPIO_RegDef_t *pRstPort;
    uint8_t RstPin;
} MFRC522_Config_t;

/*
 * Functions
 */
void MFRC522_Init(SPI_Handle_t *pSPI, GPIO_Handle_t *pCS, GPIO_Handle_t *pRST);
void MFRC522_Reset(void);
void Read_IT(uint8_t addrByte, uint8_t lenByte);
void WriteBuffer_IT(uint8_t addrByte, const uint8_t *pBuffer, uint8_t lenByte);
void WriteByte_IT(uint8_t addrByte, uint8_t value);
uint8_t GetData_RxBuff(uint8_t *destination, uint8_t length);

/*
 *  Register Definitions
 */

/* ====================================================================
 * Command and Status
 * ==================================================================== */
#define MFRC522_REG_RESERVED00      0x00    /* Reserved for future use */
#define MFRC522_REG_COMMAND         0x01    /* Starts and stops command execution */
#define MFRC522_REG_COMM_IEN        0x02    /* Enable and disable interrupt request control bits */
#define MFRC522_REG_DIV_IEN         0x03    /* Enable and disable interrupt request control bits */
#define MFRC522_REG_COMM_IRQ        0x04    /* Interrupt request bits */
#define MFRC522_REG_DIV_IRQ         0x05    /* Interrupt request bits */
#define MFRC522_REG_ERROR           0x06    /* Error bits showing the error status of the last command */
#define MFRC522_REG_STATUS1         0x07    /* Communication status bits */
#define MFRC522_REG_STATUS2         0x08    /* Communication status bits */
#define MFRC522_REG_FIFO_DATA       0x09    /* Input and output of 64 byte FIFO buffer */
#define MFRC522_REG_FIFO_LEVEL      0x0A    /* Number of bytes stored in the FIFO buffer */
#define MFRC522_REG_WATER_LEVEL     0x0B    /* Level for FIFO underflow and overflow warning */
#define MFRC522_REG_CONTROL         0x0C    /* Miscellaneous control registers */
#define MFRC522_REG_BIT_FRAMING     0x0D    /* Adjustments for bit-oriented frames */
#define MFRC522_REG_COLL            0x0E    /* Bit position of the first bit-collision detected on the RF interface */
#define MFRC522_REG_RESERVED0F      0x0F    /* Reserved for future use */

/* ====================================================================
 * Command
 * ==================================================================== */
#define MFRC522_REG_RESERVED10      0x10    /* Reserved for future use */
#define MFRC522_REG_MODE            0x11    /* Defines general modes for transmitting and receiving */
#define MFRC522_REG_TX_MODE         0x12    /* Defines transmission data rate and framing */
#define MFRC522_REG_RX_MODE         0x13    /* Defines reception data rate and framing */
#define MFRC522_REG_TX_CONTROL      0x14    /* Controls the logical behavior of the antenna driver pins TX1 and TX2 */
#define MFRC522_REG_TX_ASK          0x15    /* Controls the setting of the transmission modulation */
#define MFRC522_REG_TX_SEL          0x16    /* Selects the internal sources for the antenna driver */
#define MFRC522_REG_RX_SEL          0x17    /* Selects internal receiver settings */
#define MFRC522_REG_RX_THRESHOLD    0x18    /* Selects thresholds for the bit decoder */
#define MFRC522_REG_DEMOD           0x19    /* Defines demodulator settings */
#define MFRC522_REG_RESERVED1A      0x1A    /* Reserved for future use */
#define MFRC522_REG_RESERVED1B      0x1B    /* Reserved for future use */
#define MFRC522_REG_MF_TX           0x1C    /* Controls some MIFARE communication transmit parameters */
#define MFRC522_REG_MF_RX           0x1D    /* Controls some MIFARE communication receive parameters */
#define MFRC522_REG_RESERVED1E      0x1E    /* Reserved for future use */
#define MFRC522_REG_SERIAL_SPEED    0x1F    /* Selects the speed of the serial UART interface */

/* ====================================================================
 * Configuration
 * ==================================================================== */
#define MFRC522_REG_RESERVED20      0x20    /* Reserved for future use */
#define MFRC522_REG_CRC_RESULT_M    0x21    /* Shows the MSB and LSB values of the CRC calculation */
#define MFRC522_REG_CRC_RESULT_L    0x22    /* Shows the MSB and LSB values of the CRC calculation */
#define MFRC522_REG_RESERVED23      0x23    /* Reserved for future use */
#define MFRC522_REG_MOD_WIDTH       0x24    /* Controls the ModWidth setting */
#define MFRC522_REG_RESERVED25      0x25    /* Reserved for future use */
#define MFRC522_REG_RF_CFG          0x26    /* Configures the receiver gain */
#define MFRC522_REG_GS_N            0x27    /* Selects the conductance of the antenna driver pins TX1 and TX2 for modulation */
#define MFRC522_REG_CW_GS_P         0x28    /* Defines the conductance of the p-driver output during periods of no modulation */
#define MFRC522_REG_MOD_GS_P        0x29    /* Defines the conductance of the p-driver output during periods of modulation */
#define MFRC522_REG_T_MODE          0x2A    /* Defines settings for the internal timer */
#define MFRC522_REG_T_PRESCALER     0x2B    /* Defines settings for the internal timer */
#define MFRC522_REG_T_RELOAD_H      0x2C    /* Defines the 16-bit timer reload value */
#define MFRC522_REG_T_RELOAD_L      0x2D    /* Defines the 16-bit timer reload value */
#define MFRC522_REG_T_COUNTER_VAL_H 0x2E    /* Shows the 16-bit timer value (high byte) */
#define MFRC522_REG_T_COUNTER_VAL_L 0x2F    /* Shows the 16-bit timer value (low byte) */

/* ====================================================================
 * Test Register
 * ==================================================================== */
#define MFRC522_REG_RESERVED30      0x30    /* Reserved for future use */
#define MFRC522_REG_TEST_SEL1       0x31    /* General test signal configuration */
#define MFRC522_REG_TEST_SEL2       0x32    /* General test signal configuration */
#define MFRC522_REG_TEST_PIN_EN     0x33    /* Enables pin output driver on pins D1 to D7 */
#define MFRC522_REG_TEST_PIN_VALUE  0x34    /* Defines the values for D1 to D7 when it is used as an I/O bus */
#define MFRC522_REG_TEST_BUS        0x35    /* Shows the status of the internal test bus */
#define MFRC522_REG_AUTO_TEST       0x36    /* Controls the digital self-test */
#define MFRC522_REG_VERSION         0x37    /* Shows the software version */
#define MFRC522_REG_ANALOG_TEST     0x38    /* Controls the pins AUX1 and AUX2 */
#define MFRC522_REG_TEST_DAC1       0x39    /* Defines the test value for TestDAC1 */
#define MFRC522_REG_TEST_DAC2       0x3A    /* Defines the test value for TestDAC2 */
#define MFRC522_REG_TEST_ADC        0x3B    /* Shows the value of ADC I and Q channels */
#define MFRC522_REG_RESERVED3C      0x3C    /* Reserved for future use */
#define MFRC522_REG_RESERVED3D      0x3D    /* Reserved for future use */
#define MFRC522_REG_RESERVED3E      0x3E    /* Reserved for future use */
#define MFRC522_REG_RESERVED3F      0x3F    /* Reserved for future use */

/* ====================================================================
 * MFRC522 Commands (PCD - Proximity Coupling Device)
 * ==================================================================== */
#define MFRC522_CMD_IDLE            0x00    /* No action, cancels current command execution */
#define MFRC522_CMD_MEM             0x01    /* Stores 25 bytes into the internal buffer */
#define MFRC522_CMD_GEN_RANDOM_ID   0x02    /* Generates a 10-byte random ID number */
#define MFRC522_CMD_CALC_CRC        0x03    /* Activates the CRC coprocessor or performs a self-test */
#define MFRC522_CMD_TRANSMIT        0x04    /* Transmits data from the FIFO buffer */
#define MFRC522_CMD_NO_CMD_CHANGE   0x07    /* No command change, can be used to modify the CommandReg without affecting command, for example, the PowerDown bit */
#define MFRC522_CMD_RECEIVE         0x08    /* Activates the receiver circuits */
#define MFRC522_CMD_TRANSCEIVE      0x0C    /* Transmits data from FIFO buffer to antenna and automatically activates the receiver after transmission */
#define MFRC522_CMD_MF_AUTHENT      0x0E    /* Performs the MIFARE standard authentication as a reader */
#define MFRC522_CMD_SOFT_RESET      0x0F    /* Resets the MFRC522 */

#endif /* RFID_RC522_SPI_INC_MFRC522_C_ */
