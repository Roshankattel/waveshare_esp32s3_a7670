#ifndef CONFIG_H
#define CONFIG_H

#define MODEM_BAUDRATE (115200)
#define MODEM_DTR_PIN (45)
#define MODEM_TX_PIN (18)
#define MODEM_RX_PIN (17)
#define MODEM_RING_PIN (40)
#define MODEM_POWERKEY_PIN (33) // You have to turn off 4G from DIP swticbn before using this pin

// SD Card pins
#define SDMMC_CMD (4)
#define SDMMC_CLK (5)
#define SDMMC_DATA (6)

// I2C pins for MAX17048 fuel gauge
#define BAT_SDA_PIN (3)
#define BAT_SCL_PIN (2)
#define MAX17048_I2C_ADDRESS 0x36

#define RGB_PIN (38)

#endif // CONFIG_H