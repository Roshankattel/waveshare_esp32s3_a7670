# Waveshare ESP32-S3 A7670 Demo

PlatformIO demo for the [Waveshare ESP32-S3-A7670E-4G](https://www.waveshare.com/esp32-s3-a7670e-4g.htm) board. It exercises the onboard A7670E cellular modem over UART, sends a test SMS, reads and deletes stored messages, queries modem status, drives the RGB LED, and reports battery level from the MAX17048 fuel gauge.

## What This Demo Does

On boot, the firmware:

1. Powers on the A7670E modem and waits until it responds to `AT`
2. Sets SMS text mode (`AT+CMGF=1`)
3. Sends a test SMS (default destination: `10086`)
4. Lists all SMS (`AT+CMGL="ALL"`), reads message index 20, then deletes it
5. Queries network and SIM info (`AT+COPS?`, `AT+CGDCONT?`, `AT+SIMCOMATI`)

In the main loop it prints any modem UART output and reads battery state over I2C every second.

## Hardware Requirements

| Item | Notes |
| --- | --- |
| Waveshare ESP32-S3-A7670E-4G board | ESP32-S3R2, 16 MB Flash, 2 MB PSRAM, onboard A7670E |
| USB-C cable | For flashing and serial monitor |
| Nano SIM card | Active plan with SMS support in your region |
| LTE antenna | Connected to the board (included with the kit) |
| 18650 battery (optional) | Fits the onboard holder; useful for portable testing |

Official docs: [Waveshare ESP32-S3-A7670E-4G overview](https://docs.waveshare.com/ESP32-S3-A7670E-4G)

## DIP Switch Setup

The board shares one USB-C port between the ESP32-S3 and the 4G module. Set the rear DIP switches so USB routes to the **ESP32-S3** when flashing firmware and viewing the serial monitor.

This demo talks to the modem over **UART** (not USB passthrough). Before running it, configure the switches per the Waveshare user guide so the ESP32 can control the modem power key on GPIO 33. The note in `include/config.h` applies here: disable the **4G USB** path on the DIP switch when using `MODEM_POWERKEY_PIN`.

After changing switch positions, power-cycle the board.

## Software Requirements

- [Visual Studio Code](https://code.visualstudio.com/)
- [PlatformIO IDE extension](https://platformio.org/install/ide?install=vscode) (recommended in `.vscode/extensions.json`)

PlatformIO downloads the ESP32 platform and libraries automatically from `platformio.ini`.

## Quick Start

### 1. Clone the repository

```bash
git clone <your-repo-url>
cd waveshare_esp32s3_a7670
```

### 2. Customize before first run

Edit `src/main.cpp` and set a real destination number for the SMS test:

```cpp
SentMessage("AT+CMGS=\"10086\"");  // replace with your phone number
```

Depending on your carrier and country, you may also need to set the SMS service center. Uncomment and adapt in `setup()` if sends fail:

```cpp
SentMessage("AT+CSCA=\"+8613800755500\"");  // example for China Mobile
```

Use your carrier’s SMS center number if required.

### 3. Build and upload

Connect the board over USB-C (ESP32-S3 selected on the DIP switches), then:

```bash
pio run -t upload
```

Or in VS Code: **PlatformIO → Project Tasks → waveshare_esp32s3_a7670 → General → Upload**.

### 4. Open the serial monitor

```bash
pio device monitor
```

Baud rate: **115200** (configured in `platformio.ini`).

Expected output includes lines like:

```
Got OK!
Got OK!
...
Battery Level: 85.23%
```

The RGB LED turns on yellow during initialization.

## Project Layout

```
waveshare_esp32s3_a7670/
├── include/
│   └── config.h          # Pin map and modem settings
├── src/
│   └── main.cpp          # Demo application
└── platformio.ini        # Board, build flags, libraries
```

## Pin Configuration

Key definitions live in `include/config.h`:

| Function | Pin / Value |
| --- | --- |
| Modem UART TX | GPIO 18 |
| Modem UART RX | GPIO 17 |
| Modem baud rate | 115200 |
| Modem power key | GPIO 33 |
| RGB LED | GPIO 38 |
| Battery I2C (MAX17048) | SDA GPIO 3, SCL GPIO 2 |

Adjust these only if you have a different board revision or wiring.

## Dependencies

Managed by PlatformIO in `platformio.ini`:

- [Adafruit NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) — RGB LED
- [TinyGSM](https://github.com/vshymanskyy/TinyGSM) — listed for future cellular helpers (this demo uses direct AT commands)

## Troubleshooting

**Modem never responds (`Timeout!` on `AT`)**

- Confirm the SIM is inserted correctly and the LTE antenna is attached
- Check DIP switch positions and power-cycle the board
- Verify UART pins in `config.h` match your hardware

**SMS send fails (`Got ERROR!`)**

- Replace `10086` with a valid number in international format (e.g. `"+15551234567"`)
- Set `AT+CSCA` to your carrier’s SMS center if needed
- Ensure the SIM has SMS enabled and network registration (red network LED should flash after registration)

**No serial output**

- Confirm USB is routed to the ESP32-S3 on the DIP switches
- Use 115200 baud
- On macOS/Linux, check the port with `pio device list`

**Battery level reads as 0 or looks wrong**

- Insert a charged 18650 or power the board from USB
- The MAX17048 needs a battery connected to report meaningful values

**Upload fails**

- Hold **BOOT**, tap **RESET**, release **BOOT** to enter download mode, then retry upload
- Try a lower upload speed by changing `upload_speed` in `platformio.ini`

## Next Steps

- Replace hard-coded AT sequences with [TinyGSM](https://github.com/vshymanskyy/TinyGSM) for cleaner PPP/data/SMS APIs
- Add GNSS (`AT+CGNSSPWR=1`) or MQTT examples from the [Waveshare documentation](https://docs.waveshare.com/ESP32-S3-A7670E-4G)
- Configure APN settings for your carrier if you move beyond SMS to data connections

## License

See the repository license file if present. Waveshare hardware documentation and module AT command references remain © Waveshare / SIMCom.
