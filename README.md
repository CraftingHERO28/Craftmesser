# DIY Room Air Quality Monitor

I wanted to build a solid, reliable indoor air quality monitor to keep track of CO₂ levels, temperature, and humidity in my room. A lot of cheap commercial monitors use estimated VOC sensors instead of actual optical sensors, so I decided to design my own board around an Arduino Nano and an NDIR MH-Z19B sensor.

Through consistent through-hole (THT) construction and thermal decoupling, it delivers precise readings without self-heating distortion.

---

## How it Works & Hardware Choices

The setup is built completely with THT components on socket headers so everything stays easy to solder and swap out if needed:

* **Arduino Nano v3.x** handling the logic.
* **MH-Z19B** for actual NDIR CO₂ measurement.
* **DHT22 / AM2302** for temperature and humidity.
* **0.96" I2C OLED** for displaying live data.
* **Decoupling & Pull-up:** A `100nF` ceramic capacitor and a `100µF` electrolytic capacitor smooth out voltage drops when the CO₂ sensor warms up, paired with a `10kΩ` pull-up resistor for the DHT data line.

---

## Bill of Materials (BOM)


---

## Wiring Diagram

All components share a common `5V` and `GND` rail decoupled by the `100µF` electrolytic and `100nF` ceramic capacitors.

```text
                       +-----------------------+
                       |     ARDUINO NANO      |
                       |                       |
   +------------------>| 5V                 D2 |<---[10kΩ]---+--- 5V
   |   +-------------->| GND                   |             |
   |   |               |                    D3 |<---+        |
   |   |               |                    D4 |---+|        |
   |   |               |                    A4 |<--||---+    |
   |   |               |                    A5 |<--||--+|    |
   |   |               +-----------------------+   ||  ||    |
   |   |                                           ||  ||    |
   |   |   +---------------------------------------++  ||    |
   |   |   |   +---------------------------------------++    |
   |   |   |   |                                             |
   |   |   |   |       +-----------------------+             |
   |   |   |   |       |    DHT22 / AM2302     |             |
   +---|---|---|------>| VCC (Pin 1)           |             |
   |   |   |   |   +-->| DATA (Pin 2)          |<------------+
   |   +---|---|---|-->| GND (Pin 4)           |
   |   |   |   |   |   +-----------------------+
   |   |   |   |   |
   |   |   |   |   |   +-----------------------+
   |   |   |   |   |   |   MH-Z19B CO2 Sensor  |
   +---|---|---|------>| VIN (5V)              |
   |   +---|---|---|-->| GND                   |
   |   |   +---|---|-->| TX  (connects to D3)  |
   |   |   |   |   |   | RX  (connects to D4)  |
   |   |   |   |   |   +-----------------------+
   |   |   |   |   |
   |   |   |   |   |   +-----------------------+
   |   |   |   |   |   |   0.96" I2C OLED      |
   +---|---|---|------>| VCC                   |
   |   +---|---|---|-->| GND                   |
   |   |   |   +---|-->| SDA (connects to A4)  |
   |   |   |   |   +-->| SCL (connects to A5)  |
   |   |   |   |       +-----------------------+
   |   |   |   |
  [===] [===]  (Power Filtering across 5V & GND: 100µF Elko + 100nF Ceramic)
```

---

## How to Build It

### 1. Order the PCBs & Parts
* Export the Gerber files from the KiCad project and upload the `.zip` archive to **JLCPCB** (standard 2-layer FR4, 1.6mm thickness).
* Order all electronic components using the Reichelt links in the BOM table above.

### 2. Solder the Board (Lowest to Tallest)
1. **Passive Components:** Solder the `10kΩ` pull-up resistor and the `100nF` ceramic capacitor flat against the PCB.
2. **Socket Headers:** Solder the two `1x15` female headers for the Arduino Nano and the three `1x4` headers for the OLED, DHT22, and MH-Z19B. *Tip: Plug the headers onto the Arduino/modules first to keep them aligned while soldering.*
3. **Electrolytic Capacitor:** Solder the `100µF` capacitor into place, paying close attention to polarity (the striped side goes to `GND`).
4. **Insert Modules:** Once soldering is complete and checked for bridges, plug the Arduino Nano, OLED display, DHT22, and MH-Z19B into their respective sockets. Make sure the DHT22 sits cleanly at the edge of the board for accurate thermal isolation.

### 3. Flash the Firmware
1. Download and open the **Arduino IDE**.
2. Open the Library Manager (`Ctrl + Shift + I`) and install:
   * `Adafruit SSD1306`
   * `Adafruit GFX Library`
   * `DHT sensor library` (by Adafruit)
3. Connect the Arduino Nano via USB, select **Arduino Nano** (and **ATmega328P (Old Bootloader)** if using a clone) under *Tools*, and upload the firmware sketch.
4. After a 2-second boot screen, the OLED will display live CO₂ (ppm), temperature (°C), and humidity (%) readings.

---

## Design Notes & Things I Ran Into

* **Thermal Isolation:** The biggest challenge was keeping heat from the Arduino regulator and the CO₂ sensor away from the temperature reading. I ended up placing the DHT22 at the far corner of the PCB away from major trace paths.
* **Trace Routing:** I initially tried making decorative parallel power traces ("trace art"), but quickly realized it created routing clutter and unnecessary heat bridges toward the temp sensor. Going back to clean, single traces worked much better.
* **Edge Cuts:** I also experimented with slot cutouts on the PCB board outline (`Edge.Cuts`) for thermal isolation, but KiCad threw continuous DRC errors for open contours. Placing the sensor at the board edge using FR4's natural isolation turned out to be more than enough without messing with custom PCB millings.

_translated with google Translator_