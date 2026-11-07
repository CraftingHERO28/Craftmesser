# DIY Room Air Quality Monitor

I wanted to build a solid, reliable indoor air quality monitor to keep track of CO2 levels, temperature, and humidity in my room. A lot of cheap commercial monitors use estimated VOC sensors instead of actual optical sensors, so I decided to design my own board around an Arduino Nano and an NDIR MH-Z19B sensor.

## How it works & Hardware choices
The setup is built completely with through-hole (THT) components on socket headers so everything stays easy to solder and swap out if needed:

- **Arduino Nano v3.x** handling the logic.
- **MH-Z19B** for actual NDIR CO2 measurement.
- **DHT22 / AM2302** for temperature and humidity.
- **0.96" I2C OLED** for displaying live data.
- Decoupling caps (100nF ceramic & 100uF electrolytic) to smooth out voltage drops when the CO2 sensor warms up, plus a 10k pull-up for the DHT data line.

## Design notes & Things I ran into

- **Thermal Isolation:** The biggest challenge was keeping heat from the Arduino regulator and the CO2 sensor away from the temperature reading. I ended up placing the DHT22 at the far corner of the PCB away from major trace paths. 
- **Trace Routing:** I initially tried making decorative parallel power traces ("trace art"), but quickly realized it created routing clutter and unnecessary heat bridges toward the temp sensor. Going back to clean, single traces worked much better.
- **Edge Cuts:** I also experimented with slot cutouts on the PCB board outline (`Edge.Cuts`) for thermal isolation, but KiCad threw continuous DRC errors for open contours. Placing the sensor at the board edge using FR4's natural isolation turned out to be more than enough without messing with custom PCB millings.

## Total Cost
Total cost ended up around **€58 / ~$63 USD**, including component sourcing from Reichelt and getting 5 custom PCBs fabricated via JLCPCB (shipping included). Full component breakdown is available in the `BOM.csv` file.

